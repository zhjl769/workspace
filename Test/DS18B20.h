/*
 * DS18B20.h
 *
 *  Created on: 2015-12-7
 *      Author: zhjl769
 */

#ifndef DS18B20_H_
#define DS18B20_H_

float TPT; // ���ص���ʵ�¶�ֵ
short serial[8];   // �����кź����е�����
short serial_x[8]; // ��ȡ�����кŴ�ŵ�����


void Init_DS18B20();//��ʼ��ds18b2�Ӻ���
short Read(void);//���ֽ��Ӻ���
void Write(unsigned char cmd);//д�ֽ��Ӻ���
void read_serial(void);//�����кź���
void send_serial(); //�����кź���
void get_temperature();//�����¶Ⱥ���


#endif /* DS18B20_H_ */
