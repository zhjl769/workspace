/*
 * DC_motor.c
 *
 *  Created on: 2015-12-11
 *      Author: zhjl769
 */
#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <in430.h>
#include "DC_motor.h"


#define EN1_DIR1   P1DIR |= BIT4  //设定Motor1的DRV8833芯片使能端口为输出方向
#define EN2_DIR2   P1DIR |= BIT1  //设定Motor2的DRV8833芯片使能端口为输出方向
#define EN1_ON1    P1OUT |= BIT4  //Motor1的DRV8833芯片使能
#define EN2_ON2    P1OUT |= BIT1  //Motor2的DRV8833芯片使能
#define EN1_OFF1   P1OUT &= ~BIT4 //Motor1的DRV8833芯片关断
#define EN2_OFF2   P1OUT &= ~BIT1 //Motor2的DRV8833芯片关断


#define PWM1_CCR TA0CCR1   //Motor1的对应的CCR寄存器
#define PWM1_CTL TA0CCTL1  //Motor1的对应的CCTL寄存器
#define PWM2_CCR TA0CCR2   //Motor2的对应的CCR寄存器
#define PWM2_CTL TA0CCTL2  //Motor2的对应的CCTL寄存器

uint32_t refClock = 0; //测转速用参考时钟，目前设定下0.1ms走一次(与PWM载波频率相同)


#define RPM_COUNT 128 //转速测量时对多少次脉冲间隔进行平均

struct RPM_STRUCT //保存转速测量状态所用的结构体
{
  int valid; //初始为-1，为1时才有效
  int forward; //是否是正向旋转(目前实际是由输出电压极性来确定的)
  uint32_t lastClkCnt; //上一次有效脉冲出现的时间
  uint32_t curClkCnt; //最近一次有效脉冲出现的时间(用于对抖动进行判断过滤)

  int intePtr; //最旧的一次脉冲间隔数据记录的位置
  int32_t intervals[RPM_AVERAGE_COUNT]; //脉冲间隔数据记录
  int32_t inteSum; //脉冲间隔数据记录之和
} rpm_meas;



#pragma vector=TIMER0_A0_VECTOR
__interrupt void softTimerInterrupt()
{
  refClock++; //全局参考时钟自增
}

void Mortor_speedset(unsigned char *dis)
{
	unsigned int Speedset[11] = {0,39,80,120,160,200,239,279,319,359,399};//定义小车的速度对应的输出的PWM值
	switch(dis + 2)
	{
	case 0x01:
		PWM1_CCR = Speedset[dis + 1];break;
	case 0x02:
		PWM2_CCR = Speedset[dis + 1];break;
	}
}


#pragma vector=PORT2_VECTOR
__interrupt void p2Interrupt() //转速测量用中断
{
	if(((P2IES & BIT5) == BIT5) || ((P2IES & BIT6) == BIT6))//如果当前的触发该端口为下降沿，两个任意一个成立
	{
		if((P2IN & BIT5) == 0x00)//P2.5当前为低电平，表示触发的是P2.5
		{
			if(((P2IES & BIT6) == BIT6))//如果当前P2.6没有开始计数
			{
				refClock = 0;//refClock清零
			    TA0CCTL0 = CCIE;//开启定时器中断，
			    P2IES &= ~BIT5;//下一次检测上升沿
			    Flag_ref1 = 1;
			}
			else
			{
				refclock1 = refclock;//记录当前得到的初始值
				P2IES &= ~BIT5;//下一次检测上升沿
				Flag_ref1 = 0;
			}
		}
		if((P2IN & BIT6) == 0x00)//P2.6当前为低电平，表示触发的是P2.6
		{
			if(((P2IES & BIT5) == BIT5))//如果当前P2.5没有开始计数
			{
				refClock = 0;//refClock清零
				TA0CCTL0 = CCIE;//开启定时器中断，
				P2IES &= ~BIT6;//下一次检测上升沿
				Flag_ref2 = 1;
			}
			else
			{
				refclock2 = refclock;//记录当前得到的初始值
				P2IES &= ~BIT6;//下一次检测上升沿
				Flag_ref2 = 0;
			}
		}
	}
	else
	{
		if((P2IN & BIT6) == BIT6)//P2.6当前为高电平，表示触发的是P2.6
		{
			TA0CCTL0 &= ~CCIE;//关闭定时器中断，
			refclock1 = refClock;//获取计数值
			if(Flag_ref1 = 1)
			{
				Speed1[i] = refclock1;
				i++;
			}
			else
			{
				Speed1[i] = refclock1 - refclock1;
				i++;
			}
		}
		if((P2IN & BIT5) == BIT5)//P2.5当前为高电平，表示触发的是P2.5
				{
					TA0CCTL0 &= ~CCIE;//关闭定时器中断，
					refclock2 = refClock;//获取计数值
					if(Flag_ref2 = 1)
					{
						Speed2[i] = refclock2;
						if(i < clocknum)
						{
						 i++;
						}
						else
						{
							i =0;
						}
					}
					else
					{
						Speed2[i] = refclock2 - refclock2;
						if(i < clocknum)
						{
							i++;
						}
						else
						{
							i =0;
						}
					}
				}
	}
  P2IFG = 0; //清除中断标志
}



int getRPM() //根据当前的转速测量情况计算转速值，单位为RPM
{
  if(rpm_meas.valid < 1) //还未记录到足够计算的脉冲数
    return 0;

  if(refClock - rpm_meas.curClkCnt > 2500L) //250ms，对应速度低于60rpm，认为结果无效
  {
    rpm_meas.valid = -1;
    return 0;
  }

  int32_t val = rpm_meas.inteSum; //避免运算过程中值出现改变，先记录取值
  //计算转速，单位rpm
  //60s * 每秒时钟间隔数(10k) / (脉冲间隔平均值 / 每旋转一圈间隔数)
  val = 60L * 1000L * 10L * (RPM_AVERAGE_COUNT / 2) / 4L / val;
  if(rpm_meas.forward)
    return val;
  else
    return -val;
}



