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
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//������ʱ��������ֱ�ӵ���delay_ms (x),xΪ��Ҫ��ʱ��ʱ��


void Init_CLK(void)
{
    unsigned int i;
    BCSCTL1 = 0X00;//���Ĵ�������������
    BCSCTL1 &= ~XT2OFF + ~XTS + DIVA_0;//LFTX1�����ڵ�Ƶģʽ,XT2��������,ACLK�ķ�Ƶ����Ϊ1
    do
    {
	IFG1 &= ~OFIFG;                       // ���OSCFault��־
	for (i = 0xff; i > 0; i--);
    }
    while ((IFG1 & OFIFG) == OFIFG);      // ���OSCFault =1

    BCSCTL2 = 0X00;			//���Ĵ�������������
    BCSCTL2 |= SELM_2;		//MCLK��ʱ��ԴΪTX2CLK����Ƶ����Ϊ1
    BCSCTL2 |= SELS;		//SMCLK��ʱ��ԴΪTX2CLK����Ƶ����Ϊ1
}


void IO_Init(void)
{
   P1DIR = 0Xff; //����P1�ڷ���Ĵ���P1����Ϊ���
   P1SEL |= BIT3 ;//P1.3����Ϊ�����ź�����
   P1DIR &= ~BIT3;//����P1.3�ڷ���Ĵ�����Ϊ����
   P2DIR = 0xff;//,����P2Ϊ���
   P2OUT = 0x00;
   P2DIR &= ~BIT5;//,����P2.5Ϊ����;
   P2IES &= ~BIT5;//��ʼֵ���������Ϊ�ж�ʹ�ܣ�
//   P2IE |= BIT5;//����p2.5���ж�
   P3DIR = 0xFF; //P3��Ϊ���
   P3SEL |= BIT6+BIT7; //P3�ڵ�6��7���Ź���Χģ��ʹ��
   P4DIR = 0XFF; //����P4�ڷ���Ĵ�����ʹP4��Ϊ���
   P4SEL = 0;
   P4OUT = 0x00;
}


void UART1_Init(void) //UART1��ʼ������
 {
   U1CTL &= ~(SWRST + SSEL1); //USART���ƼĴ���UCTL1��SWRST=0x01��~SWRST=0xFE����UCTL0�Ĵ����ĵ�0λ��λ��USART�������±�����
   U1CTL = 0X00;//USART���ƼĴ���UCTL1������
   U1CTL += CHAR;//����λΪ8bit
   U1CTL &= ~SPB;//1λֹͣλ
   U1TCTL |= SSEL1;//���Ϳ��ƼĴ�������4λ��1��ѡ����ʱ��ACLK1
   U1BR0 = 0x41; //9600�����ʣ�32kHzʱ��
   U1BR1 = 0x03;
   U1MCTL = 0x00; //UART�����ʵ�����
   U1ME |= UTXE1 + URXE1; //ģ�������Ĵ���ME2,����ģ�������Ĵ���ME1��UTXE0=0x80������ME1�ĵ�7λΪ1��//UTXE1=0x80 ��������//URXE1=0x40 ��������
   U1IE |= URXIE1; //�ж������Ĵ���IE2����5λΪ1��ʹ�ܽ����жϣ�URXIE1=0x10
   U1IE |= UTXIE1; //�ж������Ĵ���IE2����6λΪ1��ʹ�ܷ����жϣ�UTXIE1=0x20
 }


void Init_ADC()
{
//	ADC12CTL0 &= 0x00;//ADC12CTL0�Ĵ�������
	ADC12CTL0 &= ~ENC;//ADC12CTL0�Ĵ���ENC����
	ADC12CTL0 |= SHT0_2 + ADC12ON + MSC;  // Set sampling time, MSCΪSHI�źſ���
//	ADC12CTL1  &= 0x00;//ADC12CTL1�Ĵ�������
	ADC12CTL1 |= CSTARTADD_7;//ת������洢��ADC12MEM7�У�
	ADC12CTL1 |= SHP + CONSEQ_2+ADC12DIV_0+ADC12SSEL_2+SHS_0;   // �������������ʽ+��ͨ����β�����ʽ+ʱ��ΪMCLK+ʱ�ӷ�Ƶ0+ADC12SC��־λ
	ADC12MCTL7 |= INCH_7+SREF_3;  // ref+=VeREF+, channel = A7
	ADC12IE |= BIT7;  //�жϱ�־λΪA7
	_nop();
	_nop();
	_nop();
	_nop();
	_nop();
	ADC12CTL0 |=  ADC12SC + ENC;//����ϴβɼ���ֵ�ﵽ��Ҫ�󣬲�ֹͣ�ˣ���ʼ����ת������ȡ��ѹֵ��
}


void Init_TimerA()
{
  //��ʱ������
  TA0CTL |= TASSEL_2 +MC_0 + ID_0 + TAIE;//SMCLK+8��Ƶ+��������ģʽ.8Mʱ��,������ж�
  TA0CCTL0 |= CCIE; //AO�����ж�
  TA0CCTL2 |= CM_0 + CCIS_0 + CAP + CCIE; //reset,����ģʽ������P1.3��ͬ�����񣬹ر������ز���
}



