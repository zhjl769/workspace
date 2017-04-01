/*
 * Function.h
 *
 *  Created on: 2017-2-28
 *      Author: zhjl769
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_


/*产生发射信号
*  函数功能：产生信号，测阵信号
*Fre_s, 信号采样率,单位：kHz
*Time_Delay, 发射信号脉宽,单位：ms
*f1_Fre   发射信号上限频率,单位：kHz
*f2_Fre，发射信号下限频率,单位：kHz
*Buffer,存储数据空间
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
			t_lfm = (float)lfm_head/Fre_s;//采样频率
			templfm = (short)(200*(cos(2*3.14159*(f1_Fre*1000)*t_lfm + 3.14159*t_Sig*t_lfm*t_lfm/t_Sigtemp)));//起始频率f1_Fre，截止频率f2_Fre,Time_Delay
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
			t_lfm = (float)lfm_head/1000000.0;//采样频率1MHz
			templfm = (short)(200*(cos(2*3.14159*(f1_Fre*1000)*t_lfm - 3.14159*t_Sig*t_lfm*t_lfm/t_Sigtemp)));//起始频率f1_Fre，截止频率f2_Fre,Time_Delay
			if(templfm < 0)
				Buffer[lfm_head] = 0x05;//GPIO2=1;GPIO1=0;GPIO0=1;
			else
				Buffer[lfm_head] = 0x03;//GPIO2=0;GPIO1=1;GPIO0=1;
		}
	}

}





#endif /* FUNCTION_H_ */
