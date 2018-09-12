/*
 * LCD1602.c
 *
 *  Created on: 2015-10-17
 *      Author: zhjl769
 */
#include <msp430f1611.h>
#include "LCD1602.h"


#define CPU_F ((double)8000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//�����ʱ��������ֱ�ӵ���delay_ms (x),xΪ��Ҫ��ʱ��ʱ��


#define CLR_RS P3OUT &= ~BIT2// rs = 0
#define SEL_RS P3OUT |= BIT2// rs = 1
#define CLR_RW P3OUT &= ~BIT1//rw = 0
#define SEL_RW P3OUT |= BIT1//rw = 1
#define CLR_EN P3OUT &= ~BIT0//en = 0
#define SEL_EN P3OUT |= BIT0//en = 1
#define IO_DIR P4DIR
#define DAT_OUT P4OUT
#define DAT_IN  P4IN



int LCD1602_read_busy()
{
  int Busy;
  DAT_OUT = 0xFF;
  IO_DIR = 0x00;//���ݿ��л�������ģʽ
  CLR_RS;//rs = 0;
  SEL_RW;//rw = 1;
  SEL_EN;//en = 1;
  delay_us(10);//��ʱ10us
  Busy = DAT_IN&0x80; //���ݿں�10000000���룬D7λ����Ϊ0��ͣ�ڴ˴�
  IO_DIR = 0xFF;//���ݿ��л������ģʽ
  CLR_EN;//en = 0; //��Ϊ0����������һ���������������þ��Ǽ��D7λ
  CLR_RW;//rw = 0
  SEL_RS;//rs = 1
  return Busy;
}


void LCD1602_Write(unsigned char i,int j)
{
  while(LCD1602_read_busy());
  DAT_OUT = i;//�����ú���Ҫд������ݣ�Ȼ��ִ��д����
  if (j == 0)   //����j=0,дָ�i=1��д���ݣ�
	  {
	   CLR_RS;
	  }
  else if (j == 1)
	  {
	   SEL_RS;
	  }      //rs = j;
  CLR_RW;//rw = 0;
  delay_us(10);//��ʱ10us
  SEL_EN;//en = 1;
  delay_us(10);//��ʱ10us;
  CLR_EN;//en = 0;
}


void LCD1602_Read(unsigned char i,int j)
{
	while(LCD1602_read_busy());
  IO_DIR = 0x00;//���ݿ�����Ϊ����
  if (j == 0)   //����j=0,��״̬�֣�i=1�������ݣ�
  	  {
  	   CLR_RS;
  	  }
    else if (j == 1)
  	  {
  	   SEL_RS;
  	  }      //rs = j;
  delay_us(10);//��ʱ10us
  SEL_RW;//rw = 1;
  delay_us(10);//��ʱ10us
  SEL_EN;//en = 1;
  delay_us(10);//��ʱ10us
  DAT_IN = i;//�����ú�д���Ȼ���ȡ����
  IO_DIR = 0xFF;//���ݿڿ�����Ϊ���
  CLR_EN;//en = 0;
}


void LCD1602_display_byte(unsigned char y,unsigned char x,unsigned char NOASC) //Y=1,2����ʼ�У�X=1~16����ʼ�У�Z=��д����ֵ,����2,3�ȵ�
{
    x = x-1;
	if(y == 2) //�Ƿ���ʾ�ڵڶ��У����ڵ�һ��Y=1,������IF ��䣬���ڵڶ��У�����IF ���
    {
      x += 0x40; //�ڶ�����ʼ��ַ��������Ϊ�ַ���ʾ��ַ
    }
  x += 0x80; //��������ָ��λ��
  LCD1602_Write(x,0);
  LCD1602_Write(NOASC+0x30,1); //д������
}


void LCD1602_display_zi(unsigned char y,unsigned char x,unsigned char a,unsigned char Table[])
//Y=1,2����ʼ�У�X=1~16����ʼ�У�aΪ�Խ�ģ����λ��ֵ  Z=��д�ַ�������
{
  unsigned int i;
  unsigned char t,z=0;
  x = x-1;
  LCD1602_Write(0x40,0);
  for(i = 0; i < 40 ; i ++ )
  	  {
	  LCD1602_Write(Table[i],1);
  	  }
  if(y == 2) //�Ƿ���ʾ�ڵڶ��У����ڵ�һ��Y=0,������IF ��䣬���ڵڶ��У�����IF ���
     {
      x+=0x40; //�ڶ�����ʼ��ַ��������Ϊ�ַ���ʾ��ַ
     }
      x+=0x80; //��������ָ��λ��
      LCD1602_Write(x,0);
   t=a + x; // ����ַ������ȼ�����ʼ��λ��
  while(x<t) //����ΪLCD ��ʾ���ַ������һ���ַ�����ֹ�ַ���û��16���ַ����Ӷ�����λ�������룻
     {
	  LCD1602_Write(z,1); //��λ��ʾ�������ַ�
      x++;
      z++;
     }
}


void LCD1602_display_text(unsigned char y,unsigned char x,unsigned char *Table) //Y��X ͬ���ַ���ʾ��table[] �ַ����磺��0.001V��
{
   unsigned char z=0;
   unsigned char t;
   t = strlen(Table) + x; // ����ַ������ȼ�����ʼ��λ��
   while(x<t) //����ΪLCD ��ʾ���ַ������һ���ַ�����ֹ�ַ���û��16���ַ����Ӷ�����λ�������룻
        {
         LCD1602_display_byte(y,x,Table[z]-0x30); //��λ��ʾ�������ַ�
         x++;
         z++;
        }
}

void LCD1602_init(void) //��ȫ����Ҫ���ʼ�����������м�ʡ����һ��дָ��38H
{
  delay_ms(15);
  LCD1602_Write(0x38,0);//8λ����,5*8����
  delay_ms(5);
  LCD1602_Write(0x38,0);
  delay_ms(10);
  LCD1602_Write(0x38,0);
  LCD1602_Write(0x38,0);
  LCD1602_Write(0x08,0);	//�ر���ʾ
  LCD1602_Write(0x01,0);    //����
  LCD1602_Write(0x06,0);	//��ʾ��ַ����,��дһ�����ݺ�,��ʾλ������һλ
  LCD1602_Write(0x0C,0);	//������ʾ��,�����,�����˸
}
