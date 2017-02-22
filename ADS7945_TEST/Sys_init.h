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
				DMA_DMACSDP_DSTBEN_NOBURST,//��ַ�����
				DMA_DMACSDP_DSTPACK_OFF,
				DMA_DMACSDP_DST_DARAM,//Ŀ�ĵ�ַΪDARAM
				DMA_DMACSDP_SRCBEN_NOBURST,
				DMA_DMACSDP_SRCPACK_OFF,
				DMA_DMACSDP_SRC_PERIPH,//Դ��ַΪ����
				DMA_DMACSDP_DATATYPE_16BIT//16bit����
		),                                       /* DMACSDP  */
		DMA_DMACCR_RMK(
				DMA_DMACCR_DSTAMODE_POSTINC,//Ŀ�ĵ�ַ�Լ�
				DMA_DMACCR_SRCAMODE_CONST,//Դ��ַΪ����
				DMA_DMACCR_ENDPROG_OFF,
				DMA_DMACCR_REPEAT_OFF,
				DMA_DMACCR_AUTOINIT_ON,//���������ȴ�ENDPROG��Ϊ1�����´δ���
				DMA_DMACCR_EN_STOP,//��ֹDMA����
				DMA_DMACCR_PRIO_LOW,//���ȼ�Ϊ��
				DMA_DMACCR_FS_DISABLE,//���ݵ�Ԫ����ȴ�ͬ���¼�
				DMA_DMACCR_SYNC_REVT0//ͬ���¼�ΪMcBSP0�����ж�
		),                                       /* DMACCR   */
		DMA_DMACICR_RMK(
				DMA_DMACICR_BLOCKIE_OFF,
				DMA_DMACICR_LASTIE_OFF,
				DMA_DMACICR_FRAMEIE_ON,//֡�ж�ʹ��
				DMA_DMACICR_FIRSTHALFIE_OFF,
				DMA_DMACICR_DROPIE_OFF,
				DMA_DMACICR_TIMEOUTIE_OFF
		),                                       /* DMACICR  */
		(DMA_AdrPtr)(MCBSP_ADDR(DRR11)),        /* DMACSSAL *///McBSP�������ݼĴ���
		0,                                     /* DMACSSAU */
		(DMA_AdrPtr) &Rev1,                   /* DMACDSAL *///��������
		0,                                     /* DMACDSAU */
		BUF_LEN_MAX,                                     /* DMACEN   *///�������ݳ���
		1,                                     /* DMACFN   */
		0,                                     /* DMACFI  */
		0                                     /* DMACEI  */
};


/* Create DMA Receive Side Configuration */
MCBSP_config  MY_MCBSP_Config = {
		EDMA_EDMACSDP_RMK(
				DMA_DMACSDP_DSTBEN_NOBURST,//��ַ�����
				DMA_DMACSDP_DSTPACK_OFF,
				DMA_DMACSDP_DST_DARAM,//Ŀ�ĵ�ַΪDARAM
				DMA_DMACSDP_SRCBEN_NOBURST,
				DMA_DMACSDP_SRCPACK_OFF,
				DMA_DMACSDP_SRC_PERIPH,//Դ��ַΪ����
				DMA_DMACSDP_DATATYPE_16BIT//16bit����
		),                                       /* DMACSDP  */
		DMA_DMACCR_RMK(
				DMA_DMACCR_DSTAMODE_POSTINC,//Ŀ�ĵ�ַ�Լ�
				DMA_DMACCR_SRCAMODE_CONST,//Դ��ַΪ����
				DMA_DMACCR_ENDPROG_OFF,
				DMA_DMACCR_REPEAT_OFF,
				DMA_DMACCR_AUTOINIT_ON,//���������ȴ�ENDPROG��Ϊ1�����´δ���
				DMA_DMACCR_EN_STOP,//��ֹDMA����
				DMA_DMACCR_PRIO_LOW,//���ȼ�Ϊ��
				DMA_DMACCR_FS_DISABLE,//���ݵ�Ԫ����ȴ�ͬ���¼�
				DMA_DMACCR_SYNC_REVT0//ͬ���¼�ΪMcBSP0�����ж�
		),                                       /* DMACCR   */
		DMA_DMACICR_RMK(
				DMA_DMACICR_BLOCKIE_OFF,
				DMA_DMACICR_LASTIE_OFF,
				DMA_DMACICR_FRAMEIE_ON,//֡�ж�ʹ��
				DMA_DMACICR_FIRSTHALFIE_OFF,
				DMA_DMACICR_DROPIE_OFF,
				DMA_DMACICR_TIMEOUTIE_OFF
		),                                       /* DMACICR  */
		(DMA_AdrPtr)(MCBSP_ADDR(DRR11)),        /* DMACSSAL *///McBSP�������ݼĴ���
		0,                                     /* DMACSSAU */
		(DMA_AdrPtr) &Rev1,                   /* DMACDSAL *///��������
		0,                                     /* DMACDSAU */
		BUF_LEN_MAX,                                     /* DMACEN   *///�������ݳ���
		1,                                     /* DMACFN   */
		0,                                     /* DMACFI  */
		0                                     /* DMACEI  */
};



#endif /* SYS_INIT_H_ */
