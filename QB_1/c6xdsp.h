//File named C6xdsp.h

/*******************************************************************************
* FILENAME
*   c641x.h
*
* DESCRIPTION
*   c641x Header File
*
*******************************************************************************/

/* Register definitions for C641x chip */

/* Define EMIF Registers  */
#define EMIFA_GCTL       0x01800000  //全局控制寄存器  GBLCTL
#define EMIFA_CE1        0x01800004  //CE1控制寄存器   CE1CTL
#define EMIFA_CE0        0x01800008  //                CE0CTL
#define EMIFA_CE2        0x01800010  //                CE2CTL
#define EMIFA_CE3        0x01800014  //                CE3CTL
#define EMIFA_SDRAMCTL   0x01800018  //SDRAM控制寄存器       SDCTL
#define EMIFA_SDRAMREF   0x0180001c  //SDRAM时许序控制寄存器 SDTIM
#define EMIFA_SDRAMEXT   0x01800020  //SDRAM扩展控制寄存器   SDEXT
#define EMIFA_CE1SECCTL  0x01800044  //CE1第二控制寄存器     CE1SEC
#define EMIFA_CE0SECCTL  0x01800048  // 					 CE0SEC
#define EMIFA_CE2SECCTL  0x01800050  //						 CE2SEC
#define EMIFA_CE3SECCTL  0x01800054  //						 CE3SEC

#define EMIFB_GCTL       0x01A80000
#define EMIFB_CE1        0x01A80004
#define EMIFB_CE0        0x01A80008
#define EMIFB_CE2        0x01A80010
#define EMIFB_CE3        0x01A80014
#define EMIFB_SDRAMCTL   0x01A80018
#define EMIFB_SDRAMREF   0x01A8001c
#define EMIFB_SDRAMEXT   0x01A80020
#define EMIFB_CE1SECCTL  0x01A80044
#define EMIFB_CE0SECCTL  0x01A80048
#define EMIFB_CE2SECCTL  0x01A80050
#define EMIFB_CE3SECCTL  0x01A80054
 
/* Define McBSP0 Registers */
#define McBSP0_DRR      0x018c0000   /* Address of data receive reg.         */
#define McBSP0_DXR      0x018c0004   /* Address of data transmit reg.        */
#define McBSP0_SPCR     0x018c0008   /* Address of serial port contl. reg.   */
#define McBSP0_RCR      0x018c000C   /* Address of receive control reg.      */
#define McBSP0_XCR      0x018c0010   /* Address of transmit control reg.     */
#define McBSP0_SRGR     0x018c0014   /* Address of sample rate generator     */
#define McBSP0_MCR      0x018c0018   /* Address of multichannel reg.         */
#define McBSP0_RCER     0x018c001C   /* Address of receive channel enable.   */
#define McBSP0_XCER     0x018c0020   /* Address of transmit channel enable.  */
#define McBSP0_PCR      0x018c0024   /* Address of pin control reg.          */

/* Define McBSP1 Registers */
#define McBSP1_DRR      0x1900000   /* Address of data receive reg.         */
#define McBSP1_DXR      0x1900004   /* Address of data transmit reg.        */
#define McBSP1_SPCR     0x1900008   /* Address of serial port contl. reg.   */
#define McBSP1_RCR      0x190000C   /* Address of receive control reg.      */
#define McBSP1_XCR      0x1900010   /* Address of transmit control reg.     */
#define McBSP1_SRGR     0x1900014   /* Address of sample rate generator     */
#define McBSP1_MCR      0x1900018   /* Address of multichannel reg.         */
#define McBSP1_RCER     0x190001C   /* Address of receive channel enable.   */
#define McBSP1_XCER     0x1900020   /* Address of transmit channel enable.  */
#define McBSP1_PCR      0x1900024   /* Address of pin control reg.          */

/* Define McBSP2 Registers */
#define McBSP2_DRR      0x1A40000   /* Address of data receive reg.         */
#define McBSP2_DXR      0x1A40004   /* Address of data transmit reg.        */
#define McBSP2_SPCR     0x1A40008   /* Address of serial port contl. reg.   */
#define McBSP2_RCR      0x1A4000C   /* Address of receive control reg.      */
#define McBSP2_XCR      0x1A40010   /* Address of transmit control reg.     */
#define McBSP2_SRGR     0x1A40014   /* Address of sample rate generator     */
#define McBSP2_MCR      0x1A40018   /* Address of multichannel reg.         */
#define McBSP2_RCER     0x1A4001C   /* Address of receive channel enable.   */
#define McBSP2_XCER     0x1A40020   /* Address of transmit channel enable.  */
#define McBSP2_PCR      0x1A40024   /* Address of pin control reg.          */

/* Define L2 Cache Registers */
#define L2CFG           0x1840000   /* Address of L2 config reg             */
#define MAR0            0x1848200   /* Address of mem attribute reg         */

/* Define Interrupt Registers */
#define MUXH            0x19c0000   /* Address of Interrupt Multiplexer High*/
#define MUXL            0x19c0004   /* Address of Interrupt Multiplexer Low */
#define EXTPOL          0x19c0008   /* Address of External Interrupt Polarity */

/* Define Timer0 Registers */
#define TIMER0_CTRL     0x1940000	/* Address of timer0 control reg.       */
#define TIMER0_PRD      0x1940004	/* Address of timer0 period reg.        */
#define TIMER0_COUNT    0x1940008	/* Address of timer0 counter reg.       */

/* Define Timer1 Registers */
#define TIMER1_CTRL     0x1980000	/* Address of timer1 control reg.       */
#define TIMER1_PRD      0x1980004	/* Address of timer1 period reg.        */
#define TIMER1_COUNT    0x1980008	/* Address of timer1 counter reg.       */

/* Define Timer1 Registers */
#define TIMER2_CTRL     0x1AC0000	/* Address of timer1 control reg.       */
#define TIMER2_PRD      0x1AC0004	/* Address of timer1 period reg.        */
#define TIMER2_COUNT    0x1AC0008	/* Address of timer1 counter reg.       */

/* Define EDMA Registers */
#define PQSR			0x01A0FFE0	/* Address of priority queue status     */
#define CIPRH			0x01A0FFA4	/* Address of channel interrupt pending */
#define CIERH			0x01A0FFA8	/* Address of channel interrupt enable  */
#define CCERH			0x01A0FFAC	/* Address of channel chain enable      */
#define ERH				0x01A0FFB0	/* Address of event register            */
#define EERH			0x01A0FFB4	/* Address of event enable register     */
#define ECRH			0x01A0FFB8	/* Address of event clear register      */
#define ESRH			0x01A0FFBC	/* Address of event set register        */
#define EPRL			0x01A0FFDC	/* Address of event set register        */
#define CIPRL			0x01A0FFE4	/* Address of channel interrupt pending */
#define CIERL			0x01A0FFE8	/* Address of channel interrupt enable  */
#define CCERL			0x01A0FFEC	/* Address of channel chain enable      */
#define ERL				0x01A0FFF0	/* Address of event register            */
#define EERL			0x01A0FFF4	/* Address of event enable register     */
#define ECRL			0x01A0FFF8	/* Address of event clear register      */
#define ESRL			0x01A0FFFC	/* Address of event set register        */

/* Define EDMA Transfer Parameter Entry Fields */
#define OPT				0*4			/* Options Parameter                    */
#define SRC				1*4			/* SRC Address Parameter                */
#define CNT				2*4			/* Count Parameter                      */
#define DST				3*4			/* DST Address Parameter                */
#define IDX				4*4			/* IDX Parameter                        */
#define LNK				5*4			/* LNK Parameter                        */
						
/* Define EDMA Parameter RAM Addresses */ 
#define EVENT0_PARAMS 0x01A00000
#define EVENT1_PARAMS EVENT0_PARAMS + 0x18
#define EVENT2_PARAMS EVENT1_PARAMS + 0x18
#define EVENT3_PARAMS EVENT2_PARAMS + 0x18
#define EVENT4_PARAMS EVENT3_PARAMS + 0x18
#define EVENT5_PARAMS EVENT4_PARAMS + 0x18
#define EVENT6_PARAMS EVENT5_PARAMS + 0x18
#define EVENT7_PARAMS EVENT6_PARAMS + 0x18
#define EVENT8_PARAMS EVENT7_PARAMS + 0x18
#define EVENT9_PARAMS EVENT8_PARAMS + 0x18
#define EVENTA_PARAMS EVENT9_PARAMS + 0x18
#define EVENTB_PARAMS EVENTA_PARAMS + 0x18
#define EVENTC_PARAMS EVENTB_PARAMS + 0x18
#define EVENTD_PARAMS EVENTC_PARAMS + 0x18
#define EVENTE_PARAMS EVENTD_PARAMS + 0x18
#define EVENTF_PARAMS EVENTE_PARAMS + 0x18
#define EVENTN_PARAMS EVENTF_PARAMS + 0x18
#define EVENTO_PARAMS EVENTN_PARAMS + 0x18
#define EVENT22_PARAMS EVENT0_PARAMS + 0x18*22
#define EVENT24_PARAMS EVENT0_PARAMS + 0x18*24
#define EVENT25_PARAMS EVENT0_PARAMS + 0x18*25


#define  EVENT13RL0_PARAMS   0x01A00600             /* EDMA OPTIONS REGISTER */
#define  EVENT13RL1_PARAMS   0x01A00618             /* EDMA OPTIONS REGISTER */

/* Define QDMA Memory Mapped Registers */
#define QDMA_OPT		0x02000000	/* Address of QDMA options register     */
#define QDMA_SRC		0x02000004	/* Address of QDMA SRC address register */
#define QDMA_CNT		0x02000008	/* Address of QDMA counts register      */
#define QDMA_DST		0x0200000C	/* Address of QDMA DST address register */
#define QDMA_IDX		0x02000010	/* Address of QDMA index register       */
 
/* Define QDMA Pseudo Registers */
#define QDMA_S_OPT		0x02000020	/* Address of QDMA options register     */
#define QDMA_S_SRC		0x02000024	/* Address of QDMA SRC address register */
#define QDMA_S_CNT		0x02000028	/* Address of QDMA counts register      */
#define QDMA_S_DST		0x0200002C	/* Address of QDMA DST address register */
#define QDMA_S_IDX		0x02000030	/* Address of QDMA index register       */

/* Definitions for the DSK Board and SW */
#define PI				(3.14159265358979323846)
#define IO_PORT			0x90080000  /* I/O port Address,top byte valid data */
#define INTERNAL_MEM_SIZE (0x3000)>>2
#define EXTERNAL_MEM_SIZE (0x400000)>>2
#define FLASH_SIZE		0x20000 
#define POST_SIZE		0x10000 
#define FLASH_WRITE_SIZE 0x80 
#define INTERNAL_MEM_START 0xD000
#define EXTERNAL_MEM_START 0x80000000
#define FLASH_START		0x90000000
#define POST_END		0x90010000 
#define FLASH_ADR1		0x90005555
#define FLASH_ADR2		0x90002AAA
#define FLASH_KEY1		0xAA
#define FLASH_KEY2		0x55
/*******************************************************************************
* FILENAME
*   c6xdsp.h
*
* DESCRIPTION
*   c641x Header File
*
*******************************************************************************/
#ifndef _C6XDSP
#define _C6XDSP 

/* Register definitions for C641x chip */
#define EMA_CE0_0        0x80000000
#define EMA_CE0_2        0x80000002
#define EMA_CE0_4        0x80000004
#define EMA_CE0_8        0x80000008
#define EMA_CE0_C        0x8000000C
#define EMA_CE0_E        0x8000000E
#define EMA_CE1          0x90000000
#define EMA_CE2          0xA0000000
#define EMA_CE3          0xB0000000
#define EMB_CE0          0x60000000
#define EMB_CE1          0x64000000
#define EMB_CE2          0x68000000
#define EMB_CE3          0x6C000000
#define baseaddr         0x00030400
/* Define EMIF Registers  */
#define EMIFA_GCTL       0x01800000  //全局控制寄存器  GBLCTL
#define EMIFA_CE1        0x01800004  //CE1控制寄存器   CE1CTL
#define EMIFA_CE0        0x01800008  //                CE0CTL
#define EMIFA_CE2        0x01800010  //                CE2CTL
#define EMIFA_CE3        0x01800014  //                CE3CTL
#define EMIFA_SDRAMCTL   0x01800018  //SDRAM控制寄存器       SDCTL
#define EMIFA_SDRAMREF   0x0180001c  //SDRAM时许序控制寄存器 SDTIM
#define EMIFA_SDRAMEXT   0x01800020  //SDRAM扩展控制寄存器   SDEXT
#define EMIFA_CE1SECCTL  0x01800044  //CE1第二控制寄存器     CE1SEC
#define EMIFA_CE0SECCTL  0x01800048  // 					 CE0SEC
#define EMIFA_CE2SECCTL  0x01800050  //						 CE2SEC
#define EMIFA_CE3SECCTL  0x01800054  //						 CE3SEC  

#define EMIFA_pdtcl  0x01800040

#define EMIFB_GCTL       0x01A80000
#define EMIFB_CE1        0x01A80004
#define EMIFB_CE0        0x01A80008
#define EMIFB_CE2        0x01A80010
#define EMIFB_CE3        0x01A80014
#define EMIFB_SDRAMCTL   0x01A80018
#define EMIFB_SDRAMREF   0x01A8001c
#define EMIFB_SDRAMEXT   0x01A80020
#define EMIFB_CE1SECCTL  0x01A80044
#define EMIFB_CE0SECCTL  0x01A80048
#define EMIFB_CE2SECCTL  0x01A80050
#define EMIFB_CE3SECCTL  0x01A80054
 
/* Define McBSP0 Registers */
#define McBSP0_DRR      0x018c0000   /* Address of data receive reg.         */
#define McBSP0_DXR      0x018c0004   /* Address of data transmit reg.        */
#define McBSP0_SPCR     0x018c0008   /* Address of serial port contl. reg.   */
#define McBSP0_RCR      0x018c000C   /* Address of receive control reg.      */
#define McBSP0_XCR      0x018c0010   /* Address of transmit control reg.     */
#define McBSP0_SRGR     0x018c0014   /* Address of sample rate generator     */
#define McBSP0_MCR      0x018c0018   /* Address of multichannel reg.         */
#define McBSP0_RCER     0x018c001C   /* Address of receive channel enable.   */
#define McBSP0_XCER     0x018c0020   /* Address of transmit channel enable.  */
#define McBSP0_PCR      0x018c0024   /* Address of pin control reg.          */

/* Define McBSP1 Registers */
#define McBSP1_DRR      0x1900000   /* Address of data receive reg.         */
#define McBSP1_DXR      0x1900004   /* Address of data transmit reg.        */
#define McBSP1_SPCR     0x1900008   /* Address of serial port contl. reg.   */
#define McBSP1_RCR      0x190000C   /* Address of receive control reg.      */
#define McBSP1_XCR      0x1900010   /* Address of transmit control reg.     */
#define McBSP1_SRGR     0x1900014   /* Address of sample rate generator     */
#define McBSP1_MCR      0x1900018   /* Address of multichannel reg.         */
#define McBSP1_RCER     0x190001C   /* Address of receive channel enable.   */
#define McBSP1_XCER     0x1900020   /* Address of transmit channel enable.  */
#define McBSP1_PCR      0x1900024   /* Address of pin control reg.          */

/* Define McBSP2 Registers */
#define McBSP2_DRR      0x1A40000   /* Address of data receive reg.         */
#define McBSP2_DXR      0x1A40004   /* Address of data transmit reg.        */
#define McBSP2_SPCR     0x1A40008   /* Address of serial port contl. reg.   */
#define McBSP2_RCR      0x1A4000C   /* Address of receive control reg.      */
#define McBSP2_XCR      0x1A40010   /* Address of transmit control reg.     */
#define McBSP2_SRGR     0x1A40014   /* Address of sample rate generator     */
#define McBSP2_MCR      0x1A40018   /* Address of multichannel reg.         */
#define McBSP2_RCER     0x1A4001C   /* Address of receive channel enable.   */
#define McBSP2_XCER     0x1A40020   /* Address of transmit channel enable.  */
#define McBSP2_PCR      0x1A40024   /* Address of pin control reg.          */

/* Define L2 Cache Registers */
#define CCFG            0x1840000   /* Address of L2 config reg             */
#define MAR0            0x1848200   /* Address of mem attribute reg         */

/* Define Interrupt Registers */
#define MUXH            0x19c0000   /* Address of Interrupt Multiplexer High*/
#define MUXL            0x19c0004   /* Address of Interrupt Multiplexer Low */
#define EXTPOL          0x19c0008   /* Address of External Interrupt Polarity */

/* Define Timer0 Registers */
#define TIMER0_CTRL     0x1940000	/* Address of timer0 control reg.       */
#define TIMER0_PRD      0x1940004	/* Address of timer0 period reg.        */
#define TIMER0_COUNT    0x1940008	/* Address of timer0 counter reg.       */

/* Define Timer1 Registers */
#define TIMER1_CTRL     0x1980000	/* Address of timer1 control reg.       */
#define TIMER1_PRD      0x1980004	/* Address of timer1 period reg.        */
#define TIMER1_COUNT    0x1980008	/* Address of timer1 counter reg.       */

/* Define Timer2 Registers */
#define TIMER2_CTRL     0x1AC0000	/* Address of timer1 control reg.       */
#define TIMER2_PRD      0x1AC0004	/* Address of timer1 period reg.        */
#define TIMER2_COUNT    0x1AC0008	/* Address of timer1 counter reg.       */

/* Define EDMA Registers */
#define PQSR			0x01A0FFE0	/* Address of priority queue status     */
#define CIPRH			0x01A0FFA4	/* Address of channel interrupt pending */
#define CIERH			0x01A0FFA8	/* Address of channel interrupt enable  */
#define CCERH			0x01A0FFAC	/* Address of channel chain enable      */
#define ERH				0x01A0FFB0	/* Address of event register            */
#define EERH			0x01A0FFB4	/* Address of event enable register     */
#define ECRH			0x01A0FFB8	/* Address of event clear register      */
#define ESRH			0x01A0FFBC	/* Address of event set register        */
#define CIPRL			0x01A0FFE4	/* Address of channel interrupt pending */
#define CIERL			0x01A0FFE8	/* Address of channel interrupt enable  */
#define CCERL			0x01A0FFEC	/* Address of channel chain enable      */
#define ERL				0x01A0FFF0	/* Address of event register            */
#define EERL			0x01A0FFF4	/* Address of event enable register     */
#define ECRL			0x01A0FFF8	/* Address of event clear register      */
#define ESRL			0x01A0FFFC	/* Address of event set register        */

/* Define EDMA Transfer Parameter Entry Fields */
#define OPT				0*4			/* Options Parameter                    */
#define SRC				1*4			/* SRC Address Parameter                */
#define CNT				2*4			/* Count Parameter                      */
#define DST				3*4			/* DST Address Parameter                */
#define IDX				4*4			/* IDX Parameter                        */
#define LNK				5*4			/* LNK Parameter                        */
						
/* Define EDMA Parameter RAM Addresses */ 
#define EVENT0_PARAMS 0x01A00000
#define EVENT1_PARAMS EVENT0_PARAMS + 0x18
#define EVENT2_PARAMS EVENT1_PARAMS + 0x18
#define EVENT3_PARAMS EVENT2_PARAMS + 0x18
#define EVENT4_PARAMS EVENT3_PARAMS + 0x18
#define EVENT5_PARAMS EVENT4_PARAMS + 0x18
#define EVENT6_PARAMS EVENT5_PARAMS + 0x18
#define EVENT7_PARAMS EVENT6_PARAMS + 0x18
#define EVENT8_PARAMS EVENT7_PARAMS + 0x18
#define EVENT9_PARAMS EVENT8_PARAMS + 0x18
#define EVENTA_PARAMS EVENT9_PARAMS + 0x18
#define EVENTB_PARAMS EVENTA_PARAMS + 0x18
#define EVENTC_PARAMS EVENTB_PARAMS + 0x18
#define EVENTD_PARAMS EVENTC_PARAMS + 0x18
#define EVENTE_PARAMS EVENTD_PARAMS + 0x18
#define EVENTF_PARAMS EVENTE_PARAMS + 0x18
#define EVENTN_PARAMS EVENTF_PARAMS + 0x18
#define EVENTO_PARAMS EVENTN_PARAMS + 0x18
#define EVENT48_PARAMS EVENT0_PARAMS + 0x18*48  /* EDMA CHANNEL GP8  */
#define EVENT49_PARAMS EVENT0_PARAMS + 0x18*49  /* EDMA CHANNEL GP9  */
#define EVENT50_PARAMS EVENT0_PARAMS + 0x18*50	/* EDMA CHANNEL GP10 */
#define EVENT51_PARAMS EVENT0_PARAMS + 0x18*51	/* EDMA CHANNEL GP11 */
#define EVENT52_PARAMS EVENT0_PARAMS + 0x18*52	/* EDMA CHANNEL GP12 */
#define EVENT53_PARAMS EVENT0_PARAMS + 0x18*53	/* EDMA CHANNEL GP13 */
#define EVENT54_PARAMS EVENT0_PARAMS + 0x18*54	/* EDMA CHANNEL GP14 */
#define EVENT55_PARAMS EVENT0_PARAMS + 0x18*55	/* EDMA CHANNEL GP15 */

#define EVENT24_PARAMS EVENT0_PARAMS + 0x18*24
#define EVENT25_PARAMS EVENT0_PARAMS + 0x18*25

#define  EVENT24L_PARAMS   0x01A00600             /* EDMA OPTIONS REGISTER */
#define  EVENT25L_PARAMS   0x01A00618             /* EDMA OPTIONS REGISTER */

/* Define QDMA Memory Mapped Registers */
#define QDMA_OPT		0x02000000	/* Address of QDMA options register     */
#define QDMA_SRC		0x02000004	/* Address of QDMA SRC address register */
#define QDMA_CNT		0x02000008	/* Address of QDMA counts register      */
#define QDMA_DST		0x0200000C	/* Address of QDMA DST address register */
#define QDMA_IDX		0x02000010	/* Address of QDMA index register       */
 
/* Define QDMA Pseudo Registers */
#define QDMA_S_OPT		0x02000020	/* Address of QDMA options register     */
#define QDMA_S_SRC		0x02000024	/* Address of QDMA SRC address register */
#define QDMA_S_CNT		0x02000028	/* Address of QDMA counts register      */
#define QDMA_S_DST		0x0200002C	/* Address of QDMA DST address register */
#define QDMA_S_IDX		0x02000030	/* Address of QDMA index register       */

/* Define GPIO Registers */
#define GPEN            0x01B00000
#define GPDIR           0x01B00004
#define GPVAL           0x01B00008
#define GPDH            0x01B00010
#define GPHM            0x01B00014
#define GPDL            0x01B00018
#define GPLM            0x01B0001C
#define GPGC            0x01B00020
#define GPPOL           0x01B00024

/* Definitions for 754 */
//--------------------------------------------------
/*罗经、同步器、深度传感器、命令偏移地址*/
#define LJ_A4       0xF0
#define SYNC_A5     0xE8
#define DP_A6       0xD8
#define ORD_A7      0xB8

#define RHR_7       0x0	
#define THR_7       0x0
#define DLL_7       0x0

#define IER_7       0x1	
#define DLH_7       0x1

#define IIR_7       0x2
#define FCR_7       0x2
#define EFR_7       0x2

#define LCR_7       0x3
#define MCR_7       0x4
#define LSR_7       0x5

#define MSR_7       0x6
#define TCR_7       0x6

#define SPR_7       0x7
#define TLR_7       0x7
#define FIFORdy_7       0x7

/* Definitions for CF */
//--------------------------------------------------
/*CF卡寄存器偏移地址,CF卡映射到EMIFA CE2空间*/
//--------------------------------------------------
#define CF_DataReg           0x2010	//Data Register, for read / write

#define CF_ErrorReg	         0x2011  //Error Register, read only
/*					Error Register								*
*D7		D6		D5		D4		D3		D2		D1		D0		*
*BBK	UNC		0		IDNF	0		ABRT	0		AMNF	*/

#define CF_FeaturesReg	     0x2011   //Features Register, write only 
#define CF_SectorsCouReg	 0x2012	// Sector Count Register, for read / write
#define CF_SectorsNumReg	 0x2013	// Sector Number Register, or LBA7:0, for read / write

#define CF_CylindersLReg	 0x2014	// Cylinder Low Register or LBA15:8, for read / write
#define CF_CylindersHReg	 0x2015	// Cylinder High Register or LBA23:16, for read / write
#define CF_HeadsReg			 0x2016	//Drive Head Register and LBA27:24, for read / write

/****************************************************************
*					Drive Head Register							*
*D7		D6		D5		D4		D3		D2		D1		D0		*
*1		LBA		1		DRV		LBA D27	LBA D26	LBA D25	LBA D24	*
*		0=CHS													*
*		1=LBA													*
*****************************************************************/

#define CF_StatusReg		 0x2017	//Status Register, read only

/*****************************************************************
*					Status Register								*
*D7		D6		D5		D4		D3		D2		D1		D0		*
*BUSY	RDY		DWY		DSC		DRQ		CORR	0		ERR 	*
*****************************************************************
*BUSY:CF卡忙,此时不能接受其它命令;								*
*RDY:卡可以接受命令;											*
*DWY:写错误;													*
*DSC:卡准备就绪;												*
*DRQ:CF卡请求数据传送;											*
*CORR:数据错误但被修正，不会终止多扇区读操作;					*
*D1:0;															*
*ERR在上一命令以某种错误结束，可以在错误寄存器中查看错误类型;	*
*****************************************************************/

#define CF_CommandReg		0x2017	// Command Register, write only

#define CF_AlternateStaReg	0x2008	// Alternate Status Register, read only

#define CF_DeviceCtrlReg	0x2016	// Device Control Register, write only. Not used in this demo

#define CF_DriveAddrReg		0x2009	// Drive Address Register, read only. Not used in this demo





/*SRAM0 SAR1地址*/
#define SRAM0_add0       0x80000000   // 片外SRAM1的起始地址；
#define SRAM1_add0       0x90000000   // 片外SRAM2的起始地址；

#define SRAM_P           0x80000000   // 片外SRAM1中P 的起始地址；
#define SRAM_VX          0x80080000   // 片外SRAM1中VX的起始地址；
#define SRAM_VY          0x80100000   // 片外SRAM1中VY的起始地址；
#define SRAM_VZ          0x80180000   // 片外SRAM1中VZ的起始地址；


/*内存地址*/
#define add0       0x00011000
#define add1       0x00012000

#endif

#define FLASH_KEY3		0xA0
#define ALL_A			0xaaaaaaaa
#define ALL_5			0x55555555
#define ALT_A5			0xa5a5a5a5
#define ALT_5A			0x5a5a5a5a
#define CE1_8			0xffffff03  /* reg to set CE1 as 8bit async */
#define CE1_32			0xffffff23  /* reg to set CE1 as 32bit async */
