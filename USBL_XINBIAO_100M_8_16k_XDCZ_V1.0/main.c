
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
	PLL_config(&PLLconfig);	//DSP时钟
	asm(" bset intm ");		//禁止全局中断
	IODOR = 0x00bf;	  		//GPIO初始化，将GPIO6作为输入，其他作为输出
	IODATA = 0x0000;
	CSL_init();	      		//CSL初始化
	DMA_init();       //DMA初始化
	EMIF_init();			//SDRAM初始化
	System_initial();//receive_752_init();//串口接收数据初始化
	McBSP_init();     //McBSP初始化，主要完成A/D驱动控制，采样率设置为40kHz
	InitADC();        //内部AD初始化，采集电池电压


	asm(" bclr intm ");//开启总中断
	Pwm_result = (volatile short *)0x0500000;//手动指定地址

	//---------------------------------------------------

	//---------------------------------------------------
	costas_flag=0;
	lfm_flag=0;
	RAMSES_ref_CIF=CIF;
	Produceref(X_ref,  RAMSES_ref_CIF);   //产生参考信号CIF
	RAMSES_ref_IIF=IIF;
	Produceref(X_ref2, RAMSES_ref_IIF);   //产生参考信号IIF
	Produce_send_signal(RAMSES_ref_CIF);  //产生发射信号IRF

	//	Producefilter();  //产生滤波器系数
	ID1 = FSK_f1; ID2 = FSK_f2;

	Producefilter_Band(ID1 ,ID2);
	variable_init();  //变量初始化

	//模式选择
	transponder_mode=1;//默认为声触发模式
	main_node = 1;     //1：相对测阵；2：主站模式；3：辅站模式
	CIF_enable= 1;
	IIF_enable= 1;

	XDCZ = 1;
	XDCZ_num = 2;     //相对测阵次数
	XDCZ_CIF_count=0;  //相对测阵次数计数
	XDCZ_max_DMA_N=97; //CIF后信号检测时间,每次41.2ms，约4s
	DMA_count = 0;     //CIF后信号检测时间计数
	XDCZ_work_mode=1;  //0:电触发；1：声应答

	Time_Delay=ZFSY;    //定时精确到us
	responder_Delay=50000;//电触发模式转发延时ms
	responder_period=3000000;  //主站模式下CIF发射周期us
	code=0x0000;

	NOISE_TH_NEW=100;  //噪声滑动门限
	NOISE_TH_huadong=100;
	NOISE_TH_NEW2=100;
	NOISE_TH_huadong2=100;
	NOISE_TH_NEW3=100;
	NOISE_TH_huadong3=100;
	NOISE_TH_NEW4=100;
	NOISE_TH_huadong4=100;

	SD_status = 0;     //SD卡状态标志位
	MMC_Init();        //SD卡初始化
	if(SD_status == 1)
	{
		memset(SD_write,0,sizeof(short)*256);
		//SD_ReadData(7672,SD_write);//读取每次保留数据扇区，存储配置信息
		NOISE_TH_NEW = SD_write[4]+256*SD_write[5];//重新计算绝对门限16bit
		//Time_Delay = (Uint32)(SD_write[8]+256*SD_write[9])*1000;
		//Time_Delay = 150000;
	}
	SectorCounter = 9400;//数据扇区
	IER0 = 0x0000;
	IFR0 = 0x0ffff;
	IER1 = 0x0000;
	IFR1 = 0x0ffff;

	asm(" bclr intm ");//开启总中断
	FLAG_yl_init = 0;
	pressure_init();   //压力传感器初始化
	URAT_reply_func();
	memset(PC_DATA,0,sizeof(short)*12);

	Senddata0();
	//Create_signal(Input_bits, 1350, Pwm_result, &len_Pwm_Mod);//使用200MHz主频，时间约为49s

	while(1)
	{
		if(transponder_mode == 1)
		{	
			IER0 = 0x0000;   //禁止IE0中的中断
			IFR0 = 0x0ffff;
			IER1 = 0x0011;   //使能串口int1中断
			IFR1 = 0x0ffff;

			if(main_node==1) //相对测阵模式
			{
				memset(delay_sig1,0,sizeof(short)*100);   //信标1时延存储空间复位
				memset(delay_sig2,0,sizeof(short)*100);   //信标2时延存储空间复位
				memset(delay_sig3,0,sizeof(short)*100);   //信标3时延存储空间复位
				memset(delay_sig4,0,sizeof(short)*100);   //信标4时延存储空间复位

				Produceref(X_ref,JCIIF1); //产生参考信号1
				Produceref(X_ref2,JCIIF2); //产生参考信号2
				Produceref(X_ref3,JCIIF3); //产生参考信号3
				Produceref(X_ref4,JCIIF4); //产生参考信号4

				Produce_send_signal(RAMSES_ref_CIF);  //产生发射信号CIF

				while(XDCZ_CIF_count<XDCZ_num)//XDCZ_CIF_count<XDCZ_num
				{
					if(XDCZ_work_mode==0)
					{
						IER0|= 0x0004; //使能外部中断0(int0)
						IFR0 = 0xffff; //清除标志位IFR0
						Flag_CIF = 0;
						sig1_flag=1;sig2_flag=1;sig3_flag=1;sig4_flag=1;
					}
					else if (XDCZ_work_mode==1)
					{
						Flag_CIF = 0;
						sig1_flag=1;sig2_flag=1;sig3_flag=1;sig4_flag=1;
						XDCZ_Tranmit_CIF();
					}

					detect_signal_XDCZ();    //检测信号
					DMA_stop(myhDma);
					XDCZ_CIF_count++;        //相对测阵次数自加
					Flag_CIF=0;

					if(FSK_flag == 1)        //检测到FSK指令
					{
						FSK_flag = 0;
						break;
					}

					if(FLAG_RS232_REVOK == 1)//检测到串口信号
					{
						URAT_reply_func();
						break;
					}
				}//while_end

				Transmit_IRF_delay();//单载波时延结果上传

			}//相对测阵模式


			//主站；//以固定周期发射CIF信号;只检测FSK；
			if (main_node == 2)
			{
				CIF_enable= 0;//只检测FSK
				IIF_enable= 0;//只检测FSK
				zhuzhan_detect_count = (responder_period-10000) / 41200;
				//计算一个主站发射间隔内需要检测多少次信号
				//由于Senddata0()函数需要等待DMA发送完毕才能结束，所以主站间隔应该减去发送信号长度10ms
				zhuzhan_detect_count_remain = (responder_period-10000) % 41200;
				//DMANum=0;
				Flag_timer = 0; //Timer0完成标志
				Produce_send_signal(RAMSES_ref_CIF);  //CIF
				//TIMER_start(mhTimer0);
				while(1)
				{
					Tranmit_CIF();
					DMA_count_zhuzhan = 0;//主站DMA计数清零
					DMA_start(myhDma);
					detect_signal();       //只检测FSK
					DMA_stop(myhDma);
					//DMANum = BUF_LEN_MAX - ((DMANum & 0x0fff) >> 1);
					//DMANum = DMANum /40;
					///////////////////////////////////////////////
					IFR1 = 0x0ffff;
					IER1 |= 0x0040; //开启定时器1中断
					timer_high_clk(zhuzhan_detect_count_remain,100);//把剩余时间计完
					IFR1 = 0x0ffff;
					IER1 &= 0x0ffbf;//关闭定时器1中断
					///////////////////////////////////////////////
					if( FSK_flag==1 )
					{
						FSK_flag=0;
						//TIMER_stop(mhTimer0);
						URAT_reply_func();
						break;
					}

					if(FLAG_RS232_REVOK==1)//检测到串口信号
					{
						URAT_reply_func();
						take_action_FSK();
						Producefilter_Band(ID1 ,ID2);
						break;
					}
				}//while_end
				system_config();           //系统重新配置
			}//主站


			//辅站；处于监听状态//检测FSK//检测CIF//检测IIF
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
				}//发射IIF
				Produceref(X_ref,  RAMSES_ref_CIF);   //产生参考信号CIF
				Produceref(X_ref2, RAMSES_ref_IIF);   //产生参考信号IIF

				while(1)
				{
					DMA_start(myhDma);
					detect_signal();
					DMA_stop(myhDma);

					if((Flag_CIF == 1) || (Flag_IIF == 1)) //检测到CIF信号
					{
						take_action_CIF_IIF();
						URAT_reply_func();
						system_config();
					}

					if(FSK_flag == 1)      //检测到指令信号
					{
						FSK_flag = 0;
						URAT_reply_func();
						break;
					}
					if(FLAG_RS232_REVOK==1)//检测到串口信号
					{
						URAT_reply_func();
						take_action_FSK();
						Producefilter_Band(ID1 ,ID2);
						break;
					}
				}//while_end

				system_config();   //系统重新配置

			} //辅站模式
		}//水下监听模式


		else if(transponder_mode == 3)//电触发模式
		{
			Produce_send_signal(RAMSES_ref_CIF);  //产生发射信号CIF
			IER0 |= 0x0004;//使能外部中断0；int0
			IFR0 = 0xffff; //清除标志位IFR0
			IER1 |= 0x0001;//使能外部中断1；RS232串口中断
			IFR1 = 0xffff;
			responder_flag = 0;
			while(1)
			{
				if(responder_flag == 1)
				{
					Respondermod();
				}
				DMA_start(myhDma);
				detect_signal();  //只检测FSK
				DMA_stop(myhDma);

				//DMANum首先去除高4位，这样得到的是外部触发到达DMA目的地址，因为数据类型为16位，除以2得到当前的数据数，
				DMANum = BUF_LEN_MAX - ((DMANum & 0x0fff) >> 1);
				DMANum = DMANum * 25 + 12;

				if(FSK_flag == 1)        //接收到FSK指令
				{
					FSK_flag = 0;
					URAT_reply_func();
					break;
				}
				if(FLAG_RS232_REVOK == 1)//接收到串口中断
				{
					URAT_reply_func();
					take_action_FSK();
					Producefilter_Band(ID1 ,ID2);
					break;
				}
			} //while_end

			system_config();   //系统重新配置

		} //电触发模式

	}   //while(1)主循环
}    //main












