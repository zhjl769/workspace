/*
 * Init.h
 *
 *  Created on: 2015-12-7
 *      Author: zhjl769
 */

#ifndef INIT_H_
#define INIT_H_

#define XT2			8000000		//��Ƶʱ��ԴXT2Ƶ��
#define SMCLK		20000000	//SMCLKʱ��Ƶ���趨ֵ

#define PWM_FREQ     10000
#define PWM_COUNT (SMCLK / PWM_FREQ - 1)

void SetVcoreUp (unsigned int level);//�ı��ں˵�ѹֵ
extern void UART1_Init(void);//����UART1��ʼ������
extern void Init_CLK(void);//����ʱ�ӳ�ʼ������;
extern void IO_Init(void);//IO�ڳ�ʼ������
extern void Init_ADC();//ADC12��ʼ������
void Init_TimerA();//TimerA��ʼ������
void Init_DMA();//DMA��ʼ������


#endif /* INIT_H_ */
