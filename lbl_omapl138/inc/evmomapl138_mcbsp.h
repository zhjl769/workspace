//-----------------------------------------------------------------------------
// \file    evmomapl138_mcbsp.h
// \brief   OMAP-L138 mcasp registers, bit definitions, and
//          function prototypes.
//
//-----------------------------------------------------------------------------

#ifndef EVMOMAPL138_MCBSP_H
#define EVMOMAPL138_MCBSP_H

//-----------------------------------------------------------------------------
// Register Structure & Defines
//-----------------------------------------------------------------------------
typedef struct
{
   volatile uint32_t DRR;         // 0x0000
   volatile uint32_t DXR;         // 0x0004
   volatile uint32_t SPCR;        // 0x0008
   volatile uint32_t RCR;         // 0x00C
   volatile uint32_t XCR;         // 0x0010
   volatile uint32_t SRGR;        // 0x0014
   volatile uint32_t MCR;         // 0x0018
   volatile uint32_t RCERE0;      // 0x001C
   volatile uint32_t XCERE0;      // 0x0020
   volatile uint32_t PCR;         // 0x0024
   volatile uint32_t RECRE1;      // 0x0028
   volatile uint32_t SCERE1;      // 0x002C
   volatile uint32_t RECRE2;      // 0x0030
   volatile uint32_t XCERE2;      // 0x0034
   volatile uint32_t RECRE3;      // 0x0038
   volatile uint32_t XCERE3;      // 0x003C
} mcbsp_regs_t;

#define MCBSP1        ((mcbsp_regs_t *)MCBSP1_REG_BASE)

// bitmask defines for GBLCTL.
#define XFRST        (0x00001000)   // bit 12
#define XSMRST       (0x00000800)   // bit 11
#define XSRCLR       (0x00000400)   // bit 10
#define XHCLKRST     (0x00000200)   // bit 9
#define XCLKRST      (0x00000100)   // bit 8
#define RFRST        (0x00000010)   // bit 4
#define RSMRST       (0x00000008)   // bit 3
#define RSRCLR       (0x00000004)   // bit 2
#define RHCLKRST     (0x00000002)   // bit 1
#define RCLKRST      (0x00000001)   // bit 0

// bitmask defines for SRCTL.
#define RRDY         (0x00000020)   // bit 5
#define XRDY         (0x00000010)   // bit 4

//-----------------------------------------------------------------------------
// Public Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public Function Prototypes
//-----------------------------------------------------------------------------

//uint32_t MCBSP_init(void);

#endif

