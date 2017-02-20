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

extern void UART1_Init(void);//声明UART1初始化函数
extern void Init_CLK(void);//声明时钟初始化函数;
extern void IO_Init(void);//IO口初始化函数
extern void Init_ADC();//ADC12初始化函数
void Init_TimerA();//TimerA初始化函数


#endif /* INIT_H_ */
