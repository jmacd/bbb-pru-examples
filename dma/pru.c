#include <pru_rpmsg.h>
#include <pru_virtio_ids.h>
#include <rsc_types.h>

#include <am335x/pru_cfg.h>
#include <am335x/pru_ctrl.h>
#include <am335x/pru_intc.h>

volatile register uint32_t __R30; /* output register for PRU */
volatile register uint32_t __R31; /* input/interrupt register for PRU */

// Set in resourceTable.rpmsg_vdev.status when the kernel is ready.
#define VIRTIO_CONFIG_S_DRIVER_OK ((uint32_t)1 << 2)

// Sizes of the virtqueues (expressed in number of buffers supported,
// and must be power of 2)
#define PRU_RPMSG_VQ0_SIZE 16
#define PRU_RPMSG_VQ1_SIZE 16

// The feature bitmap for virtio rpmsg
#define VIRTIO_RPMSG_F_NS 0 // name service notifications

// This firmware supports name service notifications as one of its features.
#define RPMSG_PRU_C0_FEATURES (1 << VIRTIO_RPMSG_F_NS)

// sysevt 16 == pr1_pru_mst_intr[0]_intr_req
#define SYSEVT_PRU_TO_ARM 16

// sysevt 17 == pr1_pru_mst_intr[1]_intr_req
#define SYSEVT_ARM_TO_PRU 17

// sysevt 18 == pr1_pru_mst_intr[2]_intr_req
#define SYSEVT_PRU_TO_EDMA 18

// sysevt 63 == tpcc_int_pend_po1
#define SYSEVT_EDMA_TO_PRU 63

// Chanel 2 is the first (of 8) PRU interrupt output channels.
#define HOST_INTERRUPT_CHANNEL_PRU_TO_ARM 2

// Channel 0 is the first (of 2) PRU interrupt input channels.
#define HOST_INTERRUPT_CHANNEL_ARM_TO_PRU 0

// Chanel 9 is the last (of 8) PRU interrupt output channels,
// a.k.a. pr1_host[7] maps to DMA channel 0 (see TRM 11.3.20).
#define HOST_INTERRUPT_CHANNEL_PRU_TO_EDMA 9

// Channel 1 is the second (of 2) PRU interrupt input channels.
#define HOST_INTERRUPT_CHANNEL_EDMA_TO_PRU 1

// Interrupt inputs set bits 30 and 31 in register R31.
#define PRU_R31_INTERRUPT_FROM_ARM ((uint32_t)1 << 30)  // Fixed, equals channel 0
#define PRU_R31_INTERRUPT_FROM_EDMA ((uint32_t)1 << 31) // Fixed, equals channel 1

// (From the internet!)
#define offsetof(st, m) ((uint32_t) & (((st *)0)->m))

#define SET GPIO_SETDATAOUT
#define CLEAR GPIO_CLEARDATAOUT

// Mapping sysevts to a channel. Each pair contains a sysevt, channel.
struct ch_map pru_intc_map[] = {
    {SYSEVT_PRU_TO_ARM, HOST_INTERRUPT_CHANNEL_PRU_TO_ARM},
    {SYSEVT_ARM_TO_PRU, HOST_INTERRUPT_CHANNEL_ARM_TO_PRU},

    {SYSEVT_EDMA_TO_PRU, HOST_INTERRUPT_CHANNEL_EDMA_TO_PRU},
    {SYSEVT_PRU_TO_EDMA, HOST_INTERRUPT_CHANNEL_PRU_TO_EDMA},
};

/* Definition for unused interrupts */
#define HOST_UNUSED 255

struct my_resource_table {
  struct resource_table base;

  uint32_t offset[3]; /* Should match 'num' in actual definition */

  struct fw_rsc_carveout carveout;

  struct fw_rsc_vdev rpmsg_vdev;
  struct fw_rsc_vdev_vring rpmsg_vring0;
  struct fw_rsc_vdev_vring rpmsg_vring1;

  struct fw_rsc_custom pru_ints;
};

#pragma DATA_SECTION(resourceTable, ".resource_table")
#pragma RETAIN(resourceTable)
struct my_resource_table resourceTable = {
    {
        1,    /* Resource table version: only version 1 is supported by the current
                 driver */
        3,    /* number of entries in the table */
        0, 0, /* reserved, must be zero */
    },
    /* offsets to entries */
    {
        offsetof(struct my_resource_table, carveout),
        offsetof(struct my_resource_table, rpmsg_vdev),
        offsetof(struct my_resource_table, pru_ints),
    },
    /* carveout */
    {
        (uint32_t)TYPE_CARVEOUT, /* type */
        (uint32_t)0,             /* da */
        (uint32_t)0,             /* pa */
        (uint32_t)1 << 23,       /* len (8MB) */
        (uint32_t)0,             /* flags */
        (uint32_t)0,             /* reserved */
        "framebufs",
    },

    /* rpmsg vdev entry */
    {
        (uint32_t)TYPE_VDEV,             // type
        (uint32_t)VIRTIO_ID_RPMSG,       // id
        (uint32_t)0,                     // notifyid
        (uint32_t)RPMSG_PRU_C0_FEATURES, // dfeatures
        (uint32_t)0,                     // gfeatures
        (uint32_t)0,                     // config_len
        (uint8_t)0,                      // status
        (uint8_t)2,                      // num_of_vrings, only two is supported
        {(uint8_t)0, (uint8_t)0},        // reserved
                                         /* no config data */
    },
    /* the two vrings */
    {
        0,                  // da, will be populated by host, can't pass it in
        16,                 // align (bytes),
        PRU_RPMSG_VQ0_SIZE, // num of descriptors
        0,                  // notifyid, will be populated, can't pass right now
        0                   // reserved
    },
    {
        0,                  // da, will be populated by host, can't pass it in
        16,                 // align (bytes),
        PRU_RPMSG_VQ1_SIZE, // num of descriptors
        0,                  // notifyid, will be populated, can't pass right now
        0                   // reserved
    },

    {
        TYPE_CUSTOM,
        TYPE_PRU_INTS,
        sizeof(struct fw_rsc_custom_ints),
        {
            /* PRU_INTS version */
            PRU_INTS_VER0,

            // See TRM 4.4.2.1.  Two interrupt input channels.
            HOST_INTERRUPT_CHANNEL_ARM_TO_PRU,
            HOST_INTERRUPT_CHANNEL_EDMA_TO_PRU,

            // Two used output interrupt channels.
            HOST_INTERRUPT_CHANNEL_PRU_TO_ARM,

            // Six unused interrupt output channels.
            HOST_UNUSED,
            HOST_UNUSED,
            HOST_UNUSED,
            HOST_UNUSED,
            HOST_UNUSED,
            HOST_UNUSED,

            HOST_INTERRUPT_CHANNEL_PRU_TO_EDMA,

            // Number of evts being mapped to channels.
            (sizeof(pru_intc_map) / sizeof(struct ch_map)),

            // The structure containing mapped events.
            pru_intc_map,
        },
    },
};

/*
 * Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
 * at linux-x.y.z/drivers/rpmsg/rpmsg_pru.c
 */
#define CHAN_NAME "rpmsg-pru"
#define CHAN_DESC "Channel 30"
#define CHAN_PORT 30

char payload[RPMSG_BUF_SIZE];

// Set up the pointers to each of the GPIO ports
uint32_t *gpio0 = (uint32_t *)0x44e07000; // GPIO Bank 0  See Table 2.2 of TRM
uint32_t *gpio1 = (uint32_t *)0x4804c000; // GPIO Bank 1
uint32_t *gpio2 = (uint32_t *)0x481ac000; // GPIO Bank 2
uint32_t *gpio3 = (uint32_t *)0x481ae000; // GPIO Bank 3

#define GPIO_CLEARDATAOUT (0x190 / 4) // For clearing the GPIO registers
#define GPIO_SETDATAOUT (0x194 / 4)   // For setting the GPIO registers

// Delay in cycles
#define DELAY 0 // 1000

void sleep();
void setRow(uint32_t row);
void testPix();
void toggleClock();
void setPix(uint32_t cycle, uint32_t pix);
void latchRows();

#define HI 1
#define LO 0

void set(uint32_t *gpio, int bit, int on) {
  if (on) {
    gpio[SET] = 1 << bit;
  } else {
    gpio[CLEAR] = 1 << bit;
  }
}

void uled1(int val) { set(gpio1, 21, val); }
void uled2(int val) { set(gpio1, 22, val); }
void uled3(int val) { set(gpio1, 23, val); }
void uled4(int val) { set(gpio1, 24, val); }

void clock(int val) { set(gpio1, 19, val); }
void latch(int val) { set(gpio1, 29, val); }
void outputEnable(int val) { set(gpio1, 28, val); }

void selA(int val) { set(gpio1, 12, val); }
void selB(int val) { set(gpio1, 13, val); }
void selC(int val) { set(gpio1, 14, val); }
void selD(int val) { set(gpio1, 15, val); }

void sleep() {
  // __delay_cycles(DELAY);
}

void setRow(uint32_t on) {
  // 0xf because 4 address lines.  If this were a x64 panel (1/32
  // scan) use 0x1f for 5 address lines.
  uint32_t off = on ^ 0xf;

  // Selector bits start at position 12 in gpio1
  gpio1[SET] = on << 12;
  gpio1[CLEAR] = off << 12;
}

void toggleClock() {
  sleep();
  clock(HI);
  sleep();
  clock(LO);
}

void latchRows() {
  outputEnable(HI);
  sleep();
  latch(HI);
  sleep();
  latch(LO);
  sleep();
  outputEnable(LO);
  sleep();
}

// J1
// gp2 |= (1U << 2);  // J1 r1 (P8-gp2)
// 07 |= (1U << 3);  // J1 g1 (P8-08)
// gp2 |= (1U << 5);  // J1 b1 (P8-09)
// gp0 |= (1U << 23); // J1 r2 (P8-13)
// gp2 |= (1U << 4);  // J1 g2 (P8-10)
// gp0 |= (1U << 26); // J1 b2 (P8-14)

// J3
// gp0 |= 1U << 30; // r1 (P9-11)
// gp1 |= 1U << 18; // g1 (P9-14)
// gp0 |= 1U << 31; // b1 (P9-13)
// gp1 |= 1U << 16; // r2 (P9-15)
// gp0 |= 1U << 3;  // g2 (P9-21)
// gp0 |= 1U << 5;  // b2 (P9-17)

const uint32_t j13_all_g0 = 1U << 23 | 1U << 26 | 1U << 30 | 1U << 31 | 1U << 3 | 1U << 5;
const uint32_t j13_all_g1 = 1U << 18 | 1U << 16;
const uint32_t j13_all_g2 = 1U << 4 | 1U << 2 | 1U << 3 | 1U << 5;

void setPix(uint32_t cycle, uint32_t pix) {
  // Using fpp/capes/bbb/panels/Octoscroller.json as a reference.

  int op;
  // // if (cycle > pix) {
  // if ((cycle % 32) >= 16) {
  op = SET;
  // } else {
  //   op = CLEAR;
  // }

  gpio0[op] = j13_all_g0;
  gpio1[op] = j13_all_g1;
  gpio2[op] = j13_all_g2;
  //  gpio3[op] = 0;
}

#include "edma.h"

void main(void) {
  struct pru_rpmsg_transport transport;
  uint16_t src, dst, len;
  volatile uint8_t *status;

  // Allow OCP master port access by the PRU so the PRU can read external
  // memories */
  CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

  // Enable PRU0 cycle counter.
  PRU0_CTRL.CTRL_bit.CTR_EN = 1;

  // Clear the system event mapped to the two input interrupts.
  CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_ARM_TO_PRU;
  CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_EDMA_TO_PRU;

  // Make sure the Linux drivers are ready for RPMsg communication
  status = &resourceTable.rpmsg_vdev.status;
  while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK)) {
  }

  // Initialize the RPMsg transport structure
  pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, SYSEVT_PRU_TO_ARM,
                 SYSEVT_ARM_TO_PRU);

  // Create the RPMsg channel between the PRU and ARM user
  // space using the *transport structure.
  while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS) {
  }

  uled1(LO);
  uled2(LO);
  uled3(LO);
  uled4(LO);

  // @@@ Test!
  setupEDMA();

  // uled1(HI);
  // uled2(HI);
  // uled3(HI);
  // uled4(HI);

  while (1) {
    // Check bit 31 of register R31 to see if the ARM has kicked us
    if (__R31 & PRU_R31_INTERRUPT_FROM_ARM) {

      // Clear the event status *\/
      CT_INTC.SICR_bit.STS_CLR_IDX = SYSEVT_ARM_TO_PRU;

      // Receive all available messages.
      if (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) == PRU_RPMSG_SUCCESS) {
        break;
      }
    }
  }

  // Send the carveout address to the ARM program.
  memcpy(payload, &resourceTable.carveout.pa, 4);
  while (pru_rpmsg_send(&transport, dst, src, payload, 4) != PRU_RPMSG_SUCCESS) {
  }

  // Initialize the carveout (testing)
  uint32_t *start = (uint32_t *)resourceTable.carveout.pa;
  uint32_t *limit = (uint32_t *)(resourceTable.carveout.pa + (1 << 23));

  // Begin display loop
  uint32_t pix, row, cycle;

  for (cycle = 0; 1; cycle++) {
    cycle %= 64;
    for (row = 0; row < 16; row++) {
      setRow(row);

      for (pix = 0; pix < 64; pix++) {
        setPix(cycle, pix);
        toggleClock();
      }

      latchRows();
    }
    if (start < limit) {
      *start++ = PRU0_CTRL.CYCLE;
      *start++ = PRU0_CTRL.STALL;
    }
  }
}
