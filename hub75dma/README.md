## Beaglebone Black PRU driving a HUB75 LED matrix using DMA

The example in this directory was copied from an [ongoing
project](https://github.com/jmacd/nerve).

[Some friends](https://www.lookingup.art/) had a done a Burning Man
project based on [FadeCandy](https://github.com/scanlime/fadecandy) a
few years ago.  The boards were already discontinued by then, but I
remember thinking how much more colorful the FadeCandy was than
whatever I was getting from the Advatek controllers and ws28xx LED
strips.  (And could we please biuld small LED sculptures, [instead of
large ones](https://www.lookingup.art/rainbow-bridge)?)

Then my friend tells me about the BeagleBone black and how I could
probably use the BBB PRU to do temporal dithering.  So, can I?  Here
we are.

This software uses a MIDI controller to input several variables that
control the R, G, and B values of the display as well as select among
three dithering modes.

### Requirements

- Tested using the AM3358 Debian 10.3 2020-04-06 release, kernel 4.19
- Uses an Octoscroller cape (or equivalent)
- Uses two HUB75 32x64 panels on J1 and J3
- Texas Instruments am335x SDK (for PRU C compiler)
- Golang 1.19 (for control rprogram)
- Midi library github.com/thestk/rtmidi
- Alsa library

### Resource table

The PRU program configures a resource table containing:

1. Carveout #1: the framebuffer (8MiB)
2. Carveout #2: the control struct (tiny)
3. RemoteProc Message vdev and vrings
4. Interrupt controller setup.

### 









