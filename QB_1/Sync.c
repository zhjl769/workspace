#include "Sync.h"
#include "serial_order.h"
#include <csl_irq.h>
#define  Sync_order_size 16   //ͬ������Ϊ16���ַ�
unsigned char Sync_order_buffer[Sync_order_size];

extern unsigned char Compass_buffer[];
extern unsigned char Sync_buffer[];  //���ڴ��ͬ����Ϣ
extern unsigned char timer0_flag;
extern unsigned char timer0_sub;
extern unsigned char Compass_Int_flag;
extern unsigned char Sync_pulse_add;
extern unsigned char Sync_Int_flag;
extern unsigned char timer0_model;
unsigned char sample_wait=1;   //ͬ���ȴ�״̬
extern unsigned char LJ_ok;
unsigned char Sync_number=0;
unsigned char Sync1_buffer[16];
extern unsigned char KELLER_buffer[];
unsigned char KELLER_TX1[4]= {0xFA,0x30,0x04,0x43};
unsigned char KELLER_RX1[10];
extern unsigned char timer1_model;


void check_Sync(void)
{    
    *(volatile unsigned int *)GPDH =0x4;
    enableSpecificINT(9);
    Sync_Int_flag=0;
	timer0_8ms_timeout();
    while(sample_wait)
	{ 
	  Command_Choose();
	  if (Sync_Int_flag==1)
	  { 
	    unsigned char order_test_start=0;
	    int Sync_buffer_index=0;  
	    Sync_Int_flag=0;
	    for(;Sync_buffer_index<64;Sync_buffer_index++)
		{
		   if (Sync_buffer[Sync_buffer_index]==0x5A)
		   { int i=0; 
		     for (;i<16;i++)
			 {
			   Sync_order_buffer[i]= Sync_buffer[Sync_buffer_index+i];
			   if (i==15)
			   {  
			     if (Sync_order_buffer[0]==0x5A&&Sync_order_buffer[1]==0x5A&&Sync_order_buffer[14]==0xA5&&Sync_order_buffer[15]==0xA5)
                 {
                   Sync_buffer_index=64;
                   order_test_start=1;
                 }
			   }
			 }
             
             
		   }		   
		}
	    while (order_test_start)
		{ 
		  
		  unsigned short order_select;
		  
		  order_test_start=0;
		  order_select=Sync_order_buffer[2];
          order_select=(order_select<<8)+Sync_order_buffer[3];
          
		  switch (order_select)
		  {
		    case 0x5654:   //�Ƿ��ѹ�ɼ�
			    //��ѹ�ɼ�����
				
				order_return();
				
				break;
			case 0x6363:   //�Ƿ�Ǳ���Լ�
			    //RTC��ʼ��
				self_check();
                order_return();
				//order_clear();
                //Sync_order_clear();
				break;
			case 0x474F:   //�Ƿ�ֱ�Ӳ���
			    order_return();
				sample_wait=0;  //�˳��ȴ�ģʽ��׼������
				break;
            case 0x7373:   //�Ƿ�ͬ��
			    disableSpecificINT(6);
			    Sync_start();
				break;			    
		  }		   
		}
	  }
	}
}

void self_check()
{  int m=0;   
   int compass_test=1;
   enableSpecificINT(7);     //��ȼ���������ж�ʹ��,�ȼ��KELLER�����Ƿ�����
   RS485_to_KELLER(KELLER_TX1,4);    //���Ϳ������KELLER
   for(;m<4000000;m++)
	   asm(" nop ");
   if((KELLER_buffer[0]==0xFA)&&(KELLER_buffer[1]==0x30))      //�жϻظ������Ƿ���ȷ
   { 
	   int n;
	   for(n=0;n<10;n++)
	   { 
	      KELLER_RX1[n]=KELLER_buffer[n];
	   }
   }
   else
   {   
       strcpy(Sync_order_buffer,"KELLER_ERROR!!!!");
       order_return();
   }
   enableSpecificINT(14);    
   *(unsigned volatile int *)TIMER0_CTRL=0x000004D5;  //������ʱ��1��׼��CF���Լ�
   Read_QB_num();//��ȡǱ���
   //дCF���������
   //��ʼ��McBSP
   disableSpecificINT(14);
   *(unsigned volatile int *)TIMER0_CTRL=0x00000415;  //�رն�ʱ��1
   
   if(timer0_flag==1)
   { 
     timer0_sub=150;  //ʱ��ԼΪ8ms*150
     timer0_flag=0;   //�����ʱ��־λ
  //CF��busy��ʱ����
	 strcpy(Sync_order_buffer,"CF_ERROR!!!!!!!!");  //CF��busy��ʱ����
	 
     order_return();
   }
   else 
   { 
     timer0_sub=150;  //ʱ��ԼΪ8ms*150
     timer0_flag=0;   //�����ʱ��־λ                 
     enableSpecificINT(14);    
     *(unsigned volatile int *)TIMER0_CTRL=0x000004D5;  //������ʱ��1��׼���޾��Լ�
	 enableSpecificINT(5);   //LJ�ж�ʹ��
	 while(compass_test)
	 {
	    if(Compass_Int_flag==1)
        {
//           int i=0;
//		   int j=0;
//		   int k=0;
//	       CompassDatas_to_buffer();
           compass_test=0;
		   disableSpecificINT(5);
           disableSpecificINT(14);
           *(unsigned volatile int *)TIMER0_CTRL=0x00000415;  //�رն�ʱ��1
           timer0_sub=150;  //ʱ��ԼΪ8ms*150
           timer0_flag=0;   //�����ʱ��־λ
           Compass_Int_flag=0;
           if(LJ_ok==0)
		      compass_error();
/*		   for(;i<64;i++)
		   {
		      if(Compass_buffer[i]==0x24)
			    { j++; }
			  if(Compass_buffer[i]==0x2A)
			    { k++; }		      
		   }
           if((j==0)||(k==0))
		   { compass_error(); }  */
        }
		else if(timer0_flag==1)
		{  
		  compass_error();
		  compass_test=0; 
		}
		
     }

   }
}

void order_return()
{  
   
   R232_To_order(Sync_order_buffer,Sync_order_size);
}

void order_clear(void)
{  
   unsigned short i=0;
   for(;i<64;i++)
   { 
     Sync_buffer[i]=0;
   }
}

void Sync_order_clear(void)
{  unsigned short i=0;
   for(;i<16;i++)
   {
     Sync_order_buffer[i]=0;
   }
}

void compass_error()
{  
   disableSpecificINT(5);
   disableSpecificINT(14);
   *(unsigned volatile int *)TIMER0_CTRL=0x00000415;  //�رն�ʱ��1
   timer0_sub=150;  //ʱ��ԼΪ8ms*150
   timer0_flag=0;   //�����ʱ��־λ
   Compass_Int_flag=0;
   strcpy(Sync_order_buffer,"COMPASS_ERROR!!!");  //LJ��ʱ����
   order_return();

}

void Sync_start()
{  unsigned char pulse_wait=1;
   unsigned char order_wait=1;
   unsigned char pulse2_wait=1;
   if(Sync_order_buffer[13]==0xAA)
   {
//      int wait0=0;
//      for(;wait0<15000000;wait0++)
//	      asm( " nop " );
	  CHIP_CRSET(ICR,0x00000010);
      enableSpecificINT(4);   //ʹ������ͬ���ж�
//	  Sync_pulse_add=0;
	  while(pulse_wait)
	  {  
	     
         
	     if(Sync_pulse_add==1)
		 {  int mmm=0; 
		    order_return();
			disableSpecificINT(4);
			Sync_pulse_add=0;
			for(;mmm<16;mmm++)
			{   
			  Sync1_buffer[mmm]= Sync_order_buffer[mmm];
			}
			order_clear();
			Sync_order_clear();
			while(order_wait)
			{   
			    if(Sync_Int_flag==1)
				{  

				   int Sync_buffer_index=0;
				   
                   for(;Sync_buffer_index<64;Sync_buffer_index++)
		           {
		                if (Sync_buffer[Sync_buffer_index]==0x5A)
		                { 
		                  int i=0; 
		                  for (;i<16;i++)
			              {
			                Sync_order_buffer[i]= Sync_buffer[Sync_buffer_index+i];
			                if (i==15)
			                {  
			                  Sync_buffer_index=64;  
			                  if ((Sync_order_buffer[0]==0x5A)&&(Sync_order_buffer[1]==0x5A)&&(Sync_order_buffer[14]==0xA5)&&(Sync_order_buffer[15]==0xA5))
                              {
							      if((Sync_order_buffer[2]==0x73)&&(Sync_order_buffer[3]==0x73)&&(Sync_order_buffer[13]==0xBB))
                                  {  
//								     int wait1=0;
//                                     for(;wait1<15000000;wait1++)
//	                                     asm( " nop " );
	                                 CHIP_CRSET(ICR,0x00000010);
                                     enableSpecificINT(4);   //ʹ������ͬ���ж�
//									 Sync_pulse_add=0;
								     while(pulse2_wait)
									 {   
									    if(Sync_pulse_add==1)
										{									  
										   order_return();
                                           pulse2_wait=0; 
                                           Sync_successed();
										}
										
	                                     
										
									 } //while(pulse2_wait)
                                  }   // if((Sync_order_buffer[2]==0x73)&&(Sync_orde......
                              }    //if ((Sync_order_buffer[0]==0x5A)&&(Sync_o.......
			                }    //if (i==15)
			              }    //for (;i<16;i++)         
		                }	 //	if (Sync_buffer[Sync_buffer_index]==0x5A)   
		           }  //for(;Sync_buffer_index<64;Sync_buffer_index++)				   
				   order_wait=0;
				   pulse_wait=0;
				   Sync_Int_flag=0;
				}  //if(Sync_Int_flag==1)			   
			}   //while(order_wait)
		 }     //if(Sync_pulse_add==1)
//         else if(Sync_pulse_add==0)
//		 {
//		    ;
//		 }
//		 else
//		 {  
//		    pulse_wait=0;
//		 }
	  }  //while(pulse_wait)



   }   //if(Sync_order_buffer[13]==0xAA)
}


void Sync_successed()      //��ʼװ������
{  int nnn=0;
   Sync_pulse_add=0;
   for(nnn=0;nnn<1000000;nnn++)
	   		asm(" nop ");
   
   disableSpecificINT(4);   //�ر�����ͬ���ж�
   //disableSpecificINT(6);
   timer0_5ms_timeout();
   Sync_number = Sync_order_buffer[5];
   Sync_number = (Sync_number<<8)+Sync_order_buffer[4];
   
   timer0_model=1;
   enableSpecificINT(14);    
   *(unsigned volatile int *)TIMER0_CTRL=0x000004D5;  //������ʱ��1��׼������ʱ
   disableSpecificINT(4);   //�ر�����ͬ���ж�
   disableSpecificINT(9);   //�ر���λ�������ж�
   ClearOrderFIFO();
   CHIP_CRSET(ICR,0xFFFFFFFF);
   enableSpecificINT(6);
   while(Sync_number)
   { 
    	if(Sync_number>1)
			Command_Choose();
		else
			disableSpecificINT(6);
   }
   disableSpecificINT(14);  //�رն�ʱ��1�ж�
   *(unsigned volatile int *)TIMER0_CTRL=0x00000415;  //�رն�ʱ��1,׼���ɼ�����
   
   //enableSpecificINT(6);    // 5.14���¼���ģ�
   sample_wait=0;     //�˳�ͬ���ȴ���׼���ɼ�

   disableSpecificINT(15);
   timer1_model=1;
   
}



