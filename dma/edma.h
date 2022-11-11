/*
 * Copyright (C) 2015-2021 Texas Instruments Incorporated - http://www.ti.com/
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

// 1D Transfer Parameters
typedef struct {
  uint32_t src;
  uint32_t dst;
  uint32_t chan;
} hostBuffer;

// EDMA PARAM registers
typedef struct {
  uint32_t sam : 1;
  uint32_t dam : 1;
  uint32_t syncdim : 1;
  uint32_t static_set : 1;
  uint32_t : 4;
  uint32_t fwid : 3;
  uint32_t tccmode : 1;
  uint32_t tcc : 6;
  uint32_t : 2;
  uint32_t tcinten : 1;
  uint32_t itcinten : 1;
  uint32_t tcchen : 1;
  uint32_t itcchen : 1;
  uint32_t privid : 4;
  uint32_t : 3;
  uint32_t priv : 1;
} edmaParamOpt;

typedef struct {
  uint32_t acnt : 16;
  uint32_t bcnt : 16;
} edmaParamABcnt;

typedef struct {
  uint32_t srcbidx : 16;
  uint32_t dstbidx : 16;
} edmaParamBidx;

typedef struct {
  uint32_t link : 16;
  uint32_t bcntrld : 16;
} edmaParamLnkRld;

typedef struct {
  uint32_t srccidx : 16;
  uint32_t dstcidx : 16;
} edmaParamCidx;

typedef struct {
  uint32_t ccnt : 16;
  uint32_t : 16;
} edmaParamCcnt;

typedef struct {
  edmaParamOpt opt;
  uint32_t src;
  edmaParamABcnt abcnt;
  uint32_t dst;
  edmaParamBidx bidx;
  edmaParamLnkRld lnkrld;
  edmaParamCidx cidx;
  edmaParamCcnt ccnt;
} edmaParam;

// Addresses for Constant Table pointer registers
// CTBIR_0 -> C24 (PRU0 DRAM)
// CTBIR_1 -> C25 (PRU1 DRAM)
#define CTBIR_0 (*(volatile uint32_t *)(0x22020))
#define CTBIR_1 (*(volatile uint32_t *)(0x22024))

// EDMA Channel Registers

// CM_PER_BASE is the Clock Module Peripheral base address.
// TODO: TRM links
#define CM_PER_BASE ((volatile uint32_t *)(0x44E00000))

// Third-Party Transfer Controller Clock
// TODO: TRM links
#define TPTC0_CLKCTRL (0x24 / 4)
#define TPCC_CLKCTRL (0xBC / 4)
#define CLK_ENABLED (0x2)

// EDMA constants

#define EDMA_BASE ((volatile uint32_t *)(0x49000000))

// Peripheral Identification Register Section 11.4.1.1
#define EDMA_PID 0

// EDMA3CC Configuration Register Section 11.4.1.2
#define EDMA_CCCFG (0x4 / 4)

// EDMA3CC System Configuration Register Section 15.1.3.2
#define EDMA_SYSCONFIG (0x10 / 4)

// Section 11.4.1.4 DMA Channel Mapping Registers
#define EDMA_DCHMAP_0 (0x100 / 4)  // DMA Channel Mapping Register 0
#define EDMA_DCHMAP_1 (0x104 / 4)  // DMA Channel Mapping Register 1
#define EDMA_DCHMAP_2 (0x108 / 4)  // DMA Channel Mapping Register 2
#define EDMA_DCHMAP_3 (0x10C / 4)  // DMA Channel Mapping Register 3
#define EDMA_DCHMAP_4 (0x110 / 4)  // DMA Channel Mapping Register 4
#define EDMA_DCHMAP_5 (0x114 / 4)  // DMA Channel Mapping Register 5
#define EDMA_DCHMAP_6 (0x118 / 4)  // DMA Channel Mapping Register 6
#define EDMA_DCHMAP_7 (0x11C / 4)  // DMA Channel Mapping Register 7
#define EDMA_DCHMAP_8 (0x120 / 4)  // DMA Channel Mapping Register 8
#define EDMA_DCHMAP_9 (0x124 / 4)  // DMA Channel Mapping Register 9
#define EDMA_DCHMAP_10 (0x128 / 4) // DMA Channel Mapping Register 10
#define EDMA_DCHMAP_11 (0x12C / 4) // DMA Channel Mapping Register 11
#define EDMA_DCHMAP_12 (0x130 / 4) // DMA Channel Mapping Register 12
#define EDMA_DCHMAP_13 (0x134 / 4) // DMA Channel Mapping Register 13
#define EDMA_DCHMAP_14 (0x138 / 4) // DMA Channel Mapping Register 14
#define EDMA_DCHMAP_15 (0x13C / 4) // DMA Channel Mapping Register 15
// ... (48 more)
// 100h to 1FCh DCHMAP_0..63 DMA Channel Mapping Registers 0-63

// Section 11.4.1.5 QDMA Channel Mapping Registers
#define EDMA_QCHMAP_0 (0x200 / 4) // QDMA Channel Mapping Register 0
#define EDMA_QCHMAP_1 (0x204 / 4) // QDMA Channel Mapping Register 1
#define EDMA_QCHMAP_2 (0x208 / 4) // QDMA Channel Mapping Register 2
#define EDMA_QCHMAP_3 (0x20C / 4) // QDMA Channel Mapping Register 3
#define EDMA_QCHMAP_4 (0x210 / 4) // QDMA Channel Mapping Register 4
#define EDMA_QCHMAP_5 (0x214 / 4) // QDMA Channel Mapping Register 5
#define EDMA_QCHMAP_6 (0x218 / 4) // QDMA Channel Mapping Register 6
#define EDMA_QCHMAP_7 (0x21C / 4) // QDMA Channel Mapping Register 7

// Section 11.4.1.6 DMA Queue Number Registers
#define EDMA_DMAQNUM_0 (0x240 / 4) // DMA Queue Number Register 0
#define EDMA_DMAQNUM_1 (0x244 / 4) // DMA Queue Number Register 1
#define EDMA_DMAQNUM_2 (0x248 / 4) // DMA Queue Number Register 2
#define EDMA_DMAQNUM_3 (0x24C / 4) // DMA Queue Number Register 3
#define EDMA_DMAQNUM_4 (0x250 / 4) // DMA Queue Number Register 4
#define EDMA_DMAQNUM_5 (0x254 / 4) // DMA Queue Number Register 5
#define EDMA_DMAQNUM_6 (0x258 / 4) // DMA Queue Number Register 6
#define EDMA_DMAQNUM_7 (0x25C / 4) // DMA Queue Number Register 7

#define EDMA_QDMAQNUM (0x260 / 4) // QDMA Queue Number Register Section 11.4.1.7
#define EDMA_QUEPRI (0x284 / 4)   // Queue Priority Register Section 11.4.1.8
#define EDMA_EMR (0x300 / 4)      // Event Missed Register Section 11.4.1.9
#define EDMA_EMRH (0x304 / 4)     // Event Missed Register High Section 11.4.1.10
#define EDMA_EMCR (0x308 / 4)     // Event Missed Clear Register Section 11.4.1.11
#define EDMA_EMCRH (0x30C / 4)    // Event Missed Clear Register High Section 11.4.1.12
#define EDMA_QEMR (0x310 / 4)     // QDMA Event Missed Register Section 11.4.1.13
#define EDMA_QEMCR (0x314 / 4)    // QDMA Event Missed Clear Register Section 11.4.1.14
#define EDMA_CCERR (0x318 / 4)    // EDMA3CC Error Register Section 11.4.1.15
#define EDMA_CCERRCLR (0x31C / 4) // EDMA3CC Error Clear Register Section 11.4.1.16
#define EDMA_EEVAL (0x320 / 4)    // Error Evaluate Register Section 11.4.1.17
#define EDMA_DRAE0 (0x340 / 4)    // DMA Region Access Enable Register for Region 0 Section 11.4.1.18
#define EDMA_DRAEH0 (0x344 / 4)   // DMA Region Access Enable Register High for Region 0 Section 11.4.1.19
#define EDMA_DRAE1 (0x348 / 4)    // DMA Region Access Enable Register for Region 1 Section 11.4.1.20
#define EDMA_DRAEH1 (0x34C / 4)   // DMA Region Access Enable Register High for Region 1 Section 11.4.1.21
#define EDMA_DRAE2 (0x350 / 4)    // DMA Region Access Enable Register for Region 2 Section 11.4.1.22
#define EDMA_DRAEH2 (0x354 / 4)   // DMA Region Access Enable Register High for Region 2 Section 11.4.1.23
#define EDMA_DRAE3 (0x358 / 4)    // DMA Region Access Enable Register for Region 3 Section 11.4.1.24
#define EDMA_DRAEH3 (0x35C / 4)   // DMA Region Access Enable Register High for Region 3 Section 11.4.1.25
#define EDMA_DRAE4 (0x360 / 4)    // DMA Region Access Enable Register for Region 4 Section 11.4.1.26
#define EDMA_DRAEH4 (0x364 / 4)   // DMA Region Access Enable Register High for Region 4 Section 11.4.1.27
#define EDMA_DRAE5 (0x368 / 4)    // DMA Region Access Enable Register for Region 5 Section 11.4.1.28
#define EDMA_DRAEH5 (0x36C / 4)   // DMA Region Access Enable Register High for Region 5 Section 11.4.1.29
#define EDMA_DRAE6 (0x370 / 4)    // DMA Region Access Enable Register for Region 6 Section 11.4.1.30
#define EDMA_DRAEH6 (0x374 / 4)   // DMA Region Access Enable Register High for Region 6 Section 11.4.1.31
#define EDMA_DRAE7 (0x378 / 4)    // DMA Region Access Enable Register for Region 7 Section 11.4.1.32
#define EDMA_DRAEH7 (0x37C / 4)   // DMA Region Access Enable Register High for Region 7 Section 11.4.1.33

// 380h to 39Ch QRAE_0 to QRAE_7 QDMA Region Access Enable Registers for Region 0-7 Section 11.4.1.34

#define EDMA_Q0E0 (0x400 / 4)  // Event Queue 0 Entry 0 Register Section 11.4.1.35
#define EDMA_Q0E1 (0x404 / 4)  // Event Queue 0 Entry 1 Register Section 11.4.1.36
#define EDMA_Q0E2 (0x408 / 4)  // Event Queue 0 Entry 2 Register Section 11.4.1.37
#define EDMA_Q0E3 (0x40C / 4)  // Event Queue 0 Entry 3 Register Section 11.4.1.38
#define EDMA_Q0E4 (0x410 / 4)  // Event Queue 0 Entry 4 Register Section 11.4.1.39
#define EDMA_Q0E5 (0x414 / 4)  // Event Queue 0 Entry 5 Register Section 11.4.1.40
#define EDMA_Q0E6 (0x418 / 4)  // Event Queue 0 Entry 6 Register Section 11.4.1.41
#define EDMA_Q0E7 (0x41C / 4)  // Event Queue 0 Entry 7 Register Section 11.4.1.42
#define EDMA_Q0E8 (0x420 / 4)  // Event Queue 0 Entry 8 Register Section 11.4.1.43
#define EDMA_Q0E9 (0x424 / 4)  // Event Queue 0 Entry 9 Register Section 11.4.1.44
#define EDMA_Q0E10 (0x428 / 4) // Event Queue 0 Entry 10 Register Section 11.4.1.45
#define EDMA_Q0E11 (0x42C / 4) // Event Queue 0 Entry 11 Register Section 11.4.1.46
#define EDMA_Q0E12 (0x430 / 4) // Event Queue 0 Entry 12 Register Section 11.4.1.47
#define EDMA_Q0E13 (0x434 / 4) // Event Queue 0 Entry 13 Register Section 11.4.1.48
#define EDMA_Q0E14 (0x438 / 4) // Event Queue 0 Entry 14 Register Section 11.4.1.49
#define EDMA_Q0E15 (0x43C / 4) // Event Queue 0 Entry 15 Register Section 11.4.1.50
#define EDMA_Q1E0 (0x440 / 4)  // Event Queue 1 Entry 0 Register Section 11.4.1.51
#define EDMA_Q1E1 (0x444 / 4)  // Event Queue 1 Entry 1 Register Section 11.4.1.52
#define EDMA_Q1E2 (0x448 / 4)  // Event Queue 1 Entry 2 Register Section 11.4.1.53
#define EDMA_Q1E3 (0x44C / 4)  // Event Queue 1 Entry 3 Register Section 11.4.1.54
#define EDMA_Q1E4 (0x450 / 4)  // Event Queue 1 Entry 4 Register Section 11.4.1.55
#define EDMA_Q1E5 (0x454 / 4)  // Event Queue 1 Entry 5 Register Section 11.4.1.56
#define EDMA_Q1E6 (0x458 / 4)  // Event Queue 1 Entry 6 Register Section 11.4.1.57
#define EDMA_Q1E7 (0x45C / 4)  // Event Queue 1 Entry 7 Register Section 11.4.1.58
#define EDMA_Q1E8 (0x460 / 4)  // Event Queue 1 Entry 8 Register Section 11.4.1.59
#define EDMA_Q1E9 (0x464 / 4)  // Event Queue 1 Entry 9 Register Section 11.4.1.60
#define EDMA_Q1E10 (0x468 / 4) // Event Queue 1 Entry 10 Register Section 11.4.1.61
#define EDMA_Q1E11 (0x46C / 4) // Event Queue 1 Entry 11 Register Section 11.4.1.62
#define EDMA_Q1E12 (0x470 / 4) // Event Queue 1 Entry 12 Register Section 11.4.1.63
#define EDMA_Q1E13 (0x474 / 4) // Event Queue 1 Entry 13 Register Section 11.4.1.64
#define EDMA_Q1E14 (0x478 / 4) // Event Queue 1 Entry 14 Register Section 11.4.1.65
#define EDMA_Q1E15 (0x47C / 4) // Event Queue 1 Entry 15 Register Section 11.4.1.66
#define EDMA_Q2E0 (0x480 / 4)  // Event Queue 2 Entry 0 Register Section 11.4.1.67
#define EDMA_Q2E1 (0x484 / 4)  // Event Queue 2 Entry 1 Register Section 11.4.1.68
#define EDMA_Q2E2 (0x488 / 4)  // Event Queue 2 Entry 2 Register Section 11.4.1.69
#define EDMA_Q2E3 (0x48C / 4)  // Event Queue 2 Entry 3 Register Section 11.4.1.70
#define EDMA_Q2E4 (0x490 / 4)  // Event Queue 2 Entry 4 Register Section 11.4.1.71
#define EDMA_Q2E5 (0x494 / 4)  // Event Queue 2 Entry 5 Register Section 11.4.1.72
#define EDMA_Q2E6 (0x498 / 4)  // Event Queue 2 Entry 6 Register Section 11.4.1.73
#define EDMA_Q2E7 (0x49C / 4)  // Event Queue 2 Entry 7 Register Section 11.4.1.74
#define EDMA_Q2E8 (0x4A0 / 4)  // Event Queue 2 Entry 8 Register Section 11.4.1.75
#define EDMA_Q2E9 (0x4A4 / 4)  // Event Queue 2 Entry 9 Register Section 11.4.1.76
#define EDMA_Q2E10 (0x4A8 / 4) // Event Queue 2 Entry 10 Register Section 11.4.1.77
#define EDMA_Q2E11 (0x4AC / 4) // Event Queue 2 Entry 11 Register Section 11.4.1.78
#define EDMA_Q2E12 (0x4B0 / 4) // Event Queue 2 Entry 12 Register Section 11.4.1.79
#define EDMA_Q2E13 (0x4B4 / 4) // Event Queue 2 Entry 13 Register Section 11.4.1.80
#define EDMA_Q2E14 (0x4B8 / 4) // Event Queue 2 Entry 14 Register Section 11.4.1.81
#define EDMA_Q2E15 (0x4BC / 4) // Event Queue 2 Entry 15 Register Section 11.4.1.82

// Section 11.4.1.83 Queue Status Registers
#define EDMA_QSTAT_0 (0x600 / 4) // Queue Status Register 0
#define EDMA_QSTAT_1 (0x604 / 4) // Queue Status Register 1
#define EDMA_QSTAT_2 (0x608 / 4) // Queue Status Register 2

#define EDMA_QWMTHRA (0x620 / 4) // Queue Watermark Threshold A Register Section 11.4.1.84
#define EDMA_CCSTAT (0x640 / 4)  // EDMA3CC Status Register Section 11.4.1.85
#define EDMA_MPFAR (0x800 / 4)   // Memory Protection Fault Address Register Section 11.4.1.86
#define EDMA_MPFSR (0x804 / 4)   // Memory Protection Fault Status Register Section 11.4.1.87
#define EDMA_MPFCR (0x808 / 4)   // Memory Protection Fault Command Register Section 11.4.1.88
#define EDMA_MPPAG (0x80C / 4)   // Memory Protection Page Attribute Register Global Section 11.4.1.89

// 810h to 82Ch MPPA_0 to MPPA_7 Memory Protection Page Attribute Registers Section 11.4.1.90

#define EDMA_ER (0x1000 / 4)    // Event Register Section 11.4.1.91
#define EDMA_ERH (0x1004 / 4)   // Event Register High Section 11.4.1.92
#define EDMA_ECR (0x1008 / 4)   // Event Clear Register Section 11.4.1.93
#define EDMA_ECRH (0x100C / 4)  // Event Clear Register High Section 11.4.1.94
#define EDMA_ESR (0x1010 / 4)   // Event Set Register Section 11.4.1.95
#define EDMA_ESRH (0x1014 / 4)  // Event Set Register High Section 11.4.1.96
#define EDMA_CER (0x1018 / 4)   // Chained Event Register Section 11.4.1.97
#define EDMA_CERH (0x101C / 4)  // Chained Event Register High Section 11.4.1.98
#define EDMA_EER (0x1020 / 4)   // Event Enable Register Section 11.4.1.99
#define EDMA_EERH (0x1024 / 4)  // Event Enable Register High Section 11.4.1.100
#define EDMA_EECR (0x1028 / 4)  // Event Enable Clear Register Section 11.4.1.101
#define EDMA_EECRH (0x102C / 4) // Event Enable Clear Register High Section 11.4.1.102
#define EDMA_EESR (0x1030 / 4)  // Event Enable Set Register Section 11.4.1.103
#define EDMA_EESRH (0x1034 / 4) // Event Enable Set Register High Section 11.4.1.104
#define EDMA_SER (0x1038 / 4)   // Secondary Event Register Section 11.4.1.105
#define EDMA_SERH (0x103C / 4)  // Secondary Event Register High Section 11.4.1.106
#define EDMA_SECR (0x1040 / 4)  // Secondary Event Clear Register Section 11.4.1.107
#define EDMA_SECRH (0x1044 / 4) // Secondary Event Clear Register High Section 11.4.1.108
#define EDMA_IER (0x1050 / 4)   // Interrupt Enable Register Section 11.4.1.109
#define EDMA_IERH (0x1054 / 4)  // Interrupt Enable Register High Section 11.4.1.110
#define EDMA_IECR (0x1058 / 4)  // Interrupt Enable Clear Register Section 11.4.1.111
#define EDMA_IECRH (0x105C / 4) // Interrupt Enable Clear Register High Section 11.4.1.112
#define EDMA_IESR (0x1060 / 4)  // Interrupt Enable Set Register Section 11.4.1.113
#define EDMA_IESRH (0x1064 / 4) // Interrupt Enable Set Register High Section 11.4.1.114
#define EDMA_IPR (0x1068 / 4)   // Interrupt Pending Register Section 11.4.1.115
#define EDMA_IPRH (0x106C / 4)  // Interrupt Pending Register High Section 11.4.1.116
#define EDMA_ICR (0x1070 / 4)   // Interrupt Clear Register Section 11.4.1.117
#define EDMA_ICRH (0x1074 / 4)  // Interrupt Clear Register High Section 11.4.1.118
#define EDMA_IEVAL (0x1078 / 4) // Interrupt Evaluate Register Section 11.4.1.119
#define EDMA_QER (0x1080 / 4)   // QDMA Event Register Section 11.4.1.120
#define EDMA_QEER (0x1084 / 4)  // QDMA Event Enable Register Section 11.4.1.121
#define EDMA_QEECR (0x1088 / 4) // QDMA Event Enable Clear Register Section 11.4.1.122
#define EDMA_QEESR (0x108C / 4) // QDMA Event Enable Set Register Section 11.4.1.123
#define EDMA_QSER (0x1090 / 4)  // QDMA Secondary Event Register Section 11.4.1.124
#define EDMA_QSECR (0x1094 / 4) // QDMA Secondary Event Clear Register Section 11.4.1.125

#define ACNT 0x100
#define BCNT 0x1
#define CCNT 0x1

// (TRM 11.3.3.1)
#define EDMA_PARAM_OFFSET (0x4000 / 4)
#define EDMA_PARAM_SIZE sizeof(edmaParam) // 32 bytes

// Note 0x4A300000 is the base of the PRU_ICSS range inside the L4
// Fast Peripheral memory map.  Example supposes this is set from the
// host?  This address falls into the 12KB PRU shared memory area.
// hostData.src = /*buf.src*/ 0x4A310000; // PRU Shared memory
// hostData.dst = /*buf.dst*/ 0x4A310100; // PRU Shared memory
// hostData.chan = /*buf.chan*/ 10;       // DMA Channel number

// Design note:
// EDMA system event 0 and 1 correspond with pr1_host[7] and pr1_host[6]
// and pr1_host[0:7] maps to channels 2-9 on the PRU.
// => EDMA event 0 == PRU channel 9
// => EDMA event 1 == PRU channel 8
#define DMA_CHANNEL 0

// bit 5 is the valid strobe to generate system events with __R31
#define R31_INTERRUPT_ENABLE (1 << 5)
#define R31_INTERRUPT_OFFSET 16

// DMA completion interrupt use tpcc_int_pend_po1

void setupEDMA(void) {
  int dmaChannel = DMA_CHANNEL;

  uint32_t dmaChannelMask;
  uint16_t paramOffset;
  edmaParam params;
  volatile edmaParam *pParams;

  // Enable the EDMA (Transfer controller, Channel controller) clocks.
  CM_PER_BASE[TPTC0_CLKCTRL] = CLK_ENABLED;
  CM_PER_BASE[TPCC_CLKCTRL] = CLK_ENABLED;

  dmaChannelMask = (1 << dmaChannel);

  // Map Channel 0 to PaRAM 0
  // DCHMAP_0 == DMA Channel 0 mapping to PaRAM set number 0.
  EDMA_BASE[EDMA_DCHMAP_0] = dmaChannel;

  // Setup EDMA region access for Shadow Region 1
  // DRAE1 == DMA Region Access Enable shadow region 1.
  EDMA_BASE[EDMA_DRAE1] |= dmaChannelMask;

  // Setup channel to submit to EDMA TC0. Note DMAQNUM1 is for
  // channels 8-15, the 0 in 0xfffff0ff corresponds with "E2" of
  // DMAQNUM0 (TRM 11.4.1.6) which is offset by 8 for DMAQNUM1, so DMA
  // event 10 maps to TC0.
  // EDMA_BASE[EDMA_DMAQNUM_1] &= 0xFFFFF0FF;
  // Channel 0 maps to queue 0
  EDMA_BASE[EDMA_DMAQNUM_0] &= 0xFFFFFFF0;

  /* Clear interrupt and secondary event registers */
  EDMA_BASE[EDMA_SECR] |= dmaChannelMask;
  EDMA_BASE[EDMA_ICR] |= dmaChannelMask;

  /* Enable channel interrupt */
  EDMA_BASE[EDMA_IESR] |= dmaChannelMask;

  // Enable channel for an event trigger.
  EDMA_BASE[EDMA_EESR] |= dmaChannelMask;

  /* Clear event missed register */
  EDMA_BASE[EDMA_EMCR] |= dmaChannelMask;

  /* Setup and store PaRAM set for transfer */
  paramOffset = EDMA_PARAM_OFFSET;

  paramOffset += ((dmaChannel << 5) / 4);

  params.lnkrld.link = 0xFFFF;
  params.lnkrld.bcntrld = 0x0000;
  params.opt.tcc = dmaChannel;
  params.opt.tcinten = 1;
  params.opt.itcchen = 1;

  params.ccnt.ccnt = CCNT;
  params.abcnt.acnt = ACNT;
  params.abcnt.bcnt = BCNT;
  params.bidx.srcbidx = 0x1;
  params.bidx.dstbidx = 0x1;
  params.src = 0x4A310000;
  params.dst = 0x4A310100;

  pParams = (volatile edmaParam *)(EDMA_BASE + paramOffset);
  *pParams = params;

  uint32_t *ptr = (uint32_t *)0x00010000;
  *ptr = 0xDEADBEEF;

  uint32_t *dest = (uint32_t *)0x00010100;
  *dest = 0;

  if (*dest == 0) {
    uled1(HI);
  }

  /* Trigger transfer (Manual) */
  // EDMA_BASE[EDMA_ESR] = (dmaChannelMask);

  // Trigger transfer.  (4.4.1.2.2 Event Interface Mapping)
  // This is pr1_pru_mst_intr[2]_intr_req, system event 18

  __R31 = R31_INTERRUPT_ENABLE | (SYSEVT_PRU_TO_EDMA - R31_INTERRUPT_OFFSET);

  /* Wait for transfer completion */
  while (!(EDMA_BASE[EDMA_IPR] & dmaChannelMask)) {
  }

  if (*dest == 0xDEADBEEF) {
    uled2(HI);
  }
}
