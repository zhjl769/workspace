/*
 * ADC12.h
 *
 *  Created on: 2015-12-9
 *      Author: zhjl769
 */

#ifndef ADC12_H_
#define ADC12_H_

#include "DSPLib.h"

volatile unsigned int Num;//�ɼ���ѹֵ�Ĵ���ֵ
float Voltage;//��ѹֵ
volatile unsigned char Flg ;//��������־λ
volatile unsigned char Flg1;//�������ɼ���ϱ�־λ






/*
extern int Rev1[1000];//���ջ���1
extern int Rev2[1000];//���ջ���2
extern int Rev[1000];//���ջ���

#pragma LOCATION(Rev1,"rev1")
short Rev1[1000];  //�ɼ�����ƹ�һ�����1

#pragma LOCATION(Rev2,"rev2")
short Rev2[1000];  //ÿ�βɼ�����ƹ�һ�����2
*/


// __interrupt void ADC12_ISR (void);
void ADC12_Start();//ADC������ʼ����
void datacla(void);


#endif /* ADC12_H_ */
