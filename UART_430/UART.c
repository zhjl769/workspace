/*
 * UART.c
 *
 *  Created on: 2015-10-17
 *      Author: zhjl769
 */
#include <msp430f1611.h>

void UART1_Init(void) //UART1初始化函数
 {
   U1CTL &= ~(SWRST + SSEL1); //USART控制寄存器UCTL1，SWRST=0x01，~SWRST=0xFE，将UCTL0寄存器的第0位复位后，USART才能重新被允许
   U1CTL = 0X00;//USART控制寄存器UCTL1，清零
   U1CTL += CHAR;//数据位为8bit
   U1CTL &= ~SPB;//1位停止位
   U1TCTL |= SSEL1;//发送控制寄存器，第4位置1，选择辅助时钟ACLK1
   U1BR0 = 0x41; //9600波特率，32kHz时钟
   U1BR1 = 0x03;
   U1MCTL = 0x00; //UART波特率调整数
   U1ME |= UTXE1 + URXE1; //模块允许寄存器ME2,设置模块允许寄存器ME1，UTXE0=0x80，设置ME1的第7位为1，//UTXE1=0x80 发送允许//URXE1=0x40 接收允许
   U1IE |= URXIE1; //中断允许寄存器IE2，第5位为1，使能接收中断，URXIE1=0x10
   U1IE |= UTXIE1; //中断允许寄存器IE2，第6位为1，使能发送中断，UTXIE1=0x20
 }


