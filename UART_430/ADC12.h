/*
 * ADC12.h
 *
 *  Created on: 2015-12-9
 *      Author: zhjl769
 */

#ifndef ADC12_H_
#define ADC12_H_

unsigned int Num;//采集电压值的次数值
float Voltage;//电压值

 __interrupt void ADC12_ISR (void);
void ADC12_Start();//ADC采样开始函数



#endif /* ADC12_H_ */
