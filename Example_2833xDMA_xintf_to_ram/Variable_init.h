/*
 * Variable_init.h
 *
 *  Created on: 2017-2-18
 *      Author: zhjl769
 */

#ifndef VARIABLE_INIT_H_
#define VARIABLE_INIT_H_

//��������

int16 Piont1, Piont2;
Uint16 FLAG = 0;                    //�˱�־��һ��ʲô��־����ʱ���жϷ���������ж�FLAG��1�Ĳ�����Ӧ���ǿ�ʼ�ɼ��ı�־
Uint16 ad_signal_num=0;

//�ο��źŲ��ֱ�������
#define PI        3.14159265
#define fs            18  //AD�Ĳ�����,��λ��kHz
#define Time_Ref      20  //�ο��ź�����,��λ��ms
#define f1_Ref        4  //�ο��ź�����Ƶ��,��λ��kHz
#define f2_Ref        4  //�ο��ź�����Ƶ��,��λ��kHz

#define BUF_SIZE   1024    // Sample buffer size    1024*2=2048=2exp11
#define Block_xcorr   Time_Ref*fs    // ��زο��źŵĳ���ֵ

#pragma DATA_SECTION(DATABuf1,"DMARAML2");    //�Լ��¼ӵģ������洢AD0�ɼ��������ݵ�DSP�ڲ�����28335_RAM_lnk�ļ��ж���
#pragma DATA_SECTION(DATABuf2,"DMARAML3");
#pragma DATA_SECTION(DATABuf3,"DMARAML6");
#pragma DATA_SECTION(DATABuf4,"DMARAML4");
#pragma DATA_SECTION(DATABuf5,"DMARAML5");

int32 DATABuf1[BUF_SIZE];    //�Լ��¼ӵģ������洢AD0�ɼ��������ݵ�DSP�ڲ�
float32 DATABuf2[BUF_SIZE];    //�Լ��¼ӵģ������洢AD0�ɼ��������ݵ�DSP�ڲ�
/*
float DATABuf3[Block_xcorr] = {1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01,
		-9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01,
		-5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01,
		7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01,
		7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01,
		-5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01,
		-9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1,
		1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01,
		1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01,
		-9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01,
		-5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01,
		7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01,
		7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01,
		-5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01,
		-9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1,
		1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01,
		1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01,
		-9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01,
		-5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01,
		7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01,
		7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01,
		-5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01,
		-9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1,
		1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01,
		1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01,
		-9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01,
		-5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01,
		7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01,
		7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01,
		-5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01,
		-9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1,
		1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01,
		1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01,
		-9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01,
		-5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01,
		7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01,
		7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01,
		-5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01,
		-9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1,
		1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01,
		1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01,
		-9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01, 7.660444e-01,
		-5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01, 7.660444e-01,
		7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01, -5.000000e-01,
		7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01, 1, 1.736482e-01, -9.396926e-01,
		-5.000000e-01, 7.660444e-01, 7.660444e-01, -5.000000e-01, -9.396926e-01, 1.736482e-01}; //�ο��źŵ�����

*/
float32 DATABuf3[Block_xcorr];  //�ο��źŵ�����
float32 DATABuf4[BUF_SIZE];     //��һ����ؽ�����
float32 DATABuf5[BUF_SIZE];     //�ڶ�����ؽ�����


#endif /* VARIABLE_INIT_H_ */