/*
 * LCD1602.h
 *
 *  Created on: 2015-12-7
 *      Author: zhjl769
 */

#ifndef LCD1602_H_
#define LCD1602_H_

void LCD1602_init(void);//LCD1602初始化
void LCD1602_display_text(unsigned char y,unsigned char x,unsigned char Table[]); //Y，X 同上字符显示，table[] 字符串数组
void LCD1602_display_zi(unsigned char y,unsigned char x,unsigned char a,unsigned char Table[]); //Y=1,2（起始行）X=1~16（起始列）a为自建模数的位数值  Z=想写字符的数组
void LCD1602_display_byte(unsigned char y,unsigned char x,unsigned char NOASC); //Y=1,2（起始行）X=1~16（起始列）Z=想写字符的ASCII 码
void LCD1602_Write(unsigned char i,int j);//LCD1602写函数
void LCD1602_Read(unsigned char i,int j);//LCD1602读函数
int LCD1602_read_busy();//LCD1602判忙函数


#endif /* LCD1602_H_ */
