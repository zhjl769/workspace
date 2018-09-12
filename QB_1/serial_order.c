
/*******************************************************************************
* FILENAME
*   serial_order.c
*
* DESCRIPTION
*   QB 
*
*******************************************************************************/
#include "serial_order.h"

unsigned char order_redundancy[64];
unsigned char Compass_buffer[512];
unsigned char Command_buffer[30];
unsigned char order_buffer[16];
//extern unsigned char Compass_Int_flag;
unsigned char Compass_To_BufferOK_flag = 0;
unsigned char Command_Int_flag = 0;  // 串口命令标志位
unsigned char LJ_ok=0;
unsigned char Sync_buffer[64];



/****************************************************************************
*void R232_To_LJ(unsigned char *pCommand,int bytesSize)                     *
*						串口命令发送函数									*
*					作用：发送控制信息给上位机、同步器					    *
*					参数1：char *pCommand待发送信息的起始地址				*
*					参数2：int byteSize待发送信息的字节长度					* 
*****************************************************************************/
void R232_To_LJ(unsigned char *pCommand,int bytesSize)
{
	int i;
	int j;
//	int k;
	for(i=bytesSize;i>0;i--)
   {	
		*(unsigned char *)(EMB_CE0+LJ_A4+THR_7) = *pCommand;//写字节到754
		pCommand++;
		for(j=0;j<100000;j++)
	   		asm(" nop ");   
   } 
}


/****************************************************************************
*void R232_To_sync(unsigned char *pCommand,int bytesSize)                  *
*						串口命令发送函数									*
*					作用：发送控制信息给上位机、同步器					    *
*					参数1：char *pCommand待发送信息的起始地址				*
*					参数2：int byteSize待发送信息的字节长度					* 
*****************************************************************************/
void R232_To_sync(unsigned char *pSync,int bytesSize)
{
	int i;
	int j;
   for(i=bytesSize;i>0;i--)
   {	
		*(unsigned char *)(EMB_CE0+SYNC_A5+THR_7) = *pSync;//写字节到754
		pSync++;
		for(j=0;j<7500;j++)
	   		asm(" nop "); 	  
   } 	
}


/****************************************************************************
*void R232_To_order(unsigned char *pCommand,int bytesSize)                 *
*						串口命令发送函数									*
*					作用：发送控制信息给上位机、同步器					    *
*					参数1：char *pCommand待发送信息的起始地址				*
*					参数2：int byteSize待发送信息的字节长度					* 
*****************************************************************************/
void R232_To_order(unsigned char *pCommand,int bytesSize)
{
   int i;
   int j;
   for(i=bytesSize;i>0;i--)
   {	
		*(unsigned char *)(EMB_CE0+ORD_A7+THR_7) = *pCommand;//写字节到754
		pCommand++;
		for(j=0;j<100000;j++)
	   		asm(" nop "); 	  
   } 
}
/****************************************************************************
*void CompassData_to_buffer(void)                                           *
*					罗经中断处理程序     									*
*作用：罗经中断使能后，为了避免罗经中断处理时间过长，将ISR放到外边          *
  	将罗经数据搬移到Compass_buffe中，并置标志位		                        *				                              				            	* 
*****************************************************************************/
/*
void CompassData_to_buffer(void)
{	
	unsigned char Lj_Buffer_index = 0;
	unsigned char temp = 0;
	Compass_Int_flag = 0;
	do{

		Compass_buffer[Lj_Buffer_index]=*(unsigned  char *)(EMB_CE0+LJ_A4+RHR_7);
		Lj_Buffer_index++;
		temp =*(unsigned  char *)(EMB_CE0+LJ_A4+LSR_7);
		temp = (temp & 0x1) && (Lj_Buffer_index<65);

	}while(temp!=0);
	Compass_To_BufferOK_flag = 1;

}
*/

void CompassData_to_buffer(void)     // 罗经中断形式收数；
{	
    unsigned char icomp=0;
	static unsigned char Lj_Buffer_index;
	static unsigned char Comp_int=0;
    char tempComp=0;
    LJ_ok=0;
	for(;icomp<64;icomp++)
	{
	    tempComp=*(unsigned  char *)(EMB_CE0+LJ_A4+RHR_7);

	    if (tempComp==0x24)         // 判断罗经头；
	    {	
	     	Lj_Buffer_index=0;
		    Comp_int=1;
	    }
	
	    if (Comp_int==1)
	    { 	
		    Compass_buffer[Lj_Buffer_index]=tempComp;
		    if(Compass_buffer[Lj_Buffer_index]!=0x2A)
			    Lj_Buffer_index++;
		    else
		    {
			    Lj_Buffer_index = 0;
			    Comp_int=0;      // 判断罗经尾；
		    	LJ_ok=1;
				icomp=64;
	    	}
    	}	
	}	
		
}

void Command_to_buffer(void)     // 串口中断形式收上位机命令；
{
 	//if()
	static unsigned char Command_Buffer_index;
	static unsigned char Command_int=0;
    char tempCommand=0;

	tempCommand=*(unsigned  char *)(EMB_CE0+SYNC_A5+RHR_7);

	if (tempCommand==0x24)         // 判断罗经头；
	{	
	 	Command_Buffer_index=0;
		Command_int=1;
	}
	
	if (Command_int==1)
	{ 	
		Command_buffer[Command_Buffer_index]=tempCommand;
		if((Command_buffer[Command_Buffer_index]!=0x23)&&Command_Buffer_index<22)
		{
			Command_Buffer_index++;
		}
		else
		{
			Command_Buffer_index = 0;
			Command_int=0;      // 判断罗经尾；
			Command_Int_flag=1;
		}
	}
}

void RS485_to_KELLER(unsigned char *pRS485,int bytesSize)
{
   int i;
   int j;
   for(i=bytesSize;i>0;i--)
   {	
		*(unsigned char *)(EMB_CE0+DP_A6+THR_7) = *pRS485;//写字节到754
		pRS485++;
		for(j=0;j<200;j++)
	   		asm(" nop "); 	  
   }    
}

/****************************************************************************
*void SyncData_to_buffer(void)                                              *
*					同步信号接收     									    *
*作用：接收同步信号	                                                        *				                              				            	
****************************************************************************/
void SyncData_to_buffer(void)
{	int j=0;
    int temp;
    int iiii=0;
    unsigned char Sync_Buffer_index = 0;
    for (;iiii<64;iiii++)
    {
    	Sync_buffer[Sync_Buffer_index]=*(unsigned  char *)(EMB_CE0+ORD_A7+RHR_7);
	    Sync_Buffer_index++;
		
        for(j=0;j<100;j++)
	   		asm(" nop ");
		temp= *(unsigned  char *)(EMB_CE0+ORD_A7+LSR_7);
		if(temp==0)
		  { iiii=64; }
    }
    iiii=0;
}
