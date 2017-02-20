/*
 * Other.c
 *
 *  Created on: 2015-10-17
 *      Author: zhjl769
 */
#include <msp430f1611.h>


#define CPU_F ((double)8000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//软件延时，程序中直接调用delay_ms (x),x为需要延时的时间



void Delay(long ms)
{
	unsigned int i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<725;j++);//8MHZ
	}

void Delay_us(long us)
{
	unsigned int i,j;
	for(i=0;i<us;i++)
		for(j=0;j<1;j++);//8MHZ
	}


void Breath(void)
{
   unsigned int t,CYCLE=2000,PWM_LOW=0;//定义周期并赋值
   Delay(1000);//特意加延时，可以看到熄灭的过程
  for(PWM_LOW=1;PWM_LOW<CYCLE;PWM_LOW++)
     {                                     //PWM_LOW表示低
                                        //电平时间，这个循环中低电平时长从1累加到CYCLE（周期）的值，即600次
	  P2OUT = 0XFF;  //点亮LED
       t = PWM_LOW;
         {
           while(--t);
          }//延时长度，600次循环中从1加至599
         P2OUT = 0X00; //熄灭LED
       t = CYCLE-PWM_LOW;
         {
           while(--t);
           }//延时长度，600次循环中从599减至1
      }
      P2OUT = 0XFF; //点亮LED
      Delay(1000);
   for(PWM_LOW=CYCLE-1;PWM_LOW>0;PWM_LOW--)
      {                                         //与逐渐变亮相反的过程
	   P2OUT = 0XFF; //点亮LED
	    t = PWM_LOW;
	      {
	        while(--t);
	       }//延时长度，600次循环中从1加至599
	     P2OUT = 0X00;  //熄灭LED
	     t = CYCLE-PWM_LOW;
	          {
	            while(--t);
	            }//延时长度，600次循环中从599减至1
       }
}


int ProcessCMD(char pBuf[],int nLen)
{
    int nTemp = -1;
    //int i;

    if(nLen <= 2) return -1;

	if (nLen == 5)
	{
		if((pBuf[0] == 'A') && (pBuf[1] == 'T')
			&& (pBuf[2] == 'E') && (pBuf[3] == '0'))
			nTemp = 1;
		if((pBuf[0] == 'A') && (pBuf[1] == 'T')
			&& (pBuf[2] == 'E') && (pBuf[3] == '1'))
			nTemp = 2;
	}

    return nTemp;
}



