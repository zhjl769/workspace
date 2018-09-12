
/*******************************************************************************
* FILENAME
*   isr.c
*
* DESCRIPTION
*   c641x Header File
*
*******************************************************************************/
#include "isr.h"
#include "serial_order.h"
#include "cf.h"
#include <csl.h>
#include <csl_irq.h>

// ����
//#define EDMABUFFER    // ����EDMA��־��buffer����־��λʱ���Ƿ�һ��



unsigned char beacon_ack_buffer[6] = {0,0,0,0};
unsigned char beacon_command_buffer[10] = {0,0,0,0,0,0,0,0,0,0};
unsigned char KELLER_buffer[10];

unsigned int Sync_Pulse_AB;
unsigned char timer1_1sOut_flag = 0;
unsigned char count_timer0_NO;
//unsigned char Compass_Int_flag = 0;
unsigned char Edma13_Int_flag = 0;
unsigned char Edma16_Int_flag = 0;
unsigned char Edma_Int_flag = 0;
//unsigned char SeriTest[2040]��

//int SerCount=0;
//int bytesSize1=120;
int Ser_OK=0;
unsigned char Sf[2040];
int what=0;

unsigned char Sync_Int_flag=1;
unsigned char timer0_sub=150;
unsigned char timer0_flag=0;
unsigned char Sync_pulse_add=0;
unsigned char timer0_model=0;
unsigned int timer0_add=0;
unsigned int timer1_add=0;
unsigned char timer1_model=0;
extern unsigned char Sync_number;
extern unsigned char sample_wait;

union 
{
	struct
	{ 
		unsigned int CIPRH32_48  : 17;
		unsigned int CIPRH49     : 1;
		unsigned int CIPRH50     : 1;
		unsigned int CIPRH51     : 1;
		unsigned int CIPRH52     : 1;
		unsigned int CIPRH53     : 1;
		unsigned int CIPRH54     : 1;
		unsigned int CIPRH55     : 1;
		unsigned int CIPRH56_63  : 8;			 
	}exp; 
	unsigned int reg;
}CIPRHvalue;

union 
{
	struct
	{ 
		unsigned int CIPRL0_12   : 13;
		unsigned int CIPRL13     : 1;
		unsigned int CIPRL14     : 1;
		unsigned int CIPRL15     : 1;
		unsigned int CIPRL16     : 1;
		unsigned int CIPRL17_31  : 15;			 
	}exp; 
	unsigned int reg;
}CIPRLvalue;

/****************************************************************************
*interrupt void c_int1()                                                    *
*						��������ͺ���									*
*					���ã�timer0 interrupt      								*
*					����1��                                 				*
*					����2��                             					* 
*****************************************************************************/
interrupt void c_int1()
{
	while(count_timer0_NO!=0)
	{
		if(Sync_Pulse_AB!=0)
			Sync_Pulse_AB--;
		else
			*(unsigned volatile int *)TIMER0_CTRL=0x00000215;
			disableSpecificINT(1);	
	}

	
}


/****************************************************************************
*interrupt void c_int2()                                                    *
*						��������ͺ���									*
*					���ã�timer1 interrupt ,��ʱ     						    *
*					����1��                                 				*
*					����2��                             					* 
*****************************************************************************/
interrupt void c_int2()
{


	timer1_1sOut_flag = 1;
	*(unsigned volatile int *)TIMER1_CTRL=0x00000215;
	disableSpecificINT(1);
	
}

interrupt void c_int4()
{
//    Sync_pulse_add++;
      Sync_pulse_add=1;
}

/****************************************************************************
*interrupt void c_int5()                                                    *
*						��������ͺ���									*
*					���ã�LJ interrupt      								*
*					����1��                                 				*
*					����2��                             					* 
*****************************************************************************/
interrupt void c_int5()  //�޾�
{


	
	CompassData_to_buffer();
	Compass_Int_flag = 1;

	
}

interrupt void c_int6()  // ��Ƶ�·ʱ�ô����ø�ͬ������ʵ���ϸ��˴�������
{ 
 	Command_to_buffer();
}

interrupt void c_int7()
{ 
   int i=0;
   int j=0;
   for(;i<10;i++)       //��ȼƷ��������10���ַ�
   {    
       KELLER_buffer[i]=*(unsigned char *)(EMB_CE0+DP_A6+RHR_7);       
        for(j=0;j<100;j++)
	   		asm(" nop ");
       j= *(unsigned char *)(EMB_CE0+DP_A6+LSR_7);
	   if (j==0)
	   {i=10;}
   }  
   i=0;
}


interrupt void c_int8(void)//EDMA interrupt
{	

	what++;
	CIPRLvalue.reg=*(unsigned int *)CIPRL;
	Edma_Int_flag = 1;
	#ifdef EDMABUFFER
	*(volatile unsigned int *)GPVAL=0x00000002;    // ���Զ˿�GPIO_1��1
    #endif
	
	if(CIPRLvalue.exp.CIPRL13 == 1)
	{
		*(volatile unsigned int *)CIPRL|=0x00002000;
		Edma13_Int_flag = 1;
		#ifdef EDMABUFFER
		*(volatile unsigned int *)GPVAL=0x00000000;    // ���Զ˿�GPIO_1��0
		#endif		
				
	}
			
	if(CIPRLvalue.exp.CIPRL16 == 1)
	{
		*(volatile unsigned int *)CIPRL|=0x00010000;
		Edma16_Int_flag = 1;
		#ifdef EDMABUFFER
	    *(volatile unsigned int *)GPVAL=0x00000000;    // ���Զ˿�GPIO_1��0
		#endif				
	}
			
}

interrupt void c_int9()
{    
//    *(volatile unsigned int *)GPDH =0xFFFFFFFF;
     *(volatile unsigned int *)GPDH =0x4;
     SyncData_to_buffer();
	 Sync_Int_flag=1;
}


interrupt void c_int13()
{
}


interrupt void c_int14()
{ 
 	if(timer0_model==0)
    {
      timer0_sub--;
	  if (timer0_sub==0)
	  { 
	    timer0_flag=1;
	  }
    }
    else 
    {
	  timer0_add++;
	  if(timer0_add==2000)
	  {  
	    timer0_add=0;
		Sync_number--;
	  }
    }     
}

interrupt void c_int15()
{ 
   
   if(timer1_model==1)   // ͬ����δͬ����ʱ����ģʽ��2min���ݰ���ʹ���˸ö�ʱ��������Ҫ�жϹ���ģʽ��
   {
 		Ser_OK=1;
   }
   
   else 
   {
        timer1_add++;
		//if(timer1_add==1800) // 2s����
		if(timer1_add==5)
		{
		   sample_wait=0;  //����һ��Сʱ����ͬ���ȴ�
           disableSpecificINT(15);
		   disableSpecificINT(9);
		   enableSpecificINT(5);
		   timer1_model=1;
		}
   }       
}

