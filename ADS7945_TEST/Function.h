/*
 * Function.h
 *
 *  Created on: 2017-2-28
 *      Author: zhjl769
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_


/*���������ź�
*  �������ܣ������źţ������ź�
*Fre_s, �źŲ�����,��λ��kHz
*Time_Delay, �����ź�����,��λ��ms
*f1_Fre   �����ź�����Ƶ��,��λ��kHz
*f2_Fre�������ź�����Ƶ��,��λ��kHz
*Buffer,�洢���ݿռ�
*/
void Build_Frenal(Uint16 Fre_s, Uint16 f1_Fre, Uint16 f2_Fre, Uint16 Time_Delay, Int16 *Buffer)
{
	volatile unsigned long lfm_head, t_Sig;
	Int32 templfm;
	volatile double t_lfm;
	float t_Sigtemp;
	t_Sigtemp = ((float) Time_Delay)/1000;
	if(f2_Fre >= f1_Fre)
	{
		t_Sig = ((unsigned long)(f2_Fre - f1_Fre))*1000;
		for(lfm_head = 0; lfm_head < Time_Delay*1000; lfm_head++)
		{
			t_lfm = (float)lfm_head/Fre_s;//����Ƶ��
			templfm = (short)(200*(cos(2*3.14159*(f1_Fre*1000)*t_lfm + 3.14159*t_Sig*t_lfm*t_lfm/t_Sigtemp)));//��ʼƵ��f1_Fre����ֹƵ��f2_Fre,Time_Delay
			if(templfm < 0)
				Buffer[lfm_head] = 0x05;//GPIO2=1;GPIO1=0;GPIO0=1;
			else
				Buffer[lfm_head] = 0x03;//GPIO2=0;GPIO1=1;GPIO0=1;
		}
	}
	else
	{
		t_Sig = ((unsigned long)(f1_Fre - f2_Fre))*1000;
		for(lfm_head = 0; lfm_head < Time_Delay*1000; lfm_head++)
		{
			t_lfm = (float)lfm_head/1000000.0;//����Ƶ��1MHz
			templfm = (short)(200*(cos(2*3.14159*(f1_Fre*1000)*t_lfm - 3.14159*t_Sig*t_lfm*t_lfm/t_Sigtemp)));//��ʼƵ��f1_Fre����ֹƵ��f2_Fre,Time_Delay
			if(templfm < 0)
				Buffer[lfm_head] = 0x05;//GPIO2=1;GPIO1=0;GPIO0=1;
			else
				Buffer[lfm_head] = 0x03;//GPIO2=0;GPIO1=1;GPIO0=1;
		}
	}

}





#endif /* FUNCTION_H_ */
