// MIT License
//
// Copyright (C) Joshua MacDonald
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

package main

import (
	"context"
	"encoding/binary"
	"fmt"
	"log"
	"math/rand"
	"os"
	"sync/atomic"
	"syscall"
	"time"
	"unsafe"

	"github.com/jmacd/launchmidi/launchctl/xl"
)

const deviceName = "/dev/rpmsg_pru30"

type (
	RPMsgDevice struct {
		file *os.File
	}

	controlStruct struct {
		framebufsAddr uint32
		framebufsSize uint32
		frameCount    uint32
		dmaWait       uint32
	}

	Frameset  [2]Framebank
	Framebank [256]Framebuf
	Framebuf  [16]DoubleRow
	DoubleRow [64]DoublePixel

	DoublePixel struct {
		Gpio0 uint32
		Gpio1 uint32
		Gpio2 uint32
		Gpio3 uint32
	}
)

func openRPMsgDevice() (*RPMsgDevice, error) {
	file, err := os.OpenFile(deviceName, os.O_RDWR, 0666)
	return &RPMsgDevice{
		file: file,
	}, err
}

func (r *RPMsgDevice) write(data []byte) error {
	n, err := r.file.Write(data)
	if err != nil {
		return nil
	}
	if n != len(data) {
		return fmt.Errorf("short write: %d != %d", n, len(data))
	}
	return nil
}

func mmap(file *os.File, addr uint32, size int) ([]byte, error) {
	return syscall.Mmap(
		int(file.Fd()),
		int64(addr),
		size,
		syscall.PROT_READ|syscall.PROT_WRITE,
		syscall.MAP_SHARED,
	)
}

func (r *RPMsgDevice) readControl() (*controlStruct, *Frameset, error) {
	var data [32]byte
	n, err := r.file.Read(data[:])
	if err != nil {
		return nil, nil, err
	}
	if n != 4 {
		return nil, nil, fmt.Errorf("expected 4 bytes control address")
	}
	addr := binary.LittleEndian.Uint32(data[0:4])

	mem, err := os.OpenFile("/dev/mem", os.O_RDWR, 0)
	if err != nil {
		return nil, nil, err
	}

	cdata, err := mmap(mem, addr, int(unsafe.Sizeof(controlStruct{})))
	if err != nil {
		return nil, nil, err
	}

	ctrl := (*controlStruct)(unsafe.Pointer(&cdata[0]))

	fdata, err := mmap(mem, ctrl.framebufsAddr, int(ctrl.framebufsSize))
	if err != nil {
		return nil, nil, err
	}
	if ctrl.framebufsSize != uint32(unsafe.Sizeof(Frameset{})) {
		return nil, nil, fmt.Errorf("frameset size mismatch: C=%d, Go=%d", ctrl.framebufsSize, unsafe.Sizeof(Frameset{}))
	}

	framebuf := (*Frameset)(unsafe.Pointer(&fdata[0]))

	// Note: should drop privileges now.
	return ctrl, framebuf, nil
}

func Main() error {
	l, err := xl.Open()
	if err != nil {
		log.Fatalf("error while opening connection to launchctl: %v", err)
	}
	defer l.Close()

	rpm, err := openRPMsgDevice()
	if err != nil {
		return err
	}

	// pru.c does not parse the message, this delivers the
	// interrupt which causes pru.c to respond with its two
	// carveout addresses.
	if err := rpm.write([]byte("wakeup")); err != nil {
		return err
	}

	var ctrl *controlStruct
	var frames *Frameset

	ctrl, frames, err = rpm.readControl()
	if err != nil {
		return err
	}

	log.Println("Starting fill...")

	go func() {
		before := atomic.LoadUint32(&ctrl.frameCount)
		for {
			time.Sleep(10 * time.Second)
			after := atomic.LoadUint32(&ctrl.frameCount)
			log.Println("frames/sec", after-before)
			before = after
		}
	}()

	setB := func(r, g, b float64) {
		for dblrowNo := 0; dblrowNo < 16; dblrowNo++ {
			for pix := 0; pix < 64; pix++ {
				for bankNo := 0; bankNo < 2; bankNo++ {
					for frameNo := 0; frameNo < 256; frameNo++ {
						pixel := &(*frames)[bankNo][frameNo][dblrowNo][pix]
						rnd := rand.Float64()
						pixel.j1r1(rnd < r)
						pixel.j1g1(rnd < g)
						pixel.j1b1(rnd < b)
						pixel.j1r2(rnd < r)
						pixel.j1g2(rnd < g)
						pixel.j1b2(rnd < b)
						pixel.j3r1(rnd < r)
						pixel.j3g1(rnd < g)
						pixel.j3b1(rnd < b)
						pixel.j3r2(rnd < r)
						pixel.j3g2(rnd < g)
						pixel.j3b2(rnd < b)
					}
				}
			}
		}
	}
	setL := func(r, g, b float64) {
		var accum [12]float64
		acc := func(pos int, inc float64) bool {
			accum[pos] += inc
			if accum[pos] >= 1 {
				accum[pos] -= 1
				return true
			}
			return false
		}
		for pix := 0; pix < 64; pix++ {
			for dblrowNo := 0; dblrowNo < 16; dblrowNo++ {
				for bankNo := 0; bankNo < 2; bankNo++ {
					for frameNo := 0; frameNo < 256; frameNo++ {
						pixel := &(*frames)[bankNo][frameNo][dblrowNo][pix]

						pixel.j1r1(acc(0, r))
						pixel.j1g1(acc(1, g))
						pixel.j1b1(acc(2, b))
						pixel.j1r2(acc(3, r))
						pixel.j1g2(acc(4, g))
						pixel.j1b2(acc(5, b))
						pixel.j3r1(acc(6, r))
						pixel.j3g1(acc(7, g))
						pixel.j3b1(acc(8, b))
						pixel.j3r2(acc(9, r))
						pixel.j3g2(acc(10, g))
						pixel.j3b2(acc(11, b))
					}
				}
			}
		}
	}
	setP := func(r, g, b float64) {
		for pix := 0; pix < 64; pix++ {
			for dblrowNo := 0; dblrowNo < 16; dblrowNo++ {
				var accum [12]float64

				acc := func(pos int, rate float64) bool {
					if accum[pos] < 1 {
						accum[pos] += rand.ExpFloat64() / rate
					}
					accum[pos]--
					return accum[pos] < 1
				}
				for bankNo := 0; bankNo < 2; bankNo++ {
					for frameNo := 0; frameNo < 256; frameNo++ {
						pixel := &(*frames)[bankNo][frameNo][dblrowNo][pix]

						pixel.j1r1(acc(0, r))
						pixel.j1g1(acc(1, g))
						pixel.j1b1(acc(2, b))
						pixel.j1r2(acc(3, r))
						pixel.j1g2(acc(4, g))
						pixel.j1b2(acc(5, b))
						pixel.j3r1(acc(6, r))
						pixel.j3g1(acc(7, g))
						pixel.j3b1(acc(8, b))
						pixel.j3r2(acc(9, r))
						pixel.j3g2(acc(10, g))
						pixel.j3b2(acc(11, b))
					}
				}
			}
		}
	}

	r := 0.8
	g := 0.05
	b := 0.15

	focus := 0.0

	setB(r, g, b)

	l.AddCallback(xl.AllChannels, xl.ControlSlider[0], func(ch int, control xl.Control, value xl.Value) {
		r = value.Float()
	})
	l.AddCallback(xl.AllChannels, xl.ControlSlider[1], func(ch int, control xl.Control, value xl.Value) {
		g = value.Float()
	})
	l.AddCallback(xl.AllChannels, xl.ControlSlider[2], func(ch int, control xl.Control, value xl.Value) {
		b = value.Float()
	})
	l.AddCallback(xl.AllChannels, xl.ControlButtonTrackFocus[0], func(ch int, control xl.Control, value xl.Value) {
		focus = 0
	})
	l.AddCallback(xl.AllChannels, xl.ControlButtonTrackFocus[1], func(ch int, control xl.Control, value xl.Value) {
		focus = 1
	})
	l.AddCallback(xl.AllChannels, xl.ControlButtonTrackFocus[2], func(ch int, control xl.Control, value xl.Value) {
		focus = 2
	})
	go func() {
		err := l.Run(context.Background())
		if err != nil {
			log.Println("LX control run:", err)
		}
		log.Println("LX control exit")
	}()

	go func() {
		for {
			time.Sleep(25 * time.Millisecond)
			switch focus {
			case 0:
				setL(r, g, b)
			case 1:
				setB(r, g, b)
			case 2:
				setP(r, g, b)
			}
		}
	}()

	select {}
}

func main() {
	if err := Main(); err != nil {
		log.Println("error:", err)
	}
}

func set(ptr *uint32, on bool, pos uint32) {
	// var val uint32
	// if on {
	// 	val++
	// }
	// *ptr = (*ptr &^ (uint32(1) << pos)) | (val << pos)

	if on {
		*ptr |= 1 << pos
	} else {
		*ptr &= ^(1 << pos)
	}
}

func (p *DoublePixel) j1r1(on bool) { set(&p.Gpio2, on, 2) }
func (p *DoublePixel) j1g1(on bool) { set(&p.Gpio2, on, 3) }
func (p *DoublePixel) j1g2(on bool) { set(&p.Gpio2, on, 4) }
func (p *DoublePixel) j1b1(on bool) { set(&p.Gpio2, on, 5) }
func (p *DoublePixel) j1r2(on bool) { set(&p.Gpio0, on, 23) }
func (p *DoublePixel) j1b2(on bool) { set(&p.Gpio0, on, 26) }

func (p *DoublePixel) j3r2(on bool) { set(&p.Gpio1, on, 16) }
func (p *DoublePixel) j3g1(on bool) { set(&p.Gpio1, on, 18) }
func (p *DoublePixel) j3g2(on bool) { set(&p.Gpio0, on, 3) }
func (p *DoublePixel) j3b2(on bool) { set(&p.Gpio0, on, 5) }
func (p *DoublePixel) j3r1(on bool) { set(&p.Gpio0, on, 30) }
func (p *DoublePixel) j3b1(on bool) { set(&p.Gpio0, on, 31) }
