/*
 * init.c
 *
 *  Created on: 2016年3月5日
 *      Author: zane
 */
#include"msp430f6638.h"
#include"main.h"

//系统时钟初始化
//ACLK:XT1;		SMCLK:20MHz;	MCLK:20MHz
void initClock()
{
	while(BAKCTL & LOCKIO)
    BAKCTL &= ~(LOCKIO);
	UCSCTL6 &= ~XT1OFF; 		//启动XT1
	P7SEL |= BIT2 + BIT3; 		//XT2引脚功能选择
	UCSCTL6 &= ~XT2OFF; 		//启动XT2
	while (SFRIFG1 & OFIFG) 	//等待XT1、XT2与DCO稳定
	{
		UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
		SFRIFG1 &= ~OFIFG;
	}

	UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK; 	//避免DCO调整中跑飞

	UCSCTL1 = DCORSEL_5; 	//6000kHz~23.7MHz
	UCSCTL2 = SMCLK / (XT2 / 16); 					//XT2频率较高，分频后作为基准可获得更高的精度
	UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__16; 		//XT2进行16分频后作为基准
	while (SFRIFG1 & OFIFG)	//等待XT1、XT2与DCO稳定
	{
		UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
		SFRIFG1 &= ~OFIFG;
	}
	UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; 					//设定几个CLK的分频
	UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; 	//设定几个CLK的时钟源
}

void initADC()
{
	ADC12CTL0 = ADC12ON + ADC12MSC;
	ADC12MCTL0 = ADC12INCH_0;
	ADC12IFG = 0;
	ADC12CTL1 = ADC12SHS_3 + ADC12CONSEQ_2 + ADC12SSEL_2 + ADC12CSTARTADD_0;
	ADC12CTL0 |= ADC12ENC;
	ADC12IE |= ADC12IE0;
}

void initTB()
{
	TBCCR0 = 4000;
	TBCCR1 = 2000;
	TBCCTL1 = OUTMOD_7;
	TBCTL = TBSSEL_2 + MC_1 + TBCLR + ID_0;
}

void initPORT()
{
	//P4.5，测试引脚
	P4DIR |= BIT5;
	P4OUT &= ~BIT5;

	P5DIR |= BIT6;
	P5OUT &= ~BIT6;

    P6SEL |= BIT0;
}

//外接2M时钟信号，波特率19200
void UART_RS232_Init(void)		//RS232接口初始化函数
{
	P8SEL|=(BIT1+BIT2+BIT3);	//模块功能接口设置，即P8.2与P8.3作为USCI的接收口与发射口
	UCA1CTL1|=UCSWRST;			//复位USCI
	UCA1CTL1|=UCSSEL_0;			//设置辅助时钟，用于发生特定波特率
	UCA1BR0=0x45;				//设置波特率
	UCA1BR1=0x00;
	UCA1MCTL=0x4A;
	UCA1CTL1&=~UCSWRST;			//结束复位
	UCA1IE|=UCRXIE;		//使能接收中断
}

void UART_RS485_Init(void)		//RS485接口初始化函数
{
	P2SEL|=(BIT0+BIT4+BIT5);	//模块功能接口设置，即P8.2与P8.3作为USCI的接收口与发射口
	UCA0CTL1|=UCSWRST;			//复位USCI
	UCA0CTL1|=UCSSEL_0;			//设置辅助时钟，用于发生特定波特率
	UCA0BR0=0x45;				//设置波特率
	UCA0BR1=0x00;
	UCA1MCTL=0x4A;
	UCA0CTL1&=~UCSWRST;			//结束复位
	UCA0IE|=UCRXIE;		//使能接收中断
}



