/*
 * ADC12.h
 *
 *  Created on: 2015-12-9
 *      Author: zhjl769
 */

#ifndef ADC12_H_
#define ADC12_H_

unsigned int Num;//�ɼ���ѹֵ�Ĵ���ֵ
float Voltage;//��ѹֵ

 __interrupt void ADC12_ISR (void);
void ADC12_Start();//ADC������ʼ����



#endif /* ADC12_H_ */
