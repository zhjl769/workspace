#include <msp430f1611.h>

int j=0;

void Sys_ClkInit(void)
{
  int i;
  BCSCTL1 &=~XT2OFF;
  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG)!=0);                   // OSCFault flag still set
  BCSCTL2 |= SELM_2 + SELS;                ////SELM_2对应时钟源为XT2CLK，DIVS1是SMCLK四分频，主时钟MCLK，8MHz，子时钟SMCLK=8m/4=2m
}
/*
 * main.c
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	_DINT();
	Sys_ClkInit();
	_EINT();
	P1DIR |= BIT1;

	P1SEL |= BIT2;			//从P1.2输出（单片机型号）
	P1DIR |= BIT2;			//从P1.2输出（单片机型号）

	P1SEL |= BIT3;			//从P1.3输出（单片机型号）
	P1DIR |= BIT3;			//从P1.3输出（单片机型号）

	TA0CTL = 0;			//清除以前设置
	TA0CTL |= MC_1;		//增计数模式
	TA0CTL |= TASSEL_2;
	TA0CTL |= ID_0;
	TA0CCTL2 = OUTMOD_0 + BIT2;
	TA0CCTL1 = OUTMOD_0 + BIT2;
	TA0CCR0 = 14;
	TA0CCR1 = 7;
	TA0CCR2 = 7;


	TBCCTL0 = CCIE;//比较器中断使能
	TBCCR0 = 10;
	TBCTL |= MC_1 + TASSEL_1 + TACLR;
	//时钟为SMCLK,比较模式，开始时清零计数器
	__bis_SR_register(LPM0_bits + GIE);//进入低功耗并开启总中断
}



#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_A (void)
{
	if (j==0)
	{
		P1OUT |= BIT1;
		TA0CCTL2 = OUTMOD_0 + BIT2;
		TA0CCTL1 = OUTMOD_0 + BIT2;
		j=7;
		TBCCR0 = 33033;
	}
	else if(j==7)
	{
		P1OUT &= ~BIT1;
		TA0CCTL2 = OUTMOD_3;
		TA0CCTL1 = OUTMOD_7;
		j = 0;
		TBCCR0 = 300;
	}
}
