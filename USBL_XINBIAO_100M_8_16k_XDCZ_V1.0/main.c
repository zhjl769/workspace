
/*************************************************
  Copyright (C)
  File name:   USBL.pjt   // �ļ���
  Author:  Wang Yongheng     Version:   1.0     Date: 2016.8.16
  Description:   ����ź�Ϊ10ms
  				 AD��������AD7663��������McBSP0�ϣ�������40kHz
				 McBSP1������SPIģʽ��430ͨ�ţ�����δ���
				 McBSP2������MMC/SDģʽ����SD��
				 752ͨ��1����RS232��752ͨ��2����RS485���ֱ����CE0��CE1�ռ�
				 SDRAM����CE2�ռ�
				 ��ʱ��0��ʱ����Ϊ100ms�������ⲿʱ��Դ10MHz����
				 ��ʱ��1��ʱ����Ϊ1us������CPUʱ��
				 DMA2����McBSP���ݴ��䣬DMA0����IRF�źŷ���
                 ��ɱ����ڴ���䣬Ӳ����������
				 ����¶ȴ������������ɼ�����ص�ѹ��ȡ������SD��д����
				 ����ű���򣬿���ʵ��Ӧ��ģʽ���ű�ģʽ��ת����Ĭ�Ϲ�����Ӧ��ģʽ��
				 ��Ӧ��ģʽת��Ϊ�ű�ģʽ�󣬷���ͬ���������ʵ��ͬ�����ܡ�
				 ���ѹ����������������RS485ͨ�ŷ�ʽ��100ms��û�лظ�Ĭ��ѹ��ֵΪ0
				 ���������д��޸ģ��ź�Դ������λ�źż������
                 2FSK������δ���ԣ�����ͷ�ָ���ȡ����ָ�����
 *************************************************/


#include <math.h>
#include <string.h>
#include <Dsplib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>
#include <csl_timer.h>
#include <csl_mmc.h>
#include <csl_i2c.h>
#include <csl_pll.h>
#include <csl_gpio.h>
#include <csl_irq.h>
#include <csl_emif.h>

#include "ID_choose.h"
#include "register.h"
#include "variable.h"
#include "function.h"
#include "mcbsp_initial.h"
#include "wendu.h"
#include "subfunction.h"
#include "interrupt_function.h"

#include "Send_tx.h"
volatile int asdf;
volatile int asdfgh=0;
extern Uint32 VECSTART;
void main()
{
	PLL_config(&PLLconfig);	//DSPʱ��
	asm(" bset intm ");		//��ֹȫ���ж�
	IODOR = 0x00bf;	  		//GPIO��ʼ������GPIO6��Ϊ���룬������Ϊ���
	IODATA = 0x0000;
	CSL_init();	      		//CSL��ʼ��
	DMA_init();       //DMA��ʼ��
	EMIF_init();			//SDRAM��ʼ��
	System_initial();//receive_752_init();//���ڽ������ݳ�ʼ��
	McBSP_init();     //McBSP��ʼ������Ҫ���A/D�������ƣ�����������Ϊ40kHz
	InitADC();        //�ڲ�AD��ʼ�����ɼ���ص�ѹ


	asm(" bclr intm ");//�������ж�
	Pwm_result = (volatile short *)0x0500000;//�ֶ�ָ����ַ

	//---------------------------------------------------

	//---------------------------------------------------
	costas_flag=0;
	lfm_flag=0;
	RAMSES_ref_CIF=CIF;
	Produceref(X_ref,  RAMSES_ref_CIF);   //�����ο��ź�CIF
	RAMSES_ref_IIF=IIF;
	Produceref(X_ref2, RAMSES_ref_IIF);   //�����ο��ź�IIF
	Produce_send_signal(RAMSES_ref_CIF);  //���������ź�IRF

	//	Producefilter();  //�����˲���ϵ��
	ID1 = FSK_f1; ID2 = FSK_f2;

	Producefilter_Band(ID1 ,ID2);
	variable_init();  //������ʼ��

	//ģʽѡ��
	transponder_mode=1;//Ĭ��Ϊ������ģʽ
	main_node = 1;     //1����Բ���2����վģʽ��3����վģʽ
	CIF_enable= 1;
	IIF_enable= 1;

	XDCZ = 1;
	XDCZ_num = 2;     //��Բ������
	XDCZ_CIF_count=0;  //��Բ����������
	XDCZ_max_DMA_N=97; //CIF���źż��ʱ��,ÿ��41.2ms��Լ4s
	DMA_count = 0;     //CIF���źż��ʱ�����
	XDCZ_work_mode=1;  //0:�紥����1����Ӧ��

	Time_Delay=ZFSY;    //��ʱ��ȷ��us
	responder_Delay=50000;//�紥��ģʽת����ʱms
	responder_period=3000000;  //��վģʽ��CIF��������us
	code=0x0000;

	NOISE_TH_NEW=100;  //������������
	NOISE_TH_huadong=100;
	NOISE_TH_NEW2=100;
	NOISE_TH_huadong2=100;
	NOISE_TH_NEW3=100;
	NOISE_TH_huadong3=100;
	NOISE_TH_NEW4=100;
	NOISE_TH_huadong4=100;

	SD_status = 0;     //SD��״̬��־λ
	MMC_Init();        //SD����ʼ��
	if(SD_status == 1)
	{
		memset(SD_write,0,sizeof(short)*256);
		//SD_ReadData(7672,SD_write);//��ȡÿ�α��������������洢������Ϣ
		NOISE_TH_NEW = SD_write[4]+256*SD_write[5];//���¼����������16bit
		//Time_Delay = (Uint32)(SD_write[8]+256*SD_write[9])*1000;
		//Time_Delay = 150000;
	}
	SectorCounter = 9400;//��������
	IER0 = 0x0000;
	IFR0 = 0x0ffff;
	IER1 = 0x0000;
	IFR1 = 0x0ffff;

	asm(" bclr intm ");//�������ж�
	FLAG_yl_init = 0;
	pressure_init();   //ѹ����������ʼ��
	URAT_reply_func();
	memset(PC_DATA,0,sizeof(short)*12);

	Senddata0();
	//Create_signal(Input_bits, 1350, Pwm_result, &len_Pwm_Mod);//ʹ��200MHz��Ƶ��ʱ��ԼΪ49s

	while(1)
	{
		if(transponder_mode == 1)
		{	
			IER0 = 0x0000;   //��ֹIE0�е��ж�
			IFR0 = 0x0ffff;
			IER1 = 0x0011;   //ʹ�ܴ���int1�ж�
			IFR1 = 0x0ffff;

			if(main_node==1) //��Բ���ģʽ
			{
				memset(delay_sig1,0,sizeof(short)*100);   //�ű�1ʱ�Ӵ洢�ռ临λ
				memset(delay_sig2,0,sizeof(short)*100);   //�ű�2ʱ�Ӵ洢�ռ临λ
				memset(delay_sig3,0,sizeof(short)*100);   //�ű�3ʱ�Ӵ洢�ռ临λ
				memset(delay_sig4,0,sizeof(short)*100);   //�ű�4ʱ�Ӵ洢�ռ临λ

				Produceref(X_ref,JCIIF1); //�����ο��ź�1
				Produceref(X_ref2,JCIIF2); //�����ο��ź�2
				Produceref(X_ref3,JCIIF3); //�����ο��ź�3
				Produceref(X_ref4,JCIIF4); //�����ο��ź�4

				Produce_send_signal(RAMSES_ref_CIF);  //���������ź�CIF

				while(XDCZ_CIF_count<XDCZ_num)//XDCZ_CIF_count<XDCZ_num
				{
					if(XDCZ_work_mode==0)
					{
						IER0|= 0x0004; //ʹ���ⲿ�ж�0(int0)
						IFR0 = 0xffff; //�����־λIFR0
						Flag_CIF = 0;
						sig1_flag=1;sig2_flag=1;sig3_flag=1;sig4_flag=1;
					}
					else if (XDCZ_work_mode==1)
					{
						Flag_CIF = 0;
						sig1_flag=1;sig2_flag=1;sig3_flag=1;sig4_flag=1;
						XDCZ_Tranmit_CIF();
					}

					detect_signal_XDCZ();    //����ź�
					DMA_stop(myhDma);
					XDCZ_CIF_count++;        //��Բ�������Լ�
					Flag_CIF=0;

					if(FSK_flag == 1)        //��⵽FSKָ��
					{
						FSK_flag = 0;
						break;
					}

					if(FLAG_RS232_REVOK == 1)//��⵽�����ź�
					{
						URAT_reply_func();
						break;
					}
				}//while_end

				Transmit_IRF_delay();//���ز�ʱ�ӽ���ϴ�

			}//��Բ���ģʽ


			//��վ��//�Թ̶����ڷ���CIF�ź�;ֻ���FSK��
			if (main_node == 2)
			{
				CIF_enable= 0;//ֻ���FSK
				IIF_enable= 0;//ֻ���FSK
				zhuzhan_detect_count = (responder_period-10000) / 41200;
				//����һ����վ����������Ҫ�����ٴ��ź�
				//����Senddata0()������Ҫ�ȴ�DMA������ϲ��ܽ�����������վ���Ӧ�ü�ȥ�����źų���10ms
				zhuzhan_detect_count_remain = (responder_period-10000) % 41200;
				//DMANum=0;
				Flag_timer = 0; //Timer0��ɱ�־
				Produce_send_signal(RAMSES_ref_CIF);  //CIF
				//TIMER_start(mhTimer0);
				while(1)
				{
					Tranmit_CIF();
					DMA_count_zhuzhan = 0;//��վDMA��������
					DMA_start(myhDma);
					detect_signal();       //ֻ���FSK
					DMA_stop(myhDma);
					//DMANum = BUF_LEN_MAX - ((DMANum & 0x0fff) >> 1);
					//DMANum = DMANum /40;
					///////////////////////////////////////////////
					IFR1 = 0x0ffff;
					IER1 |= 0x0040; //������ʱ��1�ж�
					timer_high_clk(zhuzhan_detect_count_remain,100);//��ʣ��ʱ�����
					IFR1 = 0x0ffff;
					IER1 &= 0x0ffbf;//�رն�ʱ��1�ж�
					///////////////////////////////////////////////
					if( FSK_flag==1 )
					{
						FSK_flag=0;
						//TIMER_stop(mhTimer0);
						URAT_reply_func();
						break;
					}

					if(FLAG_RS232_REVOK==1)//��⵽�����ź�
					{
						URAT_reply_func();
						take_action_FSK();
						Producefilter_Band(ID1 ,ID2);
						break;
					}
				}//while_end
				system_config();           //ϵͳ��������
			}//��վ


			//��վ�����ڼ���״̬//���FSK//���CIF//���IIF
			else if (main_node == 3)
			{
				if(costas_flag==1)
				{
					costas_flag=0;
					Produce_send_signal(24);//costas1
				}
				else if(lfm_flag==1)
				{
					lfm_flag=0;
					Producelfm();
				}
				else
				{
					Produce_send_signal(RAMSES_ref_IIF);
				}//����IIF
				Produceref(X_ref,  RAMSES_ref_CIF);   //�����ο��ź�CIF
				Produceref(X_ref2, RAMSES_ref_IIF);   //�����ο��ź�IIF

				while(1)
				{
					DMA_start(myhDma);
					detect_signal();
					DMA_stop(myhDma);

					if((Flag_CIF == 1) || (Flag_IIF == 1)) //��⵽CIF�ź�
					{
						take_action_CIF_IIF();
						URAT_reply_func();
						system_config();
					}

					if(FSK_flag == 1)      //��⵽ָ���ź�
					{
						FSK_flag = 0;
						URAT_reply_func();
						break;
					}
					if(FLAG_RS232_REVOK==1)//��⵽�����ź�
					{
						URAT_reply_func();
						take_action_FSK();
						Producefilter_Band(ID1 ,ID2);
						break;
					}
				}//while_end

				system_config();   //ϵͳ��������

			} //��վģʽ
		}//ˮ�¼���ģʽ


		else if(transponder_mode == 3)//�紥��ģʽ
		{
			Produce_send_signal(RAMSES_ref_CIF);  //���������ź�CIF
			IER0 |= 0x0004;//ʹ���ⲿ�ж�0��int0
			IFR0 = 0xffff; //�����־λIFR0
			IER1 |= 0x0001;//ʹ���ⲿ�ж�1��RS232�����ж�
			IFR1 = 0xffff;
			responder_flag = 0;
			while(1)
			{
				if(responder_flag == 1)
				{
					Respondermod();
				}
				DMA_start(myhDma);
				detect_signal();  //ֻ���FSK
				DMA_stop(myhDma);

				//DMANum����ȥ����4λ�������õ������ⲿ��������DMAĿ�ĵ�ַ����Ϊ��������Ϊ16λ������2�õ���ǰ����������
				DMANum = BUF_LEN_MAX - ((DMANum & 0x0fff) >> 1);
				DMANum = DMANum * 25 + 12;

				if(FSK_flag == 1)        //���յ�FSKָ��
				{
					FSK_flag = 0;
					URAT_reply_func();
					break;
				}
				if(FLAG_RS232_REVOK == 1)//���յ������ж�
				{
					URAT_reply_func();
					take_action_FSK();
					Producefilter_Band(ID1 ,ID2);
					break;
				}
			} //while_end

			system_config();   //ϵͳ��������

		} //�紥��ģʽ

	}   //while(1)��ѭ��
}    //main












