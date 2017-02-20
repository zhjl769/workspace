/*
 * ADC12.c
 *
 *  Created on: 2015-12-9
 *      Author: zhjl769
 */
#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <in430.h>
#include "ADC12.h"



void ADC12_Start()
{
	if(Num == 99)
	{
	 ADC12IE |= BIT7;  //�����жϱ�־λΪA7
	 Num = 0;//��ʼ��Num��ֵ
     ADC12CTL0 |=  ADC12SC + ENC;//����ϴβɼ���ֵ�ﵽ��Ҫ�󣬲�ֹͣ�ˣ���ʼ����ת������ȡ��ѹֵ��
	 }

}



// ADC12 interrupt service routine
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR (void)
{

  static unsigned long A7 = 0;//��ƽ��ֵ���ȸ���,static���徲̬������ֻ����һ�Σ�������Ӱ��ֵ
//  static int Num = 1;
  if(Num < 99)//����100����
   {
	A7 += ADC12MEM7;
	Num++;
   }
  else
  {
	ADC12IE &= ~BIT7;//�ر��ж�
	ADC12CTL0 &= ~ENC;//
	while(ADC12CTL1 & ADC12BUSY == 0X01)//�ȴ��Ų������
	ADC12CTL0 &= ~ENC;//ֹͣ����
	A7 += ADC12MEM7;//��ȡ���һ�εĽ��ֵ
	A7 /= 100;//��ƽ��ֵ�����ٴ���Ӱ��
	Voltage = ((250*A7)>>12)*5/196;//��ȡ��ʵ�ĵ�ѹֵ
  }
}

