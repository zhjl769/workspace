 
/*************************************************
  Copyright (C)
  File name:   USBL.pjt   // �ļ���
  Author:  Wang Yongheng     Version:   1.4     Date: 2014.6.16
*************************************************/

#include <math.h>
#include <stdio.h>
#include <string.h>

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
	IODOR = 0x006f;  //GPIO��ʼ������GPIO7��4��Ϊ���룬������Ϊ���
	IODATA = 0x0000;
//	IODOR = 0x00bf;	  //GPIO��ʼ������GPIO6��Ϊ���룬������Ϊ���
//	IODATA = 0x0000;
	asm(" bset intm ");	//��ֹȫ���ж�
	CSL_init();	      //CSL��ʼ��
	System_initial(); //ϵͳӲ����ʼ��������ϵͳ��Ƶ100MHz��SDRAM��ʼ����TL16C752��ʼ��
	McBSP_init();     //McBSP��ʼ������Ҫ���A/D�������ƣ�����������Ϊ40kHz
	InitADC();        //�ڲ�AD��ʼ�����ɼ���ص�ѹ
	DMA_init();       //DMA��ʼ��  

	Produceref();     //�����ο��ź�
	Producefilter();  //�����˲���ϵ��
	Producelfm1();     //���������ź�1
	Producelfm2();     //���������ź�2
	variable_init();  //������ʼ��

	Time_Delay = 150000;//75ms��Ӧ75000us����ʱ��ȷ��us
	NOISE_TH_NEW = 100;
	NOISE_TH_HD = 0;
    responder_Delay = 30000;//�ű�ģʽ���ӳ�30ms����ȥ�����ӳ�ʱ�䣬�����ӳ�ʱ��Ϊ3.933ms,26067
	responder_period = 20; //�ű�ģʽ��Ĭ��ͬ������Ϊ2s��100ms*20
    transponder_mode = 1;//Ĭ��Ϊ������ģʽ
	tongbu_flag = 0;//��⵽ͬ�������־λ
	SD_status = 0;//SD��״̬��־λ 
	release_flag = 0;//�ͷųɹ�״̬λ
    count_timer = 0;//ͬ�����������
	timer0_cnt = 0;//��ʱ��0������
	code = 0x0000;
	DMA_count = 0; 
	NOISE_TH_huadong = 100;//�����������ޡ�

	MMC_Init();//SD����ʼ��

	if(SD_status == 1)
	{
		memset(SD_write,0,sizeof(short)*256);
//		SD_ReadData(7672,SD_write);//��ȡÿ�α��������������洢������Ϣ
		NOISE_TH_NEW = SD_write[4]+256*SD_write[5];//���¼����������16bit
		if(NOISE_TH_NEW < 1000)
		{
			NOISE_TH_NEW = 100;
		}
		if(NOISE_TH_NEW > 20000)
		{
			NOISE_TH_NEW = 100;
		}
		Time_Delay = (Uint32)(SD_write[8]+256*SD_write[9])*1000;
		if(Time_Delay > 500000)
		{
			Time_Delay = 150000;
		}
		if(Time_Delay < 100000)
		{
			Time_Delay = 150000;
		}
	}
	SectorCounter = 8400;//��������
	IER0 = 0x0000;
	IFR0 = 0x0ffff;
	IER1 = 0x0000;
	IFR1 = 0x0ffff;
	
	asm(" bclr intm ");//�������ж�
	FLAG_yl_init = 0;
	pressure_init(); //ѹ����������ʼ��
	get_pressure();  //ѹ����ȡ��������ȡֵΪ0��ѹ������������������
	get_voltage();   //��ȡ��ص�ѹ
	timer1_cnt = 0;
 	IFR1 = 0x0ffff;
 	IER1 |= 0x0040;       //ʹ�ܶ�ʱ��1�ж�
	TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
 //   get_temperature();    //��ȡ�¶�
	IODATA = 0x0000;
	TIMER_stop(mhTimer1);//�رն�ʱ��1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;   //�رն�ʱ��1�ж�
	SPCR2_0 = 0x00c1;  //��McBSP
	SPCR1_0 = 0x0001;	 
    get_noise();     //��ȡ��ǰ����
 	SPCR2_0 = 0x0000;//�ر�McBSP
	SPCR1_0 = 0x0000;//����
    uart_config();	
	SD_write[2] = 0x02;//����λ
    for(i = 0; i < 28; i++)//����28�ֽ���Ϣ
	{ 
		UART_THR232 = SD_write[i];
    	do
		{
			buff = UART_LSR232;
			f_send_uart = buff & (0x0060);
		}while(f_send_uart == 0);//��ѯ����λ�Ƿ�����
	}
	memset(PC_DATA,0,sizeof(short)*12);
	IER1 |= 0x0001;//ʹ�ܴ���int1�ж�
	IFR1 = 0x0ffff;
	Senddata0();//�����ź�1
	asm(" nop ");
	while(1)
	{
		if(transponder_mode == 1)//Ӧ��ģʽ
		{	
			IER0 = 0x0000;//ʹ��DMA4������int1�ж�
			IFR0 = 0x0ffff; 
			IER1 = 0x0011;//ʹ�ܴ���int1�ж�
			IFR1 = 0x0ffff;
			SPCR2_0 = 0x00c1;//��McBSP
			SPCR1_0 = 0x0001;
			while(1)
			{
				if(transponder_mode != 1)//���յ����ڸı乤��ģʽ�������ѭ��
				{	
					FLAG_RS232_REVOK = 0;//���ڽ��ձ�־����
					break;
				}
//				SPCR2_0 = 0x00c1;//��McBSP
//				SPCR1_0 = 0x0001;	     		
				DMA_start(myhDma);//��DMA��������
				detect_signal();//����ź�
				DMA_stop(myhDma);//�ر�DMA��������
//				SPCR2_0 = 0x0000;
//				SPCR1_0 = 0x0000;
				if(Flag_answer == 1)//��⵽��λ�ź�
				{
					take_action1();//ִ�ж���1
				}
				else if(Flag_answer == 2)//��⵽�ͷ�ָ��
				{
				    take_action2();//ִ�ж���2
                    asm(" nop ");
				}
				else if(Flag_answer == 4)//��⵽��ȡ����ָ��
				{
					take_action3();//ִ�ж���3
				}//end (Flag_answer == 1)  
				if(FLAG_RS232_REVOK == 1)//��⵽�����ź�
				{
					FLAG_RS232_REVOK = 0;//���ڽ��ձ�־����
					get_parameter();
				    uart_config();	
					if(SD_status == 1)
					{
						SD_WriteData(7672,SD_write);//7672����Ϊÿ�α�����������
					}
				    for(i = 0; i < 28; i++)//����42�ֽ���Ϣ
					{ 
						UART_THR232 = SD_write[i];
				    	do
						{
							buff = UART_LSR232;
							f_send_uart = buff & (0x0060);
						}while(f_send_uart == 0);//��ѯ����λ�Ƿ�����
					}
				}
				Flag_answer = 0;
				system_config();//ϵͳ��������
			}
		}
		else if(transponder_mode== 2)//�ű�ģʽ
		{
			IER0 = 0x0010;//ʹ�ܶ�ʱ��0������1�ж�
			IFR0 = 0x0ffff;
			IER1 = 0x0001;
			IFR1 = 0x0ffff;
			TIMER_start(mhTimer0);
			Flag_timer = 0;
			FLAG_OK = 0;
			count_timer = 0;
			while(1)
			{
				if(transponder_mode != 2)
				{
					FLAG_RS232_REVOK = 0;//���ڽ��ձ�־����
					get_parameter();
					uart_config();	

				    for(i = 0; i < 28; i++)//����42�ֽ���Ϣ
					{ 
						UART_THR232 = SD_write[i];
				    	do
						{
							buff = UART_LSR232;
							f_send_uart = buff & (0x0060);
						}while(f_send_uart == 0);//��ѯ����λ�Ƿ�����
					}
					break;
				}
				if(tongbu_flag == 1) //���Ҫͬ����
				{
					if((PC_DATA[6]>0) && (PC_DATA[6]<31))
					{
						responder_period = PC_DATA[6]*10;
					}
					FLAG_RS232_REVOK = 0;

					asm(" bset intm ");//��ֹ�ж�
					IER0 |= 0x0004;
	  				IFR0 = 0x0ffff;//ʹ��int0�ж�
					asm(" bclr intm ");
					Flag_timer = 0;//ͬ�����ڼ����Ƿ���ɱ�־λ���㡢��û�е�ʱ��
					timer0_cnt = 0;//100ms��������Ҫ������ֵΪ0
					do 
					{
					;
		//			}while(Flag_timer == 0);
					}while(count_timer != 3);
					Flag_timer = 0;
				
					IER0 &= 0x0fffb;
	  				IFR0 = 0x0ffff;//ʹ��int0�ж�
					get_voltage();//��ȡ��ص�ѹ
					timer1_cnt = 0;
				 	IFR1 = 0x0ffff;
				 	IER1 |= 0x0040;//ʹ�ܶ�ʱ��1�ж�
					TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
	///			    get_temperature();//��ȡ�¶�
					IODATA = 0x0000;
					TIMER_stop(mhTimer1);//�رն�ʱ��1
					IFR1 = 0x0ffff;
					IER1 &= 0x0ffbf;//�رն�ʱ��1�ж�
				    get_pressure();//��ȡѹ����Ϣ
				    get_noise();//��ȡ��ǰ����
				    uart_config();//����
				    for(i = 0; i < 28; i++)//����42�ֽ���Ϣ
					{ 
						UART_THR232 = SD_write[i];
				    	do
						{
							buff = UART_LSR232;
							f_send_uart = buff & (0x0060);
						}while(f_send_uart == 0);//��ѯ����λ�Ƿ�����
					} 
					tongbu_flag = 0;
				}//end if(FLAG_RS232_REVOK == 1)
			Syntimermod();
			}
		}//if(transponder_mode == 1)//Ӧ��ģʽ

		else if(transponder_mode == 3)//�紥��ģʽ
		{
			IER0 = 0x0004;//ʹ�ܶ�ʱ��0������1�ж�
			IFR0 = 0x0ffff;
			IER1 = 0x0001;
			IFR1 = 0x0ffff;
			responder_flag = 0;
			while(1)
			{
				if(transponder_mode != 3)
				{
					FLAG_RS232_REVOK = 0;//���ڽ��ձ�־����
					get_parameter();
					uart_config();	
				    for(i = 0; i < 28; i++)//����42�ֽ���Ϣ
					{ 
						UART_THR232 = SD_write[i];
				    	do
						{
							buff = UART_LSR232;
							f_send_uart = buff & (0x0060);
						}while(f_send_uart == 0);//��ѯ����λ�Ƿ�����
					}
					break;
				}
				Respondermod();
			}
		}//if(transponder_mode == 1)//Ӧ��ģʽ
	}//end while(1)
}




