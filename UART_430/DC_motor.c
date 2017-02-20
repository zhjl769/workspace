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


#define EN1_DIR1   P1DIR |= BIT4  //�趨Motor1��DRV8833оƬʹ�ܶ˿�Ϊ�������
#define EN2_DIR2   P1DIR |= BIT1  //�趨Motor2��DRV8833оƬʹ�ܶ˿�Ϊ�������
#define EN1_ON1    P1OUT |= BIT4  //Motor1��DRV8833оƬʹ��
#define EN2_ON2    P1OUT |= BIT1  //Motor2��DRV8833оƬʹ��
#define EN1_OFF1   P1OUT &= ~BIT4 //Motor1��DRV8833оƬ�ض�
#define EN2_OFF2   P1OUT &= ~BIT1 //Motor2��DRV8833оƬ�ض�


#define PWM1_CCR TA0CCR1   //Motor1�Ķ�Ӧ��CCR�Ĵ���
#define PWM1_CTL TA0CCTL1  //Motor1�Ķ�Ӧ��CCTL�Ĵ���
#define PWM2_CCR TA0CCR2   //Motor2�Ķ�Ӧ��CCR�Ĵ���
#define PWM2_CTL TA0CCTL2  //Motor2�Ķ�Ӧ��CCTL�Ĵ���

uint32_t refClock = 0; //��ת���òο�ʱ�ӣ�Ŀǰ�趨��0.1ms��һ��(��PWM�ز�Ƶ����ͬ)


#define RPM_COUNT 128 //ת�ٲ���ʱ�Զ��ٴ�����������ƽ��

struct RPM_STRUCT //����ת�ٲ���״̬���õĽṹ��
{
  int valid; //��ʼΪ-1��Ϊ1ʱ����Ч
  int forward; //�Ƿ���������ת(Ŀǰʵ�����������ѹ������ȷ����)
  uint32_t lastClkCnt; //��һ����Ч������ֵ�ʱ��
  uint32_t curClkCnt; //���һ����Ч������ֵ�ʱ��(���ڶԶ��������жϹ���)

  int intePtr; //��ɵ�һ�����������ݼ�¼��λ��
  int32_t intervals[RPM_AVERAGE_COUNT]; //���������ݼ�¼
  int32_t inteSum; //���������ݼ�¼֮��
} rpm_meas;



#pragma vector=TIMER0_A0_VECTOR
__interrupt void softTimerInterrupt()
{
  refClock++; //ȫ�ֲο�ʱ������
}

void Mortor_speedset(unsigned char *dis)
{
	unsigned int Speedset[11] = {0,39,80,120,160,200,239,279,319,359,399};//����С�����ٶȶ�Ӧ�������PWMֵ
	switch(dis + 2)
	{
	case 0x01:
		PWM1_CCR = Speedset[dis + 1];break;
	case 0x02:
		PWM2_CCR = Speedset[dis + 1];break;
	}
}


#pragma vector=PORT2_VECTOR
__interrupt void p2Interrupt() //ת�ٲ������ж�
{
	if(((P2IES & BIT5) == BIT5) || ((P2IES & BIT6) == BIT6))//�����ǰ�Ĵ����ö˿�Ϊ�½��أ���������һ������
	{
		if((P2IN & BIT5) == 0x00)//P2.5��ǰΪ�͵�ƽ����ʾ��������P2.5
		{
			if(((P2IES & BIT6) == BIT6))//�����ǰP2.6û�п�ʼ����
			{
				refClock = 0;//refClock����
			    TA0CCTL0 = CCIE;//������ʱ���жϣ�
			    P2IES &= ~BIT5;//��һ�μ��������
			    Flag_ref1 = 1;
			}
			else
			{
				refclock1 = refclock;//��¼��ǰ�õ��ĳ�ʼֵ
				P2IES &= ~BIT5;//��һ�μ��������
				Flag_ref1 = 0;
			}
		}
		if((P2IN & BIT6) == 0x00)//P2.6��ǰΪ�͵�ƽ����ʾ��������P2.6
		{
			if(((P2IES & BIT5) == BIT5))//�����ǰP2.5û�п�ʼ����
			{
				refClock = 0;//refClock����
				TA0CCTL0 = CCIE;//������ʱ���жϣ�
				P2IES &= ~BIT6;//��һ�μ��������
				Flag_ref2 = 1;
			}
			else
			{
				refclock2 = refclock;//��¼��ǰ�õ��ĳ�ʼֵ
				P2IES &= ~BIT6;//��һ�μ��������
				Flag_ref2 = 0;
			}
		}
	}
	else
	{
		if((P2IN & BIT6) == BIT6)//P2.6��ǰΪ�ߵ�ƽ����ʾ��������P2.6
		{
			TA0CCTL0 &= ~CCIE;//�رն�ʱ���жϣ�
			refclock1 = refClock;//��ȡ����ֵ
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
		if((P2IN & BIT5) == BIT5)//P2.5��ǰΪ�ߵ�ƽ����ʾ��������P2.5
				{
					TA0CCTL0 &= ~CCIE;//�رն�ʱ���жϣ�
					refclock2 = refClock;//��ȡ����ֵ
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
  P2IFG = 0; //����жϱ�־
}



int getRPM() //���ݵ�ǰ��ת�ٲ����������ת��ֵ����λΪRPM
{
  if(rpm_meas.valid < 1) //��δ��¼���㹻�����������
    return 0;

  if(refClock - rpm_meas.curClkCnt > 2500L) //250ms����Ӧ�ٶȵ���60rpm����Ϊ�����Ч
  {
    rpm_meas.valid = -1;
    return 0;
  }

  int32_t val = rpm_meas.inteSum; //�������������ֵ���ָı䣬�ȼ�¼ȡֵ
  //����ת�٣���λrpm
  //60s * ÿ��ʱ�Ӽ����(10k) / (������ƽ��ֵ / ÿ��תһȦ�����)
  val = 60L * 1000L * 10L * (RPM_AVERAGE_COUNT / 2) / 4L / val;
  if(rpm_meas.forward)
    return val;
  else
    return -val;
}



