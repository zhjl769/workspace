
/*************************************************
  Copyright (C)
  File name:   USBL.pjt   // 文件名
  Author:  Wang Yongheng     Version:   1.0     Date: 2016.8.16
  Description:   检测信号为10ms
  				 AD驱动采用AD7663，挂载在McBSP0上，采样率40kHz
				 McBSP1工作于SPI模式与430通信，程序未添加
				 McBSP2工作于MMC/SD模式控制SD卡
				 752通道1控制RS232，752通道2控制RS485，分别挂在CE0和CE1空间
				 SDRAM挂在CE2空间
				 定时器0定时周期为100ms，采用外部时钟源10MHz输入
				 定时器1定时周期为1us，采用CPU时钟
				 DMA2用于McBSP数据传输，DMA0用于IRF信号发射
                 完成变量内存分配，硬件驱动配置
				 添加温度传感器，噪声采集，电池电压获取函数，SD卡写程序
				 添加信标程序，可以实现应答模式和信标模式的转换，默认工作在应答模式。
				 由应答模式转换为信标模式后，发送同步命令可以实现同步功能。
				 添加压力传感器驱动程序。RS485通信方式，100ms内没有回复默认压力值为0
				 检测参数还有待修改，信号源驱动定位信号检测正常
                 2FSK检测程序未测试，添加释放指令，获取电量指令程序
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

	Time_Delay = 150000;    //定时精确到us
	NOISE_TH_NEW = 100;
	NOISE_TH_HD = 0;
	responder_Delay = 50000;//信标模式下延迟50ms
	responder_period = 20;  //信标模式下默认同步周期为2s，100ms*20
	SD_status = 0;          //SD卡状态标志位 
	release_flag = 0;       //释放成功状态位
	count_timer = 0;        //同步命令计数器
	timer0_cnt = 0;         //定时器0计数器
	code = 0x0000;
	DMA_count = 0; 
	NOISE_TH_huadong = 100; //噪声滑动门限

	//模式选择
	transponder_mode = 1;   //默认为声触发模式
	main_node = 0;
	main_station = 2; 	
	CIF_enable = 1; 
	IIF_enable = 1; 
	XDCZ = 0; 
	FSK_flag=0; 


	MMC_Init();//SD卡初始化
	if(SD_status == 1)
	{
		memset(SD_write,0,sizeof(short)*256);
		//		SD_ReadData(7672,SD_write);//读取每次保留数据扇区，存储配置信息
		NOISE_TH_NEW = SD_write[4]+256*SD_write[5];//重新计算绝对门限16bit
		Time_Delay = (Uint32)(SD_write[8]+256*SD_write[9])*1000;
		Time_Delay = 150000;
	}
	SectorCounter = 8400;//数据扇区
	IER0 = 0x0000;
	IFR0 = 0x0ffff;
	IER1 = 0x0000;
	IFR1 = 0x0ffff;

	asm(" bclr intm ");//开启总中断
	FLAG_yl_init = 0;
	pressure_init(); //压力传感器初始化
	get_parameter();

	SD_write[2] = 0x02;    //命令位
	for(i = 0; i < 28; i++)//发送28字节信息
	{ 
		UART_THR232 = SD_write[i];
		do
		{
			buff = UART_LSR232;
			f_send_uart = buff & (0x0060);
		}while(f_send_uart == 0);
	}
	memset(PC_DATA,0,sizeof(short)*12);
	IER1 |= 0x0001;  //使能串口int1中断
	IFR1 = 0x0ffff;
	Senddata0();     //发射信号1

	asm(" nop ");
	while(1)
	{
		if(transponder_mode == 1)  
		{	
			IER0 = 0x0000;   //禁止IE0中的中断
			IFR0 = 0x0ffff; 
			IER1 = 0x0011;   //使能串口int1中断
			IFR1 = 0x0ffff;

			if(main_node == 1)////相对测阵模式
			{
				DMA_start(myhDma);  //打开DMA接收数据
				detect_signal();    //检测信号
				DMA_stop(myhDma);   //关闭DMA接收数据

				take_action_FSK(); 

				if(FLAG_RS232_REVOK == 1)//检测到串口信号
				{
					URAT_reply_func();
				}
				Flag_CIF = 0;
				system_config();     //系统重新配置

				if(transponder_mode != 1)//接收到串口改变工作模式命令，跳出循环
				{	
					FLAG_RS232_REVOK = 0;//串口接收标志清零
					break;
				}
			}//相对测阵模式


			//主站；//以固定周期发射CIF信号;只检测FSK；
			if (main_station == 1)       
			{ 
				Flag_timer = 0; //Timer0完成标志
				ProduceCIF();   //产生发射信号CIF
				while(1)
				{
					Tranmit_CIF();
					DMA_start(myhDma);
					detect_signal();    //只检测FSK
					DMA_stop(myhDma);
					if( FSK_flag==1 )
					{
						FSK_flag=0;
						TIMER_stop(mhTimer0);
						system_config();    //系统重新配置
						break;
					}
					// 接收串口配置
					if(FLAG_RS232_REVOK == 1)//检测到串口信号
					{
						URAT_reply_func();
						break;
					}
				}//while_end
			}//主站模式


			//辅站；处于监听状态//检测FSK//检测CIF//检测IIF
			else if (main_station == 2)  
			{
				FSK_flag=0;
				Flag_timer = 0;
				Producelfm();  //产生发射信号IIF

				while(1)
				{
					DMA_start(myhDma);
					detect_signal();
					DMA_stop(myhDma);
					if(Flag_CIF == 1)     //检测到CIF信号
					{
						take_action_CIF_IIF();
					}
					if(Flag_IIF == 1)     //检测到IIF信号
					{
						take_action_CIF_IIF();
					}
					if(FSK_flag == 1)     //检测到指令信号
					{
						FSK_flag=0;
						break;
					}
					if(FLAG_RS232_REVOK == 1)//检测到串口信号
					{
						URAT_reply_func();
						take_action_FSK();
					}
					Flag_CIF = 0;
					system_config();  //系统重新配置*/
					/*
						if(transponder_mode != 1)//接收到串口改变工作模式命令，跳出循环
						{	
							FLAG_RS232_REVOK = 0;//串口接收标志清零
							break;
						}
					 */
				}//while_end
			} //辅站模式
		}//水下监听模式


		else if(transponder_mode == 3)//电触发模式
		{
			IER0 |= 0x0004;//使能外部中断0；int0
			IFR0 = 0xffff;//清除标志位IFR0
			IER1 |= 0x0001;//使能外部中断1；RS232串口中断
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
		} //电触发模式

	}   //while(1)主循环
}    //main   












