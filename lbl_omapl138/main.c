/*************************************文件信息**********************************************************************************
**文件名：main.c
**版本：v1
**创建人：liuhai
**创建日期： 2013-2014
**描述: 多目标检测
-----------------------------------当前版本修改---------------------------------------------------------------------------------
**修改人：guojun
**版本：v2
**日期：2014.9.2
**描述：多目标检测
*************************************************************************************************************************************/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <c6x.h>
#include "main.h"
  
void main()
{ 
//*************************initial program***********************************//

	evm_init_edma_setup(Edma30cc_0_RegsOvly); //EDMA触发事件为MCBSP触发
	evm_init_mcbsp(); //AD采样的配置
	evm_init_gpio_extern(); //GPIO配置
	evm_init_uart(UART1);
	    
	//配置中断服务程序
	config_Interrupt_Selector(4,CSL_INTC_EVENTID_GPIO_BNK6_INT);
	disableSpecificINT(4);//同步
	config_Interrupt_Selector(5,CSL_INTC_EVENTID_BOOTCFGINT0);
 	disableSpecificINT(5);//开始/暂停命令
	config_Interrupt_Selector(7,CSL_INTC_EVENTID_EDMA3_0_CC0_INT1);
	disableSpecificINT(7);//EDMA中断
	config_Interrupt_Selector(6,CSL_INTC_EVENTID_T64P2_TINTALL);
	disableSpecificINT(6);//发射信号 
	config_Interrupt_Selector(8,CSL_INTC_EVENTID_T64P1_TINT12 );
	disableSpecificINT(8);//配置CPLD时间
	ISTP = (unsigned int)intcVectorTable;
	ICR = DSPINTC_IST_ALL_MASK_INT;

	gen_twiddle_fft32x32(w, NFFT, 2147483647.5);  //蝶形因子的使用2的31方
	init_global_var(); 	                                         //全局变量初始化
	gen_sig(Ref_fft1, Sig_flag[0], &CHIP_L, Refsig_len, F, RAMSES_CODE, w, Temp_buffer1, Temp_buffer2);        //四个定位信标发射的信号的FFT
	gen_sig(Ref_fft2, Sig_flag[1], &CHIP_L, Refsig_len, F, RAMSES_CODE, w, Temp_buffer1, Temp_buffer2);
	gen_sig(Ref_fft3, Sig_flag[2], &CHIP_L, Refsig_len, F, RAMSES_CODE, w, Temp_buffer1, Temp_buffer2);
	gen_sig(Ref_fft4, Sig_flag[3], &CHIP_L, Refsig_len, F, RAMSES_CODE, w, Temp_buffer1, Temp_buffer2);
	gen_transmit_sig(Trans_signal, &Trans_signal_len, Trans_signal_flag, F, RAMSES_CODE);                      //PWM信号
	
	enableSpecificINT(5);
	enableSpecificINT(6);
	enableSpecificINT(7);
	enableSpecificINT(8);
	enableGlobalINT();
	enableNMI();

	while(1)
	{	
		//==============开始命令与配置========================//
		if(Configure_flag == 1)  //待机状态--配置 中断？
		{	
		   
		    disableSpecificINT(4);  //关闭中断4
		    Configure_flag = 0;		
			configure_para();             //显控参数配置下传
			ICR = DSPINTC_IST_INT4;	//清除中断寄存器
			enableSpecificINT(4);  //启动中断4
			Syn_flag = 0;
		}
		//=============等待同步信号---开始信号检测==================// 
		if (Start_flag == 1 && Syn_flag == 1 ) 
		{				  
			Syn_flag = 0;
			if(Work_mode == 1)	 //应答模式 
			{	
				evm_init_timer_send(TMR2, 1); //定时器中断设定为1us
				transmit(Trans_signal,Trans_signal_len); 
			}

			AD_start();	   //开启MCBSP1,准备采集数据 

		    //进入检测
	        while(Block_num <= Max_Buffer)
	        {
		       	if(Rx_flag == 1)//接收到信号标志 
		       	{        
					Rx_flag = 0;
					
	           		if(Pp_flag == 1)
						fifo(Fifo1, buffer1, IN_LENGTH);
					else
						fifo(Fifo1, buffer2, IN_LENGTH);

					memcpy(ddr2_buffer+Block_num*IN_LENGTH, Fifo1+2*IN_LENGTH, IN_LENGTH*2);      //buffer1或2中的数存到ddr2中
					Block_num++;

					get_fft(Sig_fft, Fifo1+IN_LENGTH, NFFT, w, NFFT, Temp_buffer1);		//采到信号的FFT



					if(Sig1.flag == 0)
					{
						get_xcorr(Correlation1, Sig_fft, Ref_fft1, NFFT, w, Temp_buffer1, Temp_buffer2);      //采到的信号与本地参考信号做相关
						get_envelop(Envelop1, Correlation1, Coef_fft, NFFT, w, Temp_buffer1, Temp_buffer2);
						detection(&Sig1, Envelop1, &Parameter_sig, Block_num, Close_num, Fifo1);	
						if(Sig1.flag == 1)
						{
							get_delay(Envelop1[Sig1.corr_pos-1], Envelop1[Sig1.corr_pos], Envelop1[Sig1.corr_pos+1], &Sig1, Block_num, Turnaround[0], Work_mode);
							ddr2_beacon_detection_flag_num[0] = 1; 
							ddr2_beacon_detection_flag[0] = 1;
						}
					}
					if(Sig2.flag == 0)
					{
					    get_xcorr(Correlation2, Sig_fft, Ref_fft2, NFFT, w, Temp_buffer1, Temp_buffer2);
						get_envelop(Envelop2, Correlation2, Coef_fft, NFFT, w, Temp_buffer1, Temp_buffer2);
					  	detection(&Sig2, Envelop2, &Parameter_sig, Block_num, Close_num, Fifo1);
						if(Sig2.flag == 1)
						{
							get_delay(Envelop2[Sig2.corr_pos-1], Envelop2[Sig2.corr_pos], Envelop2[Sig2.corr_pos+1], &Sig2, Block_num, Turnaround[1], Work_mode);
							ddr2_beacon_detection_flag_num[1] = 1; 
							ddr2_beacon_detection_flag[1] = 1;
						}
					}
					if(Sig3.flag == 0)
					{						
						get_xcorr(Correlation3, Sig_fft, Ref_fft3, NFFT, w, Temp_buffer1, Temp_buffer2);
						get_envelop(Envelop3, Correlation3, Coef_fft, NFFT, w, Temp_buffer1, Temp_buffer2);
						detection(&Sig3, Envelop3, &Parameter_sig, Block_num, Close_num, Fifo1);
						if(Sig3.flag == 1)
						{
							get_delay(Envelop3[Sig3.corr_pos-1], Envelop3[Sig3.corr_pos], Envelop3[Sig3.corr_pos+1], &Sig3, Block_num, Turnaround[2], Work_mode);	
							ddr2_beacon_detection_flag_num[2] = 1;
							ddr2_beacon_detection_flag[2] = 1; 
						}
					}
					if(Sig4.flag == 0)
					{
						get_xcorr(Correlation4, Sig_fft, Ref_fft4, NFFT, w, Temp_buffer1, Temp_buffer2);
						get_envelop(Envelop4, Correlation4, Coef_fft, NFFT, w, Temp_buffer1, Temp_buffer2);
						detection(&Sig4, Envelop4, &Parameter_sig, Block_num, Close_num, Fifo1);
						if(Sig4.flag == 1)
						{
							get_delay(Envelop4[Sig4.corr_pos-1], Envelop4[Sig4.corr_pos], Envelop4[Sig4.corr_pos+1], &Sig4, Block_num, Turnaround[3], Work_mode);
					    	ddr2_beacon_detection_flag_num[3] = 1;
					    	ddr2_beacon_detection_flag[3] = 1; 
					    }
					  //  if(Sig4.delay <= -0.001 )
					//	    Sig4.delay = 0.1;
					}
/*
					if(ddr2_beacon_detection_flag[0]==1 || ddr2_beacon_detection_flag[1]==1 || ddr2_beacon_detection_flag[2]==1 || ddr2_beacon_detection_flag[3]==1)
					{
						memset(ddr2_beacon_detection_flag, 0, 4);
						DSP_interrupt_ARM();
					}*/
					


			//	    if(Block_num==10 && Sig1.flag == 0 )
			//		Block_num=10;	
	          	}
	        }
			//检测结束，关闭mcbsp
			AD_stop(); 
			close_EDMA_E4(Edma30cc_0_RegsOvly);
			evm_init_edma_setup(Edma30cc_0_RegsOvly);			
			Tcp_Info();
			DSP_interrupt_ARM();
			init_global_var(); 
	  	} 
  	} 		     		    		    																	 			  											
}

