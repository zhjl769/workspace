/*
 * Function.h
 *
 *  Created on: 2015-10-17
 *      Author: zhjl769
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

void UART1_Init(void);//声明UART1初始化函数
void Init_CLK(void);//声明时钟初始化函数;
void IO_Init(void);//IO口初始化函数
int ProcessCMD(char pBuf[],int nLen);
void Delay(long ms);//声明ms延迟函数
void Delay_us(long us);//声明us延迟函数
void Breath(void);//呼吸灯函数
void LCD1602_init(void);//LCD1602初始化
void LCD1602_display_text(unsigned char y,unsigned char x,unsigned char Table[]); //Y，X 同上字符显示，table[] 字符串数组
void LCD1602_display_zi(unsigned char y,unsigned char x,unsigned char a,unsigned char Table[]); //Y=1,2（起始行）X=1~16（起始列）a为自建模数的位数值  Z=想写字符的数组
void LCD1602_display_byte(unsigned char y,unsigned char x,unsigned char NOASC); //Y=1,2（起始行）X=1~16（起始列）Z=想写字符的ASCII 码
void LCD1602_Write(unsigned char i,int j);//LCD1602写函数
void LCD1602_Read(unsigned char i,int j);//LCD1602读函数
int LCD1602_read_busy();//LCD1602判忙函数


/*---------------------DS18B20-----------------------*/
void Init_DS18B20();//初始化ds18b2子函数
short Read(void);//读字节子函数
void Write(unsigned char cmd);//写字节子函数
void read_serial(void);//读序列号函数
void send_serial(); //发序列号函数
void get_temperature();//读出温度函数



#endif /* FUNCTION_H_ */
