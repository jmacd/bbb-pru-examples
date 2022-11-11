/*
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 512
char readBuf[MAX_BUFFER_SIZE];

#define DEVICE_NAME "/dev/rpmsg_pru30"

int main(void) {
  struct pollfd pollfds[1];
  int i;
  int result = 0;

  /* Open the rpmsg_pru character device file */
  pollfds[0].fd = open(DEVICE_NAME, O_RDWR);

  /*
   * If the RPMsg channel doesn't exist yet the character device
   * won't either.
   * Make sure the PRU firmware is loaded and that the rpmsg_pru
   * module is inserted.
   */
  if (pollfds[0].fd < 0) {
    printf("Failed to open %s\n", DEVICE_NAME);
    return -1;
  }

  /* The RPMsg channel exists and the character device is opened */
  printf("Opened %s, sending\n\n", DEVICE_NAME);

  /* Send 'hello world!' to the PRU through the RPMsg channel */
  result = write(pollfds[0].fd, "hello world!", 13);
  if (result == 0) {
    printf("could not send to PRU\n");
    return -1;
  }

  printf("about to read\n");

  result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
  if (result == 0) {
    printf("could not read from PRU\n");
    return -1;
  }
  uint32_t addr;
  memcpy(&addr, readBuf, 4);
  printf("Message %d received from PRU (%d bytes) %x\n", i, result, addr);

  /* Received all the messages the example is complete */
  printf("Closing %s\n", DEVICE_NAME);

  int fd = open("/dev/mem", O_RDWR, 0);

  printf("FD is %d\n", fd);

  uint32_t vptr = (uint32_t)mmap(NULL, 1 << 23, PROT_READ | PROT_WRITE,
                                 MAP_SHARED, fd, addr);

  printf("Mapped at addr=%x\n", vptr);

  volatile uint32_t *first = (uint32_t *)vptr;
  volatile uint32_t *ptr = first;
  volatile uint32_t *limit = (uint32_t *)(vptr + (1 << 23));

  while (ptr < limit) {
    if (*ptr == 0) {
      continue;
    }
    if (ptr > first) {
      printf("cycles: %d\n", *(ptr + 0) - *(ptr - 2));
      printf("stall: %d\n", *(ptr + 1) - *(ptr - 1));
    }
    ptr += 2;
  }

  /* Close the rpmsg_pru character device file */
  close(pollfds[0].fd);

  return 0;
}
