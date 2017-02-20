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
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//�����ʱ��������ֱ�ӵ���delay_ms (x),xΪ��Ҫ��ʱ��ʱ��


#define	  SCL_OUT      P5DIR |=  BIT0 //IICʱ�����Ŷ��������P5.0
#define	  SCL_OUT_L    P5OUT &= ~BIT0 //IICʱ�����Ŷ���͵�ƽ��P5.0
#define	  SCL_OUT_H    P5OUT |=  BIT0 //IICʱ�����Ŷ���ߵ�ƽ��P5.0

#define   SDA_DATA_IN  P5IN & BIT1    //SDA_DAT_IN,��Ƭ�����ݿ���������,P5.1
#define	  SDA_IN       P5DIR &= ~BIT1 //IIC�������Ŷ������룬P5.1
#define	  SDA_OUT      P5DIR |=  BIT1 //IIC�������Ŷ��������P5.1
#define	  SDA_OUT_L    P5OUT &= ~BIT1 //IIC�������Ŷ�������͵�ƽ��P5.1
#define	  SDA_OUT_H    P5OUT |=  BIT1 //IIC�������Ŷ�������ߵ�ƽ��P5.1
#define	  SDA_IN_L     P5IN  &= ~BIT1 //IIC�������Ŷ�������͵�ƽ��P5.1
#define	  SDA_IN_H     P5IN  |=  BIT1 //IIC�������Ŷ�������ߵ�ƽ��P5.1



