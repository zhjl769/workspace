/*
 * Xcorr.c
 *
 *  Created on: 2016-6-6
 *      Author: zhjl769
 */

#include <msp430.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Xcorr.h"
#include "DSPLib.h"
#include "Variable.h"



/*函数参数：f为产生参考信号的频率，fsc为采样率，单位为kHz，temp为产生的数组，Number为产生的点数，与temp的大小一致*/
void Build_rev(float f, unsigned long fsc,_q15 *temp, int Number)
{
	unsigned int lfm_head;
	float t_lfm, pi;
	pi = acosf(-1);
	memset(temp,0,sizeof(int)*Number);
	for(lfm_head = 0; lfm_head < Number; lfm_head++)
	{
		t_lfm = (float)lfm_head/(float)fsc;//采样频率
		temp[lfm_head] = (int)(10*(sinf(2*pi*f*t_lfm)));
		//		temp[lfm_head] = _Q15(0.1*sinf(2*pi*f*t_lfm));
	}
}


void xcorr(short number, const int *inbuf, short num, const int *ref, _q15 *result)
{
	unsigned int k = 0,j = num;

	//	uint16_t *resultPtr = (uint16_t *)result;
	/* If MPY32 is available save control context and set to fractional mode. */
	uint16_t ui16MPYState = MPY32CTL0;
	MPY32CTL0 = MPYSAT;// MPYDLYWRTEN | MPYFRAC |
	/* Reset multiplier context. */
	MPY32CTL0 &= ~MPYC;
	RESHI = 0; RESLO = 0;
	while(k <= (number - num))
	{
		/* Loop through all vector elements. */
		while (j --)//j--
		{
			/* Multiply and accumulate srcA and srcB. */
			MACS = ref[j];
			OP2 = inbuf[j + k];
		}
		/* Store result. */
		*result++ = abs(RESLO);//整型转换为Q15
		RESLO = 0;
		j = num;
		k = k + 5;
	}

	/* Restore MPY32 control context. */
	MPY32CTL0 = ui16MPYState;
}


void msp_mean_q15(short num, short div, short block, _q15 *inb, _q15 *outb)
{
	/* 32-bit accumulator for saturation check */
	int32_t result = 0;
	int temp = 0, i = 0, j;
	/* Loop through all vector elements. */
	while (div--)
	{
		j = block;
		/* Sum srcA and srcB to 32-bit accumulator. */
		while (j--)
		{
			result = (int32_t)inb[j + i] + result;
		}
		temp = ((int)(result/block)) >> 1;
		result = 0; j = block;
		while (j --)
		{
			/* Sum srcA and srcB to 32-bit accumulator. */
			outb[j + i] = inb[j + i] - temp;
		}
		i = block + i;
	}

}



void msp_Fang_q15(short num, _q15 *inb, _q15 *outb)
{
	/* 32-bit accumulator for saturation check */
	int32_t result = 0;
	int temp = 0;
	int i = num;
	/* Loop through all vector elements. */
	while (num--) {
		/* Sum srcA and srcB to 32-bit accumulator. */
		result = (int32_t)inb[num] + result;
	}
	temp = (int)(result/i);
	while (i--) {
		/* Sum srcA and srcB to 32-bit accumulator. */
		if(inb[i] > temp)
			outb[i] = 200;
		else
			outb[i] = 0;
	}
	/* Saturate and store the result to the destination vector. */
	//	*outb++ = (_q15)__saturate(result, INT16_MIN, INT16_MAX);

}



/*div数据的大小应该是总的数据除以采样
 * 例如：数据点数为125，那么采样率为5kHz，故降采样为1kHz,那么为总的数据除以5。
 */
void Savefft_q15(short num, short div, _q15 *inb, _q15 *outb)
{
	unsigned int i,j,k = num;
	j = div + 2; i = div*(Block + 1);
	while(j--)
	{
		outb[i--] = inb[k];
		k = k - 13;
	}
	Block++;
}

void DATAabs_q15(unsigned short num, _q15 *inbuf, _q15 *outbuf)
{
	while(num--)
		outbuf[num] = abs(inbuf[num]);
}


void FFTsave_out_q15(unsigned short num, _q15 *inbuf, _q15 *outbuf)
{
	unsigned short i = 249;
	while(num--)
		outbuf[num] =inbuf[i--];
}


void FFTsave_in_q15(unsigned short num, _q15 *inbuf, _q15 *outbuf)
{
	while(num--)
		outbuf[num] = inbuf[num];
}



void Neicha_q15(uint16_t *MaxIndex_in, float *Maxout, _q15 *inbuf)
{
	unsigned short position1, position2, position3;
	int Vector1, Vector2, Vector3;
	float temp1, temp2, result;
	position2 = *MaxIndex_in;
	position1 = position2 - 1;
	position3 = position2 + 1;
	Vector1 = inbuf[position1];
	Vector2 = inbuf[position2];
	Vector3 = inbuf[position3];

	temp1 = (float) (Vector1 + Vector3 - (Vector2 << 1));
	result = temp1 * 2 * position2;
	temp2 = ((float) (Vector3) - (float) (Vector1) - result)/2;

	if (temp1 == 0)
		*Maxout = (float)position2;
	else
	    temp2 = - temp2;
	    *Maxout = temp2/temp1;
}

