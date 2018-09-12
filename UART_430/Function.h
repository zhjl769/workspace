/*
 * Function.h
 *
 *  Created on: 2015-10-17
 *      Author: zhjl769
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

void UART1_Init(void);//����UART1��ʼ������
void Init_CLK(void);//����ʱ�ӳ�ʼ������;
void IO_Init(void);//IO�ڳ�ʼ������
int ProcessCMD(char pBuf[],int nLen);
void Delay(long ms);//����ms�ӳٺ���
void Delay_us(long us);//����us�ӳٺ���
void Breath(void);//�����ƺ���
void LCD1602_init(void);//LCD1602��ʼ��
void LCD1602_display_text(unsigned char y,unsigned char x,unsigned char Table[]); //Y��X ͬ���ַ���ʾ��table[] �ַ�������
void LCD1602_display_zi(unsigned char y,unsigned char x,unsigned char a,unsigned char Table[]); //Y=1,2����ʼ�У�X=1~16����ʼ�У�aΪ�Խ�ģ����λ��ֵ  Z=��д�ַ�������
void LCD1602_display_byte(unsigned char y,unsigned char x,unsigned char NOASC); //Y=1,2����ʼ�У�X=1~16����ʼ�У�Z=��д�ַ���ASCII ��
void LCD1602_Write(unsigned char i,int j);//LCD1602д����
void LCD1602_Read(unsigned char i,int j);//LCD1602������
int LCD1602_read_busy();//LCD1602��æ����


/*---------------------DS18B20-----------------------*/
void Init_DS18B20();//��ʼ��ds18b2�Ӻ���
short Read(void);//���ֽ��Ӻ���
void Write(unsigned char cmd);//д�ֽ��Ӻ���
void read_serial(void);//�����кź���
void send_serial(); //�����кź���
void get_temperature();//�����¶Ⱥ���



#endif /* FUNCTION_H_ */
