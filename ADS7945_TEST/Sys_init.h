/*
 * Sys_init.h
 *
 *  Created on: 2017-2-22
 *      Author: zhjl769
 */

#ifndef SYS_INIT_H_
#define SYS_INIT_H_


/* Pick which EDMA transfer completion interrupt we want to use */
#define TCCINTNUM   10
#define BUFF_SZ  1024         /* ping-pong buffer sizes in # of ints  */

/* Create the buffers. We want to align the buffers to be cache friendly */
/* by aligning them on an L2 cache line boundary.                        */
#pragma DATA_ALIGN(ping,128);     //把ping变量的地址安排在128字节对齐的位置上，也即ping地址的低7位一定为0，
#pragma DATA_ALIGN(pong,128);
#pragma DATA_ALIGN(outbuff,128);
Int16 ping[BUFF_SZ];
Int16 pong[BUFF_SZ];
Int16 outbuff[4000];


/* global variable used to track the ping-pong'ing */
static Int16 pingpong = 0;
static Int32 Buffdata = 0;
Uint16 Buffflag = 0;



/* declare some CSL objects */
MCBSP_Handle My_Mcbsp0;ll
MCBSP_Handle My_Mcbsp1;
EDMA_Handle hEdma;     /* Handle for the EDMA channe0                 */
EDMA_Handle hEdmaPing; /* Handle for the ping EDMA reload parameters  */
EDMA_Handle hEdmaPong; /* Handle for the pong EDMA reload parameters  */
EDMA_Config cfgEdma;   /* EDMA configuration structure                */


/* Create the EDMA configuration structure for ping transfers */
EDMA_Config Ping_EDMA_config = {
		EDMA_OPT_RMK(
				EDMA_OPT_PRI_LOW,                // pri,esize,ds2,sum,dd2,dum,tcint,tcc,tccm,atcint,atcc, pdts,pdtd,link,fs
				EDMA_OPT_ESIZE_16BIT,
				EDMA_OPT_2DS_NO,
				EDMA_OPT_SUM_NONE,               //源地址不变
				EDMA_OPT_2DD_NO,
				EDMA_OPT_DUM_INC,
				EDMA_OPT_TCINT_YES,
				EDMA_OPT_TCC_OF(TCCINTNUM),
				EDMA_OPT_TCCM_DEFAULT,
				EDMA_OPT_ATCINT_DEFAULT,
				EDMA_OPT_ATCC_OF(TCCINTNUM),
				EDMA_OPT_PDTS_DEFAULT,
				EDMA_OPT_PDTD_DEFAULT,
				EDMA_OPT_LINK_YES,
				EDMA_OPT_FS_YES
		),
		EDMA_SRC_OF(MCBSP_ADDR(DRR0)),
		EDMA_CNT_OF(BUFF_SZ),
		EDMA_DST_OF(&ping),
		EDMA_IDX_OF(0x00000004),
		EDMA_RLD_OF(0x00000000)
};

/* Create the EDMA configuration structure for ping transfers */
EDMA_Config Pong_EDMA_config = {
		EDMA_OPT_RMK(
				EDMA_OPT_PRI_LOW,                // pri,esize,ds2,sum,dd2,dum,tcint,tcc,tccm,atcint,atcc, pdts,pdtd,link,fs
				EDMA_OPT_ESIZE_16BIT,
				EDMA_OPT_2DS_NO,
				EDMA_OPT_SUM_NONE,               //源地址不变
				EDMA_OPT_2DD_NO,
				EDMA_OPT_DUM_INC,
				EDMA_OPT_TCINT_YES,
				EDMA_OPT_TCC_OF(TCCINTNUM),
				EDMA_OPT_TCCM_DEFAULT,
				EDMA_OPT_ATCINT_DEFAULT,
				EDMA_OPT_ATCC_OF(TCCINTNUM),
				EDMA_OPT_PDTS_DEFAULT,
				EDMA_OPT_PDTD_DEFAULT,
				EDMA_OPT_LINK_YES,
				EDMA_OPT_FS_YES
		),
		EDMA_SRC_OF(MCBSP_ADDR(DRR0)),
		EDMA_CNT_OF(BUFF_SZ),
		EDMA_DST_OF(&pong),
		EDMA_IDX_OF(0x00000004),
		EDMA_RLD_OF(0x00000000)
};

/* create a config structure for digital loopback mode */
static MCBSP_Config MY_MCBSP_Config = {
		MCBSP_SPCR_RMK(                              //free,soft,frst,grst,xintm,xsyncerr,xrst,dlb,rjust, clkstp,dxena,rintm,rsyncerr,rrst
				MCBSP_SPCR_FREE_NO,
				MCBSP_SPCR_SOFT_NO,
				MCBSP_SPCR_FRST_NO,
				MCBSP_SPCR_GRST_NO,
				MCBSP_SPCR_XINTM_XRDY,
				MCBSP_SPCR_XSYNCERR_NO,
				MCBSP_SPCR_XRST_YES,
				MCBSP_SPCR_DLB_OFF,
				MCBSP_SPCR_RJUST_LZF,
				MCBSP_SPCR_CLKSTP_DISABLE,
				MCBSP_SPCR_DXENA_OFF,
				MCBSP_SPCR_RINTM_RRDY,
				MCBSP_SPCR_RSYNCERR_NO,
				MCBSP_SPCR_RRST_NO
		),
		MCBSP_RCR_RMK(
				MCBSP_RCR_RPHASE_SINGLE,
				MCBSP_RCR_RFRLEN2_OF(0),
				MCBSP_RCR_RWDLEN2_16BIT,
				MCBSP_RCR_RCOMPAND_MSB,
				MCBSP_RCR_RFIG_YES,
				MCBSP_RCR_RDATDLY_0BIT,
				MCBSP_RCR_RFRLEN1_OF(1),
				MCBSP_RCR_RWDLEN1_16BIT,
				MCBSP_RCR_RWDREVRS_DISABLE
		),
		MCBSP_XCR_RMK(
				MCBSP_XCR_XPHASE_SINGLE,
				MCBSP_XCR_XFRLEN2_OF(0),
				MCBSP_XCR_XWDLEN2_8BIT,
				MCBSP_XCR_XCOMPAND_MSB,
				MCBSP_XCR_XFIG_YES,
				MCBSP_XCR_XDATDLY_0BIT,
				MCBSP_XCR_XFRLEN1_OF(0),
				MCBSP_XCR_XWDLEN1_32BIT,
				MCBSP_XCR_XWDREVRS_DISABLE
		),
		MCBSP_SRGR_RMK(
				MCBSP_SRGR_GSYNC_FREE,
				MCBSP_SRGR_CLKSP_RISING,
				MCBSP_SRGR_CLKSM_INTERNAL,
				MCBSP_SRGR_FSGM_FSG,
				MCBSP_SRGR_FPER_OF(99),
				MCBSP_SRGR_FWID_OF(88),       //在ADS7945中，此值为FPER的值减去需要的bit数，最好加上5~8，如16bit为99-16+6 = 88
				MCBSP_SRGR_CLKGDV_OF(149)      //Mcbsp输入时钟为CLK/4,即150MHz,此为Mcbsp的输出时钟，CLK/4/Value
		),
		MCBSP_MCR_DEFAULT,
		MCBSP_RCERE0_DEFAULT,
		MCBSP_RCERE1_DEFAULT,
		MCBSP_RCERE2_DEFAULT,
		MCBSP_RCERE3_DEFAULT,
		MCBSP_XCERE0_DEFAULT,
		MCBSP_XCERE1_DEFAULT,
		MCBSP_XCERE2_DEFAULT,
		MCBSP_XCERE3_DEFAULT,
		MCBSP_PCR_RMK(
				MCBSP_PCR_XIOEN_SP,
				MCBSP_PCR_RIOEN_SP,
				MCBSP_PCR_FSXM_INTERNAL,
				MCBSP_PCR_FSRM_EXTERNAL,
				MCBSP_PCR_CLKXM_OUTPUT,
				MCBSP_PCR_CLKRM_INPUT,
				MCBSP_PCR_CLKSSTAT_0,
				MCBSP_PCR_DXSTAT_0,
				MCBSP_PCR_FSXP_ACTIVELOW,
				MCBSP_PCR_FSRP_ACTIVELOW,
				MCBSP_PCR_CLKXP_RISING,
				MCBSP_PCR_CLKRP_FALLING
		),
};


#endif /* SYS_INIT_H_ */
