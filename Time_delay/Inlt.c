/*
 * Inlt.c
 *
 *  Created on: 2015-12-7
 *      Author: zhjl769
 */

#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Init.h"

#define CPU_F ((double)8000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//软件延时，程序中直接调用delay_ms (x),x为需要延时的时间


void Init_CLK(void)
{
    unsigned int i;
    BCSCTL1 = 0X00;//将寄存器的内容清零
    BCSCTL1 &= ~XT2OFF + ~XTS + DIVA_0;//LFTX1工作在低频模式,XT2震荡器开启,ACLK的分频因子为1
    do
    {
	IFG1 &= ~OFIFG;                       // 清除OSCFault标志
	for (i = 0xff; i > 0; i--);
    }
    while ((IFG1 & OFIFG) == OFIFG);      // 如果OSCFault =1

    BCSCTL2 = 0X00;			//将寄存器的内容清零
    BCSCTL2 |= SELM_2;		//MCLK的时钟源为TX2CLK，分频因子为1
    BCSCTL2 |= SELS;		//SMCLK的时钟源为TX2CLK，分频因子为1
}


void IO_Init(void)
{
   P1DIR = 0Xff; //设置P1口方向寄存器P1口作为输出
   P1SEL |= BIT3 ;//P1.3设置为捕获信号输入
   P1DIR &= ~BIT3;//设置P1.3口方向寄存器作为输入
   P2DIR = 0xff;//,设置P2为输出
   P2OUT = 0x00;
   P2DIR &= ~BIT5;//,设置P2.5为输入;
   P2IES &= ~BIT5;//初始值检测上升沿为中断使能；
//   P2IE |= BIT5;//开启p2.5的中断
   P3DIR = 0xFF; //P3口为输出
   P3SEL |= BIT6+BIT7; //P3口第6、7引脚供外围模块使用
   P4DIR = 0XFF; //设置P4口方向寄存器，使P4口为输出
   P4SEL = 0;
   P4OUT = 0x00;
}


void UART1_Init(void) //UART1初始化函数
 {
   U1CTL &= ~(SWRST + SSEL1); //USART控制寄存器UCTL1，SWRST=0x01，~SWRST=0xFE，将UCTL0寄存器的第0位复位后，USART才能重新被允许
   U1CTL = 0X00;//USART控制寄存器UCTL1，清零
   U1CTL += CHAR;//数据位为8bit
   U1CTL &= ~SPB;//1位停止位
   U1TCTL |= SSEL1;//发送控制寄存器，第4位置1，选择辅助时钟ACLK1
   U1BR0 = 0x41; //9600波特率，32kHz时钟
   U1BR1 = 0x03;
   U1MCTL = 0x00; //UART波特率调整数
   U1ME |= UTXE1 + URXE1; //模块允许寄存器ME2,设置模块允许寄存器ME1，UTXE0=0x80，设置ME1的第7位为1，//UTXE1=0x80 发送允许//URXE1=0x40 接收允许
   U1IE |= URXIE1; //中断允许寄存器IE2，第5位为1，使能接收中断，URXIE1=0x10
   U1IE |= UTXIE1; //中断允许寄存器IE2，第6位为1，使能发送中断，UTXIE1=0x20
 }


void Init_ADC()
{
//	ADC12CTL0 &= 0x00;//ADC12CTL0寄存器清零
	ADC12CTL0 &= ~ENC;//ADC12CTL0寄存器ENC清零
	ADC12CTL0 |= SHT0_2 + ADC12ON + MSC;  // Set sampling time, MSC为SHI信号控制
//	ADC12CTL1  &= 0x00;//ADC12CTL1寄存器清零
	ADC12CTL1 |= CSTARTADD_7;//转换结果存储在ADC12MEM7中；
	ADC12CTL1 |= SHP + CONSEQ_2+ADC12DIV_0+ADC12SSEL_2+SHS_0;   // 采用脉冲采样方式+单通道多次采样方式+时钟为MCLK+时钟分频0+ADC12SC标志位
	ADC12MCTL7 |= INCH_7+SREF_3;  // ref+=VeREF+, channel = A7
	ADC12IE |= BIT7;  //中断标志位为A7
	_nop();
	_nop();
	_nop();
	_nop();
	_nop();
	ADC12CTL0 |=  ADC12SC + ENC;//如果上次采集的值达到了要求，并停止了，开始启动转换，获取电压值。
}


void Init_TimerA()
{
  //定时器配置
  TA0CTL |= TASSEL_2 +MC_0 + ID_0 + TAIE;//SMCLK+8分频+连续计数模式.8M时钟,打开溢出中断
  TA0CCTL0 |= CCIE; //AO开启中断
  TA0CCTL2 |= CM_0 + CCIS_0 + CAP + CCIE; //reset,捕获模式，输入P1.3，同步捕获，关闭上升沿捕获，
}




