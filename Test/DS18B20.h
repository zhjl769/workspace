/*
 * DS18B20.h
 *
 *  Created on: 2015-12-7
 *      Author: zhjl769
 */

#ifndef DS18B20_H_
#define DS18B20_H_

float TPT; // 返回的真实温度值
short serial[8];   // 读序列号函数中的数组
short serial_x[8]; // 读取的序列号存放的数组


void Init_DS18B20();//初始化ds18b2子函数
short Read(void);//读字节子函数
void Write(unsigned char cmd);//写字节子函数
void read_serial(void);//读序列号函数
void send_serial(); //发序列号函数
void get_temperature();//读出温度函数


#endif /* DS18B20_H_ */
