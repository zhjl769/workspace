
/*************************************************
  Copyright (C)
  File name:   USBL.pjt   // �ļ���
  Author:  Wang Yongheng     Version:   1.4     Date: 2014.6.16
  Description:   ����ź�Ϊ25ms��LFM�źţ�9-14kHz���µ�Ƶ
  				 AD��������AD7663��������McBSP0�ϣ�������40kHz
				 McBSP1������SPIģʽ��430ͨ�ţ�����δ����
				 McBSP2������MMC/SDģʽ����SD��
				 752ͨ��1����RS232��752ͨ��2����RS485���ֱ����CE0��CE1�ռ�
				 SDRAM����CE2�ռ�
				 ��ʱ��0��ʱ����Ϊ100ms�������ⲿʱ��Դ10MHz����
				 ��ʱ��1��ʱ����Ϊ1us������CPUʱ��
				 DMA2����McBSP���ݴ��䣬DMA0����IRF�źŷ���
                 ��ɱ����ڴ���䣬Ӳ����������
				 �����¶ȴ������������ɼ�����ص�ѹ��ȡ������SD��д����
				 �����ű���򣬿���ʵ��Ӧ��ģʽ���ű�ģʽ��ת����Ĭ�Ϲ�����Ӧ��ģʽ��
				 ��Ӧ��ģʽת��Ϊ�ű�ģʽ�󣬷���ͬ���������ʵ��ͬ�����ܡ�
				 ����ѹ����������������RS485ͨ�ŷ�ʽ��100ms��û�лظ�Ĭ��ѹ��ֵΪ0
				 ���������д��޸ģ��ź�Դ������λ�źż������
                 2FSK������δ���ԣ������ͷ�ָ���ȡ����ָ�����
				 ��2015��8��22֮ǰ���˲����Ľ�ֹƵ��Ϊ5KHz
				 ���ڵ�Ϊ7KHz
 *************************************************/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <Dsplib.h>

#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>
#include <csl_timer.h>
#include <csl_mmc.h>
#include <csl_i2c.h>
#include <csl_pll.h>
#include <csl_gpio.h>
#include <csl_irq.h>


#include "register.h"
#include "variable.h"
#include "I2C_function.h"
#include "wendu.h"
#include "mcbsp_initial.h"
#include "subfunction.h"
#include "interrupt_function.h"

void main(void)
{
	short i;
	IODOR = 0x006f;  //GPIO��ʼ������GPIO7��4��Ϊ���룬������Ϊ���
	IODATA = 0x0040;   //GPIO6���ߣ��رշ��䲿�ֵ�Դ
	IRQ_globalDisable();//��ֹȫ���ж�
	IRQ_setVecs (0x0e00);//�ж�������ӳ���ַ
	CSL_init();	      //CSL��ʼ��
	System_initial(); //ϵͳӲ����ʼ��������ϵͳ��Ƶ100MHz��SDRAM��ʼ����TL16C752��ʼ��
	McBSP_init();     //McBSP��ʼ������Ҫ���A/D�������ƣ�����������Ϊ100kHz
	InitADC();        //�ڲ�AD��ʼ�����ɼ���ص�ѹ
	DMA_init();       //DMA��ʼ��  
	/* CSL��I2C SETUP���������⣬���Ƽ�ʹ�ã�����д��Ͷ�ȡ��������Ҫ��I2CSTR�Ĵ����е�BBλ��1*/
	I2C_FSET(I2CSTR,BB,0x1);
	I2C_config(&I2Cconfig);
	I2C_RSET(I2COAR, 0x0000);  //��IICģ�������ĵ�ַ
	delay(90000);
	memset(SD_write,0,sizeof(short)*256);
	IRQ_globalEnable();//�������ж�
	I2C_Read(0xd700, SD_write, 28, 0x0050); //����д�������
	if((SD_write[0] == 0xee) && (SD_write[27] == 0xff))//У��EEPROM����
	{
		transponder_mode = SD_write[1];//����ģʽ
		tongbu_flag = SD_write[2];//����λ
		NOISE_TH_NEW = SD_write[4] + (SD_write[5] << 8);//���¼����������16bit
		Time_Delay = (Uint32)(SD_write[8] + (SD_write[9] << 8))*1000;
		voltage_AD = SD_write[12] + (SD_write[13] << 8);//��ѹ����ֵ
		RS485_DATA[5] = SD_write[15] & 0xff;
		RS485_DATA[4] = SD_write[16] & 0xff;
		RS485_DATA[3] = SD_write[17] & 0xff;
		RS485_DATA[2] = SD_write[18] & 0xff;
		temperature = (Uint16)((SD_write[19] + (SD_write[20] << 8))*0.5);
		noise_10ms = SD_write[21] + (SD_write[22] << 8);
		SD_status = SD_write[26];
	}
	else
	{
		Time_Delay = 150000;//75ms��Ӧ75000us����ʱ��ȷ��us
		responder_Delay = 30000;//�ű�ģʽ���ӳ�30ms
		responder_period = 20; //�ű�ģʽ��Ĭ��ͬ������Ϊ2s��100ms*20
		transponder_mode = 1;//Ĭ��Ϊ��Ӧ��ģʽ
		transponder_mode1 = 1;//Ĭ��Ϊ����������л�����ģʽ
		tongbu_flag = 0;//��⵽ͬ�������־λ
		SD_status = 0;//SD��״̬��־λ
		release_flag = 0;//�ͷųɹ�״̬λ
		count_timer = 0;//ͬ�����������
		timer0_cnt = 0;//��ʱ��0������
		code = 0x0000;
		DMA_count = 0;
		NOISE_TH_huadong = 100;//������������
		NOISE_TH_NEW = 100;//���º������ֵ
		NOISE_TH_HD = 0;
	}

	Produceref();     //�����ο��ź�
	Producelfm();     //���������ź�
	variable_init();  //������ʼ��

	MMC_Init();//SD����ʼ��
	SectorCounter = 8400;//��������
	pressure_init(); //ѹ����������ʼ��
	get_pressure();  //ѹ����ȡ��������ȡֵΪ0��ѹ������������������
	get_voltage();   //��ȡ��ص�ѹ
	get_temperature();    //��ȡ�¶�
	get_noise();     //��ȡ��ǰ����
	uart_config();
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
	IODATA = 0x0000;   //GPIO6���ͣ��������䲿�ֵ�Դ
	IRQ_enable(IRQ_EVT_INT1);
	Senddata0();//�����ź�1
	while(1)
	{
		if(transponder_mode == 1)//Ӧ��ģʽ
		{	
			IRQ_enable(IRQ_EVT_DMAC4);//ʹ��DMA4������int1�ж�
			SPCR2_0 = 0x00c1;//��McBSP
			SPCR1_0 = 0x0001;
			Time_Delay = 150000;//75ms��Ӧ75000us����ʱ��ȷ��us
			while(1)
			{
				if(transponder_mode != 1)//���յ����ڸı乤��ģʽ�������ѭ��
				{	
					FLAG_RS232_REVOK = 0;//���ڽ��ձ�־����
					break;
				}
				/************************************************************************************************************************/
				if((transponder_mode1 == 1) && ((IODATA & 0x0010) == 16))//��⵽�ⲿ��Դ��ͨ���ı乤��ģʽ������ѭ��
				{
					transponder_mode = 3;//�л����ⴥ��ģʽ
					get_parameter();
					uart_config();
					if(SD_status == 1)
						SD_WriteData(7672,SD_write);//7672����Ϊÿ�α�����������
					for(i = 0; i < 28; i++)//����42�ֽ���Ϣ
					{
						UART_THR232 = SD_write[i];
						do
						{
							buff = UART_LSR232;
							f_send_uart = buff & (0x0060);
						}
						while(f_send_uart == 0);//��ѯ����λ�Ƿ�����
					}
					break;
				}
				/*********************************/
				DMA_start(myhDma);//��DMA��������
				detect_signal();//����ź�
				DMA_stop(myhDma);//�ر�DMA��������
				switch (Flag_answer)//��⵽��λ�ź�
				{
				case 1: take_action1(); break;//ִ�ж���1
				case 2: take_action2(); break;//��⵽�ͷ�ָ��,ִ�ж���2
				case 4: take_action3(); break;//��⵽��ȡ����ָ��,ִ�ж���3
				}
				if(FLAG_RS232_REVOK == 1)//��⵽�����ź�
				{	
					transponder_mode1 = 0;
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

		else if(transponder_mode == 2)//�������ȼ�����ģʽ
		{
			transponder_mode1 = 1;//�л����Զ�ģʽ
			FLAG_RS232_REVOK = 0;//���ڽ��ձ�־����
			if((IODATA & 0x0010) != 0x10)
				transponder_mode = 1;
			else if((IODATA & 0x0010) == 0x10)
				transponder_mode = 3;
			get_parameter();
			uart_config();
			if(SD_status == 1)
				SD_WriteData(7672,SD_write);//7672����Ϊÿ�α�����������}
			for(i = 0; i < 28; i++)//����42�ֽ���Ϣ
			{
				UART_THR232 = SD_write[i];
				while((UART_LSR232 & 0x0060) == 0);//��ѯ����λ�Ƿ�����
			}
		}

		else if(transponder_mode == 3)//�紥��ģʽ
		{
			IODATA = 0x0000;//�������䲿�ֵĵ�Դ
			IRQ_enable(IRQ_EVT_INT0);//�����ⲿ�ж�0
			IRQ_enable(IRQ_EVT_INT1);//�����ⲿ�ж�1,�����ж�
			IRQ_clear(IRQ_IEMASK_ALL);//������еı�־λ
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
						while((UART_LSR232 & (0x0060)) == 0)
							buff = UART_LSR232;//��ѯ����λ�Ƿ�����
					}
					break;
				}

				if((transponder_mode1 ==1) && ((IODATA & 0x0010) != 16))//��⵽�ⲿ��Դ��ʧ���л�����ģʽ������ѭ��
				{
					transponder_mode = 1;//�л�����ģʽΪӦ��ģʽ
					get_parameter();
					uart_config();
					for(i = 0; i < 28; i++)//����42�ֽ���Ϣ
					{
						UART_THR232 = SD_write[i];
						while((UART_LSR232 & (0x0060)) == 0)
							buff = UART_LSR232;//��ѯ����λ�Ƿ�����
					}
					break;
				}
				Respondermod();
			}
		}//if(transponder_mode == 1)//Ӧ��ģʽ
	}//end while(1)
}




