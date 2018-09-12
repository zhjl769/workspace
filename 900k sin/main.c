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
  BCSCTL2 |= SELM_2 + SELS;                ////SELM_2��Ӧʱ��ԴΪXT2CLK��DIVS1��SMCLK�ķ�Ƶ����ʱ��MCLK��8MHz����ʱ��SMCLK=8m/4=2m
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

	P1SEL |= BIT2;			//��P1.2�������Ƭ���ͺţ�
	P1DIR |= BIT2;			//��P1.2�������Ƭ���ͺţ�

	P1SEL |= BIT3;			//��P1.3�������Ƭ���ͺţ�
	P1DIR |= BIT3;			//��P1.3�������Ƭ���ͺţ�

	TA0CTL = 0;			//�����ǰ����
	TA0CTL |= MC_1;		//������ģʽ
	TA0CTL |= TASSEL_2;
	TA0CTL |= ID_0;
	TA0CCTL2 = OUTMOD_0 + BIT2;
	TA0CCTL1 = OUTMOD_0 + BIT2;
	TA0CCR0 = 14;
	TA0CCR1 = 7;
	TA0CCR2 = 7;


	TBCCTL0 = CCIE;//�Ƚ����ж�ʹ��
	TBCCR0 = 10;
	TBCTL |= MC_1 + TASSEL_1 + TACLR;
	//ʱ��ΪSMCLK,�Ƚ�ģʽ����ʼʱ���������
	__bis_SR_register(LPM0_bits + GIE);//����͹��Ĳ��������ж�
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
