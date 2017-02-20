#include <msp430f1611.h>
#include <stdint.h>
#include <stdio.h>
#include "dr_tft.h"


void initClock()
{
  while(BAKCTL & LOCKIO) // Unlock XT1 pins for operation
    BAKCTL &= ~(LOCKIO);
  UCSCTL6 &= ~XT1OFF; //����XT1
  P7SEL |= BIT2 + BIT3; //XT2���Ź���ѡ��
  UCSCTL6 &= ~XT2OFF; //����XT2
  while (SFRIFG1 & OFIFG) { //�ȴ�XT1��XT2��DCO�ȶ�
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }

  UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK; //����DCO�������ܷ�

  UCSCTL1 = DCORSEL_5; //6000kHz~23.7MHz
  UCSCTL2 = 20690000 / (4000000 / 16); //XT2Ƶ�ʽϸߣ���Ƶ����Ϊ��׼�ɻ�ø��ߵľ���
  UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__16; //XT2����16��Ƶ����Ϊ��׼
  while (SFRIFG1 & OFIFG) { //�ȴ�XT1��XT2��DCO�ȶ�
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }
  UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; //�趨����CLK�ķ�Ƶ
  UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; //�趨����CLK��ʱ��Դ
}


char TAPwmInit(char Clk,char Div,char Mode1,char Mode2)
{
	TA0CTL = 0;			//�����ǰ����
	TA0CTL |= MC_1;		//������ģʽ
	switch(Clk)			//ѡ��ʱ��Դ
	{
		case 'A': case 'a': TA0CTL |= TASSEL_1; break;	//ACLK
		case 'S': case 's': TA0CTL |= TASSEL_2; break;	//SMCLK
		case 'E':			TA0CTL |= TASSEL_0; break;	//TACLK
		case 'e':			TA0CTL |= TASSEL_3; break;	//TACLKȡ��
		default :			return(0);					//��������
	}
	switch(Div)			//ѡ���Ƶϵ��
	{
		case 1:		TA0CTL |= ID_0; break;	//1
		case 2:		TA0CTL |= ID_1; break;	//2
		case 4:		TA0CTL |= ID_2; break;	//4
		case 8:		TA0CTL |= ID_3; break;	//8
		default :	return(0);				//��������
	}
	switch(Mode1)		//����PWMͨ��1�����ģʽ
	{
		case 'P': case 'p': 		//����Ϊ�ߵ�ƽģʽ
			TA0CCTL1 = OUTMOD_7;	//�ߵ�ƽPWM���
			P1SEL |= BIT2;			//��P1.2�������Ƭ���ͺţ�
			P1DIR |= BIT2;			//��P1.2�������Ƭ���ͺţ�
			break;
		case 'N': case 'n':			//����Ϊ�͵�ƽģʽ
			TA0CCTL1 = OUTMOD_3;	//�ߵ�ƽPWM���
			P1SEL |= BIT2;			//��P1.2�������Ƭ���ͺţ�
			P1DIR |= BIT2;			//��P1.2�������Ƭ���ͺţ�
			break;
		case '0': case 0:			//����Ϊ����
			P1SEL &= ~BIT2;			//P1.2�ָ�Ϊ��ͨIO��
			break;
		default:
			return(0);				//��������
	}
	switch(Mode2)		//����PWMͨ��1�����ģʽ
	{
		case 'P': case 'p': 		//����Ϊ�ߵ�ƽģʽ
			TA0CCTL3 = OUTMOD_7;	//�ߵ�ƽPWM���
			P1SEL |= BIT4;			//��P1.4�������Ƭ���ͺţ�
			P1DIR |= BIT4;			//��P1.4�������Ƭ���ͺţ�
			break;
		case 'N': case 'n':			//����Ϊ�͵�ƽģʽ
			TA0CCTL3 = OUTMOD_3;	//�ߵ�ƽPWM���
			P1SEL |= BIT4;			//��P1.4�������Ƭ���ͺţ�
			P1DIR |= BIT4;			//��P1.4�������Ƭ���ͺţ�
			break;
		case '0': case 0:			//����Ϊ����
			P1SEL &= ~BIT4;			//P1.4�ָ�Ϊ��ͨIO��
			break;
		default:
			return(0);				//��������
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
	WDTCTL = WDTPW + WDTHOLD;//�رտ��Ź�
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

	TA1CCTL0 = CCIE;//�Ƚ����ж�ʹ��
	TA1CCR0 = 10;
	TA1CTL |= MC_1 + TASSEL_1 + TACLR;
	//ʱ��ΪSMCLK,�Ƚ�ģʽ����ʼʱ���������
	etft_DisplayString("Frequency: 900 KHz",0,60,65535,0);
	etft_DisplayString("Time	 : 1 S",0,100,65535,0);
	etft_DisplayString("Number	 : 8500",0,140,65535,0);
	__bis_SR_register(LPM0_bits + GIE);//����͹��Ĳ��������ж�
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

