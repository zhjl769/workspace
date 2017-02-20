/*
 * ADC12.c
 *
 *  Created on: 2015-12-9
 *      Author: zhjl769
 */
#include <msp430f6638.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <in430.h>
#include "DMA.h"
#include "Variable.h"
#include "Other.h"
#include "Init.h"
#include "ADC12.h"
#include "DMA.h"
#include "Xcorr.h"
#include "DSPLib.h"

void ADC12_Start()
{
	 DMAflg = 1;
	 memset(Rev1,0,Save);
	 memset(Rev2,0,Save);
//	 ADC12IE |= ADC12IE0;  ////�жϱ�־λΪA0,��ʹ��DMA�������ݵ�ʱ����Ҫ�Լ����жϣ�����رոñ�־λ��
	 ADC12CTL0 |=  ADC12ENC;//ʹ��ת��
	 _nop();
	 _nop();
     ADC12CTL0 |=  ADC12SC;//��ʼ����ת����
}

/*
// ADC12 interrupt service routine
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR (void)
{
	if((ADC12IFG & ADC12IFG0) != 0)
	{

		if(Flg%2 == 0)
		  Rev1[Num] = (ADC12MEM0 - 0x7FF);
		 else
		  Rev2[Num] = (ADC12MEM0 - 0x7FF);
		Num++;
		if(Num == 1000)
		  {
			  Num = 0;
			  Flg ++;
			  Flg1 = 1;//�������ɼ����
		  }
	}
}
*/
void datacla(void)
{
	unsigned int i, j;
	if(DMAdone != 0)
	{
		P1OUT &= ~BIT0;//�����ϵ�

		xcorr(Di, Rev, Dref, X_ref, Fout);

		j = 0;i = Dxcor - 1;
		while( i < Dfir)
		{
			Xout[j++] = Xout[i++];
		}

		j = 0;i = FIR_COEFF_ORDER;
		while( i < Dfir)
		{
			Xout[i++] = Fout[j++];
		}

		msp_fir_q15(&FirParams, Xout, Fout);
		msp_mean_q15(Dxcor, 5, 25, Fout, Fout);
//		msp_Fang_q15(135,Fout,Fout);
		P1OUT |= BIT0;//�����µ�
		DMAdone = 0;
		//ADC12_Start();
	}
	//temperature_get();
}

