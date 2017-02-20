/*
 * BMP180.h
 *
 *  Created on: 2016-2-25
 *      Author: zhjl769
 */

#ifndef BMP180_H_
#define BMP180_H_

#define   uchar unsigned char
#define   uint unsigned int


#define	BMP180_SlaveAddress   0xee	  //定义器件在IIC总线中的从地址
#define OSS 3	// Oversampling Setting (note: code is not set up to use other OSS values)

typedef unsigned char  BYTE;
typedef unsigned short WORD;

uchar ge,shi,bai,qian,wan,shiwan;           //显示变量
int  dis_data;                              //变量

short ac1;
short ac2;
short ac3;
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1;
short b2;
short mb;
short mc;
short md;


//------------------------------------
void BMP180_Start();
void BMP180_Stop();
void BMP180_SendACK(char ack);
char BMP180_RecvACK(void);
void BMP180_SendByte(uchar DAT);
char BMP180_RecvByte(void);
void  Single_Write(uchar SlaveAddress,uchar REG_Address,uchar REG_data);   //单个写入数据
uchar Single_Read(uchar SlaveAddress,uchar REG_Address);                                      //单个读取内部寄存器数据
long BMP180_BMP180_ReadTemperature(void);
long BMP180_ReadPressure(void);
void Init_BMP180();
void BMP180_Convert();
void conversion(long temp_data);
void BMP180_ReadPage();
void BMP180_WritePage();
static short BMP180_AddressRead2Byte(char address);//连续的读取内部寄存器数据
//-----------------------------------



#endif /* BMP180_H_ */
