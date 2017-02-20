/*
 * UART.c
 *
 *  Created on: 2015-12-10
 *      Author: zhjl769
 */
#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "LCD1602.h"
#include "DS18B20.h"
#include "ADC12.h"
#include "Interrupt.h"



// �������Դ��� 1 �Ľ����ж�
#pragma vector = USART1RX_VECTOR
__interrupt void UART1_RX (void) //�����ж�
{
  RecBuf[Rindex] = RXBUF1;
  if(RecBuf[0] == 0xff)//֡ͷ��ȷ,��������
    {
      Rindex++;
    }
  else
    {
	  Rindex = 0;
    }
  if((RecBuf[Rindex-1] == 0xff) && (Rindex > 2)) //�������
    {
//     SendLen = Rindex;
     RDat = RecBuf;
     switch(RecBuf[1])
     {
      case 0xef:
    	 switch (RecBuf[2])
    	 {
    	  case 0xef:
    		 switch (RecBuf[3])
    	     {
    	      case 0xee:
    	    	   temperature_get();
    	           Voltage_get();
    	           Flag_get = 1;
    	           break;
    	     }break;
    	 } break;
//      case 0x02:Mortor_speedset(*RDat);break;

     }
//     if(RecBuf[1] == 0xee && RecBuf[2] == 0xee && RecBuf[2] == 0xee)
//       {
//    	 Flag_get = 1;
//        }
//     memset(RecBuf,0,Rindex-1);
     RecBuf[0]  = 0x00;//�������ݰ�ͷ���㣬��ֹӰ����һ���ݰ��Ľ���
     RecBuf[Rindex-1] = 0x00;//�������ݰ�β���㣬��ֹӰ����һ���ݰ��Ľ���
     Rindex = 0;
//     IFG2 |= UTXIFG1; // �����жϱ�־�����뷢���жϳ���
   }
}


#pragma vector = USART1TX_VECTOR
__interrupt void UART1_TX (void) //�����ж�
{
      if((Tindex < SendLen) && (*(SDat) == 0xff) && (*(SDat + 4) == 0xff))
      {
         TXBUF1 = *(SDat+Tindex);
          Tindex++;
      }
      else
      {
          Tindex=0;
      }
}


void temperature_get(void)
{
 unsigned int qian,bai,shi,ge;
 TPT = TPT*100;
 qian = (unsigned char) (TPT/1000);
 bai = (unsigned char) ((TPT - qian*1000)/100);
 shi = (unsigned char) (((TPT - qian*1000)-bai*100)/10);
 ge = (unsigned char) ((((TPT - qian*1000)-bai*100)-shi*10));
 LCD1602_display_text(1,1,dis1); // ��ʾһ���ַ�//Y=1,2����ʼ�У�X=1~16����ʼ�У�  Z=��д�ַ�������
 LCD1602_display_text(1,4,dis2); // ��ʾһ���ַ�//Y=1,2����ʼ�У�X=1~16����ʼ�У�Z=��д�ַ�������
 LCD1602_display_zi(1,9,1,dis3); // ��ʾһ���ַ�//Y=1,2����ʼ�У�X=1~16����ʼ�У�aΪ�Խ�ģ����λ��ֵ  Z=��д�ַ�������
 LCD1602_display_byte(1,4,qian);
 LCD1602_display_byte(1,5,bai);
 LCD1602_display_byte(1,7,shi);
 LCD1602_display_byte(1,8,ge);
 TPT = qian*10+bai;
 memset(TecBuf,0,10);
 memset(TecBuf,0xff,5);
 TecBuf[1] = 0x63;
 TecBuf[2] = TPT;
 TecBuf[3] = 0x00;
 SDat = TecBuf;
 IFG2 |= UTXIFG1; // �����жϱ�־�����뷢���жϳ���
}

void Voltage_get(void)
{
 unsigned int Vo,bai,shi,ge;
 Voltage = Voltage*100;
 Vo = Voltage;
 bai = (unsigned char) (Voltage/100);
 shi = (unsigned char) ((Voltage - bai*100)/10);
 ge = (unsigned char) ((Voltage - bai*100)-shi*10);
// LCD1602_Write(0x01,0);//LCD1602����
 LCD1602_display_text(2,1,dis_v1); // ��ʾһ���ַ�//Y=1,2����ʼ�У�X=1~16����ʼ�У�  Z=��д�ַ�������
 LCD1602_display_text(2,4,dis_v2); // ��ʾһ���ַ�//Y=1,2����ʼ�У�X=1~16����ʼ�У�Z=��д�ַ�������
 LCD1602_display_byte(2,5,bai);
 LCD1602_display_byte(2,7,shi);
 LCD1602_display_byte(2,8,ge);
 Vo = (-(300 - Vo)/20);
 memset(TecBuf,0,10);
 memset(TecBuf,0xff,5);
 TecBuf[1] = 0x60;
 TecBuf[2] = Vo;
 TecBuf[3] = 0x00;
 SDat = TecBuf;
 IFG2 |= UTXIFG1; // �����жϱ�־�����뷢���жϳ���
}

