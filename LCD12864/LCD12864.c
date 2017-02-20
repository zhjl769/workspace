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
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//�����ʱ��������ֱ�ӵ���delay_ms (x),xΪ��Ҫ��ʱ��ʱ��


#define CLR_CS  P3OUT &= ~BIT0    //CS = 0,Һ��Ƭѡ=0,P3.0
#define SEL_CS  P3OUT |=  BIT0    //CS = 1,Һ��Ƭѡ=1,P3.0
#define CLR_SID P3OUT &= ~BIT1    //SID = 0,Һ�����ݿ�=0,P3.1
#define SEL_SID P3OUT |=  BIT1    //SID = 1,Һ�����ݿ�=1,P3.1
#define CLR_CLK P3OUT &= ~BIT2    //CLR_CLK,Һ��ʱ��=0,P3.2
#define SEL_CLK P3OUT |=  BIT2    //SEL_CLK,Һ��ʱ��=1,P3.2

#define SID_IN  P3DIR &= ~BIT1    //SID_IN,��Ƭ�����ݿ����뷽��_0,P3.1
#define SID_OUT P3DIR |=  BIT1    //SID_OUT,��Ƭ�����ݿ��������_1,P3.1
#define DAT_IN  P3IN & BIT1       //DAT_IN,��Ƭ�����ݿ���������,P3.1
#define DAT_IN_L  P3IN  &= ~BIT1  //SEL_CLK,��Ƭ�����ݿ���������_0,P3.1
#define DAT_IN_H  P3IN  |=  BIT1  //SEL_CLK,��Ƭ�����ݿ���������_1,P3.1
#define DAT_OUT_L P3OUT &= ~BIT1  //SEL_CLK,��Ƭ�����ݿ��������_0,P3.1
#define DAT_OUT_H P3OUT |=  BIT1  //SEL_CLK,��Ƭ�����ݿ��������_1,P3.1



void LCD12864_write_byte(uchar byte)		//д�ֽں���
{
	uchar i;
	SID_OUT; //SID�л����������
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
		CLR_CLK; //lcd_clk����
		_nop();
		SEL_CLK;//lcd_clk����
		_nop();
	}
}

uchar LCD12864_read_byte()		//���ֽں���
{
	uchar i,temp1 = 0,temp2 = 0;
	SID_IN; //SID�л������뷽��
	for(i = 0;i < 8;i++)		//������λ
	{
		temp1 = temp1 << 1;
		CLR_CLK;  //lcd_clk����
		_nop();
		SEL_CLK;  //lcd_clk����
		_nop();
		CLR_CLK;  //lcd_clk����
		_nop();
		if(DAT_IN == BIT1)
		{
			temp1++;
		}
	}
	for(i = 0;i < 8;i++)		//������λ
	{
		temp2 = temp2 << 1;
		CLR_CLK;  //lcd_clk����
		_nop();
		SEL_CLK;  //lcd_clk����
		_nop();
		CLR_CLK;  //lcd_clk����
		_nop();
		if(DAT_IN == BIT1)
		{
		   temp2++;
		}
	}
	return ((0xf0 & temp1) + (0x0f & temp2));
}

void LCD12864_busy()		//���æ����
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

void LCD12864_write(uchar dat_com,uchar byte) //д�뺯��(��дcom_dat����д����ָ�������)
{
	uchar temp;
	if(dat_com == 0) //Ϊ��,д��ָ��
		temp = 0xf8; //11111,A(0),B(0),0
	else //����,д������
		temp = 0xfa;  //11111,A(0),B(1),0
	SEL_CS; //CS����
	LCD12864_busy(); //�ж�LCD12864�Ƿ���æµ״̬
	LCD12864_write_byte(temp);
	_nop();
	LCD12864_write_byte(byte & 0xf0); //д�����λ
	_nop();
	LCD12864_write_byte(byte << 4); //д�����λ
	_nop();
	CLR_CS; //CS����
}

void LCD12864_write_txt(uchar row,uchar position,uchar *byte) //д�뺯��(��дrow_���Լ�position_�У���д��������)positionΪ1-16,���ַ���ʾ
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
	    LCD12864_write(1,0x20);//����ASCII��Ŀո�
	    _nop();
	}
		while(byte[i] != '\0')
		{
			LCD12864_write(1,byte[i]);
			_nop();
			i++;
		}
}

void LCD12864_init()		//��ʼ������
{
	LCD12864_write(0,0x30); //����ָ���.
	_nop();
	LCD12864_write(0,0x0c); //��ʾ��,�ع��,���׹�.
	_nop();
	LCD12864_write(0,0x01); //����ָ��.
	_nop();
	LCD12864_write(0,0x06); //AC �Զ���һ
	_nop();
	LCD12864_write(0,0x02); //��ʾ��,�ع��,���׹�.
	_nop();
}

void LCD12864_write_bmp(uchar *puts)		//��ͼ����(����ʱ��������дͼ����������)
{
	uint x=0;
	uchar i,j;
	LCD12864_write(0,0x34); //��չָ��
	_nop();
	LCD12864_write(0,0x36); //�򿪻�ͼ
	_nop();
	for(i=0;i<32;i++)
	{
		LCD12864_write(0,0x80 | i);//�� 32 ��,˫���ṹ���ϰ���
		_nop();
		LCD12864_write(0,0x80); //X ��ַ����,�Զ��� 1.
		_nop();
		for(j = 0;j < 16;j++) //16*8=256
			LCD12864_write(1,puts[x++]);
		    _nop();
	}
	for(i = 0;i < 32;i++)
	{
		LCD12864_write(0,0x80 | i);
		_nop();
		LCD12864_write(0,0x88); //�°���
		_nop();
		for(j = 0;j < 16;j++)
			LCD12864_write(1,puts[x++]);
		    _nop();
	}
}

void LCD12864_lcd_clear()	//��������
{
	LCD12864_write(0,0x30); //����ָ���.
	_nop();
	LCD12864_write(0,0x01); //����ָ��.
	_nop();
}

void LCD12864_bmp_clear()	//��ͼ�κ���
{
	uchar i,j;
	LCD12864_write(0,0x34);	//8Bit ����ָ�,��ʹ�� 36H ҲҪд����
	_nop();
	LCD12864_write(0,0x36);	 //��ͼ ON,����ָ����� 36H ���ܿ���ͼ
	_nop();
	for(i = 0;i < 32;i++)	//12864 ʵ��Ϊ 256x32
	{
		LCD12864_write(0,0x80 | i);	//��λ��
		_nop();
		LCD12864_write(0,0x80);		//��λ��
		_nop();
		for(j = 0;j < 32;j++)  //256/8=32 byte //12864 ʵ��Ϊ 256x32
		{
		   LCD12864_write(1,0);
		   _nop();
		}
	}
}

void LCD12864_txt_clear()		//���ı�����
{
	uchar i;
	LCD12864_write(0,0x30);		//8BitMCU,����ָ���
	_nop();
	LCD12864_write(0,0x80);		//AC ����ʼλ
	_nop();
	for(i = 0;i < 64;i++)
		LCD12864_write(1,0x20);
	    _nop();
}

void LCD12864_write_zi(uchar x,uchar y,uchar *buff)		//�������������������������ַ�����������
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
//��������:��ʾ16X32ͼ��,������st7920��Һ��
//��ʽ����:uchar x,uchar y,uchar *bmp
//�в�˵��:������X�У�������Y�У�Ҫ��ʾ��ͼ��BMP
************************************************************************************/
void write1632GDRAM(uchar x,uchar y,uchar *bmp)
{
	uchar i,j,basex,basey;
	switch(y)	 //��y�����궨���ϰ��������°���
	{
	  case 1: basex = 0x80; break;  //�ϰ���
	  case 2: basex = 0x80; break;  //���ϰ������������°�����
	  case 3: basex = 0x88; break;  //�°���
	  default:   return;   //����򷵻�
	}
	basey = basex + x - 1;
	LCD12864_write(0,0x36);
	if(y == 1||y == 3)	//��Ϊ��һ�����У���ֱ������ͬһ������ֱ�ӻ���32�е�����ݡ�
	{
		  for(i=0;i<32;i++)	 //д��32�е���
		  {
			  LCD12864_write(0,0x80+i);  //��д�봹ֱλַ,ѡ����32�е���һ��,�������°��������ж�Ϊ0X80
			  LCD12864_write(0,basey);     //��д��ˮƽλַ(ѡ���°���)
			    for(j=0;j<2;j++)           //2��8λԪ������,��16BIT���
			    	LCD12864_write(1,*bmp++);
		  }
	}
	if(y == 2)  //�ӵڶ��п�ʼ��ͼ�����°������У������Ȼ����ϰ���16�У��ٻ��°���16�С�
	{
		  for(i=0;i<16;i++)	 //д���ϰ���16�е���
		  {
			  LCD12864_write(0,0x90+i);  //��д�봹ֱλַ,ѡ����32�е���һ��,�������°��������ж�Ϊ0X80���ڶ���Ϊ0X90��
			  LCD12864_write(0,basey);    //(ѡ�ϰ���)��д��ˮƽλַ
			    for(j=0;j<2;j++)           //2��8λԪ������,��16BIT���
			    	LCD12864_write(1,*bmp++);
		  }
		  for(i=0;i<16;i++)	 //д���°���16�е���
		  {
			  LCD12864_write(0,0x80+i);  //��д�봹ֱλַ,ѡ����32�е���һ��,�������°��������ж�Ϊ0X80
			  LCD12864_write(0,basey+8);   //(ѡ�°���)��д��ˮƽλַ
			    for(j=0;j<2;j++)           //2��8λԪ������,��16BIT���
			    	LCD12864_write(1,*bmp++);
		  }
	}
	LCD12864_write(0,0x36);  //д�����ݣ���ͼƬ��ʾ
}
/**************************************************************************************
** ��������: init_12864_GDRAM()
** ��������: �ڳ�дGDRAMʱ���ʼ��12864
**************************************************************************************/
void init_12864_GDRAM()
{
	LCD12864_write(0,0x30);     //����ָ�����(����ָ�����Ϊ:0x34)
	LCD12864_write(0,0x0C);     //������ʾON���α�OFF���α�λ��OFF
	LCD12864_write(0,0x06);  	 //�������,DDRAMλַ������(AC)��1,�������ƶ�
	LCD12864_write(0,0x01);           //���� (��DDRAM)
}

/**************************************************************************************
//��������:Clean_12864_GDRAM(void)
//��������:��������
//ʹ��˵��:GDRAM����0
************************************************************************************/
void Clean_12864_GDRAM(void)
{
    uchar x,y;
    LCD12864_write(0,0x36);
    init_12864_GDRAM();		//������չָ������ֲ�˵�����������˻�ͼλ��
    LCD12864_write(0,0x36);	//��Ҫ���Σ�����������չָ���
    for (y=0;y<32;y++)
    {
    	LCD12864_write(0,0x80+y);  //����y=1000+00xx��y+1������һ��
    	LCD12864_write(0,0x80);	//����x=1000 0000
        for (x=0;x<16;x++)
        {
        	LCD12864_write(1,0x00);   //���ֽ�����
        	LCD12864_write(1,0x00);	//���ֽ�����
        }
    }
}




