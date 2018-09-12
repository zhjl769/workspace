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
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//软件延时，程序中直接调用delay_ms (x),x为需要延时的时间

void main( ) //主函数
 {
   WDTCTL = WDTPW + WDTHOLD; //关看门狗
   IO_Init();//IO口初始化
   Init_CLK();//时钟初始化
//   UART1_Init(); //调用UART1初始化函数
   Init_TimerA();//调用TimerA初始化函数
   LCD1602_init();//LCD1602初始化函数
   P2OUT |= BIT0;
   delay_ms(200);
   P2OUT &= ~BIT0;
   Num = 8;
   //_BIS_SR(LPM1_bits); //调用C编译器内部对状态寄存器某位置位的函数，
    //LPM_bits=SCG0+CPUOFF，
    // SCG0=0x0040，进入LPM1低功耗工作模式
    // CPUOFF=0x0010 关闭CPU，唤醒所有允许的中断
  // _nop(); //调用C编译器内部空操作函数
  // 进入处理循环
    P2IE |= BIT5;//开启p2.5的中断
    delay_ms(10);
    _EINT(); //调用C编译器内部函数使能中断
   // delay_ms(20);
    while(1)
     {
    	if(Point == Len)
    	{
    		P2IE &= ~BIT5;//关闭P2.5的中断，
    		TA0CTL |=  MC_0 ;//关闭定时器
    		TA0CCTL2 |= CM_0;//关闭捕获
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
    			     temp = dis[i];//找数组中的最大值
    			   }
    			 if(dis[i] < temp1)
    			   {
    			    temp1 = dis[i];//找数组中的最小值
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
    		P2IE |= BIT5;//	开启P2.5的中断，
    	}
    	_nop();
    	_nop();
    	_nop();
     }
 }



#pragma vector = PORT2_VECTOR
__interrupt void P2Interrupt() //输入信号的端口触发中断测量
{
   P2IE &= ~BIT5;//关闭P2.5的中断
  if((P2IFG & BIT5) == BIT5)
   {
   // TA0CCR0 = 0xfff0;//65520
   // TA0CCTL0 |= CCIE; //AO开启中断
    TA0CCTL2 |= CM_2;//开启捕获中断加上升沿捕获
    TA0CTL |=  MC_2;//连续计数模式.开启定时器
    P2IFG &= ~BIT5; //清除中断标志
    refClock  = 0;//清零
    Tip = 1;//标志位置1
   }
}


#pragma vector = TIMERA1_VECTOR
__interrupt void Capture2Interrupt()//捕获模式下的中断
{
 unsigned long Time_temp1,Time;
 switch(TAIV)
 {
 case 4:
	 if(Tip == 1)
	 {
      TA0CCTL2 |= CM_0;//关闭捕获
      //TA0CCR0 = 0;//关闭定时器A0
     // _nop();
      TA0CTL |=  MC_0 ;//关闭定时器A0
      TAR = 0x00;//计数器清零
     // TA0CCTL0 &= ~CCIE; //关闭AO的比较中断
     // TA0CCTL2 &= ~CCIE;//关闭A2的比较中断
      TA0CCTL2 &= ~COV;//清除A2的捕获标志位
      TA0CCTL2 &= ~CCIFG;//清除捕获模式下的中断
      Time_temp1 = TA0CCR2;
      TA0CCR2 = 0x00;//A2清零
      Time = refClock*65536 + Time_temp1;
      dis[Point] = Time;
      Point++;
      Tip = 0;//标志位置0
      P2IE |= BIT5;//开启P2.5的中断
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
	 refClock = refClock + 1; //全局参考时钟自增
	 break;
   }
}

