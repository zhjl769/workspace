#include <msp430f1611.h>
#include <stdint.h>
#include <stdio.h>
#include "dr_tft.h"


void initClock()
{
  while(BAKCTL & LOCKIO) // Unlock XT1 pins for operation
    BAKCTL &= ~(LOCKIO);
  UCSCTL6 &= ~XT1OFF; //启动XT1
  P7SEL |= BIT2 + BIT3; //XT2引脚功能选择
  UCSCTL6 &= ~XT2OFF; //启动XT2
  while (SFRIFG1 & OFIFG) { //等待XT1、XT2与DCO稳定
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }

  UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK; //避免DCO调整中跑飞

  UCSCTL1 = DCORSEL_5; //6000kHz~23.7MHz
  UCSCTL2 = 20690000 / (4000000 / 16); //XT2频率较高，分频后作为基准可获得更高的精度
  UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__16; //XT2进行16分频后作为基准
  while (SFRIFG1 & OFIFG) { //等待XT1、XT2与DCO稳定
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }
  UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; //设定几个CLK的分频
  UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; //设定几个CLK的时钟源
}


char TAPwmInit(char Clk,char Div,char Mode1,char Mode2)
{
	TA0CTL = 0;			//清除以前设置
	TA0CTL |= MC_1;		//增计数模式
	switch(Clk)			//选择时钟源
	{
		case 'A': case 'a': TA0CTL |= TASSEL_1; break;	//ACLK
		case 'S': case 's': TA0CTL |= TASSEL_2; break;	//SMCLK
		case 'E':			TA0CTL |= TASSEL_0; break;	//TACLK
		case 'e':			TA0CTL |= TASSEL_3; break;	//TACLK取反
		default :			return(0);					//参数错误
	}
	switch(Div)			//选择分频系数
	{
		case 1:		TA0CTL |= ID_0; break;	//1
		case 2:		TA0CTL |= ID_1; break;	//2
		case 4:		TA0CTL |= ID_2; break;	//4
		case 8:		TA0CTL |= ID_3; break;	//8
		default :	return(0);				//参数错误
	}
	switch(Mode1)		//设置PWM通道1的输出模式
	{
		case 'P': case 'p': 		//设置为高电平模式
			TA0CCTL1 = OUTMOD_7;	//高电平PWM输出
			P1SEL |= BIT2;			//从P1.2输出（单片机型号）
			P1DIR |= BIT2;			//从P1.2输出（单片机型号）
			break;
		case 'N': case 'n':			//设置为低电平模式
			TA0CCTL1 = OUTMOD_3;	//高电平PWM输出
			P1SEL |= BIT2;			//从P1.2输出（单片机型号）
			P1DIR |= BIT2;			//从P1.2输出（单片机型号）
			break;
		case '0': case 0:			//设置为禁用
			P1SEL &= ~BIT2;			//P1.2恢复为普通IO口
			break;
		default:
			return(0);				//参数有误
	}
	switch(Mode2)		//设置PWM通道1的输出模式
	{
		case 'P': case 'p': 		//设置为高电平模式
			TA0CCTL3 = OUTMOD_7;	//高电平PWM输出
			P1SEL |= BIT4;			//从P1.4输出（单片机型号）
			P1DIR |= BIT4;			//从P1.4输出（单片机型号）
			break;
		case 'N': case 'n':			//设置为低电平模式
			TA0CCTL3 = OUTMOD_3;	//高电平PWM输出
			P1SEL |= BIT4;			//从P1.4输出（单片机型号）
			P1DIR |= BIT4;			//从P1.4输出（单片机型号）
			break;
		case '0': case 0:			//设置为禁用
			P1SEL &= ~BIT4;			//P1.4恢复为普通IO口
			break;
		default:
			return(0);				//参数有误
	}
	return(1);
}

void TAPwmSetPeriod(unsigned int Period)
{
	TA0CCR0 = Period;
}

void TAPwmSetDuty(char Channel,unsigned int Duty)
{
	switch(Channel)
	{
		case 1: TA0CCR1 = Duty;	break;
		case 2: TA0CCR3 = Duty;	break;
	}
}

void TAPwmSetPermill(char Channel,unsigned int Percent)
{
	unsigned long int Period;
	unsigned int Duty;
	Period = TA0CCR0;
	Duty = Period * Percent / 1000;
	TAPwmSetDuty(Channel,Duty);
}

int i=0;

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;//关闭看门狗
	 _DINT();
	  initClock();
	  initTFT();
	  _EINT();
	  etft_AreaSet(0,0,319,239,0);
	TAPwmInit('S',1,'P','P');
	TAPwmSetPeriod(22);
//	TAPwmSetDuty(1,200);
	TAPwmSetPermill(2,500);
	TA0CCTL3 = OUTMOD_0;

	P6DIR |= BIT7;

	TA1CCTL0 = CCIE;//比较器中断使能
	TA1CCR0 = 10;
	TA1CTL |= MC_1 + TASSEL_1 + TACLR;
	//时钟为SMCLK,比较模式，开始时清零计数器
	etft_DisplayString("Frequency: 900 KHz",0,60,65535,0);
	etft_DisplayString("Time	 : 1 S",0,100,65535,0);
	etft_DisplayString("Number	 : 8500",0,140,65535,0);
	__bis_SR_register(LPM0_bits + GIE);//进入低功耗并开启总中断
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A (void)
{
	if (i==0)
	{
		P6OUT &= ~BIT7;
		TA0CCTL3 = OUTMOD_0;
		i=7;
		TA1CCR0 = 33333;
	}
	else if(i==7)
	{
		P6OUT |= BIT7;
		TA0CCTL3 = OUTMOD_7;
		i = 0;
		TA1CCR0 = 30;
	}
}

