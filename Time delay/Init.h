/*
 * Init.h
 *
 *  Created on: 2015-12-7
 *      Author: zhjl769
 */

#ifndef INIT_H_
#define INIT_H_

#define MCLK  8000000
#define SMCLK 8000000

#define PWM_FREQ     10000
#define PWM_COUNT (SMCLK / PWM_FREQ - 1)

extern void UART1_Init(void);//����UART1��ʼ������
extern void Init_CLK(void);//����ʱ�ӳ�ʼ������;
extern void IO_Init(void);//IO�ڳ�ʼ������
extern void Init_ADC();//ADC12��ʼ������
void Init_TimerA();//TimerA��ʼ������


#endif /* INIT_H_ */
