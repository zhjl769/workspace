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
//�����ʱ��������ֱ�ӵ���delay_ms (x),xΪ��Ҫ��ʱ��ʱ��

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
	P7SEL |= BIT2 + BIT3;                        //set P7.2��P7.3 XT2
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
	UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; 	//����DCO�������ܷ�
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
   P1OUT |= BIT0;//P1.0ʹ�ܶ�,���ͷ���ʹ�ܶ�
   P1DIR = 0xFF; //����P1�ڷ���Ĵ���P1����Ϊ���
   P1SEL &= ~BIT0;//P1.0ѡ��I/O����
   P2IES &= ~BIT5 + ~BIT6;//��ʼֵ���������Ϊ�ж�ʹ�ܣ�
   P2DIR = 0xff;//,����P2Ϊ���
   P2DIR &= ~BIT5 + BIT6;//,����P2.5��P2.6Ϊ����;
   P3DIR = 0xFF; //P3��Ϊ���
   P3SEL |= BIT4; //P3�ڵ�6��7���Ź���Χģ��ʹ��
   P4DIR = 0XFF; //����P4�ڷ���Ĵ�����ʹP4��Ϊ���

   P9SEL &= ~BIT3;//P9.3��ΪI/O

}


void UART1_Init(void) //UART1��ʼ������
 {
   P8SEL |= BIT2 + BIT3;//ģ�鹦�ܽӿ����ã���P8.2��P8.3��ΪUSCI�Ľ��տ��뷢���
   UCA1CTL0 = 0x00;//USCI���ƼĴ���UCA1CTL0������
   UCA1CTL0 |= UCMSB ; //������λ���ͼĴ���Ϊ��λ���ȷ���ģʽ
   UCA1CTL1 = 0x00;//USCI���ƼĴ���UCA1CTL1������
   UCA1CTL1 |= UCSSEL_1;//ѡ��ʱ��SMCLK
   UCA1BR0 = 0x03;		//���ò�����
   UCA1BR1 = 0x00;
   UCA1MCTL = UCBRS_3+UCBRF_0;
   UCA1CTL1 &= ~UCSWRST;//������λ
   UCA1IE |= UCRXIE + UCTXIE;//ʹ�ܽ��ա������ж�
 }


void Init_ADC()
{
	P6SEL |= BIT0 + BIT1;//P6.0 P6.1����ΪAD������ص�ѹֵ�������ź�
	P6DIR &= ~BIT0 + ~BIT1;//P6.0 P6.1����Ϊ����
	ADC12CTL0 &= ~ADC12ENC;//ADC12CTL0�Ĵ���ENC����
	ADC12CTL0 |= ADC12SHT0_4 + ADC12ON + ADC12MSC;//ADC12SHT0_4
	// Set sampling time, MSCΪSHI�ź��״ο��Ʋ�������16��������=(16+9)/(20*10^6/4/2)=25/2.5^6 100kHz//SHTO_2,CLK = 2.5M
	ADC12MCTL1 |= ADC12INCH_4 + ADC12SREF_3;// ref+=VeREF+, channel = A0
	ADC12CTL1 |= ADC12CSTARTADD_0;//ת������洢��ADC12MEM0�У�
	ADC12CTL1 |= ADC12SHP + ADC12CONSEQ_2 + ADC12DIV_0 + ADC12SSEL_3 + ADC12SHS_0;
	// �������������ʽ+��ͨ����β�����ʽ+ʱ��ΪMCLK+ʱ�ӷ�Ƶ2+ADC12SC��־λ
//	ADC12CTL2 &= ~ADC12RES_3;//ADC12ת���ֱ���Ϊ8λ;
	ADC12CTL2 |= ADC12PDIV ;//�ο�ʱ��Ԥ��Ƶ4 //AD������Ϊ68.6kHz//SHTO_4,CLK = 5M//+ ADC12DF
//	ADC12IE |= ADC12IE0;//�жϱ�־λΪA0,��ʹ��DMA�������ݵ�ʱ����Ҫ�Լ����жϣ�����رոñ�־λ��
}


void Init_TimerA()
{
  //��ʱ������
  TA0CTL = TASSEL_2 + MC_1 + ID_0;//SMCLK+2��Ƶ+������ģʽ.8Mʱ��
  TA0CCR0 = PWM_COUNT; //399
//  TA0CCTL0 = CCIE + OUTMOD_4; //100us�ж�
  TA0CCR1 = PWM_COUNT/4;
  TA0CCTL1 = OUTMOD_2; //reset������͵�ƽ
  TA0CCR2 = PWM_COUNT/4;
  TA0CCTL2 = OUTMOD_2; //reset������͵�ƽ
}


void Init_DMA()
{
  //DMA����
  DMAflg = 1;////DMA�������ݿ��־λΪ1.��ʾ��ǰ���Ƶ�Rev1��
  DMACTL0 = DMA0TSEL_24;//DMA0�Ĵ���ԴΪADC12IFG0
  DMACTL4 = DMARMWDIS;//��ֹ����д��������λ
  DMA0CTL &= ~DMAIFG;//����жϱ�־λ
  DMA0CTL = DMADT_4 + DMADSTINCR_3  + DMAIFG + DMAEN + DMAIE;
  //�ظ����δ��䣬Ŀ���ַ��1���ж�ʹ��
  DMA0SZ = Save;
  //����Ĵ�С
  __data16_write_addr((unsigned short) &DMA0SA,(unsigned long)&ADC12MEM0);
  //����Դ��ַ
  __data16_write_addr((unsigned short) &DMA0DA,(unsigned long)&Rev1);
 // ����Ŀ�ĵ�ַ
}

