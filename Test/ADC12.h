/*
 * ADC12.h
 *
 *  Created on: 2015-12-9
 *      Author: zhjl769
 */

#ifndef ADC12_H_
#define ADC12_H_

#include "DSPLib.h"

volatile unsigned int Num;//采集电压值的次数值
float Voltage;//电压值
volatile unsigned char Flg ;//缓存区标志位
volatile unsigned char Flg1;//缓存区采集完毕标志位






/*
extern int Rev1[1000];//接收缓存1
extern int Rev2[1000];//接收缓存2
extern int Rev[1000];//接收缓存

#pragma LOCATION(Rev1,"rev1")
short Rev1[1000];  //采集数据乒乓缓存区1

#pragma LOCATION(Rev2,"rev2")
short Rev2[1000];  //每次采集数据乒乓缓存区2
*/


// __interrupt void ADC12_ISR (void);
void ADC12_Start();//ADC采样开始函数
void datacla(void);


#endif /* ADC12_H_ */
