/*
 * uart.c
 *
 *  Created on: 2016Äê3ÔÂ21ÈÕ
 *      Author: zane
 */
#include"msp430f6638.h"
#include"main.h"

void SEND_BYTE_232(unsigned char send)
{
	while(!(UCA1IFG&UCTXIFG));
		UCA1TXBUF=send;
}

void SEND_STR_232(unsigned char *pos)
{
	while(*pos != '\0')
	{
		SEND_BYTE_232(*pos++);
	}
}

void SEND_BYTE_485(unsigned char send)
{
	while(!(UCA0IFG&UCTXIFG));
		UCA0TXBUF=send;
}

void SEND_STR_485(unsigned char *pos)
{
	while(*pos != '\0')
	{
		SEND_BYTE_485(*pos++);
	}
}


