#include"msp430f6638.h"
#include"main.h"
#include"init.h"
#include"uart.h"
#include"irig.h"

#include "Mod_SDCard\mmc.h"
#include "Mod_SDCard\fAT16.h"
#include "predef.h"
#include <stdio.h>
#include <string.h>

uint32 FileSize = 0;
uint8 status = 1;
uint16 timeout = 0;
int i = 0;
/*
 * main.c
 */
extern unsigned char time[32];
unsigned char old_time[32];
extern char judge;

unsigned char as[32]={"RECORDED TIME"};

unsigned char PC_TO_DSP[32];
unsigned char PC_POS=0;
unsigned char PC_judge=0;

unsigned char DSP_TO_PC[32];
unsigned char DSP_POS=0;
unsigned char DSP_judge=0;


void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	//�رտ��Ź�

	initClock();				//��ʼ��ʱ��
	initPORT();					//��ʼ���˿�
	initADC();					//��ʼ��AD
	initTB();					//��ʼ����ʱ��TimerB
	UART_RS232_Init();
	UART_RS485_Init();
    __bis_SR_register(GIE);		//ʹ��ȫ���ж�


//	myDIR FileDir;
//	uint16 Index = 0;
//    while (status != 0)
//    {
//    	status = initMMC();
//    	timeout++;
//    	if (timeout == 50)
//    	{
//    		printf("No MMC/SD-card found!! %x\n", status);
//    		timeout = 0;
//    	}
//    }
//	ReadBPB();
//
//	if (!GetFileID("TEST0009DOC", &FileDir, &Index))
//	{
//		CreateFile("TEST0009DOC");
//		FileSize = 0;
//	}
//	else
//		FileSize = FileDir.FilePosit.Size;
//	OperateFile(1, "TEST0009DOC", FileSize, 32, as);
//	mmc_GoIdle();
//
//	P2DIR |= BIT7;
//	P2OUT &= ~BIT7;
    while(1)
    {
    	if(judge)
    	{
    		*strncpy(old_time, time, 20);
    		SEND_STR_232(time);
    		judge=0;
    	}
    	if(PC_judge)
    	{
//    		if((PC_TO_DSP[0]=='A')&(PC_TO_DSP[2]=='B'))
//    		{
//    			if(PC_TO_DSP[1]=='2')
//    			{
//    					switch(PC_TO_DSP[3])
//    					{
//    					case	'0':	P4OUT |= BIT7;	break;
//    					case	'1':	P4OUT &= ~BIT7;	break;
//    					case	'2':
////��������ʱ���ַ������Ƚ����λ���ж�ʱ���Ƿ����
//    						if (!GetFileID("TEST0009DOC", &FileDir, &Index))
//    						{
//    							CreateFile("TEST0009DOC");
//    							FileSize = 0;
//    						}
//    						else
//    							FileSize = FileDir.FilePosit.Size+512;
//    							OperateFile(1, "TEST0009DOC", FileSize, 32, old_time);
//    							mmc_GoIdle();
//    						break;
//    					default:	break;
//    					}
//    			}
//    		}
    		SEND_STR_485(PC_TO_DSP);
    		PC_judge=0;
    	}
    	if(DSP_judge)
    	{
    		SEND_STR_232(DSP_TO_PC);
    		DSP_judge=0;
    	}
    }
}

//PC���������ж�
#pragma vector=USCI_A1_VECTOR	//USCI�жϷ�����
__interrupt void USCI_A1_ISR(void)
{
	switch(__even_in_range(UCA1IV,4))
	{
	case 0:break;			//���ж�
	case 2:					//�����жϴ���
		if(UCA1RXBUF=='\n')
		{
			PC_TO_DSP[PC_POS]='\0';
			PC_POS=0;
			PC_judge=1;
		}
		else
		{
			PC_TO_DSP[PC_POS]=UCA1RXBUF;
			PC_POS++;
		}
		break;
	case 4:
		break;			//�����жϲ�����
	default:break;			//��������޲���
	}
}

//DSP���������ж�
#pragma vector=USCI_A0_VECTOR	//USCI�жϷ�����
__interrupt void USCI_A0_ISR(void)
{
	switch(__even_in_range(UCA0IV,4))
	{
	case 0:break;			//���ж�
	case 2:					//�����жϴ���
		if(UCA0RXBUF=='\n')
		{
			DSP_TO_PC[DSP_POS]='\0';
			DSP_POS=0;
			DSP_judge=1;
		}
		else
		{
			DSP_TO_PC[DSP_POS]=UCA0RXBUF;
			DSP_POS++;
		}
		break;
	case 4:
		break;			//�����жϲ�����
	default:break;			//��������޲���
	}
}


#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
	switch(__even_in_range(ADC12IV,36))
	{
		case 6:
    		IRIG();				//����
    		ADC12IFG &= ~BIT0;	//���AD�жϱ�־
			break;
		default:
			break;
	}
}
