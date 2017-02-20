/*
 * Math_Function.h
 *
 *  Created on: 2017-1-12
 *      Author: zhjl769
 */

#ifndef MATH_FUNCTION_H_
#define MATH_FUNCTION_H_

/*
 * �������ܣ�������ľ�ֵ�������е�Ԫ�����ֵ�����
 * DATAΪ�������飬NumΪ�����С
 */

void DATA_Mean(int32 *DATA, int16 Num)
{
	int64 Sum = 0;
	int32 Sum1 = 0;
	int16 i;
	for(i = 0; i < Num; i++)
		Sum += DATA[i];
	Sum1 = (int32)(Sum/Num);       //ֱ����
	for(i = 0; i < Num; i++)
		DATA[i] = DATA[i] - Sum1;	 //��Ϊ����int���ź�
}


/*
 * �������ܣ��о�����
 * int32 Fix_DATAΪ�ݹ�����еĹ̶���ʼֵ, int16 Fix_NumΪΪ�ݹ�����еĹ̶���, int32 DATA_INΪ�������� , float DATA_OUTΪ�������
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
		Y1 = Y0*(Fix_Num/(Fix_Num-1)) + (Fix_DATA*Fix_DATA)/Fix_Num;            //������ֵ�ݹ����
		if(Y1 == 0x00100000)
			Y1 = 0x00200000;
		Y0 = Y1;
	}
}

/*-------------------------------------------------**
 ** Function:void xcorr(float *x,float *y,float *z) **
 ** 1.��x��y������� ���� z                         **
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
 ** 1.�������ݳ���Ϊsize�������е����ֵ�������е�λ�� **
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
	memset(DATABuf3,0,sizeof(short)*Block_xcorr);  //�ο��ź�
	if(f2_Ref >= f1_Ref)
	{
		t_Ref = ((unsigned long)(f2_Ref-f1_Ref))*1000;
		for(lfm_head = 0; lfm_head <Time_Ref*fs; lfm_head++)
		{
			t_lfm = (float) lfm_head/(((float) fs)*1000);//����Ƶ��18kHz
			DATABuf3[lfm_head] = (float)(cos((double)(2*PI*(f2_Ref*1000)*t_lfm - PI*t_Ref*t_lfm*t_lfm/t_temp)));
		}
	}
	else
	{
		t_Ref = ((unsigned long)(f1_Ref-f2_Ref))*1000;
		for(lfm_head = 0; lfm_head <Time_Ref*fs; lfm_head++)
		{
			t_lfm = (float)lfm_head/(((float) fs)*1000);//����Ƶ��18kHz
			DATABuf3[lfm_head] = (float)(cos((double)(2*PI*(f2_Ref*1000)*t_lfm + PI*t_Ref*t_lfm*t_lfm/t_temp)));
		}
	}
}


#endif /* MATH_FUNCTION_H_ */