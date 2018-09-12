#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Init.h"
#include "LCD1602.h"
#include "Variable.h"
#include "Other.h"

#define CPU_F ((double)8000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//�����ʱ��������ֱ�ӵ���delay_ms (x),xΪ��Ҫ��ʱ��ʱ��

void main( ) //������
 {
   WDTCTL = WDTPW + WDTHOLD; //�ؿ��Ź�
   IO_Init();//IO�ڳ�ʼ��
   Init_CLK();//ʱ�ӳ�ʼ��
//   UART1_Init(); //����UART1��ʼ������
   Init_TimerA();//����TimerA��ʼ������
   LCD1602_init();//LCD1602��ʼ������
   P2OUT |= BIT0;
   delay_ms(200);
   P2OUT &= ~BIT0;
   Num = 8;
   //_BIS_SR(LPM1_bits); //����C�������ڲ���״̬�Ĵ���ĳλ��λ�ĺ�����
    //LPM_bits=SCG0+CPUOFF��
    // SCG0=0x0040������LPM1�͹��Ĺ���ģʽ
    // CPUOFF=0x0010 �ر�CPU����������������ж�
  // _nop(); //����C�������ڲ��ղ�������
  // ���봦��ѭ��
    P2IE |= BIT5;//����p2.5���ж�
    delay_ms(10);
    _EINT(); //����C�������ڲ�����ʹ���ж�
   // delay_ms(20);
    while(1)
     {
    	if(Point == Len)
    	{
    		P2IE &= ~BIT5;//�ر�P2.5���жϣ�
    		TA0CTL |=  MC_0 ;//�رն�ʱ��
    		TA0CCTL2 |= CM_0;//�رղ���
    		Point = 0;
    		i = 0;
    		j = 0;
    		for(i = 0 ;i < Len;i++)
    		{
    		if(abs(dis[i+2] - dis[i]) <= 50*Num)
    		  {
    		   temp = dis[i];
    		   temp1 = dis[i];
    		   j =j + 1;
    		   temp2 += dis[i];
    		   break;
    		  }
    		}
    //		temp2 = dis[0];

    		for(i = i + 1 ;i < Len;i++)
    		{
    			if(abs(dis[i+1] - dis[i]) <= 50*Num)
    			{
    			 temp2 += dis[i];
    			 j = j + 1;
    			 if(dis[i] > temp)
    			   {
    			     temp = dis[i];//�������е����ֵ
    			   }
    			 if(dis[i] < temp1)
    			   {
    			    temp1 = dis[i];//�������е���Сֵ
    			   }
    			}
    		}
    		temp2 = temp2/j;
    		if(temp > temp2)
    		{
    		 dif_positive = (temp - temp2)/Num;
    		}
    		else
    		{
    		dif_positive = (temp2 - temp)/Num;
    		}
    		if(temp1 > temp2)
    		 {
    		  dif_negative = (temp1 - temp2)/Num;
    		 }
    		else
    		 {
    		  dif_negative = (temp2 - temp1)/Num;
    		 }

    		temp2 = temp2/Num;
    		seven = (temp2/1000000);
    		six = temp2%1000000/100000;
    		five = temp2%1000000%100000/10000;
    		four = temp2%1000000%100000%10000/1000;
    		three = temp2%1000000%100000%10000%10000/1000;
    		two = temp2%1000000%100000%10000%1000%100/10;
    		one = temp2%1000000%100000%10000%1000%100%10/1;
    		LCD1602_display_byte(2,1,seven);
    		LCD1602_display_byte(2,2,six);
    		LCD1602_display_byte(2,3,five);
    		LCD1602_display_byte(2,4,four);
    		LCD1602_display_byte(2,5,three);
    		LCD1602_display_byte(2,6,two);
    		LCD1602_display_byte(2,7,one);
    		five = dif_positive%1000000%100000/10000;
    		four = dif_positive%1000000%100000%10000/1000;
    		three = dif_positive%1000000%100000%10000%10000/1000;
    		two = dif_positive%1000000%100000%10000%1000%100/10;
    		one = dif_positive%1000000%100000%10000%1000%100%10/1;
    		LCD1602_display_text(1,1,dis1);
    		LCD1602_display_byte(1,3,five);
    		LCD1602_display_byte(1,4,four);
    		LCD1602_display_byte(1,5,three);
    		LCD1602_display_byte(1,6,two);
    		LCD1602_display_byte(1,7,one);
    		five = dif_negative%1000000%100000/10000;
    		four = dif_negative%1000000%100000%10000/1000;
    		three = dif_negative%1000000%100000%10000%10000/1000;
    		two = dif_negative%1000000%100000%10000%1000%100/10;
    		one = dif_negative%1000000%100000%10000%1000%100%10/1;
    		LCD1602_display_text(1,9,dis2);
    		LCD1602_display_byte(1,11,five);
    		LCD1602_display_byte(1,12,four);
    		LCD1602_display_byte(1,13,three);
    		LCD1602_display_byte(1,14,two);
    		LCD1602_display_byte(1,15,one);
    		_nop();
    		P2IE |= BIT5;//	����P2.5���жϣ�
    	}
    	_nop();
    	_nop();
    	_nop();
     }
 }



#pragma vector = PORT2_VECTOR
__interrupt void P2Interrupt() //�����źŵĶ˿ڴ����жϲ���
{
   P2IE &= ~BIT5;//�ر�P2.5���ж�
  if((P2IFG & BIT5) == BIT5)
   {
   // TA0CCR0 = 0xfff0;//65520
   // TA0CCTL0 |= CCIE; //AO�����ж�
    TA0CCTL2 |= CM_2;//���������жϼ������ز���
    TA0CTL |=  MC_2;//��������ģʽ.������ʱ��
    P2IFG &= ~BIT5; //����жϱ�־
    refClock  = 0;//����
    Tip = 1;//��־λ��1
   }
}


#pragma vector = TIMERA1_VECTOR
__interrupt void Capture2Interrupt()//����ģʽ�µ��ж�
{
 unsigned long Time_temp1,Time;
 switch(TAIV)
 {
 case 4:
	 if(Tip == 1)
	 {
      TA0CCTL2 |= CM_0;//�رղ���
      //TA0CCR0 = 0;//�رն�ʱ��A0
     // _nop();
      TA0CTL |=  MC_0 ;//�رն�ʱ��A0
      TAR = 0x00;//����������
     // TA0CCTL0 &= ~CCIE; //�ر�AO�ıȽ��ж�
     // TA0CCTL2 &= ~CCIE;//�ر�A2�ıȽ��ж�
      TA0CCTL2 &= ~COV;//���A2�Ĳ����־λ
      TA0CCTL2 &= ~CCIFG;//�������ģʽ�µ��ж�
      Time_temp1 = TA0CCR2;
      TA0CCR2 = 0x00;//A2����
      Time = refClock*65536 + Time_temp1;
      dis[Point] = Time;
      Point++;
      Tip = 0;//��־λ��0
      P2IE |= BIT5;//����P2.5���ж�
      break;
	 }
 }
}

#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A0()
{
  switch(TAIV)
   {
	 case 10:
	 refClock = refClock + 1; //ȫ�ֲο�ʱ������
	 break;
   }
}

