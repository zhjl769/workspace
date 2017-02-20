/*
 * DMA.c
 *
 *  Created on: 2016-6-3
 *      Author: zhjl769
 */

#include <msp430f6638.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <in430.h>
#include "DMA.h"
#include "ADC12.h"
#include "Variable.h"
#include "dsplib.h"


// DMA interrupt service routine
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR (void)
{
	unsigned int i, j = 0;
	switch(__even_in_range(DMAIV,16))
	{
	case 0:break;			//无中断
	case 2:
		if(DMAflg == 1)
		{
			__data16_write_addr((unsigned short) &DMA0DA,(unsigned long)&Rev2);
			// 设置目的地址
			DMAflg = 2;//DMA接收数据块标志位为2.表示下次搬移到Rev2中

//			P1OUT &= ~BIT0;//发射上电
			i = Save;
			while( i < Di)
			{
				Rev[j++] = Rev[i++];
			}
			i = Ds; j = 0;
			while(j < Save)
			{
				Rev[i++] = (Rev1[j++] - 0x800) >> 3;//无符号数转换为有符号数
			}
//			P1OUT |= BIT0;//发射下电

		}
		else if(DMAflg != 1)
		{
			__data16_write_addr((unsigned short) &DMA0DA,(unsigned long)&Rev1);
			// 设置目的地址
			DMAflg = 1;//DMA接收数据块标志位为1.表示下次搬移到Rev1中

			i = Save;
			while( i < Di)
			{
				Rev[j++] = Rev[i++];
			}
			i = Ds; j = 0;
			while(j < Save)
			{
				Rev[i++] = (Rev2[j++] - 0x800) >> 3;//无符号数转换为有符号数
			}
		}
		DMAdone ++;
//		datacla();
		break;			//DMA0通道产生中断
	case 4:break;			//DMA1通道产生中断
	case 6:break;			//DMA2通道产生中断
	case 8:break;			//DMA3通道产生中断
	case 10:break;			//DMA4通道产生中断
	case 12:break;			//DMA5通道产生中断
	case 14:break;			//DMA6通道产生中断
	case 16:break;			//DMA7通道产生中断
	default: break;
	}
}




