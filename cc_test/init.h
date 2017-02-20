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
/********************************************系统时钟初始化******************************************/
/*************************************************************************************************/
void CLK_init(void) //定义函数CLK_init（返回一个空类型，且该函数不含参数）
{
	ioport unsigned int *clkmd;
	clkmd = (unsigned int *)0x1c00; //把地址0x1c00进行强制类型转换后赋值给指针clkmd。0x1c00为时钟发生器寄存器的端口地址。
	*clkmd = 0x2CB2;//8M晶振<0x2CB2=100MHz><0x2c92=200MHz><0x2912=144MHz>
}



/************************************************************************************************/
/*************************************************SDRAM初始化*************************************/
/************************************************************************************************/
void SDRAM_init(void)
{
	ioport unsigned int *ebsr  = (unsigned int *)0x6c00;//外部总线选择寄存器（控制HPI与EMIF）
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
	//ioport unsigned int *sdc3  = (unsigned int *)0x0814;  // 此寄存器只用于5510,5509中无

	*ebsr   = 0x0211;    // full emif 23d

	*egcr   = 0x0210;         //0x0210频率等于系统时钟的1/2；0x0020频率等于系统时钟

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
	*init   = 0x0000;//为SDRAM赋初值
}


/********************************************************************************************/
/*************************************DMA初始化&计时器初始化*************************************/
/********************************************************************************************/
/*DMA_Config  dmaConfigForRev = { //接收配置(未修改20161110)
  DMA_DMACSDP_RMK(
    DMA_DMACSDP_DSTBEN_NOBURST,//地址不打包
    DMA_DMACSDP_DSTPACK_OFF,
    DMA_DMACSDP_DST_DARAM,//目的地址为DARAM
    DMA_DMACSDP_SRCBEN_NOBURST,
    DMA_DMACSDP_SRCPACK_OFF,
    DMA_DMACSDP_SRC_PERIPH,//源地址为外设
    DMA_DMACSDP_DATATYPE_16BIT//16bit数据
  ),                                       //DMACSDP
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
  ),                                       //DMACCR
  DMA_DMACICR_RMK(
    DMA_DMACICR_BLOCKIE_OFF,
    DMA_DMACICR_LASTIE_OFF,
    DMA_DMACICR_FRAMEIE_ON,//帧中断使能
    DMA_DMACICR_FIRSTHALFIE_OFF,
    DMA_DMACICR_DROPIE_OFF,
    DMA_DMACICR_TIMEOUTIE_OFF
  ),                                       //DMACICR
    (DMA_AdrPtr)(MCBSP_ADDR(DRR11)),        //DMACSSAL McBSP接收数据寄存器
    0,                                     //DMACSSAU
    (DMA_AdrPtr)&Rev1,                      //DMACDSAL 接收数组
    0,                                     //DMACDSAU
    BUF_LEN_MAX,                           //DMACEN   接收数据长度
    1,                                     //DMACFN
    0,                                     //DMACFI
    0                                     //DMACEI
};*/
DMA_Config  dmaConfigForSend = { //发送配置
  DMA_DMACSDP_RMK(
    DMA_DMACSDP_DSTBEN_NOBURST,    /*目标地址突发模式：不使用*/
    DMA_DMACSDP_DSTPACK_OFF,       /*目标地址打包：不使用*/
    DMA_DMACSDP_DST_PERIPH,         /*选择目标类型：外设(GPIO)*/
    DMA_DMACSDP_SRCBEN_NOBURST,    /*源地址突发模式：不使用*/
    DMA_DMACSDP_SRCPACK_OFF,       /*源地址打包：不使用*/
    DMA_DMACSDP_SRC_EMIF,         /*选择源类型：EMIF（SDRAM）*/
    DMA_DMACSDP_DATATYPE_16BIT     /*通道传输的数据类型：16BIT*/
  ),                                       /* DMACSDP寄存器*/
  DMA_DMACCR_RMK(
    DMA_DMACCR_DSTAMODE_CONST,   /*目标地址寻址模式：常数地址*/
    DMA_DMACCR_SRCAMODE_POSTINC,   /*源地址寻址模式：地址自加*/
    DMA_DMACCR_ENDPROG_OFF,         /*编程结束标志位*/
    DMA_DMACCR_REPEAT_OFF,         /*循环条件：等待ENPROG=1*/
    DMA_DMACCR_AUTOINIT_ON,        /*自动初始化：开启*/
    DMA_DMACCR_EN_STOP,            /*通道使能控制位*/
    DMA_DMACCR_PRIO_HI,            /*通道优先级：高*/
    DMA_DMACCR_FS_ELEMENT,         /*同步类型：单元同步*/
    DMA_DMACCR_SYNC_TIM2INT           /*同步源：计数器2*/
  ),                                       /* DMACCR寄存器*/
  DMA_DMACICR_RMK(
    DMA_DMACICR_BLOCKIE_OFF,       /*所有块中断使能位*/
    DMA_DMACICR_LASTIE_OFF,        /*最后帧中断使能位*/
    DMA_DMACICR_FRAMEIE_ON,        /*所有帧中断使能位*/
    DMA_DMACICR_FIRSTHALFIE_OFF,   /*半帧中断使能位*/
    DMA_DMACICR_DROPIE_OFF,        /*同步事件旁落中断使能位*/
    DMA_DMACICR_TIMEOUTIE_OFF      /*超时中断使能位*/
  ),                                      /* DMACICR寄存器*/
    (DMA_AdrPtr)&Send_sig,                /* DMACSSAL源起始地址寄存器（低位） *///CE2空间
    0,                     				 /* DMACSSAU源起始地址寄存器（高位）  */
    (DMA_AdrPtr)GPIO_ADDR(IODATA),                   /* DMACDSAL目标起始地址寄存器（低位）  */
    0,                                     /* DMACDSAU目标起始地址寄存器（高位）  */
    4096,		                           /* DMACEN单元数寄存器   */
    1,                                     /* DMACFN帧数寄存器   */
    0,                                     /* DMACFI单元索引寄存器   */
    0                                      /* DMACEI帧索引寄存器   */
};
#define TIMER_CTRL0    TIMER_TCR_RMK(\
                      TIMER_TCR_IDLEEN_DEFAULT,    /* IDLEEN == 0 */ \
                      TIMER_TCR_FUNC_OF(01),        /* FUNC   == 00CPU时钟源 */ \
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
                      TIMER_TCR_FUNC_OF(00),        /* FUNC   == 00CPU时钟源 */ \
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

TIMER_Config timCfg0 = {	//Timer0为100ms  外部时钟频率=10MHz
   TIMER_CTRL0,               /* TCR0 */
   0xf423u,                  /* PRD0=62499 */
   0x000fu                    /* PRSC=15 */
};


TIMER_Config timCfg1 = {	//Timer1为1uS
   TIMER_CTRL1,               /* TCR0 */
   0x0063u,                  /* PRD0 200MHz->0x00c7; 144MHz->0x008f;100MHz->0x0063*/
   0x0000                    /* PRSC */
};
void DMA_init(void)
{/*
	RevsrcAddrHi = (Uint16)(((Uint32)(MCBSP_ADDR(DRR10))) >> 15) & 0xFFFFu;//McBSP接收寄存器高字节地址
    RevsrcAddrLo = (Uint16)(((Uint32)(MCBSP_ADDR(DRR10))) << 1) & 0xFFFFu;//McBSP接收寄存器低字节地址
    Revdst1AddrHi = (Uint16)(((Uint32)(&Rev1)) >> 15) & 0xFFFFu;//重叠保留数组1高字节地址
    Revdst1AddrLo = (Uint16)(((Uint32)(&Rev1)) << 1) & 0xFFFFu;//重叠保留数组1低字节地址
	Revdst2AddrHi = (Uint16)(((Uint32)(&Rev2)) >> 15) & 0xFFFFu;//重叠保留数组2高字节地址
    Revdst2AddrLo = (Uint16)(((Uint32)(&Rev2)) << 1) & 0xFFFFu;//重叠保留数组2低字节地址

    myConfig.dmacssal = (DMA_AdrPtr)RevsrcAddrLo;//DMA通道源地址低字节
    myConfig.dmacssau = RevsrcAddrHi;//DMA通道源地址高字节
    myConfig.dmacdsal = (DMA_AdrPtr)Revdst1AddrLo;//DMA通道目的地址低字节
    myConfig.dmacdsau = Revdst1AddrHi;//DMA通道目的地址高字节
*/
	sendsrc0AddrHi = (Uint16)(((Uint32)(dmaConfigForSend.dmacssal)) >> 15) & 0xFFFFu;//DMA0发射源地址高字节
    sendsrc0AddrLo = (Uint16)(((Uint32)(dmaConfigForSend.dmacssal)) << 1) & 0xFFFFu;//DMA0发射源地址低字节
    senddst0AddrHi = (Uint16)(((Uint32)(dmaConfigForSend.dmacdsal)) >> 15) & 0xFFFFu;//DMA0发射目的地址高字节
    senddst0AddrLo = (Uint16)(((Uint32)(dmaConfigForSend.dmacdsal)) << 1) & 0xFFFFu;//DMA0发射目的地址低字节

    dmaConfigForSend.dmacssal = (DMA_AdrPtr)sendsrc0AddrLo;//发射源地址EMIF
    dmaConfigForSend.dmacssau = sendsrc0AddrHi;
    dmaConfigForSend.dmacdsal = (DMA_AdrPtr)senddst0AddrLo;//发射目的地址GPIO
    dmaConfigForSend.dmacdsau = senddst0AddrHi;

    hDmaSend0 = DMA_open(DMA_CHA0,DMA_OPEN_RESET);//DMA0用于GPIO发射

    //myhDma = DMA_open(DMA_CHA2,DMA_OPEN_RESET);  //DMA2用于McBSP采集数据

	mhTimer0 = TIMER_open(TIMER_DEV0, TIMER_OPEN_RESET);//定时器0作为同步时钟
	mhTimer1 = TIMER_open(TIMER_DEV1, TIMER_OPEN_RESET);//定时器1作为发射信号同步事件

    //DMA_config(myhDma,&myConfig);
    DMA_config(hDmaSend0,&dmaConfigForSend);

	TIMER_config(mhTimer0, &timCfg0);
	TIMER_config(mhTimer1, &timCfg1);
}

/*************************************************************************************************/
/********************************************GPIO初始化*********************************************/
/*************************************************************************************************/
void GPIO_init(void)
{
	GPIO_RSET(IODIR,0xFF);//配置所有引脚为输出
	GPIO_RSET(IODATA,0x00);//配置所有引脚输出0
}




































#endif /* INIT_H_ */
