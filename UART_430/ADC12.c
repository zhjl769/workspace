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
	 ADC12IE |= BIT7;  //开启中断标志位为A7
	 Num = 0;//初始化Num的值
     ADC12CTL0 |=  ADC12SC + ENC;//如果上次采集的值达到了要求，并停止了，开始启动转换，获取电压值。
	 }

}



// ADC12 interrupt service routine
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR (void)
{

  static unsigned long A7 = 0;//求平均值，先赋零,static定义静态变量，只声明一次，后续不影响值
//  static int Num = 1;
  if(Num < 99)//采样100个数
   {
	A7 += ADC12MEM7;
	Num++;
   }
  else
  {
	ADC12IE &= ~BIT7;//关闭中断
	ADC12CTL0 &= ~ENC;//
	while(ADC12CTL1 & ADC12BUSY == 0X01)//等待才采样完成
	ADC12CTL0 &= ~ENC;//停止采样
	A7 += ADC12MEM7;//获取最后一次的结果值
	A7 /= 100;//求平均值，减少错点的影响
	Voltage = ((250*A7)>>12)*5/196;//获取真实的电压值
  }
}

