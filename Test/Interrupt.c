/*
 * UART.c
 *
 *  Created on: 2015-12-10
 *      Author: zhjl769
 */
#include <msp430f6638.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "DS18B20.h"
#include "ADC12.h"
#include "Interrupt.h"



// 处理来自串口 1 的接收中断
#pragma vector=USCI_A1_VECTOR	//USCI中断服务函数
__interrupt void USCI_A1_ISR(void)
{
	switch(__even_in_range(UCA1IV,4))
	{
	case 0:break;			//无中断
	case 2:					//接收中断处理
		while(!(UCA1IFG & UCRXIFG));//等待完成接收
		if(Rindex < 2)
		{
		   RecBuf[Rindex]=UCA1RXBUF;//数据读出
		   Rindex++;
		}
		else
		   Rindex = 0;
		if(Rindex == 1 || RecBuf[0] == 1 || RecBuf[1] == 2)
		{
			temperature_get();
		}
		  break;
	case 4:
		while(Tindex < 10)
		{
		UCA1TXBUF = *(SDat+Tindex);
		 Tindex++;
		 while(!(UCA1IFG & UCTXIFG));//等待完成发送
		}
		break;			//发送中断不处理
	default:break;			//其他情况无操作

	}
}


void temperature_get(void)
{
 unsigned int qian,bai;
 TPT = TPT*100;
 qian = (unsigned char) (TPT/1000);
 bai = (unsigned char) ((TPT - qian*1000)/100);
 //shi = (unsigned char) (((TPT - qian*1000)-bai*100)/10);
 //ge = (unsigned char) ((((TPT - qian*1000)-bai*100)-shi*10));

 TPT = qian*10+bai;
 memset(TecBuf,0,10);
 memset(TecBuf,0xff,5);
 TecBuf[1] = 0x63;
 TecBuf[2] = TPT;
 TecBuf[3] = 0x00;
 SDat = TecBuf;
 UCA1IFG |= UCTXIFG; // 设置中断标志，进入发送中断程序
}

void Voltage_get(void)
{
 unsigned int Vo;
 Voltage = Voltage*100;
 Vo = Voltage;
 //bai = (unsigned char) (Voltage/100);
 //shi = (unsigned char) ((Voltage - bai*100)/10);

 Vo = (-(300 - Vo)/20);
 memset(TecBuf,0,10);
 memset(TecBuf,0xff,5);
 TecBuf[1] = 0x60;
 TecBuf[2] = Vo;
 TecBuf[3] = 0x00;
 SDat = TecBuf;
 UCA1IFG |= UCTXIFG; // 设置中断标志，进入发送中断程序
}


