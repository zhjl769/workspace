/*
 * LCD12864.c
 *
 *  Created on: 2016-2-25
 *      Author: zhjl769
 */
#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "LCD12864.h"

#define CPU_F ((double)8000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//软件延时，程序中直接调用delay_ms (x),x为需要延时的时间


#define CLR_CS  P3OUT &= ~BIT0    //CS = 0,液晶片选=0,P3.0
#define SEL_CS  P3OUT |=  BIT0    //CS = 1,液晶片选=1,P3.0
#define CLR_SID P3OUT &= ~BIT1    //SID = 0,液晶数据口=0,P3.1
#define SEL_SID P3OUT |=  BIT1    //SID = 1,液晶数据口=1,P3.1
#define CLR_CLK P3OUT &= ~BIT2    //CLR_CLK,液晶时钟=0,P3.2
#define SEL_CLK P3OUT |=  BIT2    //SEL_CLK,液晶时钟=1,P3.2

#define SID_IN  P3DIR &= ~BIT1    //SID_IN,单片机数据口输入方向_0,P3.1
#define SID_OUT P3DIR |=  BIT1    //SID_OUT,单片机数据口输出方向_1,P3.1
#define DAT_IN  P3IN & BIT1       //DAT_IN,单片机数据口输入数据,P3.1
#define DAT_IN_L  P3IN  &= ~BIT1  //SEL_CLK,单片机数据口输入数据_0,P3.1
#define DAT_IN_H  P3IN  |=  BIT1  //SEL_CLK,单片机数据口输入数据_1,P3.1
#define DAT_OUT_L P3OUT &= ~BIT1  //SEL_CLK,单片机数据口输出数据_0,P3.1
#define DAT_OUT_H P3OUT |=  BIT1  //SEL_CLK,单片机数据口输出数据_1,P3.1



void LCD12864_write_byte(uchar byte)		//写字节函数
{
	uchar i;
	SID_OUT; //SID切换到输出方向
	for(i = 0;i < 8;i++)
	{

		if ((byte << i) & 0x80)
		{
			DAT_OUT_H;
		}
		else
		{
			DAT_OUT_L;
		}
		CLR_CLK; //lcd_clk拉低
		_nop();
		SEL_CLK;//lcd_clk拉高
		_nop();
	}
}

uchar LCD12864_read_byte()		//读字节函数
{
	uchar i,temp1 = 0,temp2 = 0;
	SID_IN; //SID切换到输入方向
	for(i = 0;i < 8;i++)		//读高四位
	{
		temp1 = temp1 << 1;
		CLR_CLK;  //lcd_clk拉低
		_nop();
		SEL_CLK;  //lcd_clk拉高
		_nop();
		CLR_CLK;  //lcd_clk拉低
		_nop();
		if(DAT_IN == BIT1)
		{
			temp1++;
		}
	}
	for(i = 0;i < 8;i++)		//读低四位
	{
		temp2 = temp2 << 1;
		CLR_CLK;  //lcd_clk拉低
		_nop();
		SEL_CLK;  //lcd_clk拉高
		_nop();
		CLR_CLK;  //lcd_clk拉低
		_nop();
		if(DAT_IN == BIT1)
		{
		   temp2++;
		}
	}
	return ((0xf0 & temp1) + (0x0f & temp2));
}

void LCD12864_busy()		//检测忙函数
{
	uchar Busy;
	do
	{
		LCD12864_write_byte(0xfc);	//11111,A(1),B(0),0
		_nop();
		Busy = (0x00 & LCD12864_read_byte());
		_nop();
	}
	while(Busy);
}

void LCD12864_write(uchar dat_com,uchar byte) //写入函数(先写com_dat，再写所需指令或数据)
{
	uchar temp;
	if(dat_com == 0) //为零,写入指令
		temp = 0xf8; //11111,A(0),B(0),0
	else //否则,写入数据
		temp = 0xfa;  //11111,A(0),B(1),0
	SEL_CS; //CS拉高
	LCD12864_busy(); //判断LCD12864是否处于忙碌状态
	LCD12864_write_byte(temp);
	_nop();
	LCD12864_write_byte(byte & 0xf0); //写入高四位
	_nop();
	LCD12864_write_byte(byte << 4); //写入低四位
	_nop();
	CLR_CS; //CS拉低
}

void LCD12864_write_txt(uchar row,uchar position,uchar *byte) //写入函数(先写row_行以及position_列，再写所需数据)position为1-16,半字符显示
{
	uchar temp,i = 0;
	switch (row)
	{
	  case 1:temp = 0x80;break;
	  case 2:temp = 0x90;break;
	  case 3:temp = 0x88;break;
	  case 4:temp = 0x98;break;
	}
	temp = temp + position/2;
	LCD12864_write(0,0x30);
	if(position%2 == 1)
	{
	   LCD12864_write(0,temp);
	   _nop();
	}
	else
	{
		LCD12864_write(0,temp - 1);
		_nop();
	    LCD12864_write(1,0x20);//发送ASCII码的空格
	    _nop();
	}
		while(byte[i] != '\0')
		{
			LCD12864_write(1,byte[i]);
			_nop();
			i++;
		}
}

void LCD12864_init()		//初始化函数
{
	LCD12864_write(0,0x30); //基本指令功能.
	_nop();
	LCD12864_write(0,0x0c); //显示打开,关光标,反白关.
	_nop();
	LCD12864_write(0,0x01); //清屏指令.
	_nop();
	LCD12864_write(0,0x06); //AC 自动加一
	_nop();
	LCD12864_write(0,0x02); //显示打开,关光标,反白关.
	_nop();
}

void LCD12864_write_bmp(uchar *puts)		//绘图函数(调用时括号里面写图形数组名称)
{
	uint x=0;
	uchar i,j;
	LCD12864_write(0,0x34); //扩展指令
	_nop();
	LCD12864_write(0,0x36); //打开绘图
	_nop();
	for(i=0;i<32;i++)
	{
		LCD12864_write(0,0x80 | i);//列 32 行,双屏结构中上半屏
		_nop();
		LCD12864_write(0,0x80); //X 地址坐标,自动加 1.
		_nop();
		for(j = 0;j < 16;j++) //16*8=256
			LCD12864_write(1,puts[x++]);
		    _nop();
	}
	for(i = 0;i < 32;i++)
	{
		LCD12864_write(0,0x80 | i);
		_nop();
		LCD12864_write(0,0x88); //下半屏
		_nop();
		for(j = 0;j < 16;j++)
			LCD12864_write(1,puts[x++]);
		    _nop();
	}
}

void LCD12864_lcd_clear()	//清屏函数
{
	LCD12864_write(0,0x30); //基本指令功能.
	_nop();
	LCD12864_write(0,0x01); //清屏指令.
	_nop();
}

void LCD12864_bmp_clear()	//清图形函数
{
	uchar i,j;
	LCD12864_write(0,0x34);	//8Bit 扩充指令集,即使是 36H 也要写两次
	_nop();
	LCD12864_write(0,0x36);	 //绘图 ON,基本指令集里面 36H 不能开绘图
	_nop();
	for(i = 0;i < 32;i++)	//12864 实际为 256x32
	{
		LCD12864_write(0,0x80 | i);	//行位置
		_nop();
		LCD12864_write(0,0x80);		//列位置
		_nop();
		for(j = 0;j < 32;j++)  //256/8=32 byte //12864 实际为 256x32
		{
		   LCD12864_write(1,0);
		   _nop();
		}
	}
}

void LCD12864_txt_clear()		//清文本函数
{
	uchar i;
	LCD12864_write(0,0x30);		//8BitMCU,基本指令集合
	_nop();
	LCD12864_write(0,0x80);		//AC 归起始位
	_nop();
	for(i = 0;i < 64;i++)
		LCD12864_write(1,0x20);
	    _nop();
}

void LCD12864_write_zi(uchar x,uchar y,uchar *buff)		//括号中依次填行数，列数，字符串数组名称
{
	uchar addr,i = 0;
	if(x == 1)
		addr = 0x80;
	else if(x == 2)
		addr = 0x90;
	else if(x == 3)
		addr = 0x88;
	else if(x == 4)
		addr = 0x98;
	addr = addr + (y - 1);
	LCD12864_write(0,0x30);
	_nop();
	LCD12864_write(0,addr);
	_nop();
	while(buff[i] != '\0')
	{
		LCD12864_write(1,buff[i]);
		_nop();
		i++;
	}
}


/**************************************************************************************
//函数功能:显示16X32图形,适用于st7920型液晶
//形式参数:uchar x,uchar y,uchar *bmp
//行参说明:横坐标X列，纵坐标Y行，要显示的图形BMP
************************************************************************************/
void write1632GDRAM(uchar x,uchar y,uchar *bmp)
{
	uchar i,j,basex,basey;
	switch(y)	 //由y纵坐标定是上半屏还是下半屏
	{
	  case 1: basex = 0x80; break;  //上半屏
	  case 2: basex = 0x80; break;  //先上半屏，下面再下半屏。
	  case 3: basex = 0x88; break;  //下半屏
	  default:   return;   //别的则返回
	}
	basey = basex + x - 1;
	LCD12864_write(0,0x36);
	if(y == 1||y == 3)	//如为第一第三行，则直接是在同一半屏，直接绘完32行点陈数据。
	{
		  for(i=0;i<32;i++)	 //写入32行点阵
		  {
			  LCD12864_write(0,0x80+i);  //先写入垂直位址,选上下32行的哪一行,不管上下半屏，首行都为0X80
			  LCD12864_write(0,basey);     //再写入水平位址(选上下半屏)
			    for(j=0;j<2;j++)           //2个8位元的数据,即16BIT宽度
			    	LCD12864_write(1,*bmp++);
		  }
	}
	if(y == 2)  //从第二行开始则画图将上下半屏都有，所以先画完上半屏16行，再画下半屏16行。
	{
		  for(i=0;i<16;i++)	 //写入上半屏16行点阵
		  {
			  LCD12864_write(0,0x90+i);  //先写入垂直位址,选上下32行的哪一行,不管上下半屏，首行都为0X80，第二行为0X90。
			  LCD12864_write(0,basey);    //(选上半屏)再写入水平位址
			    for(j=0;j<2;j++)           //2个8位元的数据,即16BIT宽度
			    	LCD12864_write(1,*bmp++);
		  }
		  for(i=0;i<16;i++)	 //写入下半屏16行点阵
		  {
			  LCD12864_write(0,0x80+i);  //先写入垂直位址,选上下32行的哪一行,不管上下半屏，首行都为0X80
			  LCD12864_write(0,basey+8);   //(选下半屏)再写入水平位址
			    for(j=0;j<2;j++)           //2个8位元的数据,即16BIT宽度
			    	LCD12864_write(1,*bmp++);
		  }
	}
	LCD12864_write(0,0x36);  //写完数据，开图片显示
}
/**************************************************************************************
** 函数名称: init_12864_GDRAM()
** 功能描述: 在程写GDRAM时序初始化12864
**************************************************************************************/
void init_12864_GDRAM()
{
	LCD12864_write(0,0x30);     //基本指令操作(扩充指令操作为:0x34)
	LCD12864_write(0,0x0C);     //整体显示ON，游标OFF，游标位置OFF
	LCD12864_write(0,0x06);  	 //光标右移,DDRAM位址计数器(AC)加1,不整屏移动
	LCD12864_write(0,0x01);           //清屏 (清DDRAM)
}

/**************************************************************************************
//函数名称:Clean_12864_GDRAM(void)
//函数功能:清屏函数
//使用说明:GDRAM填满0
************************************************************************************/
void Clean_12864_GDRAM(void)
{
    uchar x,y;
    LCD12864_write(0,0x36);
    init_12864_GDRAM();		//设置扩展指令集，按手册说明，仅设置了绘图位，
    LCD12864_write(0,0x36);	//需要两次，本次设置扩展指令集。
    for (y=0;y<32;y++)
    {
    	LCD12864_write(0,0x80+y);  //设置y=1000+00xx，y+1则往下一行
    	LCD12864_write(0,0x80);	//设置x=1000 0000
        for (x=0;x<16;x++)
        {
        	LCD12864_write(1,0x00);   //高字节数据
        	LCD12864_write(1,0x00);	//低字节数据
        }
    }
}




