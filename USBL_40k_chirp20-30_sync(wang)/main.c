 
/*************************************************
  Copyright (C)
  File name:   USBL.pjt   // 文件名
  Author:  Wang Yongheng     Version:   1.4     Date: 2015.06.16
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
	IODOR = 0x00bf;	  //GPIO初始化，将GPIO6作为输入，其他作为输出
	IODATA = 0x0000;
	asm(" bset intm ");	//禁止全局中断
	CSL_init();	      //CSL初始化
	System_initial(); //系统硬件初始化，包括系统主频100MHz，SDRAM初始化，TL16C752初始化
	McBSP_init();     //McBSP初始化，主要完成A/D驱动控制，采样率设置为40kHz
	InitADC();        //内部AD初始化，采集电池电压
	DMA_init();       //DMA初始化  

	Produceref();     //产生参考信号
	Producefilter();  //产生滤波器系数
	Producelfm();     //产生发射信号
	variable_init();  //变量初始化

	Time_Delay = 150000;//75ms对应75000us，定时精确到us
	NOISE_TH_NEW = 100;
	NOISE_TH_HD = 0;
    responder_Delay = 30000;//信标模式下延迟30ms，减去光耦延迟时间，光耦延迟时间为3.933ms,26067
	responder_period = 20; //信标模式下默认同步周期为2s，100ms*20
    transponder_mode = 1;//默认为声触发模式
	tongbu_flag = 0;//检测到同步命令标志位
	SD_status = 0;//SD卡状态标志位 
	release_flag = 0;//释放成功状态位
    count_timer = 0;//同步命令计数器
	timer0_cnt = 0;//定时器0计数器
	code = 0x0000;
	DMA_count = 0; 
	NOISE_TH_huadong = 100;//噪声滑动门限。

	MMC_Init();//SD卡初始化

	if(SD_status == 1)
	{
		memset(SD_write,0,sizeof(short)*256);
//		SD_ReadData(7672,SD_write);//读取每次保留数据扇区，存储配置信息
		NOISE_TH_NEW = SD_write[4]+256*SD_write[5];//重新计算绝对门限16bit
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
	SectorCounter = 8400;//数据扇区
	IER0 = 0x0000;
	IFR0 = 0x0ffff;
	IER1 = 0x0000;
	IFR1 = 0x0ffff;
	
	asm(" bclr intm ");//开启总中断
	FLAG_yl_init = 0;
	pressure_init(); //压力传感器初始化
	get_pressure();  //压力获取函数，获取值为0则压力传感器连接有问题
	get_voltage();   //获取电池电压
	timer1_cnt = 0;
 	IFR1 = 0x0ffff;
 	IER1 |= 0x0040;       //使能定时器1中断
	TIMER_start(mhTimer1);//打开定时器1定时
 //   get_temperature();    //读取温度
	IODATA = 0x0000;
	TIMER_stop(mhTimer1);//关闭定时器1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;   //关闭定时器1中断
	SPCR2_0 = 0x00c1;  //打开McBSP
	SPCR1_0 = 0x0001;	 
    get_noise();     //获取当前噪声
 	SPCR2_0 = 0x0000;//关闭McBSP
	SPCR1_0 = 0x0000;
    //串口	
    uart_config();	
	SD_write[2] = 0x02;//命令位
    for(i = 0; i < 28; i++)//发送28字节信息
	{ 
		UART_THR232 = SD_write[i];
    	do
		{
			buff = UART_LSR232;
			f_send_uart = buff & (0x0060);
		}while(f_send_uart == 0);//查询发送位是否清零
	}
	memset(PC_DATA,0,sizeof(short)*12);
	IER1 |= 0x0001;//使能串口int1中断
	IFR1 = 0x0ffff;
	Senddata0();//发射信号1
	asm(" nop ");
	while(1)
	{
		if(transponder_mode == 1)//应答模式
		{	
			IER0 = 0x0000;//使能DMA4，串口int1中断
			IFR0 = 0x0ffff; 
			IER1 = 0x0011;//使能串口int1中断
			IFR1 = 0x0ffff;
			SPCR2_0 = 0x00c1;//打开McBSP
			SPCR1_0 = 0x0001;
			while(1)
			{
				if(transponder_mode != 1)//接收到串口改变工作模式命令，跳出循环
				{	
					FLAG_RS232_REVOK = 0;//串口接收标志清零
					break;
				}
//				SPCR2_0 = 0x00c1;//打开McBSP
//				SPCR1_0 = 0x0001;	     		
				DMA_start(myhDma);//打开DMA接收数据
				detect_signal();//检测信号
				DMA_stop(myhDma);//关闭DMA接收数据
//				SPCR2_0 = 0x0000;
//				SPCR1_0 = 0x0000;
				if(Flag_answer == 1)//检测到定位信号
				{
					take_action1();//执行动作1
				}
				else if(Flag_answer == 2)//检测到释放指令
				{
				    take_action2();//执行动作2
                    asm(" nop ");
				}
				else if(Flag_answer == 4)//检测到获取电量指令
				{
					take_action3();//执行动作3
				}//end (Flag_answer == 1)  
				if(FLAG_RS232_REVOK == 1)//检测到串口信号
				{
					FLAG_RS232_REVOK = 0;//串口接收标志清零
					get_parameter();
				    uart_config();	
					if(SD_status == 1)
					{
						SD_WriteData(7672,SD_write);//7672扇区为每次保留数据扇区
					}
				    for(i = 0; i < 28; i++)//发送42字节信息
					{ 
						UART_THR232 = SD_write[i];
				    	do
						{
							buff = UART_LSR232;
							f_send_uart = buff & (0x0060);
						}while(f_send_uart == 0);//查询发送位是否清零
					}
				}
				Flag_answer = 0;
				system_config();//系统重新配置
			}
		}
		else if(transponder_mode== 2)//信标模式
		{
			IER0 = 0x0010;//使能定时器0，串口1中断
			IFR0 = 0x0ffff;
			IER1 = 0x0001;
			IFR1 = 0x0ffff;
			TIMER_start(mhTimer0);
			Flag_timer = 0;
			FLAG_OK = 0;
			while(1)
			{
				if(transponder_mode != 2)
				{
					FLAG_RS232_REVOK = 0;//串口接收标志清零
					get_parameter();
					uart_config();	

				    for(i = 0; i < 28; i++)//发送42字节信息
					{ 
						UART_THR232 = SD_write[i];
				    	do
						{
							buff = UART_LSR232;
							f_send_uart = buff & (0x0060);
						}while(f_send_uart == 0);//查询发送位是否清零
					}
					break;
				}
				if(tongbu_flag == 1) //接收到串口命令 
				{
					if((PC_DATA[6]>0) && (PC_DATA[6]<31))
					{
						responder_period = PC_DATA[6]*10;
					}
					FLAG_RS232_REVOK = 0;

					asm(" bset intm ");//禁止中断
					IER0 |= 0x0004;
	  				IFR0 = 0x0ffff;//使能int0中断
					asm(" bclr intm ");
					Flag_timer = 0;
					timer0_cnt = 0;
					do 
					{
					;
					}while(Flag_timer == 0);
					Flag_timer = 0;
				
					IER0 &= 0x0fffb;
	  				IFR0 = 0x0ffff;//使能int0中断
					get_voltage();//获取电池电压
					timer1_cnt = 0;
				 	IFR1 = 0x0ffff;
				 	IER1 |= 0x0040;//使能定时器1中断
					TIMER_start(mhTimer1);//打开定时器1定时
	///			    get_temperature();//读取温度
					IODATA = 0x0000;
					TIMER_stop(mhTimer1);//关闭定时器1
					IFR1 = 0x0ffff;
					IER1 &= 0x0ffbf;//关闭定时器1中断
				    get_pressure();//获取压力信息
				    get_noise();//获取当前噪声
				    uart_config();//串口
				    for(i = 0; i < 28; i++)//发送42字节信息
					{ 
						UART_THR232 = SD_write[i];
				    	do
						{
							buff = UART_LSR232;
							f_send_uart = buff & (0x0060);
						}while(f_send_uart == 0);//查询发送位是否清零
					} 
					tongbu_flag = 0;
				}//end if(FLAG_RS232_REVOK == 1)
				Syntimermod();
			}
		}//if(transponder_mode == 1)//应答模式

		else if(transponder_mode == 3)//电触发模式
		{
			IER0 = 0x0004;//使能定时器0，串口1中断
			IFR0 = 0x0ffff;
			IER1 = 0x0001;
			IFR1 = 0x0ffff;
			responder_flag = 0;
			while(1)
			{
				if(transponder_mode != 3)
				{
					FLAG_RS232_REVOK = 0;//串口接收标志清零
					get_parameter();
					uart_config();	
				    for(i = 0; i < 28; i++)//发送42字节信息
					{ 
						UART_THR232 = SD_write[i];
				    	do
						{
							buff = UART_LSR232;
							f_send_uart = buff & (0x0060);
						}while(f_send_uart == 0);//查询发送位是否清零
					}
					break;
				}
				Respondermod();
			}
		}//if(transponder_mode == 1)//应答模式
	}//end while(1)
}





