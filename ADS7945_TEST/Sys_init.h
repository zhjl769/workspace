/*
 * Sys_init.h
 *
 *  Created on: 2017-2-22
 *      Author: zhjl769
 */

#ifndef SYS_INIT_H_
#define SYS_INIT_H_

/* Create DMA Receive Side Configuration */
DMA_Config  myConfig = {
		EDMA_EDMACSDP_RMK(
				DMA_DMACSDP_DSTBEN_NOBURST,//地址不打包
				DMA_DMACSDP_DSTPACK_OFF,
				DMA_DMACSDP_DST_DARAM,//目的地址为DARAM
				DMA_DMACSDP_SRCBEN_NOBURST,
				DMA_DMACSDP_SRCPACK_OFF,
				DMA_DMACSDP_SRC_PERIPH,//源地址为外设
				DMA_DMACSDP_DATATYPE_16BIT//16bit数据
		),                                       /* DMACSDP  */
		DMA_DMACCR_RMK(
				DMA_DMACCR_DSTAMODE_POSTINC,//目的地址自加
				DMA_DMACCR_SRCAMODE_CONST,//源地址为常数
				DMA_DMACCR_ENDPROG_OFF,
				DMA_DMACCR_REPEAT_OFF,
				DMA_DMACCR_AUTOINIT_ON,//传输结束后等待ENDPROG变为1进行下次传输
				DMA_DMACCR_EN_STOP,//禁止DMA传输
				DMA_DMACCR_PRIO_LOW,//优先级为低
				DMA_DMACCR_FS_DISABLE,//数据单元传输等待同步事件
				DMA_DMACCR_SYNC_REVT0//同步事件为McBSP0接收中断
		),                                       /* DMACCR   */
		DMA_DMACICR_RMK(
				DMA_DMACICR_BLOCKIE_OFF,
				DMA_DMACICR_LASTIE_OFF,
				DMA_DMACICR_FRAMEIE_ON,//帧中断使能
				DMA_DMACICR_FIRSTHALFIE_OFF,
				DMA_DMACICR_DROPIE_OFF,
				DMA_DMACICR_TIMEOUTIE_OFF
		),                                       /* DMACICR  */
		(DMA_AdrPtr)(MCBSP_ADDR(DRR11)),        /* DMACSSAL *///McBSP接收数据寄存器
		0,                                     /* DMACSSAU */
		(DMA_AdrPtr) &Rev1,                   /* DMACDSAL *///接收数组
		0,                                     /* DMACDSAU */
		BUF_LEN_MAX,                                     /* DMACEN   *///接收数据长度
		1,                                     /* DMACFN   */
		0,                                     /* DMACFI  */
		0                                     /* DMACEI  */
};


/* Create DMA Receive Side Configuration */
MCBSP_config  MY_MCBSP_Config = {
		EDMA_EDMACSDP_RMK(
				DMA_DMACSDP_DSTBEN_NOBURST,//地址不打包
				DMA_DMACSDP_DSTPACK_OFF,
				DMA_DMACSDP_DST_DARAM,//目的地址为DARAM
				DMA_DMACSDP_SRCBEN_NOBURST,
				DMA_DMACSDP_SRCPACK_OFF,
				DMA_DMACSDP_SRC_PERIPH,//源地址为外设
				DMA_DMACSDP_DATATYPE_16BIT//16bit数据
		),                                       /* DMACSDP  */
		DMA_DMACCR_RMK(
				DMA_DMACCR_DSTAMODE_POSTINC,//目的地址自加
				DMA_DMACCR_SRCAMODE_CONST,//源地址为常数
				DMA_DMACCR_ENDPROG_OFF,
				DMA_DMACCR_REPEAT_OFF,
				DMA_DMACCR_AUTOINIT_ON,//传输结束后等待ENDPROG变为1进行下次传输
				DMA_DMACCR_EN_STOP,//禁止DMA传输
				DMA_DMACCR_PRIO_LOW,//优先级为低
				DMA_DMACCR_FS_DISABLE,//数据单元传输等待同步事件
				DMA_DMACCR_SYNC_REVT0//同步事件为McBSP0接收中断
		),                                       /* DMACCR   */
		DMA_DMACICR_RMK(
				DMA_DMACICR_BLOCKIE_OFF,
				DMA_DMACICR_LASTIE_OFF,
				DMA_DMACICR_FRAMEIE_ON,//帧中断使能
				DMA_DMACICR_FIRSTHALFIE_OFF,
				DMA_DMACICR_DROPIE_OFF,
				DMA_DMACICR_TIMEOUTIE_OFF
		),                                       /* DMACICR  */
		(DMA_AdrPtr)(MCBSP_ADDR(DRR11)),        /* DMACSSAL *///McBSP接收数据寄存器
		0,                                     /* DMACSSAU */
		(DMA_AdrPtr) &Rev1,                   /* DMACDSAL *///接收数组
		0,                                     /* DMACDSAU */
		BUF_LEN_MAX,                                     /* DMACEN   *///接收数据长度
		1,                                     /* DMACFN   */
		0,                                     /* DMACFI  */
		0                                     /* DMACEI  */
};



#endif /* SYS_INIT_H_ */
