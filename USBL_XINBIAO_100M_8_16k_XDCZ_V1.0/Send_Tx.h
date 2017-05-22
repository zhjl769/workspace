/*
 * Send_Tx.h
 *
 *  Created on: 2017-4-21
 *      Author: Administrator
 */

#ifndef SEND_TX_H_
#define SEND_TX_H_

#include <stdint.h>

#include <csl.h>
#include <csl_dma.h>
#include <csl_timer.h>
#include <csl_chip.h>

#include "variable.h"
#include "function_declare.h"

//void Produce_LFM_test()//9-15k,1.62s,LFM
//{
//	volatile unsigned long lfm_head;
//	volatile double t_lfm;
//	volatile short templfm;
//
//	for(lfm_head = 0; lfm_head < 1555200; lfm_head++)
//	{
//		t_lfm = (float)lfm_head/960000.0;//960kHz
//		templfm = (short)(20000*(cos(2*PI*9000*t_lfm + PI*6000*t_lfm*t_lfm/1.62)));
//
//		if(templfm < 0)
//		{
//			Pwm_result[lfm_head]=0x0005;//GPIO2=1;GPIO1=0;GPIO0=1;
//		}
//		else
//		{
//			Pwm_result[lfm_head]=0x0003;//GPIO2=0;GPIO1=1;GPIO0=1;
//		}
//	}
//}

/*发送通信信号前的准备工作，主要就是配置DMA第一帧的地址*/
void Send_Tx_prepare(volatile short *Src)
{
	int i;
	AD_temp = Src;//取址
	for(i=0;i<480;i++)//把待发送序列前480个点转移到Sig_Tx_temp_1中，此处避免使用指针，而是使用数组的方式访问，防止出现跨页错误
	{
		Sig_Tx_temp_1[i] = AD_temp[i];
	}
	/*配置第一帧源地址*/
	addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_1)) << 1)	& 0xFFFFu;
	DMA_RSETH(hDmaSend1,DMACSSAL,addr_temp);
	addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_1)) >> 15)	& 0xFFFFu;
	DMA_RSETH(hDmaSend1,DMACSSAU,addr_temp);
}
/*发送通信信号*/
void Send_Tx(volatile short *Src)
{
	/*发送通信信号*/
	DMA_send_over_flag = 0;			//清零标志位
	DMA_frame_send_over_flag = 0;	//清零帧发送完毕标志位
	Frame_count = 0;				//清零帧计数
	/*开始发送数据*/
	DMA_FSETH(hDmaSend1,DMACCR,ENDPROG,1);
	DMA_start(hDmaSend1);
	TIMER_start(mhTimer1);
	/*转移第2帧数据*/
	volatile long int i;
	AD_temp = Src;//取址
	for(i=0;i<480;i++)//把待发送序列第481-第960个点转移到Sig_Tx_temp_2中作为第二帧
	{
		Sig_Tx_temp_2[i] = AD_temp[i+480];
	}
	/*配置第2帧源地址*/
	addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_2)) << 1)	& 0xFFFFu;
	DMA_RSETH(hDmaSend1,DMACSSAL,addr_temp);
	addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_2)) >> 15)	& 0xFFFFu;
	DMA_RSETH(hDmaSend1,DMACSSAU,addr_temp);
	/*乒乓发射配置DMA*/
	while(1)
	{

		if(DMA_frame_send_over_flag == 1)//当前帧发送完毕，且下一帧已经开始发送了！！！
		{
			DMA_frame_send_over_flag = 0;//清零标志位
			/*配置奇数帧*/
			if(Frame_count%2 == 1)//注：Frame_count表示的是已经发完的帧数，Frame_count+1是现在正在发送的帧，接下来应该配置的是Frame_count+2，但是+2并不影响判断奇偶，所以省略了
			{
				/*转移数据*/
				for(i=0;i<480;i++)
				{
					Sig_Tx_temp_1[i] = Pwm_result[i+480*(Frame_count+1)];
				}
				/*配置奇数帧源地址*/
				addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_1)) << 1)	& 0xFFFFu;
				DMA_RSETH(hDmaSend1,DMACSSAL,addr_temp);
				addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_1)) >> 15)	& 0xFFFFu;
				DMA_RSETH(hDmaSend1,DMACSSAU,addr_temp);
			}
			/*配置偶数帧*/
			else
			{
				/*转移数据*/
				for(i=0;i<480;i++)
				{
					Sig_Tx_temp_2[i] = Pwm_result[i+480*(Frame_count+1)];
				}
				/*配置偶数帧源地址*/
				addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_2)) << 1)	& 0xFFFFu;
				DMA_RSETH(hDmaSend1,DMACSSAL,addr_temp);
				addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_2)) >> 15)	& 0xFFFFu;
				DMA_RSETH(hDmaSend1,DMACSSAU,addr_temp);
			}
		}
		if(Frame_count == 3239)
		{
			break;//最后一帧配好了，跳出当前循环
		}

	}
	/*查询DMA是否发送完毕*/
	while(1)
	{
		if(DMA_send_over_flag == 1)
		{
			break;
		}
	}
	DMA_send_over_flag=0;//清零标志位
}

void Produce_LFM()//9-15k,40ms,LFM
{
	volatile unsigned long lfm_head;
	volatile double t_lfm;
	volatile short templfm;

	for(lfm_head = 0; lfm_head < 38400; lfm_head++)
	{
		t_lfm = (float)lfm_head/960000.0;//960kHz
		templfm = (short)(20000*(sin(2*PI*9000*t_lfm + PI*6000*t_lfm*t_lfm/0.04)));

		if(templfm < 0)
		{
			Sig_LFM[lfm_head]=0x0005;//GPIO2=1;GPIO1=0;GPIO0=1;
		}
		else
		{
			Sig_LFM[lfm_head]=0x0003;//GPIO2=0;GPIO1=1;GPIO0=1;
		}
	}
}
//void Produce_long_LFM()//9-16k,1.62s,LFM
//{
//	volatile unsigned long lfm_head;
//	volatile double t_lfm;
//	volatile short templfm;
//
//	for(lfm_head = 0; lfm_head < 1555200; lfm_head++)
//	{
//		t_lfm = (float)lfm_head/960000.0;//1MHz
//		templfm = (short)(20000*(sin(2*PI*9000*t_lfm + PI*7000*t_lfm*t_lfm/1.62)));
//
//		if(templfm < 0)
//		{
//			Pwm_result[lfm_head]=0x0005;//GPIO2=1;GPIO1=0;GPIO0=1;
//		}
//		else
//		{
//			Pwm_result[lfm_head]=0x0003;//GPIO2=0;GPIO1=1;GPIO0=1;
//		}
//	}
//}
void Produce_CW()//9k,50ms,CW
{
	volatile unsigned long CW_head;
	volatile double t_CW;
	volatile short tempCW;

	for(CW_head = 0; CW_head < 48000; CW_head++)
	{
		t_CW = (float)CW_head/960000.0;//1MHz
		tempCW = (short)(20000*(cos(2*PI*9000*t_CW)));

		if(tempCW < 0)
		{
			Sig_CW[CW_head]=0x0005;//GPIO2=1;GPIO1=0;GPIO0=1;
		}
		else
		{
			Sig_CW[CW_head]=0x0003;//GPIO2=0;GPIO1=1;GPIO0=1;
		}
	}
}

void Send_pre_sig(unsigned short *Src , unsigned length , short model)
{//model=1时发送1遍（LFM）；model=2时发送2遍（CW）
	/*发送通信信号*/
	DMA_send_over_flag=0;//清零标志位
	AD_temp = (volatile short *)Src;//取址
	/*配置DMA0源地址*/
	addr_temp = (Uint16)(((Uint32)(AD_temp)) << 1) 	& 0xFFFFu;
	DMA_RSETH(hDmaSend3,DMACSSAL,addr_temp);
	addr_temp = (Uint16)(((Uint32)(AD_temp)) >> 15) & 0xFFFFu;
	DMA_RSETH(hDmaSend3,DMACSSAU,addr_temp);
	/*配置发送长度*/
	DMA_RSETH(hDmaSend3,DMACEN,length);
	/*开始发送数据*/
	DMA_FSETH(hDmaSend3,DMACCR,ENDPROG,1);
	DMA_start(hDmaSend3);
	TIMER_start(mhTimer1);
	while(1)//查询DMA是否发送完毕
	{
		if(DMA_send_over_flag == 1)
		{
			break;
		}
	}
	DMA_send_over_flag=0;//清零标志位
	if(model == 2)//再发一遍
	{
		/*开始发送数据*/
		DMA_FSETH(hDmaSend3,DMACCR,ENDPROG,1);
		while(1)//查询DMA是否发送完毕
		{
			if(DMA_send_over_flag == 1)
			{
				break;
			}
		}
		DMA_send_over_flag=0;//清零标志位
	}
	TIMER_stop(mhTimer1);
	DMA_stop(hDmaSend3);
	GPIO_RSET(IODATA,0x00);//GPIO输出为低

}


/*低速时钟计时器*/
void timer_low_clk(volatile Uint32 time,Uint16 clk)//time为预期计时时间单位us; clk为系统时钟频率单位MHz
{
	//CHIP_FSET(ST1_55,XF,1);//XF引脚置高
	//CHIP_FSET(ST1_55,XF,0);//XF引脚置低
	volatile Uint16 a=0,b=0,temp_PRD=0,temp_PRSC=0;
	time = time*clk;//转换成时钟周期
	a = time>>15;//取商（移位除法，等效于除以32768）
	b = time-(((unsigned long)a)<<15);//取余
	a = a-1;//商的值减去1，加到余数里，这样可以保证余数永远大于等于32768，防止余数过小，对余数定时时间太短
	b = b+32768-1;//余数值预先减1，准备赋值给寄存器
	temp_PRD = TIMER_RGET(PRD1);//保存之前的寄存器
	temp_PRSC = TIMER_RGET(PRSC1);//保存之前的寄存器
	TIMER_RSET(PRD1,0x7FFFu);//0x7FFFu = 十进制32767
	TIMER_RSET(PRSC1,0x0000u);//高位清零
	TIMER_FSET(TCR1,TLB,1);//TLB置1，使得PRD、TDDR中的值装入TIM、PSC（确保定时器已停止才能置位！）
	timer1_cnt = 0;
	TIMER_start(mhTimer1);
	while(timer1_cnt < a)
	{
		;
	}
	TIMER_stop(mhTimer1);
	TIMER_RSET(PRD1,b);
	TIMER_FSET(TCR1,TLB,1);//TLB置1，使得PRD、TDDR中的值装入TIM、PSC（确保定时器已停止才能置位！）
	TIMER_start(mhTimer1);
	while(timer1_cnt < a+1)
	{
		;
	}
	TIMER_stop(mhTimer1);
	TIMER_RSET(PRD1,temp_PRD);
	TIMER_RSET(PRSC1,temp_PRSC);//恢复程序，恢复PRD,PRSC寄存器
	TIMER_FSET(TCR1,TLB,1);//TLB置1，使得PRD、TDDR中的值装入TIM、PSC（确保定时器已停止才能置位！）
	timer1_cnt = 0;
}




#endif /* SEND_TX_H_ */
