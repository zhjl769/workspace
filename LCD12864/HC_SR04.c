/*
 * HC_SR04.c
 *
 *  Created on: 2016-3-31
 *      Author: zhjl769
 */


#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "HC-SR04.h"
#include "LCD12864.h"

#define CPU_F ((double)8000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//软件延时，程序中直接调用delay_ms (x),x为需要延时的时间


#define	  SCL_OUT      P5DIR |=  BIT0 //IIC时钟引脚定义输出，P5.0
#define	  SCL_OUT_L    P5OUT &= ~BIT0 //IIC时钟引脚定义低电平，P5.0
#define	  SCL_OUT_H    P5OUT |=  BIT0 //IIC时钟引脚定义高电平，P5.0

#define   SDA_DATA_IN  P5IN & BIT1    //SDA_DAT_IN,单片机数据口输入数据,P5.1
#define	  SDA_IN       P5DIR &= ~BIT1 //IIC数据引脚定义输入，P5.1
#define	  SDA_OUT      P5DIR |=  BIT1 //IIC数据引脚定义输出，P5.1
#define	  SDA_OUT_L    P5OUT &= ~BIT1 //IIC数据引脚定义输出低电平，P5.1
#define	  SDA_OUT_H    P5OUT |=  BIT1 //IIC数据引脚定义输出高电平，P5.1
#define	  SDA_IN_L     P5IN  &= ~BIT1 //IIC数据引脚定义输入低电平，P5.1
#define	  SDA_IN_H     P5IN  |=  BIT1 //IIC数据引脚定义输入高电平，P5.1



