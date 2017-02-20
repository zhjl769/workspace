
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

#include <stdio.h>
#include <string.h>

#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>
#include <csl_timer.h>
#include <csl_mmc.h>

#include "register.h"
#include "variable.h"
#include "function.h"
#include "mcbsp_initial.h"
#include "wendu.c"
#include "subfunction.c"
#include "interrupt_function.c"

void main()

{

	short i;
	IODOR = 0x00bf;	  //GPIO��ʼ������GPIO6��Ϊ���룬������Ϊ���
	IODATA = 0x0000;
	asm(" bset intm ");	//��ֹȫ���ж�
	CSL_init();	      //CSL��ʼ��
	System_initial(); //ϵͳӲ����ʼ��������ϵͳ��Ƶ100MHz��SDRAM��ʼ����TL16C752��ʼ��
	McBSP_init();     //McBSP��ʼ������Ҫ���A/D�������ƣ�����������Ϊ40kHz
	InitADC();        //�ڲ�AD��ʼ�����ɼ���ص�ѹ
	DMA_init();       //DMA��ʼ��  

	Produceref();     //�����ο��ź�
	Producefilter();  //�����˲���ϵ��
	Producelfm();     //���������ź�
	variable_init();  //������ʼ��

	Time_Delay = 150000;    //��ʱ��ȷ��us
	NOISE_TH_NEW = 100;
	NOISE_TH_HD = 0;
	responder_Delay = 50000;//�ű�ģʽ���ӳ�50ms
	responder_period = 20;  //�ű�ģʽ��Ĭ��ͬ������Ϊ2s��100ms*20
	SD_status = 0;          //SD��״̬��־λ 
	release_flag = 0;       //�ͷųɹ�״̬λ
	count_timer = 0;        //ͬ�����������
	timer0_cnt = 0;         //��ʱ��0������
	code = 0x0000;
	DMA_count = 0; 
	NOISE_TH_huadong = 100; //������������

	//ģʽѡ��
	transponder_mode = 1;   //Ĭ��Ϊ������ģʽ
	main_node = 0;
	main_station = 2; 	
	CIF_enable = 1; 
	IIF_enable = 1; 
	XDCZ = 0; 
	FSK_flag=0; 


	MMC_Init();//SD����ʼ��
	if(SD_status == 1)
	{
		memset(SD_write,0,sizeof(short)*256);
		//		SD_ReadData(7672,SD_write);//��ȡÿ�α��������������洢������Ϣ
		NOISE_TH_NEW = SD_write[4]+256*SD_write[5];//���¼����������16bit
		Time_Delay = (Uint32)(SD_write[8]+256*SD_write[9])*1000;
		Time_Delay = 150000;
	}
	SectorCounter = 8400;//��������
	IER0 = 0x0000;
	IFR0 = 0x0ffff;
	IER1 = 0x0000;
	IFR1 = 0x0ffff;

	asm(" bclr intm ");//�������ж�
	FLAG_yl_init = 0;
	pressure_init(); //ѹ����������ʼ��
	get_parameter();

	SD_write[2] = 0x02;    //����λ
	for(i = 0; i < 28; i++)//����28�ֽ���Ϣ
	{ 
		UART_THR232 = SD_write[i];
		do
		{
			buff = UART_LSR232;
			f_send_uart = buff & (0x0060);
		}while(f_send_uart == 0);
	}
	memset(PC_DATA,0,sizeof(short)*12);
	IER1 |= 0x0001;  //ʹ�ܴ���int1�ж�
	IFR1 = 0x0ffff;
	Senddata0();     //�����ź�1

	asm(" nop ");
	while(1)
	{
		if(transponder_mode == 1)  
		{	
			IER0 = 0x0000;   //��ֹIE0�е��ж�
			IFR0 = 0x0ffff; 
			IER1 = 0x0011;   //ʹ�ܴ���int1�ж�
			IFR1 = 0x0ffff;

			if(main_node == 1)////��Բ���ģʽ
			{
				DMA_start(myhDma);  //��DMA��������
				detect_signal();    //����ź�
				DMA_stop(myhDma);   //�ر�DMA��������

				take_action_FSK(); 

				if(FLAG_RS232_REVOK == 1)//��⵽�����ź�
				{
					URAT_reply_func();
				}
				Flag_CIF = 0;
				system_config();     //ϵͳ��������

				if(transponder_mode != 1)//���յ����ڸı乤��ģʽ�������ѭ��
				{	
					FLAG_RS232_REVOK = 0;//���ڽ��ձ�־����
					break;
				}
			}//��Բ���ģʽ


			//��վ��//�Թ̶����ڷ���CIF�ź�;ֻ���FSK��
			if (main_station == 1)       
			{ 
				Flag_timer = 0; //Timer0��ɱ�־
				ProduceCIF();   //���������ź�CIF
				while(1)
				{
					Tranmit_CIF();
					DMA_start(myhDma);
					detect_signal();    //ֻ���FSK
					DMA_stop(myhDma);
					if( FSK_flag==1 )
					{
						FSK_flag=0;
						TIMER_stop(mhTimer0);
						system_config();    //ϵͳ��������
						break;
					}
					// ���մ�������
					if(FLAG_RS232_REVOK == 1)//��⵽�����ź�
					{
						URAT_reply_func();
						break;
					}
				}//while_end
			}//��վģʽ


			//��վ�����ڼ���״̬//���FSK//���CIF//���IIF
			else if (main_station == 2)  
			{
				FSK_flag=0;
				Flag_timer = 0;
				Producelfm();  //���������ź�IIF

				while(1)
				{
					DMA_start(myhDma);
					detect_signal();
					DMA_stop(myhDma);
					if(Flag_CIF == 1)     //��⵽CIF�ź�
					{
						take_action_CIF_IIF();
					}
					if(Flag_IIF == 1)     //��⵽IIF�ź�
					{
						take_action_CIF_IIF();
					}
					if(FSK_flag == 1)     //��⵽ָ���ź�
					{
						FSK_flag=0;
						break;
					}
					if(FLAG_RS232_REVOK == 1)//��⵽�����ź�
					{
						URAT_reply_func();
						take_action_FSK();
					}
					Flag_CIF = 0;
					system_config();  //ϵͳ��������*/
					/*
						if(transponder_mode != 1)//���յ����ڸı乤��ģʽ�������ѭ��
						{	
							FLAG_RS232_REVOK = 0;//���ڽ��ձ�־����
							break;
						}
					 */
				}//while_end
			} //��վģʽ
		}//ˮ�¼���ģʽ


		else if(transponder_mode == 3)//�紥��ģʽ
		{
			IER0 |= 0x0004;//ʹ���ⲿ�ж�0��int0
			IFR0 = 0xffff;//�����־λIFR0
			IER1 |= 0x0001;//ʹ���ⲿ�ж�1��RS232�����ж�
			IFR1 = 0xffff;
			responder_flag = 0;
			while(1)
			{
				if(transponder_mode != 3)
				{
					URAT_reply_func();
					break; 
				} 
				Respondermod(); 


			} 
		} //�紥��ģʽ

	}   //while(1)��ѭ��
}    //main   












