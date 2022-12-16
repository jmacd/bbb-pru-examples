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

// CM_PER_BASE is the Clock Module Peripheral base address.
// TRM 8.1.12.1
#define CM_PER_BASE ((volatile uint32_t *)(0x44E00000))

// Third-Party Transfer Controller Clock
// TRM 8.1.12.1
#define CM_PER_TPTC0_CLKCTRL (0x24 / WORDSZ)
#define CM_PER_TPCC_CLKCTRL (0xBC / WORDSZ)
#define CM_PER_CLK_ENABLED (0x2)

// EDMA constants

#define EDMA_BASE ((volatile uint32_t *)(0x49000000))

// Peripheral Identification Register Section 11.4.1.1
#define EDMA_PID 0

// EDMA3CC Configuration Register Section 11.4.1.2
#define EDMA_CCCFG (0x4 / WORDSZ)

// EDMA3CC System Configuration Register Section 15.1.3.2
#define EDMA_SYSCONFIG (0x10 / WORDSZ)

// Section 11.4.1.4 DMA Channel Mapping Registers
#define EDMA_DCHMAP_0 (0x100 / WORDSZ)  // DMA Channel Mapping Register 0
#define EDMA_DCHMAP_1 (0x104 / WORDSZ)  // DMA Channel Mapping Register 1
#define EDMA_DCHMAP_2 (0x108 / WORDSZ)  // DMA Channel Mapping Register 2
#define EDMA_DCHMAP_3 (0x10C / WORDSZ)  // DMA Channel Mapping Register 3
#define EDMA_DCHMAP_4 (0x110 / WORDSZ)  // DMA Channel Mapping Register 4
#define EDMA_DCHMAP_5 (0x114 / WORDSZ)  // DMA Channel Mapping Register 5
#define EDMA_DCHMAP_6 (0x118 / WORDSZ)  // DMA Channel Mapping Register 6
#define EDMA_DCHMAP_7 (0x11C / WORDSZ)  // DMA Channel Mapping Register 7
#define EDMA_DCHMAP_8 (0x120 / WORDSZ)  // DMA Channel Mapping Register 8
#define EDMA_DCHMAP_9 (0x124 / WORDSZ)  // DMA Channel Mapping Register 9
#define EDMA_DCHMAP_10 (0x128 / WORDSZ) // DMA Channel Mapping Register 10
#define EDMA_DCHMAP_11 (0x12C / WORDSZ) // DMA Channel Mapping Register 11
#define EDMA_DCHMAP_12 (0x130 / WORDSZ) // DMA Channel Mapping Register 12
#define EDMA_DCHMAP_13 (0x134 / WORDSZ) // DMA Channel Mapping Register 13
#define EDMA_DCHMAP_14 (0x138 / WORDSZ) // DMA Channel Mapping Register 14
#define EDMA_DCHMAP_15 (0x13C / WORDSZ) // DMA Channel Mapping Register 15
// ... (48 more)
// 100h to 1FCh DCHMAP_0..63 DMA Channel Mapping Registers 0-63

// Section 11.4.1.5 QDMA Channel Mapping Registers
#define EDMA_QCHMAP_0 (0x200 / WORDSZ) // QDMA Channel Mapping Register 0
#define EDMA_QCHMAP_1 (0x204 / WORDSZ) // QDMA Channel Mapping Register 1
#define EDMA_QCHMAP_2 (0x208 / WORDSZ) // QDMA Channel Mapping Register 2
#define EDMA_QCHMAP_3 (0x20C / WORDSZ) // QDMA Channel Mapping Register 3
#define EDMA_QCHMAP_4 (0x210 / WORDSZ) // QDMA Channel Mapping Register 4
#define EDMA_QCHMAP_5 (0x214 / WORDSZ) // QDMA Channel Mapping Register 5
#define EDMA_QCHMAP_6 (0x218 / WORDSZ) // QDMA Channel Mapping Register 6
#define EDMA_QCHMAP_7 (0x21C / WORDSZ) // QDMA Channel Mapping Register 7

// Section 11.4.1.6 DMA Queue Number Registers
#define EDMA_DMAQNUM_0 (0x240 / WORDSZ) // DMA Queue Number Register 0
#define EDMA_DMAQNUM_1 (0x244 / WORDSZ) // DMA Queue Number Register 1
#define EDMA_DMAQNUM_2 (0x248 / WORDSZ) // DMA Queue Number Register 2
#define EDMA_DMAQNUM_3 (0x24C / WORDSZ) // DMA Queue Number Register 3
#define EDMA_DMAQNUM_4 (0x250 / WORDSZ) // DMA Queue Number Register 4
#define EDMA_DMAQNUM_5 (0x254 / WORDSZ) // DMA Queue Number Register 5
#define EDMA_DMAQNUM_6 (0x258 / WORDSZ) // DMA Queue Number Register 6
#define EDMA_DMAQNUM_7 (0x25C / WORDSZ) // DMA Queue Number Register 7

#define EDMA_QDMAQNUM (0x260 / WORDSZ) // QDMA Queue Number Register Section 11.4.1.7
#define EDMA_QUEPRI (0x284 / WORDSZ)   // Queue Priority Register Section 11.4.1.8
#define EDMA_EMR (0x300 / WORDSZ)      // Event Missed Register Section 11.4.1.9
#define EDMA_EMRH (0x304 / WORDSZ)     // Event Missed Register High Section 11.4.1.10
#define EDMA_EMCR (0x308 / WORDSZ)     // Event Missed Clear Register Section 11.4.1.11
#define EDMA_EMCRH (0x30C / WORDSZ)    // Event Missed Clear Register High Section 11.4.1.12
#define EDMA_QEMR (0x310 / WORDSZ)     // QDMA Event Missed Register Section 11.4.1.13
#define EDMA_QEMCR (0x314 / WORDSZ)    // QDMA Event Missed Clear Register Section 11.4.1.14
#define EDMA_CCERR (0x318 / WORDSZ)    // EDMA3CC Error Register Section 11.4.1.15
#define EDMA_CCERRCLR (0x31C / WORDSZ) // EDMA3CC Error Clear Register Section 11.4.1.16

#define EDMA_DRAE0 (0x340 / WORDSZ)  // DMA Region Access Enable Register for Region 0 Section 11.4.1.18
#define EDMA_DRAEH0 (0x344 / WORDSZ) // DMA Region Access Enable Register High for Region 0 Section 11.4.1.19
#define EDMA_DRAE1 (0x348 / WORDSZ)  // DMA Region Access Enable Register for Region 1 Section 11.4.1.20
#define EDMA_DRAEH1 (0x34C / WORDSZ) // DMA Region Access Enable Register High for Region 1 Section 11.4.1.21
#define EDMA_DRAE2 (0x350 / WORDSZ)  // DMA Region Access Enable Register for Region 2 Section 11.4.1.22
#define EDMA_DRAEH2 (0x354 / WORDSZ) // DMA Region Access Enable Register High for Region 2 Section 11.4.1.23
#define EDMA_DRAE3 (0x358 / WORDSZ)  // DMA Region Access Enable Register for Region 3 Section 11.4.1.24
#define EDMA_DRAEH3 (0x35C / WORDSZ) // DMA Region Access Enable Register High for Region 3 Section 11.4.1.25
#define EDMA_DRAE4 (0x360 / WORDSZ)  // DMA Region Access Enable Register for Region 4 Section 11.4.1.26
#define EDMA_DRAEH4 (0x364 / WORDSZ) // DMA Region Access Enable Register High for Region 4 Section 11.4.1.27
#define EDMA_DRAE5 (0x368 / WORDSZ)  // DMA Region Access Enable Register for Region 5 Section 11.4.1.28
#define EDMA_DRAEH5 (0x36C / WORDSZ) // DMA Region Access Enable Register High for Region 5 Section 11.4.1.29
#define EDMA_DRAE6 (0x370 / WORDSZ)  // DMA Region Access Enable Register for Region 6 Section 11.4.1.30
#define EDMA_DRAEH6 (0x374 / WORDSZ) // DMA Region Access Enable Register High for Region 6 Section 11.4.1.31
#define EDMA_DRAE7 (0x378 / WORDSZ)  // DMA Region Access Enable Register for Region 7 Section 11.4.1.32
#define EDMA_DRAEH7 (0x37C / WORDSZ) // DMA Region Access Enable Register High for Region 7 Section 11.4.1.33

// 380h to 39Ch QRAE_0 to QRAE_7 QDMA Region Access Enable Registers for Region 0-7 Section 11.4.1.34

#define EDMA_Q0E0 (0x400 / WORDSZ)  // Event Queue 0 Entry 0 Register Section 11.4.1.35
#define EDMA_Q0E1 (0x404 / WORDSZ)  // Event Queue 0 Entry 1 Register Section 11.4.1.36
#define EDMA_Q0E2 (0x408 / WORDSZ)  // Event Queue 0 Entry 2 Register Section 11.4.1.37
#define EDMA_Q0E3 (0x40C / WORDSZ)  // Event Queue 0 Entry 3 Register Section 11.4.1.38
#define EDMA_Q0E4 (0x410 / WORDSZ)  // Event Queue 0 Entry 4 Register Section 11.4.1.39
#define EDMA_Q0E5 (0x414 / WORDSZ)  // Event Queue 0 Entry 5 Register Section 11.4.1.40
#define EDMA_Q0E6 (0x418 / WORDSZ)  // Event Queue 0 Entry 6 Register Section 11.4.1.41
#define EDMA_Q0E7 (0x41C / WORDSZ)  // Event Queue 0 Entry 7 Register Section 11.4.1.42
#define EDMA_Q0E8 (0x420 / WORDSZ)  // Event Queue 0 Entry 8 Register Section 11.4.1.43
#define EDMA_Q0E9 (0x424 / WORDSZ)  // Event Queue 0 Entry 9 Register Section 11.4.1.44
#define EDMA_Q0E10 (0x428 / WORDSZ) // Event Queue 0 Entry 10 Register Section 11.4.1.45
#define EDMA_Q0E11 (0x42C / WORDSZ) // Event Queue 0 Entry 11 Register Section 11.4.1.46
#define EDMA_Q0E12 (0x430 / WORDSZ) // Event Queue 0 Entry 12 Register Section 11.4.1.47
#define EDMA_Q0E13 (0x434 / WORDSZ) // Event Queue 0 Entry 13 Register Section 11.4.1.48
#define EDMA_Q0E14 (0x438 / WORDSZ) // Event Queue 0 Entry 14 Register Section 11.4.1.49
#define EDMA_Q0E15 (0x43C / WORDSZ) // Event Queue 0 Entry 15 Register Section 11.4.1.50
#define EDMA_Q1E0 (0x440 / WORDSZ)  // Event Queue 1 Entry 0 Register Section 11.4.1.51
#define EDMA_Q1E1 (0x444 / WORDSZ)  // Event Queue 1 Entry 1 Register Section 11.4.1.52
#define EDMA_Q1E2 (0x448 / WORDSZ)  // Event Queue 1 Entry 2 Register Section 11.4.1.53
#define EDMA_Q1E3 (0x44C / WORDSZ)  // Event Queue 1 Entry 3 Register Section 11.4.1.54
#define EDMA_Q1E4 (0x450 / WORDSZ)  // Event Queue 1 Entry 4 Register Section 11.4.1.55
#define EDMA_Q1E5 (0x454 / WORDSZ)  // Event Queue 1 Entry 5 Register Section 11.4.1.56
#define EDMA_Q1E6 (0x458 / WORDSZ)  // Event Queue 1 Entry 6 Register Section 11.4.1.57
#define EDMA_Q1E7 (0x45C / WORDSZ)  // Event Queue 1 Entry 7 Register Section 11.4.1.58
#define EDMA_Q1E8 (0x460 / WORDSZ)  // Event Queue 1 Entry 8 Register Section 11.4.1.59
#define EDMA_Q1E9 (0x464 / WORDSZ)  // Event Queue 1 Entry 9 Register Section 11.4.1.60
#define EDMA_Q1E10 (0x468 / WORDSZ) // Event Queue 1 Entry 10 Register Section 11.4.1.61
#define EDMA_Q1E11 (0x46C / WORDSZ) // Event Queue 1 Entry 11 Register Section 11.4.1.62
#define EDMA_Q1E12 (0x470 / WORDSZ) // Event Queue 1 Entry 12 Register Section 11.4.1.63
#define EDMA_Q1E13 (0x474 / WORDSZ) // Event Queue 1 Entry 13 Register Section 11.4.1.64
#define EDMA_Q1E14 (0x478 / WORDSZ) // Event Queue 1 Entry 14 Register Section 11.4.1.65
#define EDMA_Q1E15 (0x47C / WORDSZ) // Event Queue 1 Entry 15 Register Section 11.4.1.66
#define EDMA_Q2E0 (0x480 / WORDSZ)  // Event Queue 2 Entry 0 Register Section 11.4.1.67
#define EDMA_Q2E1 (0x484 / WORDSZ)  // Event Queue 2 Entry 1 Register Section 11.4.1.68
#define EDMA_Q2E2 (0x488 / WORDSZ)  // Event Queue 2 Entry 2 Register Section 11.4.1.69
#define EDMA_Q2E3 (0x48C / WORDSZ)  // Event Queue 2 Entry 3 Register Section 11.4.1.70
#define EDMA_Q2E4 (0x490 / WORDSZ)  // Event Queue 2 Entry 4 Register Section 11.4.1.71
#define EDMA_Q2E5 (0x494 / WORDSZ)  // Event Queue 2 Entry 5 Register Section 11.4.1.72
#define EDMA_Q2E6 (0x498 / WORDSZ)  // Event Queue 2 Entry 6 Register Section 11.4.1.73
#define EDMA_Q2E7 (0x49C / WORDSZ)  // Event Queue 2 Entry 7 Register Section 11.4.1.74
#define EDMA_Q2E8 (0x4A0 / WORDSZ)  // Event Queue 2 Entry 8 Register Section 11.4.1.75
#define EDMA_Q2E9 (0x4A4 / WORDSZ)  // Event Queue 2 Entry 9 Register Section 11.4.1.76
#define EDMA_Q2E10 (0x4A8 / WORDSZ) // Event Queue 2 Entry 10 Register Section 11.4.1.77
#define EDMA_Q2E11 (0x4AC / WORDSZ) // Event Queue 2 Entry 11 Register Section 11.4.1.78
#define EDMA_Q2E12 (0x4B0 / WORDSZ) // Event Queue 2 Entry 12 Register Section 11.4.1.79
#define EDMA_Q2E13 (0x4B4 / WORDSZ) // Event Queue 2 Entry 13 Register Section 11.4.1.80
#define EDMA_Q2E14 (0x4B8 / WORDSZ) // Event Queue 2 Entry 14 Register Section 11.4.1.81
#define EDMA_Q2E15 (0x4BC / WORDSZ) // Event Queue 2 Entry 15 Register Section 11.4.1.82

// Section 11.4.1.83 Queue Status Registers
#define EDMA_QSTAT_0 (0x600 / WORDSZ) // Queue Status Register 0
#define EDMA_QSTAT_1 (0x604 / WORDSZ) // Queue Status Register 1
#define EDMA_QSTAT_2 (0x608 / WORDSZ) // Queue Status Register 2

#define EDMA_QWMTHRA (0x620 / WORDSZ) // Queue Watermark Threshold A Register Section 11.4.1.84
#define EDMA_CCSTAT (0x640 / WORDSZ)  // EDMA3CC Status Register Section 11.4.1.85
#define EDMA_MPFAR (0x800 / WORDSZ)   // Memory Protection Fault Address Register Section 11.4.1.86
#define EDMA_MPFSR (0x804 / WORDSZ)   // Memory Protection Fault Status Register Section 11.4.1.87
#define EDMA_MPFCR (0x808 / WORDSZ)   // Memory Protection Fault Command Register Section 11.4.1.88
#define EDMA_MPPAG (0x80C / WORDSZ)   // Memory Protection Page Attribute Register Global Section 11.4.1.89

// 810h to 82Ch MPPA_0 to MPPA_7 Memory Protection Page Attribute Registers Section 11.4.1.90

#define EDMAREG_ER 0x00    // Event Register Section 11.4.1.91
#define EDMAREG_ERH 0x04   // Event Register High Section 11.4.1.92
#define EDMAREG_ECR 0x08   // Event Clear Register Section 11.4.1.93
#define EDMAREG_ECRH 0x0C  // Event Clear Register High Section 11.4.1.94
#define EDMAREG_ESR 0x10   // Event Set Register Section 11.4.1.95
#define EDMAREG_ESRH 0x14  // Event Set Register High Section 11.4.1.96
#define EDMAREG_CER 0x18   // Chained Event Register Section 11.4.1.97
#define EDMAREG_CERH 0x1C  // Chained Event Register High Section 11.4.1.98
#define EDMAREG_EER 0x20   // Event Enable Register Section 11.4.1.99
#define EDMAREG_EERH 0x24  // Event Enable Register High Section 11.4.1.100
#define EDMAREG_EECR 0x28  // Event Enable Clear Register Section 11.4.1.101
#define EDMAREG_EECRH 0x2C // Event Enable Clear Register High Section 11.4.1.102
#define EDMAREG_EESR 0x30  // Event Enable Set Register Section 11.4.1.103
#define EDMAREG_EESRH 0x34 // Event Enable Set Register High Section 11.4.1.104
#define EDMAREG_SER 0x38   // Secondary Event Register Section 11.4.1.105
#define EDMAREG_SERH 0x3C  // Secondary Event Register High Section 11.4.1.106
#define EDMAREG_SECR 0x40  // Secondary Event Clear Register Section 11.4.1.107
#define EDMAREG_SECRH 0x44 // Secondary Event Clear Register High Section 11.4.1.108
#define EDMAREG_IER 0x50   // Interrupt Enable Register Section 11.4.1.109
#define EDMAREG_IERH 0x54  // Interrupt Enable Register High Section 11.4.1.110
#define EDMAREG_IECR 0x58  // Interrupt Enable Clear Register Section 11.4.1.111
#define EDMAREG_IECRH 0x5C // Interrupt Enable Clear Register High Section 11.4.1.112
#define EDMAREG_IESR 0x60  // Interrupt Enable Set Register Section 11.4.1.113
#define EDMAREG_IESRH 0x64 // Interrupt Enable Set Register High Section 11.4.1.114
#define EDMAREG_IPR 0x68   // Interrupt Pending Register Section 11.4.1.115
#define EDMAREG_IPRH 0x6C  // Interrupt Pending Register High Section 11.4.1.116
#define EDMAREG_ICR 0x70   // Interrupt Clear Register Section 11.4.1.117
#define EDMAREG_ICRH 0x74  // Interrupt Clear Register High Section 11.4.1.118
#define EDMAREG_IEVAL 0x78 // Interrupt Evaluate Register Section 11.4.1.119
#define EDMAREG_QER 0x80   // QDMA Event Register Section 11.4.1.120
#define EDMAREG_QEER 0x84  // QDMA Event Enable Register Section 11.4.1.121
#define EDMAREG_QEECR 0x88 // QDMA Event Enable Clear Register Section 11.4.1.122
#define EDMAREG_QEESR 0x8C // QDMA Event Enable Set Register Section 11.4.1.123
#define EDMAREG_QSER 0x90  // QDMA Secondary Event Register Section 11.4.1.124
#define EDMAREG_QSECR 0x94 // QDMA Secondary Event Clear Register Section 11.4.1.125

#define SHADOW1(reg) ((0x2200 + reg) / WORDSZ)

// (TRM 11.3.3.1)
#define EDMA_PARAM_OFFSET (0x4000 / WORDSZ)
#define EDMA_PARAM_SIZE sizeof(edmaParam) // 32 bytes
#define EDMA_PARAM_NUM 256

// bit 5 is the valid strobe to generate system events with __R31
#define R31_INTERRUPT_ENABLE (1 << 5)
#define R31_INTERRUPT_OFFSET 16
