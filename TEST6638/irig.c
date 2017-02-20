/*
 * irig.c
 *
 *  Created on: 2016��3��5��
 *      Author: zane
 */
#include"msp430f6638.h"
#include"main.h"
#include"irig.h"
#include"uart.h"


int ADC_in = 0;					//AD���β���ֵ��ת��Ϊ�з�������
int	ADC_fir[FIR_number] = {0};	//FIR�˲����У�AD������������ֵ���κ���1λ��
int FIR_pos = 0;				//FIR�˲�����Ԫ��λ��
long FIR_out = 0;				//FIR��ͨ�˲����
int HIGH_num = 0;				//FIR�˲��߷�����ֵ
int LOW_num = 0;				//FIR�˲��ͷ�����ֵ

char state_judge = 0;			//1ms��Ԫ�о�
char state_machine = 0;			//״̬��״̬

char LAST_LEVEL =0;

int state[200] = {0};			//��Ԫ״̬�洢
int state_pos = 101;

//�˲������ݺ�����10��FIR��ͨ�˲�����ͨ��Ƶ��300Hz�����Ƶ��900Hz
int FIR_Hn[FIR_number] = {-296,400,1063,1840,2359,2359,1840,1063,400,-296};

unsigned char time[32]={"000000000000"};
char judge=0;

long FIR(int ADC_temp)
{
	long FIR_temp = 0;
	long temp = 0;
	long temp_h = 0;
	for(FIR_pos=(FIR_number-1);FIR_pos>0;FIR_pos--)
	{
		ADC_fir[FIR_pos] = ADC_fir[FIR_pos-1];
	}
	ADC_fir[0] = ADC_temp>>4;

	for(FIR_pos=0;FIR_pos<FIR_number;FIR_pos++)
	{
		MPYS = ADC_fir[FIR_pos];
		OP2 = ADC_fir[FIR_pos];
		temp_h = RESHI;
		temp = (temp_h<<16) + RESLO;
		MPYS = FIR_Hn[FIR_pos];
		OP2 = temp;
		temp_h = temp_h + RESHI;
		FIR_temp = FIR_temp + (temp_h<<16) + RESLO;
	}
	return FIR_temp;
}


void IRIG()
{
	ADC_ready();			//AD���β������
	if(FIR_out>FIR_value)	//FIR����������ޣ��ôβ���Ϊ�߷�
	{FIR_high();}			//FIR����߷�����
	else					//FIR����������ޣ��ôβ���Ϊ�ͷ�
	{FIR_low();}			//FIR����ͷ�����
}

void ADC_ready()			//AD���β�������
{
	ADC_in = (int)(ADC12MEM0 - 0x800);	//������ֵת��Ϊ�з�����
	FIR_out = FIR(ADC_in);				//�ôβ���ֵ����FIR�˲�
}

void FIR_high()
{
	HIGH_num++;
	LOW_num = 0;
	if(HIGH_num == 1)
	{
	switch (state_pos)
	{
		case	 1:	time[0]='A';	time[1]='1';	time[11]='B';						break;
		case 	 5:
			time[10]=48+state[1]+(state[2]<<1)+(state[3]<<2)+(state[4]<<3);
			break;
		case 	10:
			time[9]=48+state[6]+(state[7]<<1)+(state[8]<<2);
			break;
		case 	15:
			time[8]=48+state[10]+(state[11]<<1)+(state[12]<<2)+(state[13]<<3);
			break;
		case 	20:
			time[7]=48+state[15]+(state[16]<<1)+(state[17]<<2);
			break;
		case 	25:
			time[6]=48+state[20]+(state[21]<<1)+(state[22]<<2)+(state[23]<<3);
			break;
		case 	30:
			time[5]=48+state[25]+(state[26]<<1);
			break;
		case 	35:
			time[4]=48+state[30]+(state[31]<<1)+(state[32]<<2)+(state[33]<<3);
			break;
		case 	40:
			time[3]=48+state[35]+(state[36]<<1)+(state[37]<<2)+(state[38]<<3);
			break;
		case 	45:
			time[2]=48+state[40]+(state[41]<<1);
			break;
		case 	100:judge=1;			break;
		default	  :						break;
	}
	}
}


void FIR_low()
{
	if(LOW_num<4)
	{LOW_num++;}
    else
    {
    	if(HIGH_num>30)
    	{
    		state_judge = 2;
    		if(state_machine == 'I')
    		{
    			state_machine = 'A';
    		}
    		else if(state_machine == 'A')
    		{
    			state_machine = 'B';
    			state_pos = 0;
    		}
    	    state[state_pos] = state_judge;
    	    state_pos++;
    	}
    	else if(HIGH_num>15)
    	{
    		state_judge = 1;
    		state_machine = 'I';
    	    state[state_pos] = state_judge;
    	    state_pos++;
    	}
    	else if(HIGH_num>5)
    	{
    		state_judge = 0;
    		state_machine = 'I';
    	    state[state_pos] = state_judge;
    	    state_pos++;
    	}
    	HIGH_num = 0;
    	LOW_num = 0;
    }
}



