/*
 * main.c
 */
/*************************************************/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_edma.h>
#include <csl_pll.h>
#include <csl_irq.h>
#include <csl_gpio.h>
#include "Sys_init.h"
#include "Interrupt.h"
#include "Function.h"

/*---------Function prototypes---------*/
extern far void vectors();

void main(void)
{
	CSL_init();
	IRQ_setVecs(vectors);
	IRQ_nmiEnable();
	IRQ_globalDisable();
	Build_Frenal(200, 60, 60, 20, outbuff);
	IRQ_map(IRQ_EVT_EDMAINT, 8);
	IRQ_reset(IRQ_EVT_EDMAINT);    //清除EDMA中断
	IRQ_map(IRQ_EVT_RINT0, 13);
	IRQ_reset(IRQ_EVT_RINT0);    //清除EDMA中断
	EDMA_clearPram(0x00000000);     //该函数用指定的值清除所有EDMA参数RAM
	/* Lets open up the EDMA channel associated with Mcbsp0_rev #1. */
	hEdma = EDMA_open(EDMA_CHA_REVT0, EDMA_OPEN_RESET);
	/* We also need two EDMA reload parameter sets so let's allocate them */
	/* here. Notice the -1, this means allocate any availale tale.        */
	hEdmaPing = EDMA_allocTable(-1);
	hEdmaPong = EDMA_allocTable(-1);
	cfgEdma = Ping_EDMA_config;

	/* Let's initialize the link fields of the configuration structures */
	Ping_EDMA_config.rld = EDMA_RLD_RMK(0,hEdmaPing);
	Pong_EDMA_config.rld = EDMA_RLD_RMK(0,hEdmaPong);
	cfgEdma.rld     = EDMA_RLD_RMK(0,hEdmaPing);

	/* Now let's program up the EDMA channel with the configuration structure */
	EDMA_config(hEdma, &cfgEdma);

	/* Let's also configure the reload parameter tables in the EDMA PRAM */
	/* with the values in the configuration structures.                  */
	EDMA_config(hEdmaPing, &Ping_EDMA_config);
	EDMA_config(hEdmaPong, &Pong_EDMA_config);
	/* Enable the related interrupts */
	IRQ_enable(IRQ_EVT_EDMAINT);
	EDMA_intDisable(TCCINTNUM);    //该函数通过修改CIPR来清除传输完成中断标志
	EDMA_intClear(TCCINTNUM);      //该函数通过修改CIPR来清除传输完成中断标志
	EDMA_intEnable(TCCINTNUM);

	/* Enable the EDMA channel 0 */
	EDMA_enableChannel(hEdma);

	memset(ping,0,sizeof(int)*BUFF_SZ);
	memset(pong,0,sizeof(int)*BUFF_SZ);
	My_Mcbsp0 = MCBSP_open(MCBSP_DEV0, MCBSP_OPEN_RESET);
	My_Mcbsp1 = MCBSP_open(MCBSP_DEV1, MCBSP_OPEN_RESET);
	MCBSP_config(My_Mcbsp0, &MY_MCBSP_Config);
	MCBSP_config(My_Mcbsp1, &MY_MCBSP_Config);
	IRQ_disable(IRQ_EVT_EDMAINT);
	IRQ_disable(IRQ_EVT_XINT1);
	IRQ_enable(IRQ_EVT_RINT0);
	MCBSP_start(My_Mcbsp0, MCBSP_XMIT_START | MCBSP_SRGR_START| MCBSP_SRGR_FRAMESYNC, MCBSP_SRGR_DEFAULT_DELAY);
	MCBSP_start(My_Mcbsp1, MCBSP_XMIT_START | MCBSP_SRGR_START| MCBSP_SRGR_FRAMESYNC, MCBSP_SRGR_DEFAULT_DELAY);
	IRQ_globalEnable();
	while(1)
		;
}


