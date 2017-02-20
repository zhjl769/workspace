#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Variable.h"
#include "Other.h"
#include "Init.h"
#include "LCD1602.h"
#include "ADC12.h"
#include "DS18B20.h"



void main( ) //������
 {
   WDTCTL = WDTPW + WDTHOLD; //�ؿ��Ź�
   IO_Init();//IO�ڳ�ʼ��
   Init_CLK();//ʱ�ӳ�ʼ��
   Init_ADC();//ADC12��ʼ��
   UART1_Init(); //����UART1��ʼ������
//   Init_TimerA();//����TimerA��ʼ������
   LCD1602_init();//LCD1602��ʼ������
   P1OUT |= BIT1 ;//+ BIT2;
   P1OUT &= ~BIT3+ ~BIT4;
   P2OUT |= BIT0;
   Delay(2000);
   P2OUT &= ~BIT0;
    _EINT(); //����C�������ڲ�����ʹ���ж�
   //_BIS_SR(LPM1_bits); //����C�������ڲ���״̬�Ĵ���ĳλ��λ�ĺ�����
    //LPM_bits=SCG0+CPUOFF��
    // SCG0=0x0040������LPM1�͹��Ĺ���ģʽ
    // CPUOFF=0x0010 �ر�CPU����������������ж�
   //_NOP(); //����C�������ڲ��ղ�������
  // ���봦��ѭ��
    while(1)
     {
    	//Breath();
    	ADC12_Start();//��ȡ��ѹֵ
    	get_temperature();//��ȡ�¶�ֵ

     }
 }






