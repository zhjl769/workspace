#include <csl.h>
#include <csl_chip.h>
#include <csl_timer.h>
#include <csl_dma.h>
#include "init.h"
#include "variable.h"
#include "interrupt.h"


#define IER0 *(volatile unsigned int *)0x00000
#define IFR0 *(volatile unsigned int *)0x00001

//extern void VECSTART(void);
//extern Uint32 myVec;
extern Uint32 VECSTART;



//int _system_pre_init(void)
//{
/* Insert your low-level initializations here */

/* Disable Watchdog timer to prevent reset during */
/* long variable initialization sequences. */
//	CLK_init(); //���ú���CLK_init��ʹDSP������Ԥ���ʱ����
//		CSL_init();
//		SDRAM_init();
/*==================================*/
/* Choose if segment initialization */
/* should be done or not. */
/* Return: 0 to omit initialization */
/* 1 to run initialization */
/*==================================*/
//return 1;
//}

void main() //������
{
	CLK_init(); //���ú���CLK_init��ʹDSP������Ԥ���ʱ����
	SDRAM_init();
	CSL_init();
	GPIO_init();
	DMA_init();

	IRQ_globalDisable();
	IRQ_setVecs((Uint32)(&VECSTART));
	IRQ_plug(IRQ_EVT_TINT0,&timer0_Isr);
	IRQ_enable(IRQ_EVT_TINT0);
	//IRQ_enable(IRQ_EVT_TINT1);
	IRQ_globalEnable();
	//IRQ_setVecs((Uint32)(&VECSTART));

	unsigned int i=0; //�������i


	for(i=0;i<4096;i++)
	{
		if(i%2==0)
		{
			Send_sig[i]=0x03;
		}
		else
		{
			Send_sig[i]=0x05;
		}


	}



	TIMER_start(mhTimer0);
	//TIMER_start(mhTimer1);


	while(1)
	{

		if(send_flag==1)
		{
			send_flag=0;
			DMA_FSETH(hDmaSend0,DMACCR,ENDPROG,1);
			DMA_start(hDmaSend0);
			TIMER_start(mhTimer1);
		}

	}


	while(1) //�˴�����һ����ѭ��
	{
		i=i+1;
	}
}
