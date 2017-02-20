/*
 * Other.c
 *
 *  Created on: 2015-10-17
 *      Author: zhjl769
 */
#include <msp430f1611.h>


#define CPU_F ((double)8000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//�����ʱ��������ֱ�ӵ���delay_ms (x),xΪ��Ҫ��ʱ��ʱ��



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
   unsigned int t,CYCLE=2000,PWM_LOW=0;//�������ڲ���ֵ
   Delay(1000);//�������ʱ�����Կ���Ϩ��Ĺ���
  for(PWM_LOW=1;PWM_LOW<CYCLE;PWM_LOW++)
     {                                     //PWM_LOW��ʾ��
                                        //��ƽʱ�䣬���ѭ���е͵�ƽʱ����1�ۼӵ�CYCLE�����ڣ���ֵ����600��
	  P2OUT = 0XFF;  //����LED
       t = PWM_LOW;
         {
           while(--t);
          }//��ʱ���ȣ�600��ѭ���д�1����599
         P2OUT = 0X00; //Ϩ��LED
       t = CYCLE-PWM_LOW;
         {
           while(--t);
           }//��ʱ���ȣ�600��ѭ���д�599����1
      }
      P2OUT = 0XFF; //����LED
      Delay(1000);
   for(PWM_LOW=CYCLE-1;PWM_LOW>0;PWM_LOW--)
      {                                         //���𽥱����෴�Ĺ���
	   P2OUT = 0XFF; //����LED
	    t = PWM_LOW;
	      {
	        while(--t);
	       }//��ʱ���ȣ�600��ѭ���д�1����599
	     P2OUT = 0X00;  //Ϩ��LED
	     t = CYCLE-PWM_LOW;
	          {
	            while(--t);
	            }//��ʱ���ȣ�600��ѭ���д�599����1
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



