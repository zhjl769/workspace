//------------------------------------------- 
// filename :"register.h"			          
// Type:  VC5509A                             
// Athor: JufengFei                            
// Time:  2007-11-27 					      
//------------------------------------------- 
#ifndef _REGISTER_H
#define _REGISTER_H


//---------------------------------------------------------------            
//INTERUPT 
#define       IER0			*(volatile unsigned int *)0x00000
#define       IFR0			*(volatile unsigned int *)0x00001
#define       ST0_55   	   *(volatile unsigned int *)0x00002
#define       ST1_55   	   *(volatile unsigned int *)0x00003
#define       ST3_55   	   *(volatile unsigned int *)0x00004
#define       IER1			*(volatile unsigned int *)0x00045
#define       IFR1			*(volatile unsigned int *)0x00046
#define       IVPD			*(volatile unsigned int *)0x00049
#define       IVPH			*(volatile unsigned int *)0x0004A
#define       ST2_55  	   *(volatile unsigned int *)0x0004B


//---------------------------------------------------------------  
// EMIF REGISTERS 
#define       EGCR         *(volatile ioport unsigned int*)0x0800
#define       EMI_RST      *(volatile ioport unsigned int*)0x0801
#define       EMI_BE       *(volatile ioport unsigned int*)0x0802
#define       CE0_1	   	   *(volatile ioport unsigned int*)0x0803
#define       CE0_2	   	   *(volatile ioport unsigned int*)0x0804
#define       CE0_3	   	   *(volatile ioport unsigned int*)0x0805
#define       CE1_1        *(volatile ioport unsigned int*)0x0806
#define       CE1_2	   	   *(volatile ioport unsigned int*)0x0807
#define       CE1_3	   	   *(volatile ioport unsigned int*)0x0808
#define       CE2_1        *(volatile ioport unsigned int*)0x0809
#define       CE2_2	   	   *(volatile ioport unsigned int*)0x080A
#define       CE2_3	   	   *(volatile ioport unsigned int*)0x080B
#define       CE3_1	   	   *(volatile ioport unsigned int*)0x080C
#define       CE3_2	   	   *(volatile ioport unsigned int*)0x080D
#define       CE3_3	   	   *(volatile ioport unsigned int*)0x080E

#define       SDC1	       *(volatile ioport unsigned int*)0x080F
#define       SDPER	   	   *(volatile ioport unsigned int*)0x0810
#define       SDCNT	   	   *(volatile ioport unsigned int*)0x0811
#define       INIT	   	   *(volatile ioport unsigned int*)0x0812
#define       SDC2	   	   *(volatile ioport unsigned int*)0x0813

//---------------------------------------------------------------  
//CLOCK GENERATOR REGISTER 

#define       CLKMD        *(volatile ioport unsigned int *)0x01C00

//---------------------------------------------------------------  
// EXTERNAL BUS SELECTION REGISTER  
#define       EBSR   	     *(volatile ioport unsigned int *)0x06C00

//---------------------------------------------------------------  
//McBSP0 REGISTERS 

#define       DRR2_0         *(volatile ioport unsigned int*)0x02800
#define       DRR1_0         *(volatile ioport unsigned int*)0x02801
#define       DXR2_0         *(volatile ioport unsigned int*)0x02802
#define       DXR1_0         *(volatile ioport unsigned int*)0x02803
#define       SPCR2_0        *(volatile ioport unsigned int*)0x02804
#define       SPCR1_0        *(volatile ioport unsigned int*)0x02805
#define       RCR2_0         *(volatile ioport unsigned int*)0x02806
#define       RCR1_0         *(volatile ioport unsigned int*)0x02807
#define       XCR2_0         *(volatile ioport unsigned int*)0x02808
#define       XCR1_0         *(volatile ioport unsigned int*)0x02809
#define       SRGR2_0        *(volatile ioport unsigned int*)0x0280A
#define       SRGR1_0        *(volatile ioport unsigned int*)0x0280B
#define       MCR2_0         *(volatile ioport unsigned int*)0x0280C
#define       MCR1_0         *(volatile ioport unsigned int*)0x0280D
#define       RCERA_0        *(volatile ioport unsigned int*)0x0280E
#define       RCERB_0        *(volatile ioport unsigned int*)0x0280F
#define       XCERA_0        *(volatile ioport unsigned int*)0x02810
#define       XCERB_0        *(volatile ioport unsigned int*)0x02811
#define       PCR0           *(volatile ioport unsigned int*)0x02812
#define       RCERC_0        *(volatile ioport unsigned int*)0x02813
#define       RCERD_0        *(volatile ioport unsigned int*)0x02814
#define       XCERC_0        *(volatile ioport unsigned int*)0x02815
#define       XCERD_0        *(volatile ioport unsigned int*)0x02816
#define       RCERE_0        *(volatile ioport unsigned int*)0x02817
#define       RCERF_0        *(volatile ioport unsigned int*)0x02818
#define       XCERE_0        *(volatile ioport unsigned int*)0x02819
#define       XCERF_0        *(volatile ioport unsigned int*)0x0281A
#define       RCERG_0        *(volatile ioport unsigned int*)0x0281B
#define       RCERH_0        *(volatile ioport unsigned int*)0x0281C
#define       XCERG_0        *(volatile ioport unsigned int*)0x0281D
#define       XCERH_0        *(volatile ioport unsigned int*)0x0281E

//McBSP1 REGISTERS 
#define       DRR2_1         *(volatile ioport unsigned int*)0x02C00 
#define       DRR1_1         *(volatile ioport unsigned int*)0x02C01
#define       DXR2_1         *(volatile ioport unsigned int*)0x02C02
#define       DXR1_1         *(volatile ioport unsigned int*)0x02C03
#define       SPCR2_1        *(volatile ioport unsigned int*)0x02C04
#define       SPCR1_1        *(volatile ioport unsigned int*)0x02C05
#define       RCR2_1         *(volatile ioport unsigned int*)0x02C06
#define       RCR1_1         *(volatile ioport unsigned int*)0x02C07
#define       XCR2_1         *(volatile ioport unsigned int*)0x02C08
#define       XCR1_1         *(volatile ioport unsigned int*)0x02C09
#define       SRGR2_1        *(volatile ioport unsigned int*)0x02C0A
#define       SRGR1_1        *(volatile ioport unsigned int*)0x02C0B
#define       MCR2_1         *(volatile ioport unsigned int*)0x02C0C
#define       MCR1_1         *(volatile ioport unsigned int*)0x02C0D
#define       RCERA_1        *(volatile ioport unsigned int*)0x02C0E
#define       RCERB_1        *(volatile ioport unsigned int*)0x02C0F
#define       XCERA_1        *(volatile ioport unsigned int*)0x02C10
#define       XCERB_1        *(volatile ioport unsigned int*)0x02C11
#define       PCR1           *(volatile ioport unsigned int*)0x02C12
#define       RCERC_1        *(volatile ioport unsigned int*)0x02C13
#define       RCERD_1        *(volatile ioport unsigned int*)0x02C14
#define       XCERC_1        *(volatile ioport unsigned int*)0x02C15
#define       XCERD_1        *(volatile ioport unsigned int*)0x02C16
#define       RCERE_1        *(volatile ioport unsigned int*)0x02C17
#define       RCERF_1        *(volatile ioport unsigned int*)0x02C18
#define       XCERE_1        *(volatile ioport unsigned int*)0x02C19
#define       XCERF_1        *(volatile ioport unsigned int*)0x02C1A
#define       RCERG_1        *(volatile ioport unsigned int*)0x02C1B
#define       RCERH_1        *(volatile ioport unsigned int*)0x02C1C
#define       XCERG_1        *(volatile ioport unsigned int*)0x02C1D
#define       XCERH_1        *(volatile ioport unsigned int*)0x02C1E

/*
//McBSP2 REGISTERS 
#define       DRR2_2         *(volatile ioport unsigned int*)0x03000 
#define       DRR1_2         *(volatile ioport unsigned int*)0x03001
#define       DXR2_2         *(volatile ioport unsigned int*)0x03002
#define       DXR1_2         *(volatile ioport unsigned int*)0x03003
#define       SPCR2_2        *(volatile ioport unsigned int*)0x03004
#define       SPCR1_2        *(volatile ioport unsigned int*)0x03005
#define       RCR2_2         *(volatile ioport unsigned int*)0x03006
#define       RCR1_2         *(volatile ioport unsigned int*)0x03007
#define       XCR2_2         *(volatile ioport unsigned int*)0x03008
#define       XCR1_2         *(volatile ioport unsigned int*)0x03009
#define       SRGR2_2        *(volatile ioport unsigned int*)0x0300A
#define       SRGR1_2        *(volatile ioport unsigned int*)0x0300B
#define       MCR2_2         *(volatile ioport unsigned int*)0x0300C
#define       MCR1_2         *(volatile ioport unsigned int*)0x0300D
#define       RCERA_2        *(volatile ioport unsigned int*)0x0300E
#define       RCERB_2        *(volatile ioport unsigned int*)0x0300F
#define       XCERA_2        *(volatile ioport unsigned int*)0x03010
#define       XCERB_2        *(volatile ioport unsigned int*)0x03011
#define       PCR2           *(volatile ioport unsigned int*)0x03012
#define       RCERC_2        *(volatile ioport unsigned int*)0x03013
#define       RCERD_2        *(volatile ioport unsigned int*)0x03014
#define       XCERC_2        *(volatile ioport unsigned int*)0x03015
#define       XCERD_2        *(volatile ioport unsigned int*)0x03016
#define       RCERE_2        *(volatile ioport unsigned int*)0x03017
#define       RCERF_2        *(volatile ioport unsigned int*)0x03018
#define       XCERE_2        *(volatile ioport unsigned int*)0x03019
#define       XCERF_2        *(volatile ioport unsigned int*)0x0301A
#define       RCERG_2        *(volatile ioport unsigned int*)0x0301B
#define       RCERH_2        *(volatile ioport unsigned int*)0x0301C
#define       XCERG_2        *(volatile ioport unsigned int*)0x0301D
#define       XCERH_2        *(volatile ioport unsigned int*)0x0301E
*/
//---------------------------------------------------------------  
//GPIO REGISTERS 
#define       IODOR          *(volatile ioport unsigned int *)0x03400
#define       IODATA         *(volatile ioport unsigned int *)0x03401


//---------------------------------------------------------------  
//TIMES REGISTERS 
#define       TIM0  	   *(volatile ioport unsigned int*)0x01000
#define       PRD0	   	   *(volatile ioport unsigned int*)0x01001
#define       TCR0	   	   *(volatile ioport unsigned int*)0x01002
#define       PRSC0	   	   *(volatile ioport unsigned int*)0x01003
#define       TIM1	   	   *(volatile ioport unsigned int*)0x02400
#define       PRD1	   	   *(volatile ioport unsigned int*)0x02401
#define       TCR1	   	   *(volatile ioport unsigned int*)0x02402
#define       PRSC1	   	   *(volatile ioport unsigned int*)0x02403

//#define       DMA_DMACDEI2		*(volatile ioport unsigned int*)0x0C4E
#define       DMA_CFN2		*(volatile ioport unsigned int*)0x0C49

/*
//---------------------------------------------------------------  
//-DMA  REGISTERS- 
#define       DMA_GCR		*(volatile ioport unsigned int*)0x0E00 
#define       DMA_GSCR		*(volatile ioport unsigned int*)0x0E02
#define       DMA_GTCR		*(volatile ioport unsigned int*)0x0E03

//通道0寄存器 
#define       DMA_CSDP0		*(volatile ioport unsigned int*)0x0C00 
#define       DMA_CCR0		*(volatile ioport unsigned int*)0x0C01
#define       DMA_CICR0		*(volatile ioport unsigned int*)0x0C02
#define       DMA_CSR0		*(volatile ioport unsigned int*)0x0C03
#define       DMA_CSSA_L0	*(volatile ioport unsigned int*)0x0C04
#define       DMA_CSSA_U0	*(volatile ioport unsigned int*)0x0C05
#define       DMA_CDSA_L0	*(volatile ioport unsigned int*)0x0C06
#define       DMA_CDSA_U0	*(volatile ioport unsigned int*)0x0C07
#define       DMA_CEN0		*(volatile ioport unsigned int*)0x0C08
#define       DMA_CFN0		*(volatile ioport unsigned int*)0x0C09

//通道1寄存器 
#define       DMA_CSDP1		*(volatile ioport unsigned int*)0x0C20 
#define       DMA_CCR1		*(volatile ioport unsigned int*)0x0C21
#define       DMA_CICR1		*(volatile ioport unsigned int*)0x0C22
#define       DMA_CSR1		*(volatile ioport unsigned int*)0x0C23
#define       DMA_CSSA_L1	*(volatile ioport unsigned int*)0x0C24
#define       DMA_CSSA_U1	*(volatile ioport unsigned int*)0x0C25
#define       DMA_CDSA_L1	*(volatile ioport unsigned int*)0x0C26
#define       DMA_CDSA_U1	*(volatile ioport unsigned int*)0x0C27
#define       DMA_CEN1		*(volatile ioport unsigned int*)0x0C28
#define       DMA_CFN1		*(volatile ioport unsigned int*)0x0C29

//通道2寄存器 
#define       DMA_CSDP2		*(volatile ioport unsigned int*)0x0C40 
#define       DMA_CCR2		*(volatile ioport unsigned int*)0x0C41
#define       DMA_CICR2		*(volatile ioport unsigned int*)0x0C42
#define       DMA_CSR2		*(volatile ioport unsigned int*)0x0C43
#define       DMA_CSSA_L2	*(volatile ioport unsigned int*)0x0C44
#define       DMA_CSSA_U2	*(volatile ioport unsigned int*)0x0C45
#define       DMA_CDSA_L2	*(volatile ioport unsigned int*)0x0C46
#define       DMA_CDSA_U2	*(volatile ioport unsigned int*)0x0C47
#define       DMA_CEN2		*(volatile ioport unsigned int*)0x0C48
#define       DMA_CFN2		*(volatile ioport unsigned int*)0x0C49

//通道3寄存器 
#define       DMA_CSDP3		*(volatile ioport unsigned int*)0x0C60 
#define       DMA_CCR3		*(volatile ioport unsigned int*)0x0C61
#define       DMA_CICR3		*(volatile ioport unsigned int*)0x0C62
#define       DMA_CSR3		*(volatile ioport unsigned int*)0x0C63
#define       DMA_CSSA_L3	*(volatile ioport unsigned int*)0x0C64
#define       DMA_CSSA_U3	*(volatile ioport unsigned int*)0x0C65
#define       DMA_CDSA_L3	*(volatile ioport unsigned int*)0x0C66
#define       DMA_CDSA_U3	*(volatile ioport unsigned int*)0x0C67
#define       DMA_CEN3		*(volatile ioport unsigned int*)0x0C68
#define       DMA_CFN3		*(volatile ioport unsigned int*)0x0C69

//通道4寄存器 
#define       DMA_CSDP4		*(volatile ioport unsigned int*)0x0C80 
#define       DMA_CCR4		*(volatile ioport unsigned int*)0x0C81
#define       DMA_CICR4		*(volatile ioport unsigned int*)0x0C82
#define       DMA_CSR4		*(volatile ioport unsigned int*)0x0C83
#define       DMA_CSSA_L4	*(volatile ioport unsigned int*)0x0C84
#define       DMA_CSSA_U4	*(volatile ioport unsigned int*)0x0C85
#define       DMA_CDSA_L4	*(volatile ioport unsigned int*)0x0C86
#define       DMA_CDSA_U4	*(volatile ioport unsigned int*)0x0C87
#define       DMA_CEN4		*(volatile ioport unsigned int*)0x0C88
#define       DMA_CFN4		*(volatile ioport unsigned int*)0x0C89

//通道5寄存器 
#define       DMA_CSDP5		*(volatile ioport unsigned int*)0x0CA0 
#define       DMA_CCR5		*(volatile ioport unsigned int*)0x0CA1
#define       DMA_CICR5		*(volatile ioport unsigned int*)0x0CA2
#define       DMA_CSR5		*(volatile ioport unsigned int*)0x0CA3
#define       DMA_CSSA_L5	*(volatile ioport unsigned int*)0x0CA4
#define       DMA_CSSA_U5	*(volatile ioport unsigned int*)0x0CA5
#define       DMA_CDSA_L5	*(volatile ioport unsigned int*)0x0CA6
#define       DMA_CDSA_U5	*(volatile ioport unsigned int*)0x0CA7
#define       DMA_CEN5		*(volatile ioport unsigned int*)0x0CA8
#define       DMA_CFN5		*(volatile ioport unsigned int*)0x0CA9

//---------------------------------------------------------------  
//DATA PAGE 
//
#define       DP           *(volatile unsigned int *)0x0002e
#define       DPH          *(volatile unsigned int *)0x0002b
#define       PDP          *(volatile unsigned int *)0x0002f   
#define       SP           *(volatile unsigned int *)0x0004d
#define       SPH          *(volatile unsigned int *)0x0004e 
*/ 
//---------------------------------------------------------------  
// ASSISTANT REGISTERS 
#define       AR0          *(volatile unsigned int *)0x00010 
#define       AR1          *(volatile unsigned int *)0x00011 
#define       AR2          *(volatile unsigned int *)0x00012 
#define       AR3          *(volatile unsigned int *)0x00013 
#define       AR4          *(volatile unsigned int *)0x00014 
#define       AR5          *(volatile unsigned int *)0x00015 
#define       AR6          *(volatile unsigned int *)0x00016 
#define       AR7          *(volatile unsigned int *)0x00017

//访问CE0空间的串口通道1
#define       UART_RHR232  *(volatile unsigned int *)0x20000
#define       UART_THR232  *(volatile unsigned int *)0x20000
#define       UART_IER232  *(volatile unsigned int *)0x20001
#define		  UART_IIR232  *(volatile unsigned int *)0x20002
#define		  UART_FCR232  *(volatile unsigned int *)0x20002
#define		  UART_LCR232  *(volatile unsigned int *)0x20003
#define		  UART_MCR232  *(volatile unsigned int *)0x20004
#define		  UART_LSR232  *(volatile unsigned int *)0x20005
#define		  UART_MSR232  *(volatile unsigned int *)0x20006
#define		  UART_SPR232  *(volatile unsigned int *)0x20007
#define		  UART_DLL232  *(volatile unsigned int *)0x20000
#define		  UART_DLH232  *(volatile unsigned int *)0x20001
#define		  UART_EFR232  *(volatile unsigned int *)0x20002
#define		  UART_TCR232  *(volatile unsigned int *)0x20006
#define		  UART_TLR232  *(volatile unsigned int *)0x20007

//访问CE0空间的串口通道2
#define       UART_RHR485  *(volatile unsigned int *)0x200000
#define       UART_THR485  *(volatile unsigned int *)0x200000
#define       UART_IER485  *(volatile unsigned int *)0x200001
#define		  UART_IIR485  *(volatile unsigned int *)0x200002
#define		  UART_FCR485  *(volatile unsigned int *)0x200002
#define		  UART_LCR485  *(volatile unsigned int *)0x200003
#define		  UART_MCR485  *(volatile unsigned int *)0x200004
#define		  UART_LSR485  *(volatile unsigned int *)0x200005
#define		  UART_MSR485  *(volatile unsigned int *)0x200006
#define		  UART_SPR485  *(volatile unsigned int *)0x200007
#define		  UART_DLL485  *(volatile unsigned int *)0x200000
#define		  UART_DLH485  *(volatile unsigned int *)0x200001
#define		  UART_EFR485  *(volatile unsigned int *)0x200002
#define		  UART_TCR485  *(volatile unsigned int *)0x200006
#define		  UART_TLR485  *(volatile unsigned int *)0x200007

//---------------------------------------------------------------  
// AD  ----------------------------------------------------------
#define ADCCTL		*(volatile ioport unsigned int*)0x6800
#define ADCDATA		*(volatile ioport unsigned int*)0x6801
#define ADCCLKDIV	*(volatile ioport unsigned int*)0x6802
#define ADCCLKCTL   *(volatile ioport unsigned int*)0x6803

//---------------------------------------------------------------
//MMC/SD---------------------------------------------------------
#define MMCFCLK_1 	*(volatile ioport unsigned int*)0x4800
#define MMCCTL_1 	*(volatile ioport unsigned int*)0x4801
#define MMCCLK_1 	*(volatile ioport unsigned int*)0x4802
#define MMCST0_1 	*(volatile ioport unsigned int*)0x4803
#define MMCST1_1 	*(volatile ioport unsigned int*)0x4804
#define MMCIE_1 	*(volatile ioport unsigned int*)0x4805
#define MMCTOR_1 	*(volatile ioport unsigned int*)0x4806
#define MMCTOD_1 	*(volatile ioport unsigned int*)0x4807
#define MMCBLEN_1 	*(volatile ioport unsigned int*)0x4808
#define MMCNBLK_1 	*(volatile ioport unsigned int*)0x4809
#define MMCNBLC_1 	*(volatile ioport unsigned int*)0x480A
#define MMCDRR_1	*(volatile ioport unsigned int*)0x480B
#define MMCDXR_1 	*(volatile ioport unsigned int*)0x480C
#define MMCCMD_1 	*(volatile ioport unsigned int*)0x480D
#define MMCARGL_1 	*(volatile ioport unsigned int*)0x480E
#define MMCARGH_1 	*(volatile ioport unsigned int*)0x480F
#define MMCRSP0_1 	*(volatile ioport unsigned int*)0x4810
#define MMCRSP1_1 	*(volatile ioport unsigned int*)0x4811
#define MMCRSP2_1 	*(volatile ioport unsigned int*)0x4812
#define MMCRSP3_1 	*(volatile ioport unsigned int*)0x4813
#define MMCRSP4_1 	*(volatile ioport unsigned int*)0x4814
#define MMCRSP5_1 	*(volatile ioport unsigned int*)0x4815
#define MMCRSP6_1 	*(volatile ioport unsigned int*)0x4816
#define MMCRSP7_1 	*(volatile ioport unsigned int*)0x4817
#define MMCDRSP_1 	*(volatile ioport unsigned int*)0x4818
#define MMCCIDX_1 	*(volatile ioport unsigned int*)0x481A

#endif //_REGISTER_H 
