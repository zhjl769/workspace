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
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//软件延时，程序中直接调用delay_ms (x),x为需要延时的时间


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
  IO_DIR = 0x00;//数据口切换到输入模式
  CLR_RS;//rs = 0;
  SEL_RW;//rw = 1;
  SEL_EN;//en = 1;
  delay_us(10);//延时10us
  Busy = DAT_IN&0x80; //数据口和10000000相与，D7位若不为0，停在此处
  IO_DIR = 0xFF;//数据口切换到输出模式
  CLR_EN;//en = 0; //若为0跳出进入下一步；这条语句的作用就是检测D7位
  CLR_RW;//rw = 0
  SEL_RS;//rs = 1
  return Busy;
}


void LCD1602_Write(unsigned char i,int j)
{
  while(LCD1602_read_busy());
  DAT_OUT = i;//先设置好需要写入的数据，然后执行写命令
  if (j == 0)   //其中j=0,写指令；i=1，写数据；
	  {
	   CLR_RS;
	  }
  else if (j == 1)
	  {
	   SEL_RS;
	  }      //rs = j;
  CLR_RW;//rw = 0;
  delay_us(10);//延时10us
  SEL_EN;//en = 1;
  delay_us(10);//延时10us;
  CLR_EN;//en = 0;
}


void LCD1602_Read(unsigned char i,int j)
{
	while(LCD1602_read_busy());
  IO_DIR = 0x00;//数据口设置为输入
  if (j == 0)   //其中j=0,读状态字；i=1，读数据；
  	  {
  	   CLR_RS;
  	  }
    else if (j == 1)
  	  {
  	   SEL_RS;
  	  }      //rs = j;
  delay_us(10);//延时10us
  SEL_RW;//rw = 1;
  delay_us(10);//延时10us
  SEL_EN;//en = 1;
  delay_us(10);//延时10us
  DAT_IN = i;//先设置好写命令，然后读取数据
  IO_DIR = 0xFF;//数据口口设置为输出
  CLR_EN;//en = 0;
}


void LCD1602_display_byte(unsigned char y,unsigned char x,unsigned char NOASC) //Y=1,2（起始行）X=1~16（起始列）Z=想写的数值,比如2,3等等
{
    x = x-1;
	if(y == 2) //是否显示在第二行（若在第一行Y=1,不进入IF 语句，若在第二行，进入IF 语句
    {
      x += 0x40; //第二行起始地址加上列数为字符显示地址
    }
  x += 0x80; //设置数据指针位置
  LCD1602_Write(x,0);
  LCD1602_Write(NOASC+0x30,1); //写入数据
}


void LCD1602_display_zi(unsigned char y,unsigned char x,unsigned char a,unsigned char Table[])
//Y=1,2（起始行）X=1~16（起始列）a为自建模数的位数值  Z=想写字符的数组
{
  unsigned int i;
  unsigned char t,z=0;
  x = x-1;
  LCD1602_Write(0x40,0);
  for(i = 0; i < 40 ; i ++ )
  	  {
	  LCD1602_Write(Table[i],1);
  	  }
  if(y == 2) //是否显示在第二行（若在第一行Y=0,不进入IF 语句，若在第二行，进入IF 语句
     {
      x+=0x40; //第二行起始地址加上列数为字符显示地址
     }
      x+=0x80; //设置数据指针位置
      LCD1602_Write(x,0);
   t=a + x; // 求得字符串长度加上起始列位置
  while(x<t) //功能为LCD 显示到字符串最后一个字符，防止字符串没有16个字符，从而不够位产生乱码；
     {
	  LCD1602_Write(z,1); //逐位显示数组内字符
      x++;
      z++;
     }
}


void LCD1602_display_text(unsigned char y,unsigned char x,unsigned char *Table) //Y，X 同上字符显示，table[] 字符串如：“0.001V”
{
   unsigned char z=0;
   unsigned char t;
   t = strlen(Table) + x; // 求得字符串长度加上起始列位置
   while(x<t) //功能为LCD 显示到字符串最后一个字符，防止字符串没有16个字符，从而不够位产生乱码；
        {
         LCD1602_display_byte(y,x,Table[z]-0x30); //逐位显示数组内字符
         x++;
         z++;
        }
}

void LCD1602_init(void) //完全按照要求初始化流程来，中间省略了一步写指令38H
{
  delay_ms(15);
  LCD1602_Write(0x38,0);//8位数据,5*8字形
  delay_ms(5);
  LCD1602_Write(0x38,0);
  delay_ms(10);
  LCD1602_Write(0x38,0);
  LCD1602_Write(0x38,0);
  LCD1602_Write(0x08,0);	//关闭显示
  LCD1602_Write(0x01,0);    //清屏
  LCD1602_Write(0x06,0);	//显示地址递增,即写一个数据后,显示位置右移一位
  LCD1602_Write(0x0C,0);	//开启显示屏,开光标,光标闪烁
}
