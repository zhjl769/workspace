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

/*����ͨ���ź�ǰ��׼����������Ҫ��������DMA��һ֡�ĵ�ַ*/
void Send_Tx_prepare(volatile short *Src)
{
	int i;
	AD_temp = Src;//ȡַ
	for(i=0;i<480;i++)//�Ѵ���������ǰ480����ת�Ƶ�Sig_Tx_temp_1�У��˴�����ʹ��ָ�룬����ʹ������ķ�ʽ���ʣ���ֹ���ֿ�ҳ����
	{
		Sig_Tx_temp_1[i] = AD_temp[i];
	}
	/*���õ�һ֡Դ��ַ*/
	addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_1)) << 1)	& 0xFFFFu;
	DMA_RSETH(hDmaSend1,DMACSSAL,addr_temp);
	addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_1)) >> 15)	& 0xFFFFu;
	DMA_RSETH(hDmaSend1,DMACSSAU,addr_temp);
}
/*����ͨ���ź�*/
void Send_Tx(volatile short *Src)
{
	/*����ͨ���ź�*/
	DMA_send_over_flag = 0;			//�����־λ
	DMA_frame_send_over_flag = 0;	//����֡������ϱ�־λ
	Frame_count = 0;				//����֡����
	/*��ʼ��������*/
	DMA_FSETH(hDmaSend1,DMACCR,ENDPROG,1);
	DMA_start(hDmaSend1);
	TIMER_start(mhTimer1);
	/*ת�Ƶ�2֡����*/
	volatile long int i;
	AD_temp = Src;//ȡַ
	for(i=0;i<480;i++)//�Ѵ��������е�481-��960����ת�Ƶ�Sig_Tx_temp_2����Ϊ�ڶ�֡
	{
		Sig_Tx_temp_2[i] = AD_temp[i+480];
	}
	/*���õ�2֡Դ��ַ*/
	addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_2)) << 1)	& 0xFFFFu;
	DMA_RSETH(hDmaSend1,DMACSSAL,addr_temp);
	addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_2)) >> 15)	& 0xFFFFu;
	DMA_RSETH(hDmaSend1,DMACSSAU,addr_temp);
	/*ƹ�ҷ�������DMA*/
	while(1)
	{

		if(DMA_frame_send_over_flag == 1)//��ǰ֡������ϣ�����һ֡�Ѿ���ʼ�����ˣ�����
		{
			DMA_frame_send_over_flag = 0;//�����־λ
			/*��������֡*/
			if(Frame_count%2 == 1)//ע��Frame_count��ʾ�����Ѿ������֡����Frame_count+1���������ڷ��͵�֡��������Ӧ�����õ���Frame_count+2������+2����Ӱ���ж���ż������ʡ����
			{
				/*ת������*/
				for(i=0;i<480;i++)
				{
					Sig_Tx_temp_1[i] = Pwm_result[i+480*(Frame_count+1)];
				}
				/*��������֡Դ��ַ*/
				addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_1)) << 1)	& 0xFFFFu;
				DMA_RSETH(hDmaSend1,DMACSSAL,addr_temp);
				addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_1)) >> 15)	& 0xFFFFu;
				DMA_RSETH(hDmaSend1,DMACSSAU,addr_temp);
			}
			/*����ż��֡*/
			else
			{
				/*ת������*/
				for(i=0;i<480;i++)
				{
					Sig_Tx_temp_2[i] = Pwm_result[i+480*(Frame_count+1)];
				}
				/*����ż��֡Դ��ַ*/
				addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_2)) << 1)	& 0xFFFFu;
				DMA_RSETH(hDmaSend1,DMACSSAL,addr_temp);
				addr_temp = (Uint16)(((Uint32)(Sig_Tx_temp_2)) >> 15)	& 0xFFFFu;
				DMA_RSETH(hDmaSend1,DMACSSAU,addr_temp);
			}
		}
		if(Frame_count == 3239)
		{
			break;//���һ֡����ˣ�������ǰѭ��
		}

	}
	/*��ѯDMA�Ƿ������*/
	while(1)
	{
		if(DMA_send_over_flag == 1)
		{
			break;
		}
	}
	DMA_send_over_flag=0;//�����־λ
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
{//model=1ʱ����1�飨LFM����model=2ʱ����2�飨CW��
	/*����ͨ���ź�*/
	DMA_send_over_flag=0;//�����־λ
	AD_temp = (volatile short *)Src;//ȡַ
	/*����DMA0Դ��ַ*/
	addr_temp = (Uint16)(((Uint32)(AD_temp)) << 1) 	& 0xFFFFu;
	DMA_RSETH(hDmaSend3,DMACSSAL,addr_temp);
	addr_temp = (Uint16)(((Uint32)(AD_temp)) >> 15) & 0xFFFFu;
	DMA_RSETH(hDmaSend3,DMACSSAU,addr_temp);
	/*���÷��ͳ���*/
	DMA_RSETH(hDmaSend3,DMACEN,length);
	/*��ʼ��������*/
	DMA_FSETH(hDmaSend3,DMACCR,ENDPROG,1);
	DMA_start(hDmaSend3);
	TIMER_start(mhTimer1);
	while(1)//��ѯDMA�Ƿ������
	{
		if(DMA_send_over_flag == 1)
		{
			break;
		}
	}
	DMA_send_over_flag=0;//�����־λ
	if(model == 2)//�ٷ�һ��
	{
		/*��ʼ��������*/
		DMA_FSETH(hDmaSend3,DMACCR,ENDPROG,1);
		while(1)//��ѯDMA�Ƿ������
		{
			if(DMA_send_over_flag == 1)
			{
				break;
			}
		}
		DMA_send_over_flag=0;//�����־λ
	}
	TIMER_stop(mhTimer1);
	DMA_stop(hDmaSend3);
	GPIO_RSET(IODATA,0x00);//GPIO���Ϊ��

}


/*����ʱ�Ӽ�ʱ��*/
void timer_low_clk(volatile Uint32 time,Uint16 clk)//timeΪԤ�ڼ�ʱʱ�䵥λus; clkΪϵͳʱ��Ƶ�ʵ�λMHz
{
	//CHIP_FSET(ST1_55,XF,1);//XF�����ø�
	//CHIP_FSET(ST1_55,XF,0);//XF�����õ�
	volatile Uint16 a=0,b=0,temp_PRD=0,temp_PRSC=0;
	time = time*clk;//ת����ʱ������
	a = time>>15;//ȡ�̣���λ��������Ч�ڳ���32768��
	b = time-(((unsigned long)a)<<15);//ȡ��
	a = a-1;//�̵�ֵ��ȥ1���ӵ�������������Ա�֤������Զ���ڵ���32768����ֹ������С����������ʱʱ��̫��
	b = b+32768-1;//����ֵԤ�ȼ�1��׼����ֵ���Ĵ���
	temp_PRD = TIMER_RGET(PRD1);//����֮ǰ�ļĴ���
	temp_PRSC = TIMER_RGET(PRSC1);//����֮ǰ�ļĴ���
	TIMER_RSET(PRD1,0x7FFFu);//0x7FFFu = ʮ����32767
	TIMER_RSET(PRSC1,0x0000u);//��λ����
	TIMER_FSET(TCR1,TLB,1);//TLB��1��ʹ��PRD��TDDR�е�ֵװ��TIM��PSC��ȷ����ʱ����ֹͣ������λ����
	timer1_cnt = 0;
	TIMER_start(mhTimer1);
	while(timer1_cnt < a)
	{
		;
	}
	TIMER_stop(mhTimer1);
	TIMER_RSET(PRD1,b);
	TIMER_FSET(TCR1,TLB,1);//TLB��1��ʹ��PRD��TDDR�е�ֵװ��TIM��PSC��ȷ����ʱ����ֹͣ������λ����
	TIMER_start(mhTimer1);
	while(timer1_cnt < a+1)
	{
		;
	}
	TIMER_stop(mhTimer1);
	TIMER_RSET(PRD1,temp_PRD);
	TIMER_RSET(PRSC1,temp_PRSC);//�ָ����򣬻ָ�PRD,PRSC�Ĵ���
	TIMER_FSET(TCR1,TLB,1);//TLB��1��ʹ��PRD��TDDR�е�ֵװ��TIM��PSC��ȷ����ʱ����ֹͣ������λ����
	timer1_cnt = 0;
}




#endif /* SEND_TX_H_ */
