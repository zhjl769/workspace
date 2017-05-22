/*
 * Creat_signal.c
 *
 *  Created on: 2017-4-6
 *      Author: Administrator
 */
/* ================================ ͷ�ļ����� ======================================= */
/* ϵͳģ������ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>


/* Ԥ��ʼ���������� */
#include "function_declare.h"
#include "var_creat_signal.h" /* Ԥ������� */


/* ================================ �궨������ ======================================= */
#define Fs  (48e3) /* �źŲ���Ƶ�� */
#define Rb  (2e3) /* ������ */

#define PWM_Fs      	(960e3) /* PWM�������ǲ��Ĳ���Ƶ�� */
#define PWM_Fw      	(96e3) /* PWM����ʱ���ǲ��Ļ�Ƶ */

#define Pwm_lpf_order	(256) /* PWM���ƹ��̲�ֵ�˲������� */
#define len_Pwm_lpf		(257) /* �˲������� */

#define pT_order		(144) /* ��������˲������� */
#define len_pT	    	(145) /* ��������˲������� */

#define K				(7) /* ������������Ƴ���+1 */
#define len_train    	(500) /* ѵ�����г��� */
#define CONSTELLATION  (2) /*  ����ͼѡ��: BPSK = 2, QPSK = 4 */

#define LEN_CRC			(7) /* CRCУ�����ʽ���� */
#define LEN_TAIL		(8) /* ��Ϣ���ؽ�β������ */
void Create_signal(char *Input_bits,long int len_input, volatile short *Pwm_mod,long int *len_Pwm_Mod)
{
/* �ֲ�������ʼ�� */
	int CONSTELLATION_LOG2 = log2(CONSTELLATION);
	int len_crc = (len_input+LEN_CRC+LEN_TAIL-1);
	int len_encode = (len_crc+K-1)*2;
	int len_Send_symbol = len_encode+len_train;
	int len_symbol_map = len_Send_symbol*2/CONSTELLATION_LOG2;
	long unsigned int len_Match = len_symbol_map*Fs/Rb;
	float *w_1024;//FFT��ת���ӣ���ʱ����

	cross_page_set0_float(Match_Data , 524288);
	cross_page_set0_float(Up_Spread , 1048576);

	/* ���ɾ����ʼ�� */
	short gen[2] = {91, 121};

	/* ��̬�����ڴ� */
	char  *Crc_bits          = (char *)calloc(len_crc,          sizeof(char));
	char  *Encode_symbol     = (char  *)calloc(len_encode,      sizeof(char));
	char  *Interleave_symbol = (char  *)calloc(len_encode,      sizeof(char));
	char  *Send_symbol       = (char  *)calloc(len_Send_symbol, sizeof(char));
	float *Symbol_map        = (float *)calloc(len_symbol_map,  sizeof(float));
	//float *Match_Data        = (float *)calloc(len_Match,       sizeof(float));


	/* ����PWM�����źų��� */
	*len_Pwm_Mod = len_Match*PWM_Fs/Fs/2;

	/* BPSK���ƹ��� */
	/* �������ӳ�ʼ�� */
	//tw_gen(w_1024, N_1024);

	/* CRCУ�� */
	memcpy(Crc_bits, Input_bits, sizeof(char)*len_input); /* ��ֵ��Ϣ���� */
	memcpy(Crc_bits+len_input, tail_bit, sizeof(char)*LEN_TAIL); /* �����β�� */
	crc(Crc_bits, len_crc, Crc_bits, crc_check_bit, LEN_CRC); /* CRCУ�� */

	/* �ݹ�ϵͳ�������� */
	Rsc_encoder(Crc_bits, len_crc, Encode_symbol, gen);//����ͨ��

	/* �����֯ */
	Interleaver(Encode_symbol, len_encode, Interleave_symbol, index_2740);//����ͨ��

	/* �������� */
	Symbol_combination(Trainsymbol, len_train, Interleave_symbol, len_encode, Send_symbol);

	/* ����ӳ�� */
	Symbol_Mapper(Send_symbol, len_Send_symbol, Symbol_map, CONSTELLATION);//����ͨ��

	/* ��������Լ����� */
	UpFIR(Symbol_map, len_symbol_map, Fs, Rb, pT_fft, len_pT, pT_order, N_1024, w_1024, CosCoeff, SinCoeff, Match_Data , Up_Spread);//����ͨ��,��һ������matlab�Ա����0.8%

	/*���㻺��*/
	cross_page_set0_float(Up_Spread , 1048576);

	/* ����ռ� */
	Pwm_mod = (short *)&Up_Spread;

	/* PWM���� */
	Overlap_Pwm(Match_Data, len_Match, Fs, PWM_Fs, Sawtooth, PWM_Fw, Pwm_lpf_fft, len_Pwm_lpf, Pwm_lpf_order, N_1024, w_1024, Pwm_mod );
	//������ڻ���Up_Spread��

	/* �ͷ��ڴ� */
	free(Crc_bits);
	free(Encode_symbol);
	free(Interleave_symbol);
	free(Send_symbol);
	free(Symbol_map);
	//free(Match_Data);
}


