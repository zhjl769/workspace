
/*************************************************
  Copyright (C)
  File name:   USBL.pjt   // 文件名
  Author:  Wang Yongheng     Version:   1.4     Date: 2014.6.16
  Description:   检测信号为25ms长LFM信号，9-14kHz，下调频
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
				 在2015年8月22之前的滤波器的截止频率为5KHz
				 现在的为7KHz
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
	IODOR = 0x006f;  //GPIO初始化，将GPIO7、4作为输入，其他作为输出
	IODATA = 0x0040;   //GPIO6拉高，关闭发射部分电源
	IRQ_globalDisable();//禁止全局中断
	IRQ_setVecs (0x0e00);//中断向量表映射地址
	CSL_init();	      //CSL初始化
	System_initial(); //系统硬件初始化，包括系统主频100MHz，SDRAM初始化，TL16C752初始化
	McBSP_init();     //McBSP初始化，主要完成A/D驱动控制，采样率设置为100kHz
	InitADC();        //内部AD初始化，采集电池电压
	DMA_init();       //DMA初始化  
	/* CSL的I2C SETUP函数有问题，不推荐使用，包括写入和读取函数，需要将I2CSTR寄存器中的BB位置1*/
	I2C_FSET(I2CSTR,BB,0x1);
	I2C_config(&I2Cconfig);
	I2C_RSET(I2COAR, 0x0000);  //主IIC模块自身的地址
	delay(90000);
	memset(SD_write,0,sizeof(short)*256);
	IRQ_globalEnable();//开启总中断
	I2C_Read(0xd700, SD_write, 28, 0x0050); //读出写入的数据
	if((SD_write[0] == 0xee) && (SD_write[27] == 0xff))//校验EEPROM数据
	{
		transponder_mode = SD_write[1];//工作模式
		tongbu_flag = SD_write[2];//命令位
		NOISE_TH_NEW = SD_write[4] + (SD_write[5] << 8);//重新计算绝对门限16bit
		Time_Delay = (Uint32)(SD_write[8] + (SD_write[9] << 8))*1000;
		voltage_AD = SD_write[12] + (SD_write[13] << 8);//电压量化值
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
		Time_Delay = 150000;//75ms对应75000us，定时精确到us
		responder_Delay = 30000;//信标模式下延迟30ms
		responder_period = 20; //信标模式下默认同步周期为2s，100ms*20
		transponder_mode = 1;//默认为声应答模式
		transponder_mode1 = 1;//默认为外电检测优先切换工作模式
		tongbu_flag = 0;//检测到同步命令标志位
		SD_status = 0;//SD卡状态标志位
		release_flag = 0;//释放成功状态位
		count_timer = 0;//同步命令计数器
		timer0_cnt = 0;//定时器0计数器
		code = 0x0000;
		DMA_count = 0;
		NOISE_TH_huadong = 100;//噪声滑动门限
		NOISE_TH_NEW = 100;//更新后的噪声值
		NOISE_TH_HD = 0;
	}

	Produceref();     //产生参考信号
	Producelfm();     //产生发射信号
	variable_init();  //变量初始化

	MMC_Init();//SD卡初始化
	SectorCounter = 8400;//数据扇区
	pressure_init(); //压力传感器初始化
	get_pressure();  //压力获取函数，获取值为0则压力传感器连接有问题
	get_voltage();   //获取电池电压
	get_temperature();    //读取温度
	get_noise();     //获取当前噪声
	uart_config();
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
	IODATA = 0x0000;   //GPIO6拉低，开启发射部分电源
	IRQ_enable(IRQ_EVT_INT1);
	Senddata0();//发射信号1
	while(1)
	{
		if(transponder_mode == 1)//应答模式
		{	
			IRQ_enable(IRQ_EVT_DMAC4);//使能DMA4，串口int1中断
			SPCR2_0 = 0x00c1;//打开McBSP
			SPCR1_0 = 0x0001;
			Time_Delay = 150000;//75ms对应75000us，定时精确到us
			while(1)
			{
				if(transponder_mode != 1)//接收到串口改变工作模式命令，跳出循环
				{	
					FLAG_RS232_REVOK = 0;//串口接收标志清零
					break;
				}
				/************************************************************************************************************************/
				if((transponder_mode1 == 1) && ((IODATA & 0x0010) == 16))//检测到外部电源接通，改变工作模式，跳出循环
				{
					transponder_mode = 3;//切换到外触发模式
					get_parameter();
					uart_config();
					if(SD_status == 1)
						SD_WriteData(7672,SD_write);//7672扇区为每次保留数据扇区
					for(i = 0; i < 28; i++)//发送42字节信息
					{
						UART_THR232 = SD_write[i];
						do
						{
							buff = UART_LSR232;
							f_send_uart = buff & (0x0060);
						}
						while(f_send_uart == 0);//查询发送位是否清零
					}
					break;
				}
				/*********************************/
				DMA_start(myhDma);//打开DMA接收数据
				detect_signal();//检测信号
				DMA_stop(myhDma);//关闭DMA接收数据
				switch (Flag_answer)//检测到定位信号
				{
				case 1: take_action1(); break;//执行动作1
				case 2: take_action2(); break;//检测到释放指令,执行动作2
				case 4: take_action3(); break;//检测到获取电量指令,执行动作3
				}
				if(FLAG_RS232_REVOK == 1)//检测到串口信号
				{	
					transponder_mode1 = 0;
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

		else if(transponder_mode == 2)//串口优先级降级模式
		{
			transponder_mode1 = 1;//切换到自动模式
			FLAG_RS232_REVOK = 0;//串口接收标志清零
			if((IODATA & 0x0010) != 0x10)
				transponder_mode = 1;
			else if((IODATA & 0x0010) == 0x10)
				transponder_mode = 3;
			get_parameter();
			uart_config();
			if(SD_status == 1)
				SD_WriteData(7672,SD_write);//7672扇区为每次保留数据扇区}
			for(i = 0; i < 28; i++)//发送42字节信息
			{
				UART_THR232 = SD_write[i];
				while((UART_LSR232 & 0x0060) == 0);//查询发送位是否清零
			}
		}

		else if(transponder_mode == 3)//电触发模式
		{
			IODATA = 0x0000;//开启发射部分的电源
			IRQ_enable(IRQ_EVT_INT0);//开启外部中断0
			IRQ_enable(IRQ_EVT_INT1);//开启外部中断1,串口中断
			IRQ_clear(IRQ_IEMASK_ALL);//清除所有的标志位
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
						while((UART_LSR232 & (0x0060)) == 0)
							buff = UART_LSR232;//查询发送位是否清零
					}
					break;
				}

				if((transponder_mode1 ==1) && ((IODATA & 0x0010) != 16))//检测到外部电源丢失，切换工作模式，跳出循环
				{
					transponder_mode = 1;//切换工作模式为应答模式
					get_parameter();
					uart_config();
					for(i = 0; i < 28; i++)//发送42字节信息
					{
						UART_THR232 = SD_write[i];
						while((UART_LSR232 & (0x0060)) == 0)
							buff = UART_LSR232;//查询发送位是否清零
					}
					break;
				}
				Respondermod();
			}
		}//if(transponder_mode == 1)//应答模式
	}//end while(1)
}





