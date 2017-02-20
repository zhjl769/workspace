/*
 * Variable.h
 *
 *  Created on: 2015-12-7
 *      Author: zhjl769
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include "DSPLib.h"
/*
#pragma DATA_SECTION(Rev1,"rev1")
short Rev1[1000];  //�ɼ�����ƹ�һ�����1

#pragma DATA_SECTION(Rev2,"rev2")
short Rev2[1000];  //ÿ�βɼ�����ƹ�һ�����2
*/

#define FIR_COEFF_ORDER    32
#define FIR_COEFF_LENGTH   (FIR_COEFF_ORDER+2)

extern _q15 Fir_coeffs[FIR_COEFF_LENGTH];


/* Allocate DSPLib parameter structures. */
extern msp_fir_q15_params FirParams;
extern msp_fft_q15_params FFTParams;


#define f1 12000 //�ο��ź�1
#define fs 65000 //������
#define Di 1665  //����ռ����
#define Ds 40  //������ر����ĵ���

#define Save 1625  //������ر����ĵ��� Di - Ds

#define Dfir 388  //������ر����ĵ��� (Di - Ds)/5 + 64 + 2,32Ϊ�˲���ϵ��
#define Dxcor 326 //�������ĵ���  Dfir - FIR_COEFF_ORDER - 1

/*
 * ˵�����˴�ÿ�θ��µĵ���Ϊ125�������˲���ÿ�ε�ǰ32������ݶ�ʧ��ÿ�εĺ����40����Ƶ�ǰ�棬Ϊ�˱�֤ÿ�θ��µ�Ϊ�����ݣ�
 * ���Դ˴��ĵ���Ϊ166��Ҳ�����ڱ�֤��ʧ������32��Ը��µ�����û��Ӱ�죬ǰ���������Ѿ������˵�32���㣬Ϊ�˸����µ����ݣ�
 * �˴����˵�8���������˲���������ռ�Ӧ�ñ�֤Ϊż�������Դ˴�Ϊ166���㣬��֪����
 */

#define Dref 40  //�ο��ռ����

int Rev1[Save];//���ջ���1
int Rev2[Save];//���ջ���2
int Rev[Di];//���ջ���
int X_ref[Dref];//volatile


_q15 Xout[Dfir];
_q15 Fout[Dxcor + FIR_COEFF_ORDER];
_q15 FFTout[256];
_q15 FFTout_save[25];


unsigned int Block;//FFT���������Ŀ���
unsigned int Last_result;//������Ϊ�ϴ��Ƿ��⵽������ϴμ�⵽����ô������Ϊ1����֮Ϊ0��
unsigned int Result_out;//�������������

/* Result maximum value */
_q15 q15MaxVector;

/* Index of result */
uint16_t uint16MaxIndex;

/* Index of result used interpolated */
float  MaxIndex_out;



extern void temperature_get(void);//�¶Ȼ�ȡ����
extern void Voltage_get_get(void);//��ѹ��ȡ����



#endif /* VARIABLE_H_ */