Beaglbone Black PRU example code is hard to find!

I had read through [Mark Yoder's PRU
Cookbook](https://github.com/MarkAYoder/PRUCookbook) but couldn't find
what I needed.

Much of the documentation is written for the "UIO" library, which gave
user-level access to the PRU.  However, Texas Instruments never liked
UIO and instead we have RemoteProc.  The examples here use RemoteProc.

Caution this is a work in progress.  I built and tested these examples
using the AM3358 Debian 10.3 2020-04-06 build using the 4.19 kernel.

1. [Driving HUB75 LED matrix from the PRU using a DMA program.](./hub75dma)
