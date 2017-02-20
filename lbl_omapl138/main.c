/*************************************�ļ���Ϣ**********************************************************************************
**�ļ�����main.c
**�汾��v1
**�����ˣ�liuhai
**�������ڣ� 2013-2014
**����: ��Ŀ����
-----------------------------------��ǰ�汾�޸�---------------------------------------------------------------------------------
**�޸��ˣ�guojun
**�汾��v2
**���ڣ�2014.9.2
**��������Ŀ����
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

	evm_init_edma_setup(Edma30cc_0_RegsOvly); //EDMA�����¼�ΪMCBSP����
	evm_init_mcbsp(); //AD����������
	evm_init_gpio_extern(); //GPIO����
	evm_init_uart(UART1);
	    
	//�����жϷ������
	config_Interrupt_Selector(4,CSL_INTC_EVENTID_GPIO_BNK6_INT);
	disableSpecificINT(4);//ͬ��
	config_Interrupt_Selector(5,CSL_INTC_EVENTID_BOOTCFGINT0);
 	disableSpecificINT(5);//��ʼ/��ͣ����
	config_Interrupt_Selector(7,CSL_INTC_EVENTID_EDMA3_0_CC0_INT1);
	disableSpecificINT(7);//EDMA�ж�
	config_Interrupt_Selector(6,CSL_INTC_EVENTID_T64P2_TINTALL);
	disableSpecificINT(6);//�����ź� 
	config_Interrupt_Selector(8,CSL_INTC_EVENTID_T64P1_TINT12 );
	disableSpecificINT(8);//����CPLDʱ��
	ISTP = (unsigned int)intcVectorTable;
	ICR = DSPINTC_IST_ALL_MASK_INT;

	gen_twiddle_fft32x32(w, NFFT, 2147483647.5);  //�������ӵ�ʹ��2��31��
	init_global_var(); 	                                         //ȫ�ֱ�����ʼ��
	gen_sig(Ref_fft1, Sig_flag[0], &CHIP_L, Refsig_len, F, RAMSES_CODE, w, Temp_buffer1, Temp_buffer2);        //�ĸ���λ�ű귢����źŵ�FFT
	gen_sig(Ref_fft2, Sig_flag[1], &CHIP_L, Refsig_len, F, RAMSES_CODE, w, Temp_buffer1, Temp_buffer2);
	gen_sig(Ref_fft3, Sig_flag[2], &CHIP_L, Refsig_len, F, RAMSES_CODE, w, Temp_buffer1, Temp_buffer2);
	gen_sig(Ref_fft4, Sig_flag[3], &CHIP_L, Refsig_len, F, RAMSES_CODE, w, Temp_buffer1, Temp_buffer2);
	gen_transmit_sig(Trans_signal, &Trans_signal_len, Trans_signal_flag, F, RAMSES_CODE);                      //PWM�ź�
	
	enableSpecificINT(5);
	enableSpecificINT(6);
	enableSpecificINT(7);
	enableSpecificINT(8);
	enableGlobalINT();
	enableNMI();

	while(1)
	{	
		//==============��ʼ����������========================//
		if(Configure_flag == 1)  //����״̬--���� �жϣ�
		{	
		   
		    disableSpecificINT(4);  //�ر��ж�4
		    Configure_flag = 0;		
			configure_para();             //�Կز��������´�
			ICR = DSPINTC_IST_INT4;	//����жϼĴ���
			enableSpecificINT(4);  //�����ж�4
			Syn_flag = 0;
		}
		//=============�ȴ�ͬ���ź�---��ʼ�źż��==================// 
		if (Start_flag == 1 && Syn_flag == 1 ) 
		{				  
			Syn_flag = 0;
			if(Work_mode == 1)	 //Ӧ��ģʽ 
			{	
				evm_init_timer_send(TMR2, 1); //��ʱ���ж��趨Ϊ1us
				transmit(Trans_signal,Trans_signal_len); 
			}

			AD_start();	   //����MCBSP1,׼���ɼ����� 

		    //������
	        while(Block_num <= Max_Buffer)
	        {
		       	if(Rx_flag == 1)//���յ��źű�־ 
		       	{        
					Rx_flag = 0;
					
	           		if(Pp_flag == 1)
						fifo(Fifo1, buffer1, IN_LENGTH);
					else
						fifo(Fifo1, buffer2, IN_LENGTH);

					memcpy(ddr2_buffer+Block_num*IN_LENGTH, Fifo1+2*IN_LENGTH, IN_LENGTH*2);      //buffer1��2�е����浽ddr2��
					Block_num++;

					get_fft(Sig_fft, Fifo1+IN_LENGTH, NFFT, w, NFFT, Temp_buffer1);		//�ɵ��źŵ�FFT



					if(Sig1.flag == 0)
					{
						get_xcorr(Correlation1, Sig_fft, Ref_fft1, NFFT, w, Temp_buffer1, Temp_buffer2);      //�ɵ����ź��뱾�زο��ź������
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
			//���������ر�mcbsp
			AD_stop(); 
			close_EDMA_E4(Edma30cc_0_RegsOvly);
			evm_init_edma_setup(Edma30cc_0_RegsOvly);			
			Tcp_Info();
			DSP_interrupt_ARM();
			init_global_var(); 
	  	} 
  	} 		     		    		    																	 			  											
}

