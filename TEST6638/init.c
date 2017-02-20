/*
 * init.c
 *
 *  Created on: 2016��3��5��
 *      Author: zane
 */
#include"msp430f6638.h"
#include"main.h"

//ϵͳʱ�ӳ�ʼ��
//ACLK:XT1;		SMCLK:20MHz;	MCLK:20MHz
void initClock()
{
	while(BAKCTL & LOCKIO)
    BAKCTL &= ~(LOCKIO);
	UCSCTL6 &= ~XT1OFF; 		//����XT1
	P7SEL |= BIT2 + BIT3; 		//XT2���Ź���ѡ��
	UCSCTL6 &= ~XT2OFF; 		//����XT2
	while (SFRIFG1 & OFIFG) 	//�ȴ�XT1��XT2��DCO�ȶ�
	{
		UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
		SFRIFG1 &= ~OFIFG;
	}

	UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK; 	//����DCO�������ܷ�

	UCSCTL1 = DCORSEL_5; 	//6000kHz~23.7MHz
	UCSCTL2 = SMCLK / (XT2 / 16); 					//XT2Ƶ�ʽϸߣ���Ƶ����Ϊ��׼�ɻ�ø��ߵľ���
	UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__16; 		//XT2����16��Ƶ����Ϊ��׼
	while (SFRIFG1 & OFIFG)	//�ȴ�XT1��XT2��DCO�ȶ�
	{
		UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
		SFRIFG1 &= ~OFIFG;
	}
	UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; 					//�趨����CLK�ķ�Ƶ
	UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; 	//�趨����CLK��ʱ��Դ
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
	//P4.5����������
	P4DIR |= BIT5;
	P4OUT &= ~BIT5;

	P5DIR |= BIT6;
	P5OUT &= ~BIT6;

    P6SEL |= BIT0;
}

//���2Mʱ���źţ�������19200
void UART_RS232_Init(void)		//RS232�ӿڳ�ʼ������
{
	P8SEL|=(BIT1+BIT2+BIT3);	//ģ�鹦�ܽӿ����ã���P8.2��P8.3��ΪUSCI�Ľ��տ��뷢���
	UCA1CTL1|=UCSWRST;			//��λUSCI
	UCA1CTL1|=UCSSEL_0;			//���ø���ʱ�ӣ����ڷ����ض�������
	UCA1BR0=0x45;				//���ò�����
	UCA1BR1=0x00;
	UCA1MCTL=0x4A;
	UCA1CTL1&=~UCSWRST;			//������λ
	UCA1IE|=UCRXIE;		//ʹ�ܽ����ж�
}

void UART_RS485_Init(void)		//RS485�ӿڳ�ʼ������
{
	P2SEL|=(BIT0+BIT4+BIT5);	//ģ�鹦�ܽӿ����ã���P8.2��P8.3��ΪUSCI�Ľ��տ��뷢���
	UCA0CTL1|=UCSWRST;			//��λUSCI
	UCA0CTL1|=UCSSEL_0;			//���ø���ʱ�ӣ����ڷ����ض�������
	UCA0BR0=0x45;				//���ò�����
	UCA0BR1=0x00;
	UCA1MCTL=0x4A;
	UCA0CTL1&=~UCSWRST;			//������λ
	UCA0IE|=UCRXIE;		//ʹ�ܽ����ж�
}



