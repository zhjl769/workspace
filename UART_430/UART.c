/*
 * UART.c
 *
 *  Created on: 2015-10-17
 *      Author: zhjl769
 */
#include <msp430f1611.h>

void UART1_Init(void) //UART1��ʼ������
 {
   U1CTL &= ~(SWRST + SSEL1); //USART���ƼĴ���UCTL1��SWRST=0x01��~SWRST=0xFE����UCTL0�Ĵ����ĵ�0λ��λ��USART�������±�����
   U1CTL = 0X00;//USART���ƼĴ���UCTL1������
   U1CTL += CHAR;//����λΪ8bit
   U1CTL &= ~SPB;//1λֹͣλ
   U1TCTL |= SSEL1;//���Ϳ��ƼĴ�������4λ��1��ѡ����ʱ��ACLK1
   U1BR0 = 0x41; //9600�����ʣ�32kHzʱ��
   U1BR1 = 0x03;
   U1MCTL = 0x00; //UART�����ʵ�����
   U1ME |= UTXE1 + URXE1; //ģ������Ĵ���ME2,����ģ������Ĵ���ME1��UTXE0=0x80������ME1�ĵ�7λΪ1��//UTXE1=0x80 ��������//URXE1=0x40 ��������
   U1IE |= URXIE1; //�ж�����Ĵ���IE2����5λΪ1��ʹ�ܽ����жϣ�URXIE1=0x10
   U1IE |= UTXIE1; //�ж�����Ĵ���IE2����6λΪ1��ʹ�ܷ����жϣ�UTXIE1=0x20
 }


