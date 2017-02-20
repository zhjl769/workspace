/*
 * BMP180.c
 *
 *  Created on: 2016-2-25
 *      Author: zhjl769
 */
#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "BMP180.h"
#include "LCD12864.h"

#define CPU_F ((double)8000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//�����ʱ��������ֱ�ӵ���delay_ms (x),xΪ��Ҫ��ʱ��ʱ��


#define	  SCL_OUT      P5DIR |=  BIT0 //IICʱ�����Ŷ��������P5.0
#define	  SCL_OUT_L    P5OUT &= ~BIT0 //IICʱ�����Ŷ���͵�ƽ��P5.0
#define	  SCL_OUT_H    P5OUT |=  BIT0 //IICʱ�����Ŷ���ߵ�ƽ��P5.0

#define   SDA_DATA_IN  P5IN & BIT1    //SDA_DAT_IN,��Ƭ�����ݿ���������,P5.1
#define	  SDA_IN       P5DIR &= ~BIT1 //IIC�������Ŷ������룬P5.1
#define	  SDA_OUT      P5DIR |=  BIT1 //IIC�������Ŷ��������P5.1
#define	  SDA_OUT_L    P5OUT &= ~BIT1 //IIC�������Ŷ�������͵�ƽ��P5.1
#define	  SDA_OUT_H    P5OUT |=  BIT1 //IIC�������Ŷ�������ߵ�ƽ��P5.1
#define	  SDA_IN_L     P5IN  &= ~BIT1 //IIC�������Ŷ�������͵�ƽ��P5.1
#define	  SDA_IN_H     P5IN  |=  BIT1 //IIC�������Ŷ�������ߵ�ƽ��P5.1


/**************************************
��ʼ�ź�
**************************************/
void BMP180_Start()
{

	SDA_OUT;//IIC�������Ŷ��������P5.1
	SCL_OUT;//IICʱ�����Ŷ��������P5.0
	SDA_OUT_H; //����������
	SCL_OUT_H; //����ʱ����
	delay_us(5);//��ʱ5us
	SDA_OUT_L; //�����½���
	delay_us(5);//��ʱ5us
	SCL_OUT_L; //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void BMP180_Stop()
{
	SDA_OUT;//IIC�������Ŷ��������P5.1
	SCL_OUT;//IICʱ�����Ŷ��������P5.0
	SDA_OUT_L;//����������
	SCL_OUT_H;//����ʱ����
	delay_us(5);//��ʱ5us
	SDA_OUT_H;//����������
	delay_us(5);//��ʱ5us
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void BMP180_SendACK(char ack)
{
	SDA_OUT;//IIC�������Ŷ��������P5.1
	SCL_OUT;//IICʱ�����Ŷ��������P5.0
	SCL_OUT_L;//����ʱ����
	delay_us(5);//��ʱ5us
	if((ack & 0x01) == 0x00)
		SDA_OUT_L;
	else
		SDA_OUT_H; //дӦ���ź�
    SCL_OUT_H;//����ʱ����
    delay_us(5);//��ʱ5us
    SCL_OUT_L;//����ʱ����
    delay_us(5);//��ʱ5us
}

/**************************************
����Ӧ���ź�
**************************************/
char BMP180_RecvACK(void)
{
	char CY;
	SDA_IN;//IIC�������Ŷ������룬P5.1
	SCL_OUT;//IICʱ�����Ŷ��������P5.0
	SCL_OUT_H;//����ʱ����
	delay_us(5);//��ʱ5us
    if((SDA_DATA_IN) == 0)
    	CY = 0;
    else
    	CY = 1;//��Ӧ���ź�

    SCL_OUT_L;//����ʱ����
    delay_us(5);//��ʱ5us

    return CY;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/

void BMP180_SendByte(uchar DAT)
{
    int i;

    SDA_OUT;//IIC�������Ŷ��������P5.1
    SCL_OUT;//IICʱ�����Ŷ��������P5.0
    SCL_OUT_L;//����ʱ����
    delay_us(5);//��ʱ5us
    for (i=0; i<8; i++) //8λ������
    {
        if((DAT & 0x80) == 0x00)
        	SDA_OUT_L;
        else
        	SDA_OUT_H;
        SCL_OUT_H;//����ʱ����
        delay_us(5);//��ʱ5us
        SCL_OUT_L;//����ʱ����
        delay_us(5);//��ʱ5us
        DAT = DAT << 1;//�Ƴ����ݵ����λ
    }
}


/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
char BMP180_RecvByte(void)
{
    int i;
    char DAT = 0;

    SDA_IN;//IIC�������Ŷ������룬P5.1
    SCL_OUT;//IICʱ�����Ŷ��������P5.0

    SDA_OUT_H;//ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)//8λ������
    {
        DAT <<= 1;
        SCL_OUT_H;//����ʱ����
        delay_us(5);//��ʱ5us
        if((SDA_DATA_IN) == 0)
        	DAT |= 0X00;//������
        else
        	DAT |= 0X01;//������
        SCL_OUT_L;//����ʱ����
        delay_us(5);//��ʱ5us
    }
    return DAT;
}

//���ֽ�д��BMP180�ڲ�����*******************************

void Single_Write(uchar SlaveAddress,uchar REG_Address,uchar REG_data)
{
    BMP180_Start();                  //��ʼ�ź�
    BMP180_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    BMP180_RecvACK();//Ӧ���ź�

    BMP180_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ
    BMP180_RecvACK();//Ӧ���ź�

    BMP180_SendByte(REG_data);       //�ڲ��Ĵ�������
    BMP180_RecvACK();//Ӧ���ź�

    BMP180_Stop();                   //����ֹͣ�ź�
}


//���ֽڶ�ȡBMP180�ڲ�����********************************

uchar Single_Read(uchar SlaveAddress,uchar REG_Address)
{
	uchar REG_data;
    BMP180_Start();                          //��ʼ�ź�
    BMP180_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    BMP180_RecvACK();//Ӧ���ź�

    BMP180_SendByte(REG_Address);            //���ʹ洢��Ԫ��ַ
    BMP180_RecvACK();//Ӧ���ź�

    BMP180_Start();                          //��ʼ�ź�
    BMP180_SendByte(BMP180_SlaveAddress | 0x01);      //�����豸��ַ+���ź�
    BMP180_RecvACK();//Ӧ���ź�

    REG_data = BMP180_RecvByte();              //�����Ĵ�������
	BMP180_SendACK(1);

	BMP180_Stop();                           //ֹͣ�ź�
    return REG_data;
}

//*********************************************************
//����BMP180�ڲ�����,��������
//*********************************************************
static short BMP180_AddressRead2Byte(char address)
{
	char msb = 0x00, lsb = 0x00;

	msb = Single_Read(BMP180_SlaveAddress,address);
	lsb = Single_Read(BMP180_SlaveAddress,address + 1);

	return ( ((long)msb << 8) + lsb) ;
}

//********************************************************************
long BMP180_ReadTemperature(void)
{
	long Temperature = 0;
	char msb = 0,lsb = 0;

	Single_Write(BMP180_SlaveAddress,0xF4,0x2E);
	switch (OSS)
	{
	case 0:delay_ms(5);break;
	case 1:delay_ms(8);break;
	case 2:delay_ms(14);break;
	case 3:delay_ms(26);break;
	}

	msb = Single_Read(BMP180_SlaveAddress,0xF6);
	lsb = Single_Read(BMP180_SlaveAddress,0xF6 + 1);

	Temperature |= ((long)msb << 8) + lsb;

	return Temperature;
}
//*************************************************************
long BMP180_ReadPressure(void)
{
	long pressure = 0;
	char msb = 0,lsb = 0,xlsb = 0;

    Single_Write(BMP180_SlaveAddress,0xF4,0x34 + (OSS << 6));
    switch (OSS)
    	{
    	case 0:delay_ms(5);break;
    	case 1:delay_ms(8);break;
    	case 2:delay_ms(14);break;
    	case 3:delay_ms(26);break;
    	}

    msb = Single_Read(BMP180_SlaveAddress,0xF6);
    lsb = Single_Read(BMP180_SlaveAddress,0xF6 + 1);
    xlsb = Single_Read(BMP180_SlaveAddress,0xF6 + 2);

	pressure = (((long)msb << 16) + ((long)lsb << 8) + xlsb) >> (8 - OSS);

	return pressure;
	//return (long) BMP180_ReadShort(0xF6);
}

//**************************************************************

//��ʼ��BMP180��������Ҫ��ο�pdf�����޸�**************
void Init_BMP180()
{
	ac1 = BMP180_AddressRead2Byte(0xAA);
	ac2 = BMP180_AddressRead2Byte(0xAC);
	ac3 = BMP180_AddressRead2Byte(0xAE);
	ac4 = BMP180_AddressRead2Byte(0xB0);
	ac5 = BMP180_AddressRead2Byte(0xB2);
	ac6 = BMP180_AddressRead2Byte(0xB4);
	b1 =  BMP180_AddressRead2Byte(0xB6);
	b2 =  BMP180_AddressRead2Byte(0xB8);
	mb =  BMP180_AddressRead2Byte(0xBA);
	mc =  BMP180_AddressRead2Byte(0xBC);
	md =  BMP180_AddressRead2Byte(0xBE);
}
//*********************************************************
void conversion(long temp_data)
{

    shiwan=temp_data/100000+0x30 ;
    temp_data=temp_data%100000;   //ȡ������
    wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //ȡ������
	qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //ȡ������
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //ȡ������
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //ȡ������
    ge=temp_data+0x30;
}

//***********************************************************************
void BMP180_Convert()
{
	long ut;
	long up;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;
	long  temperature,pressure,altitude;

	ut = BMP180_ReadTemperature();	   // ��ȡ�¶�

	up = BMP180_ReadPressure();  // ��ȡѹǿ

	x1 = (((long)ut - (long)ac6) * (long)ac5 )>> 15;
	x2 = ((long) mc << 11) / (x1 + (long)md);
	b5 = x1 + x2;
	temperature = (b5 + 8) >> 4;

/****************************************************
		�¶ȼ�������ʾ
****************************************************/

	 conversion(temperature);
	 LCD12864_write(0,0x94);//��ַ���ڶ��ŵ�5��
	 LCD12864_write(1,0x3A);//���ݡ�ASCII��":"
	 LCD12864_write(1,bai );
	 LCD12864_write(1,shi );
	 LCD12864_write(1,0x2E);//���ݡ�ASCII��"."
	 LCD12864_write(1,ge );
	 LCD12864_write(1,0x27);//���ݡ�ASCII��"'"
	 LCD12864_write(1,0x43);//���ݡ�ASCII��"C"

     //*************

	b6 = b5 - 4000;
	x1 = ((long)b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ((long)ac2 * b6) >> 11;
	x3 = x1 + x2;
	b3 = ((((long)ac1 * 4 + x3) << OSS) + 2)/4;
	x1 = ((long)ac3 * b6) >> 13;
	x2 = ((long)b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = ((long)ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) up - b3) * (50000 >> OSS);
	if( b7 < 0x80000000)
	     p = (b7 * 2) / b4 ;
           else
		    p = (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	 pressure = p + ((x1 + x2 + 3791) >> 4);

/****************************************************
	 ����ѹ��������ʾ
****************************************************/
	 conversion(pressure);
	 LCD12864_write(0,0x98);//��ַ����4�ŵ�1��

	 if(shiwan > 0x30)
	 {
	   LCD12864_write(1,shiwan);
	 }

	 LCD12864_write(1,wan );
	 LCD12864_write(1,qian );
	 LCD12864_write(1,0x2E);//���ݡ�ASCII��"."
	 LCD12864_write(1,bai );
	 LCD12864_write(1,shi );
	 LCD12864_write(1,ge );
	 LCD12864_write(1,0x6B);//���ݡ�ASCII��"k"
	 LCD12864_write(1,0x70);//���ݡ�ASCII��"p"
	 LCD12864_write(1,0x61);//���ݡ�ASCII��"a"

/****************************************************
	 �߶ȼ�������ʾ
****************************************************/

	 altitude = (44330.0 * (1.0 - pow(pressure/101325.0,1.0/5.255)));
	 conversion(altitude);
	 LCD12864_write(0,0x8C);//��ַ����3�ŵ�1��
	 LCD12864_write(1,0x3A);//���ݡ�ASCII��":"

	 if(qian > 0x30)
	   {
	 	 LCD12864_write(1,qian);
	   }
	 if(bai > 0x30)
	   {
	 	LCD12864_write(1,bai);
	   }
	 LCD12864_write(1,shi );
	 LCD12864_write(1,ge );
	 LCD12864_write(1,0x6D);//���ݡ�ASCII��"k"

}


