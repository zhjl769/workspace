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
	case 0:break;			//���ж�
	case 2:
		if(DMAflg == 1)
		{
			__data16_write_addr((unsigned short) &DMA0DA,(unsigned long)&Rev2);
			// ����Ŀ�ĵ�ַ
			DMAflg = 2;//DMA�������ݿ��־λΪ2.��ʾ�´ΰ��Ƶ�Rev2��

//			P1OUT &= ~BIT0;//�����ϵ�
			i = Save;
			while( i < Di)
			{
				Rev[j++] = Rev[i++];
			}
			i = Ds; j = 0;
			while(j < Save)
			{
				Rev[i++] = (Rev1[j++] - 0x800) >> 3;//�޷�����ת��Ϊ�з�����
			}
//			P1OUT |= BIT0;//�����µ�

		}
		else if(DMAflg != 1)
		{
			__data16_write_addr((unsigned short) &DMA0DA,(unsigned long)&Rev1);
			// ����Ŀ�ĵ�ַ
			DMAflg = 1;//DMA�������ݿ��־λΪ1.��ʾ�´ΰ��Ƶ�Rev1��

			i = Save;
			while( i < Di)
			{
				Rev[j++] = Rev[i++];
			}
			i = Ds; j = 0;
			while(j < Save)
			{
				Rev[i++] = (Rev2[j++] - 0x800) >> 3;//�޷�����ת��Ϊ�з�����
			}
		}
		DMAdone ++;
//		datacla();
		break;			//DMA0ͨ�������ж�
	case 4:break;			//DMA1ͨ�������ж�
	case 6:break;			//DMA2ͨ�������ж�
	case 8:break;			//DMA3ͨ�������ж�
	case 10:break;			//DMA4ͨ�������ж�
	case 12:break;			//DMA5ͨ�������ж�
	case 14:break;			//DMA6ͨ�������ж�
	case 16:break;			//DMA7ͨ�������ж�
	default: break;
	}
}




