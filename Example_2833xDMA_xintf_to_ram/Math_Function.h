/*
 * Math_Function.h
 *
 *  Created on: 2017-1-12
 *      Author: zhjl769
 */

#ifndef MATH_FUNCTION_H_
#define MATH_FUNCTION_H_

/*
 * 函数功能：求数组的均值，数组中的元素与均值相减，
 * DATA为输入数组，Num为数组大小
 */

void DATA_Mean(int32 *DATA, int16 Num)
{
	int64 Sum = 0;
	int32 Sum1 = 0;
	int16 i;
	for(i = 0; i < Num; i++)
		Sum += DATA[i];
	Sum1 = (int32)(Sum/Num);       //直流量
	for(i = 0; i < Num; i++)
		DATA[i] = DATA[i] - Sum1;	 //变为符号int型信号
}


/*
 * 函数功能：判决门限
 * int32 Fix_DATA为递归估计中的固定初始值, int16 Fix_Num为为递归估计中的固定量, int32 DATA_IN为输入数组 , float DATA_OUT为输出数组
 */

void DATA_Gate(int32 Fix_DATA, int16 Fix_Num, int32 *DATA_IN, float *DATA_OUT)
{
	//long A=0x00500000;
	//int  N=16;
	int16 i, N = sizeof(DATA_IN);
	int32 Y0 = 0x00000000, Y1 = 0x00000000;
	for(i = 0; i < N; i++)
	{
		if(DATA_IN[i] > Y0)
			DATA_OUT[i] = (float)(DATA_IN[i]);
		Y1 = Y0*(Fix_Num/(Fix_Num-1)) + (Fix_DATA*Fix_DATA)/Fix_Num;            //噪声幅值递归估计
		if(Y1 == 0x00100000)
			Y1 = 0x00200000;
		Y0 = Y1;
	}
}

/*-------------------------------------------------**
 ** Function:void xcorr(float *x,float *y,float *z) **
 ** 1.将x，y做互相关 存入 z                         **
 **-------------------------------------------------*/

void xcorr(float *DATA_IN1, int16 Num1, float *DATA_IN2, int16 Num2, float *DATA_OUT)
{
	int16 i = 0, j = 0;
	float64 Sum_data = 0.0;
	while(i < Num2)
	{
		if(i < (Num2 - Num1))
		{
			Sum_data = 0.0;
			for(j = 0; j < Num1; j++)
				Sum_data += (float64)(DATA_IN1[j] * DATA_IN2[i + j]);
			DATA_OUT[i] = (float32)Sum_data;
			i ++;
		}
		else
		{
			for(j = 0; j < (Num2  - i); j++)
				Sum_data += (float64)(DATA_IN1[j] * DATA_IN2[i + j]);
			DATA_OUT[i] = (float32)Sum_data;
			i ++;
		}
	}
}

/*----------------------------------------------------**
 ** Function:int max(float *z,int BUF_SIZE)             **
 ** 1.返回数据长度为size的数组中的最大值在数组中的位置 **
 **----------------------------------------------------*/

int16 DATA_Max(float32 *DATA_IN, int16 SIZE)
{
	int16 i, adress;
	float *max, *q;
	q = DATA_IN;
	max = DATA_IN;
	for(i = 0; i < SIZE; i++)
	{
		if(*max < *q)
		{
			*max = *q;
			adress = i;
		}
		q++;
	}
	return adress;
}

void Xcorr_refdata()
{
	unsigned long lfm_head,t_Ref;
	double t_lfm;
	float t_temp;
	t_temp = ((float) Time_Ref)/1000;
	memset(DATABuf3,0,sizeof(short)*Block_xcorr);  //参考信号
	if(f2_Ref >= f1_Ref)
	{
		t_Ref = ((unsigned long)(f2_Ref-f1_Ref))*1000;
		for(lfm_head = 0; lfm_head <Time_Ref*fs; lfm_head++)
		{
			t_lfm = (float) lfm_head/(((float) fs)*1000);//采样频率18kHz
			DATABuf3[lfm_head] = (float)(cos((double)(2*PI*(f2_Ref*1000)*t_lfm - PI*t_Ref*t_lfm*t_lfm/t_temp)));
		}
	}
	else
	{
		t_Ref = ((unsigned long)(f1_Ref-f2_Ref))*1000;
		for(lfm_head = 0; lfm_head <Time_Ref*fs; lfm_head++)
		{
			t_lfm = (float)lfm_head/(((float) fs)*1000);//采样频率18kHz
			DATABuf3[lfm_head] = (float)(cos((double)(2*PI*(f2_Ref*1000)*t_lfm + PI*t_Ref*t_lfm*t_lfm/t_temp)));
		}
	}
}


#endif /* MATH_FUNCTION_H_ */
