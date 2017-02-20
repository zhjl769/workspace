/*
 * init.h
 *
 *  Created on: 2016-11-7
 *      Author: Administrator
 */

#ifndef INIT_H_
#define INIT_H_



//#define IODATA *(volatile ioport unsigned int *)0x03400
#include <csl_dma.h>
#include <csl_timer.h>
#include "variable.h"
#include <csl_gpio.h>


/*************************************************************************************************/
/********************************************ϵͳʱ�ӳ�ʼ��******************************************/
/*************************************************************************************************/
void CLK_init(void) //���庯��CLK_init������һ�������ͣ��Ҹú�������������
{
	ioport unsigned int *clkmd;
	clkmd = (unsigned int *)0x1c00; //�ѵ�ַ0x1c00����ǿ������ת����ֵ��ָ��clkmd��0x1c00Ϊʱ�ӷ������Ĵ����Ķ˿ڵ�ַ��
	*clkmd = 0x2CB2;//8M����<0x2CB2=100MHz><0x2c92=200MHz><0x2912=144MHz>
}



/************************************************************************************************/
/*************************************************SDRAM��ʼ��*************************************/
/************************************************************************************************/
void SDRAM_init(void)
{
	ioport unsigned int *ebsr  = (unsigned int *)0x6c00;//�ⲿ����ѡ��Ĵ���������HPI��EMIF��
	ioport unsigned int *egcr  = (unsigned int *)0x0800;
	ioport unsigned int *emirst = (unsigned int *)0x0801;
	ioport unsigned int *emibe = (unsigned int *)0x0802;
	ioport unsigned int *ce01  = (unsigned int *)0x0803;
	ioport unsigned int *ce02  = (unsigned int *)0x0804;
	ioport unsigned int *ce03  = (unsigned int *)0x0805;
	ioport unsigned int *ce11  = (unsigned int *)0x0806;
	ioport unsigned int *ce12  = (unsigned int *)0x0807;
	ioport unsigned int *ce13  = (unsigned int *)0x0808;
	ioport unsigned int *ce21  = (unsigned int *)0x0809;
	ioport unsigned int *ce22  = (unsigned int *)0x080A;
	ioport unsigned int *ce23  = (unsigned int *)0x080B;
	ioport unsigned int *ce31  = (unsigned int *)0x080C;
	ioport unsigned int *ce32  = (unsigned int *)0x080D;
	ioport unsigned int *ce33  = (unsigned int *)0x080E;
	ioport unsigned int *sdc1  = (unsigned int *)0x080F;
	ioport unsigned int *sdper = (unsigned int *)0x0810;
	ioport unsigned int *sdcnt = (unsigned int *)0x0811;
	ioport unsigned int *init  = (unsigned int *)0x0812;
	ioport unsigned int *sdc2  = (unsigned int *)0x0813;
	//ioport unsigned int *sdc3  = (unsigned int *)0x0814;  // �˼Ĵ���ֻ����5510,5509����

	*ebsr   = 0x0211;    // full emif 23d

	*egcr   = 0x0210;         //0x0210Ƶ�ʵ���ϵͳʱ�ӵ�1/2��0x0020Ƶ�ʵ���ϵͳʱ��

	//*ce01   = 0x3FFF;        // CE0_1:  CE0 space control register 1
	//         MTYPE = 011, Synchronous DRAM (SDRAM), 16-bit data bus width
	//*ce02   = 0xFFFF;   // CE0_2:  CE0 space control register 2
	//*ce03   = 0x00FF;   // CE0_3:  CE0 space control register 3

	//*ce11   = 0x3FFF;        // CE1_1:  CE1 space control register 1
	//         MTYPE = 011, Synchronous DRAM (SDRAM), 16-bit data bus width
	//*ce12   = 0xFFFF;   // CE1_2:  CE1 space control register 2
	//*ce13   = 0x00FF;   // CE1_3:  CE1 space control register 3

	*ce21   = 0x3000;        // CE0_1:  CE2 space control register 1
	//*ce22   = 0xFFFF;   // CE0_2:  CE2 space control register 2
	//*ce23   = 0x00FF;   // CE0_3:  CE2 space control register 3

	*ce31   = 0x3000;        // CE3_1:  CE3 space control register 1
	//*ce32   = 0xFFFF;   // CE3_2:  CE3 space control register 2
	//*ce33   = 0x00FF;   // CE3_3:  CE3 space control register 3

	*emirst = 0xFFFF;    // EMIRST: any write to this register resets the EMIF state machine
	*sdc1   = 0x2111;
	*sdc2   = 0x0231;
	//*sdc3   = 0x0003;
	*sdper  = 0x030D;

	*egcr   = *egcr | 0x0020;     // the MEMCEN = 1
	*init   = 0x0000;//ΪSDRAM����ֵ
}


/********************************************************************************************/
/*************************************DMA��ʼ��&��ʱ����ʼ��*************************************/
/********************************************************************************************/
/*DMA_Config  dmaConfigForRev = { //��������(δ�޸�20161110)
  DMA_DMACSDP_RMK(
    DMA_DMACSDP_DSTBEN_NOBURST,//��ַ�����
    DMA_DMACSDP_DSTPACK_OFF,
    DMA_DMACSDP_DST_DARAM,//Ŀ�ĵ�ַΪDARAM
    DMA_DMACSDP_SRCBEN_NOBURST,
    DMA_DMACSDP_SRCPACK_OFF,
    DMA_DMACSDP_SRC_PERIPH,//Դ��ַΪ����
    DMA_DMACSDP_DATATYPE_16BIT//16bit����
  ),                                       //DMACSDP
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
  ),                                       //DMACCR
  DMA_DMACICR_RMK(
    DMA_DMACICR_BLOCKIE_OFF,
    DMA_DMACICR_LASTIE_OFF,
    DMA_DMACICR_FRAMEIE_ON,//֡�ж�ʹ��
    DMA_DMACICR_FIRSTHALFIE_OFF,
    DMA_DMACICR_DROPIE_OFF,
    DMA_DMACICR_TIMEOUTIE_OFF
  ),                                       //DMACICR
    (DMA_AdrPtr)(MCBSP_ADDR(DRR11)),        //DMACSSAL McBSP�������ݼĴ���
    0,                                     //DMACSSAU
    (DMA_AdrPtr)&Rev1,                      //DMACDSAL ��������
    0,                                     //DMACDSAU
    BUF_LEN_MAX,                           //DMACEN   �������ݳ���
    1,                                     //DMACFN
    0,                                     //DMACFI
    0                                     //DMACEI
};*/
DMA_Config  dmaConfigForSend = { //��������
  DMA_DMACSDP_RMK(
    DMA_DMACSDP_DSTBEN_NOBURST,    /*Ŀ���ַͻ��ģʽ����ʹ��*/
    DMA_DMACSDP_DSTPACK_OFF,       /*Ŀ���ַ�������ʹ��*/
    DMA_DMACSDP_DST_PERIPH,         /*ѡ��Ŀ�����ͣ�����(GPIO)*/
    DMA_DMACSDP_SRCBEN_NOBURST,    /*Դ��ַͻ��ģʽ����ʹ��*/
    DMA_DMACSDP_SRCPACK_OFF,       /*Դ��ַ�������ʹ��*/
    DMA_DMACSDP_SRC_EMIF,         /*ѡ��Դ���ͣ�EMIF��SDRAM��*/
    DMA_DMACSDP_DATATYPE_16BIT     /*ͨ��������������ͣ�16BIT*/
  ),                                       /* DMACSDP�Ĵ���*/
  DMA_DMACCR_RMK(
    DMA_DMACCR_DSTAMODE_CONST,   /*Ŀ���ַѰַģʽ��������ַ*/
    DMA_DMACCR_SRCAMODE_POSTINC,   /*Դ��ַѰַģʽ����ַ�Լ�*/
    DMA_DMACCR_ENDPROG_OFF,         /*��̽�����־λ*/
    DMA_DMACCR_REPEAT_OFF,         /*ѭ���������ȴ�ENPROG=1*/
    DMA_DMACCR_AUTOINIT_ON,        /*�Զ���ʼ��������*/
    DMA_DMACCR_EN_STOP,            /*ͨ��ʹ�ܿ���λ*/
    DMA_DMACCR_PRIO_HI,            /*ͨ�����ȼ�����*/
    DMA_DMACCR_FS_ELEMENT,         /*ͬ�����ͣ���Ԫͬ��*/
    DMA_DMACCR_SYNC_TIM2INT           /*ͬ��Դ��������2*/
  ),                                       /* DMACCR�Ĵ���*/
  DMA_DMACICR_RMK(
    DMA_DMACICR_BLOCKIE_OFF,       /*���п��ж�ʹ��λ*/
    DMA_DMACICR_LASTIE_OFF,        /*���֡�ж�ʹ��λ*/
    DMA_DMACICR_FRAMEIE_ON,        /*����֡�ж�ʹ��λ*/
    DMA_DMACICR_FIRSTHALFIE_OFF,   /*��֡�ж�ʹ��λ*/
    DMA_DMACICR_DROPIE_OFF,        /*ͬ���¼������ж�ʹ��λ*/
    DMA_DMACICR_TIMEOUTIE_OFF      /*��ʱ�ж�ʹ��λ*/
  ),                                      /* DMACICR�Ĵ���*/
    (DMA_AdrPtr)&Send_sig,                /* DMACSSALԴ��ʼ��ַ�Ĵ�������λ�� *///CE2�ռ�
    0,                     				 /* DMACSSAUԴ��ʼ��ַ�Ĵ�������λ��  */
    (DMA_AdrPtr)GPIO_ADDR(IODATA),                   /* DMACDSALĿ����ʼ��ַ�Ĵ�������λ��  */
    0,                                     /* DMACDSAUĿ����ʼ��ַ�Ĵ�������λ��  */
    4096,		                           /* DMACEN��Ԫ���Ĵ���   */
    1,                                     /* DMACFN֡���Ĵ���   */
    0,                                     /* DMACFI��Ԫ�����Ĵ���   */
    0                                      /* DMACEI֡�����Ĵ���   */
};
#define TIMER_CTRL0    TIMER_TCR_RMK(\
                      TIMER_TCR_IDLEEN_DEFAULT,    /* IDLEEN == 0 */ \
                      TIMER_TCR_FUNC_OF(01),        /* FUNC   == 00CPUʱ��Դ */ \
                      TIMER_TCR_TLB_RESET,         /* TLB    == 1 */ \
                      TIMER_TCR_SOFT_BRKPTNOW,     /* SOFT   == 0 */ \
                      TIMER_TCR_FREE_WITHSOFT,     /* FREE   == 0 */ \
                      TIMER_TCR_PWID_OF(0),        /* PWID   == 01 */ \
                      TIMER_TCR_ARB_RESET,         /* ARB    == 1 */ \
                      TIMER_TCR_TSS_STOP,         /* TSS    == 1 */ \
                      TIMER_TCR_CP_CLOCK,          /* CP     == 0 */ \
                      TIMER_TCR_POLAR_HIGH,         /* POLAR  == 1 */ \
                      TIMER_TCR_DATOUT_0           /* DATOUT == 0 */ \
)

#define TIMER_CTRL1    TIMER_TCR_RMK(\
                      TIMER_TCR_IDLEEN_DEFAULT,    /* IDLEEN == 0 */ \
                      TIMER_TCR_FUNC_OF(00),        /* FUNC   == 00CPUʱ��Դ */ \
                      TIMER_TCR_TLB_RESET,         /* TLB    == 1 */ \
                      TIMER_TCR_SOFT_BRKPTNOW,     /* SOFT   == 0 */ \
                      TIMER_TCR_FREE_WITHSOFT,     /* FREE   == 0 */ \
                      TIMER_TCR_PWID_OF(0),        /* PWID   == 01 */ \
                      TIMER_TCR_ARB_RESET,         /* ARB    == 1 */ \
                      TIMER_TCR_TSS_STOP,         /* TSS    == 1 */ \
                      TIMER_TCR_CP_PULSE,          /* CP     == 0 */ \
                      TIMER_TCR_POLAR_HIGH,         /* POLAR  == 1 */ \
                      TIMER_TCR_DATOUT_0           /* DATOUT == 0 */ \
)

TIMER_Config timCfg0 = {	//Timer0Ϊ100ms  �ⲿʱ��Ƶ��=10MHz
   TIMER_CTRL0,               /* TCR0 */
   0xf423u,                  /* PRD0=62499 */
   0x000fu                    /* PRSC=15 */
};


TIMER_Config timCfg1 = {	//Timer1Ϊ1uS
   TIMER_CTRL1,               /* TCR0 */
   0x0063u,                  /* PRD0 200MHz->0x00c7; 144MHz->0x008f;100MHz->0x0063*/
   0x0000                    /* PRSC */
};
void DMA_init(void)
{/*
	RevsrcAddrHi = (Uint16)(((Uint32)(MCBSP_ADDR(DRR10))) >> 15) & 0xFFFFu;//McBSP���ռĴ������ֽڵ�ַ
    RevsrcAddrLo = (Uint16)(((Uint32)(MCBSP_ADDR(DRR10))) << 1) & 0xFFFFu;//McBSP���ռĴ������ֽڵ�ַ
    Revdst1AddrHi = (Uint16)(((Uint32)(&Rev1)) >> 15) & 0xFFFFu;//�ص���������1���ֽڵ�ַ
    Revdst1AddrLo = (Uint16)(((Uint32)(&Rev1)) << 1) & 0xFFFFu;//�ص���������1���ֽڵ�ַ
	Revdst2AddrHi = (Uint16)(((Uint32)(&Rev2)) >> 15) & 0xFFFFu;//�ص���������2���ֽڵ�ַ
    Revdst2AddrLo = (Uint16)(((Uint32)(&Rev2)) << 1) & 0xFFFFu;//�ص���������2���ֽڵ�ַ

    myConfig.dmacssal = (DMA_AdrPtr)RevsrcAddrLo;//DMAͨ��Դ��ַ���ֽ�
    myConfig.dmacssau = RevsrcAddrHi;//DMAͨ��Դ��ַ���ֽ�
    myConfig.dmacdsal = (DMA_AdrPtr)Revdst1AddrLo;//DMAͨ��Ŀ�ĵ�ַ���ֽ�
    myConfig.dmacdsau = Revdst1AddrHi;//DMAͨ��Ŀ�ĵ�ַ���ֽ�
*/
	sendsrc0AddrHi = (Uint16)(((Uint32)(dmaConfigForSend.dmacssal)) >> 15) & 0xFFFFu;//DMA0����Դ��ַ���ֽ�
    sendsrc0AddrLo = (Uint16)(((Uint32)(dmaConfigForSend.dmacssal)) << 1) & 0xFFFFu;//DMA0����Դ��ַ���ֽ�
    senddst0AddrHi = (Uint16)(((Uint32)(dmaConfigForSend.dmacdsal)) >> 15) & 0xFFFFu;//DMA0����Ŀ�ĵ�ַ���ֽ�
    senddst0AddrLo = (Uint16)(((Uint32)(dmaConfigForSend.dmacdsal)) << 1) & 0xFFFFu;//DMA0����Ŀ�ĵ�ַ���ֽ�

    dmaConfigForSend.dmacssal = (DMA_AdrPtr)sendsrc0AddrLo;//����Դ��ַEMIF
    dmaConfigForSend.dmacssau = sendsrc0AddrHi;
    dmaConfigForSend.dmacdsal = (DMA_AdrPtr)senddst0AddrLo;//����Ŀ�ĵ�ַGPIO
    dmaConfigForSend.dmacdsau = senddst0AddrHi;

    hDmaSend0 = DMA_open(DMA_CHA0,DMA_OPEN_RESET);//DMA0����GPIO����

    //myhDma = DMA_open(DMA_CHA2,DMA_OPEN_RESET);  //DMA2����McBSP�ɼ�����

	mhTimer0 = TIMER_open(TIMER_DEV0, TIMER_OPEN_RESET);//��ʱ��0��Ϊͬ��ʱ��
	mhTimer1 = TIMER_open(TIMER_DEV1, TIMER_OPEN_RESET);//��ʱ��1��Ϊ�����ź�ͬ���¼�

    //DMA_config(myhDma,&myConfig);
    DMA_config(hDmaSend0,&dmaConfigForSend);

	TIMER_config(mhTimer0, &timCfg0);
	TIMER_config(mhTimer1, &timCfg1);
}

/*************************************************************************************************/
/********************************************GPIO��ʼ��*********************************************/
/*************************************************************************************************/
void GPIO_init(void)
{
	GPIO_RSET(IODIR,0xFF);//������������Ϊ���
	GPIO_RSET(IODATA,0x00);//���������������0
}




































#endif /* INIT_H_ */
