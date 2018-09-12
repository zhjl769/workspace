/*
 * IO_Init.c
 *
 *  Created on: 2015-10-17
 *      Author: zhjl769
 */

#include <msp430f1611.h>

void IO_Init(void)
{
   P1DIR = 0X00; //设置P1口方向寄存器P1口作为输入
   P1IES = 0X00; //设置P1口中断边沿选择寄存器，置1为下跳沿，置0为上跳沿
   P1IE = 0XFF; //设置P1中断使能寄存器，置1为允许中断，置0为禁止中断
   P2DIR = 0XFF;//,设置P2为输出。设置P2口方向寄存器，置0为输入，置1为输出
   P2OUT = 0xFF;
   P3DIR = 0xFF; //P3口为输出
   P3SEL |= BIT6+BIT7; //P3口第6、7引脚供外围模块使用
   P4DIR = 0XFF; //设置P4口方向寄存器，使P4口为输出
   P4SEL = 0;
   P4OUT = 0x00;
   P6SEL = 0;
   P6DIR = 0XFF;
   P6OUT = 0X00;//P6.3为高电平。
}
