/*
 * LCD1602.h
 *
 *  Created on: 2015-12-7
 *      Author: zhjl769
 */

#ifndef LCD1602_H_
#define LCD1602_H_

void LCD1602_init(void);//LCD1602��ʼ��
void LCD1602_display_text(unsigned char y,unsigned char x,unsigned char Table[]); //Y��X ͬ���ַ���ʾ��table[] �ַ�������
void LCD1602_display_zi(unsigned char y,unsigned char x,unsigned char a,unsigned char Table[]); //Y=1,2����ʼ�У�X=1~16����ʼ�У�aΪ�Խ�ģ����λ��ֵ  Z=��д�ַ�������
void LCD1602_display_byte(unsigned char y,unsigned char x,unsigned char NOASC); //Y=1,2����ʼ�У�X=1~16����ʼ�У�Z=��д�ַ���ASCII ��
void LCD1602_Write(unsigned char i,int j);//LCD1602д����
void LCD1602_Read(unsigned char i,int j);//LCD1602������
int LCD1602_read_busy();//LCD1602��æ����


#endif /* LCD1602_H_ */
