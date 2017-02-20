/*
 * init.c
 *
 *  Created on: 2015��12��10��
 *      Author: Zane
 */

#include "msp430f6638.h"
#include "init.h"

void initPort()
{
  P9DIR |= BIT2 + BIT3 + BIT6 + BIT7;
  P7DIR |= BIT5 + BIT6 + BIT7;
  P8DIR |= BIT2 + BIT4 + BIT5;
  P8SEL |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6;
  P6SEL = BIT1 + BIT0 ;        //��ADC����
  P4SEL |= BIT1;
  P4DIR |= BIT1;
  P3DIR |= BIT4;
  P3SEL |= BIT4;
}
void initADC()
{
  ADC12CTL0 = ADC12ON + ADC12MSC;
  ADC12MCTL1 = ADC12INCH_1;
  ADC12IFG = 0;
  ADC12CTL1 = ADC12SHS_3 + ADC12CONSEQ_2 + ADC12SSEL_2 + ADC12CSTARTADD_1;
  ADC12CTL0 |= ADC12ENC;
  TBCCR0 = 124;
  TBCCR1 = 49;
  TBCCTL1 = OUTMOD_7;
  TBCTL = TBSSEL_2 + MC_1 + TBCLR + ID_2;

}

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
  UCSCTL2 = FLLN1 + FLLN0 + FLLN4 + FLLD__1 ; //XT2Ƶ�ʽϸߣ���Ƶ����Ϊ��׼�ɻ�ø��ߵľ���
  UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__8; //XT2����16��Ƶ����Ϊ��׼
  while (SFRIFG1 & OFIFG) { //�ȴ�XT1��XT2��DCO�ȶ�
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }
  UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; //�趨����CLK�ķ�Ƶ
  UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; //�趨����CLK��ʱ��Դ
}


void initUart()
{
  //���ô���ģ��
  UCA1CTL1 |= UCSWRST;
  UCA1CTL0 = 0;
  UCA1CTL1 = UCSWRST + UCSSEL__SMCLK;
  UCA1BRW = SMCLK_FREQ / 9600; //Ĭ�ϲ�����
  UCA1CTL1 &= ~UCSWRST;
  UCA1ICTL |= BIT0;

}
void initSpi()
{
  UCB1CTL1 |= UCSWRST;
  UCB1CTL0 = 0x069;
  UCB1CTL1 = UCSWRST + UCSSEL__SMCLK;
  UCB1BRW = SMCLK_FREQ / 100000;
  UCB1CTL1 &= ~UCSWRST;
}



