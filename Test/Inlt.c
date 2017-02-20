/*
 * Inlt.c
 *
 *  Created on: 2015-12-7
 *      Author: zhjl769
 */

#include <msp430f6638.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Init.h"
#include "ADC12.h"
#include "DMA.h"
#include "Variable.h"

#define CPU_F ((double)20000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))
//软件延时，程序中直接调用delay_ms (x),x为需要延时的时间

void SetVcoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}

void Init_CLK(void)
{
	P7SEL |= BIT2 + BIT3;                        //set P7.2、P7.3 XT2
	while(BAKCTL & LOCKIO)
		BAKCTL &= ~LOCKIO;                     //unlock XT1
	UCSCTL6 &= ~XT1OFF; //open XT1
	UCSCTL6 &= ~XT2OFF; //open XT2
	do
	{
		UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
		SFRIFG1 &= ~OFIFG;
	}
	while(SFRIFG1 & OFIFG);                   //clear error flag
	__bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_6;                      // Select DCO range 24MHz operation
	UCSCTL2 = FLLD__1  + 19;                        // Set DCO Multiplier for 20MHz
		                                            // (N + 1) * FLLRef = SMLK
		                                            // (19 + 1) * 1 = 20MHz
		                                            // Set FLL Div = fDCOCLK/1
	UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__8;         // Set DCO FLL reference = XT2, Set FLLREFDIV = 8 Fref = 8M/8=1M
	UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; 	//避免DCO调整中跑飞
	__bic_SR_register(SCG0);                  // Enable the FLL control loop

	UCSCTL6 |= XT1DRIVE_0;
	UCSCTL6 |= XT2DRIVE_1;
	delay_us(3750);
	  // Loop until XT1,XT2 & DCO fault flag is cleared
	  do
	  {
	    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                            // Clear XT2,XT1,DCO fault flags
	    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	  }while (SFRIFG1 & OFIFG);                   // Test oscillator fault flag
}



void IO_Init(void)
{
   P1OUT |= BIT0;//P1.0使能端,拉低发射使能端
   P1DIR = 0xFF; //设置P1口方向寄存器P1口作为输出
   P1SEL &= ~BIT0;//P1.0选择I/O功能
   P2IES &= ~BIT5 + ~BIT6;//初始值检测上升沿为中断使能；
   P2DIR = 0xff;//,设置P2为输出
   P2DIR &= ~BIT5 + BIT6;//,设置P2.5、P2.6为输入;
   P3DIR = 0xFF; //P3口为输出
   P3SEL |= BIT4; //P3口第6、7引脚供外围模块使用
   P4DIR = 0XFF; //设置P4口方向寄存器，使P4口为输出

   P9SEL &= ~BIT3;//P9.3设为I/O

}


void UART1_Init(void) //UART1初始化函数
 {
   P8SEL |= BIT2 + BIT3;//模块功能接口设置，即P8.2与P8.3作为USCI的接收口与发射口
   UCA1CTL0 = 0x00;//USCI控制寄存器UCA1CTL0，清零
   UCA1CTL0 |= UCMSB ; //控制移位发送寄存器为高位优先发送模式
   UCA1CTL1 = 0x00;//USCI控制寄存器UCA1CTL1，清零
   UCA1CTL1 |= UCSSEL_1;//选择时钟SMCLK
   UCA1BR0 = 0x03;		//设置波特率
   UCA1BR1 = 0x00;
   UCA1MCTL = UCBRS_3+UCBRF_0;
   UCA1CTL1 &= ~UCSWRST;//结束复位
   UCA1IE |= UCRXIE + UCTXIE;//使能接收、发送中断
 }


void Init_ADC()
{
	P6SEL |= BIT0 + BIT1;//P6.0 P6.1设置为AD，检测电池电压值与输入信号
	P6DIR &= ~BIT0 + ~BIT1;//P6.0 P6.1设置为输入
	ADC12CTL0 &= ~ADC12ENC;//ADC12CTL0寄存器ENC清零
	ADC12CTL0 |= ADC12SHT0_4 + ADC12ON + ADC12MSC;//ADC12SHT0_4
	// Set sampling time, MSC为SHI信号首次控制采样周期16，采样率=(16+9)/(20*10^6/4/2)=25/2.5^6 100kHz//SHTO_2,CLK = 2.5M
	ADC12MCTL1 |= ADC12INCH_4 + ADC12SREF_3;// ref+=VeREF+, channel = A0
	ADC12CTL1 |= ADC12CSTARTADD_0;//转换结果存储在ADC12MEM0中；
	ADC12CTL1 |= ADC12SHP + ADC12CONSEQ_2 + ADC12DIV_0 + ADC12SSEL_3 + ADC12SHS_0;
	// 采用脉冲采样方式+单通道多次采样方式+时钟为MCLK+时钟分频2+ADC12SC标志位
//	ADC12CTL2 &= ~ADC12RES_3;//ADC12转换分辨率为8位;
	ADC12CTL2 |= ADC12PDIV ;//参考时钟预分频4 //AD采样率为68.6kHz//SHTO_4,CLK = 5M//+ ADC12DF
//	ADC12IE |= ADC12IE0;//中断标志位为A0,在使用DMA搬运数据的时候不需要自己的中断，必须关闭该标志位。
}


void Init_TimerA()
{
  //定时器配置
  TA0CTL = TASSEL_2 + MC_1 + ID_0;//SMCLK+2分频+增计数模式.8M时钟
  TA0CCR0 = PWM_COUNT; //399
//  TA0CCTL0 = CCIE + OUTMOD_4; //100us中断
  TA0CCR1 = PWM_COUNT/4;
  TA0CCTL1 = OUTMOD_2; //reset，输出低电平
  TA0CCR2 = PWM_COUNT/4;
  TA0CCTL2 = OUTMOD_2; //reset，输出低电平
}


void Init_DMA()
{
  //DMA配置
  DMAflg = 1;////DMA接收数据块标志位为1.表示当前搬移到Rev1中
  DMACTL0 = DMA0TSEL_24;//DMA0的触发源为ADC12IFG0
  DMACTL4 = DMARMWDIS;//禁止读、写操作控制位
  DMA0CTL &= ~DMAIFG;//清除中断标志位
  DMA0CTL = DMADT_4 + DMADSTINCR_3  + DMAIFG + DMAEN + DMAIE;
  //重复单次传输，目标地址加1，中断使能
  DMA0SZ = Save;
  //传输的大小
  __data16_write_addr((unsigned short) &DMA0SA,(unsigned long)&ADC12MEM0);
  //设置源地址
  __data16_write_addr((unsigned short) &DMA0DA,(unsigned long)&Rev1);
 // 设置目的地址
}

