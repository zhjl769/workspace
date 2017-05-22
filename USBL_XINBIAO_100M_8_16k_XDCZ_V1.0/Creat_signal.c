/*
 * Creat_signal.c
 *
 *  Created on: 2017-4-6
 *      Author: Administrator
 */
/* ================================ 头文件声明 ======================================= */
/* 系统模块声明 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>


/* 预初始化数据声明 */
#include "function_declare.h"
#include "var_creat_signal.h" /* 预定义参数 */


/* ================================ 宏定义声明 ======================================= */
#define Fs  (48e3) /* 信号采样频率 */
#define Rb  (2e3) /* 符号率 */

#define PWM_Fs      	(960e3) /* PWM调制三角波的采样频率 */
#define PWM_Fw      	(96e3) /* PWM调制时三角波的基频 */

#define Pwm_lpf_order	(256) /* PWM调制过程插值滤波器阶数 */
#define len_Pwm_lpf		(257) /* 滤波器长度 */

#define pT_order		(144) /* 脉冲成型滤波器阶数 */
#define len_pT	    	(145) /* 脉冲成形滤波器长度 */

#define K				(7) /* 卷积码编码的限制长度+1 */
#define len_train    	(500) /* 训练序列长度 */
#define CONSTELLATION  (2) /*  星座图选择: BPSK = 2, QPSK = 4 */

#define LEN_CRC			(7) /* CRC校验多项式长度 */
#define LEN_TAIL		(8) /* 信息比特结尾符长度 */
void Create_signal(char *Input_bits,long int len_input, volatile short *Pwm_mod,long int *len_Pwm_Mod)
{
/* 局部变量初始化 */
	int CONSTELLATION_LOG2 = log2(CONSTELLATION);
	int len_crc = (len_input+LEN_CRC+LEN_TAIL-1);
	int len_encode = (len_crc+K-1)*2;
	int len_Send_symbol = len_encode+len_train;
	int len_symbol_map = len_Send_symbol*2/CONSTELLATION_LOG2;
	long unsigned int len_Match = len_symbol_map*Fs/Rb;
	float *w_1024;//FFT旋转因子，暂时无用

	cross_page_set0_float(Match_Data , 524288);
	cross_page_set0_float(Up_Spread , 1048576);

	/* 生成矩阵初始化 */
	short gen[2] = {91, 121};

	/* 动态分配内存 */
	char  *Crc_bits          = (char *)calloc(len_crc,          sizeof(char));
	char  *Encode_symbol     = (char  *)calloc(len_encode,      sizeof(char));
	char  *Interleave_symbol = (char  *)calloc(len_encode,      sizeof(char));
	char  *Send_symbol       = (char  *)calloc(len_Send_symbol, sizeof(char));
	float *Symbol_map        = (float *)calloc(len_symbol_map,  sizeof(float));
	//float *Match_Data        = (float *)calloc(len_Match,       sizeof(float));


	/* 计算PWM调制信号长度 */
	*len_Pwm_Mod = len_Match*PWM_Fs/Fs/2;

	/* BPSK调制过程 */
	/* 蝶形因子初始化 */
	//tw_gen(w_1024, N_1024);

	/* CRC校验 */
	memcpy(Crc_bits, Input_bits, sizeof(char)*len_input); /* 赋值信息序列 */
	memcpy(Crc_bits+len_input, tail_bit, sizeof(char)*LEN_TAIL); /* 加入结尾符 */
	crc(Crc_bits, len_crc, Crc_bits, crc_check_bit, LEN_CRC); /* CRC校验 */

	/* 递归系统卷积码编码 */
	Rsc_encoder(Crc_bits, len_crc, Encode_symbol, gen);//测试通过

	/* 随机交织 */
	Interleaver(Encode_symbol, len_encode, Interleave_symbol, index_2740);//测试通过

	/* 符号连接 */
	Symbol_combination(Trainsymbol, len_train, Interleave_symbol, len_encode, Send_symbol);

	/* 符号映射 */
	Symbol_Mapper(Send_symbol, len_Send_symbol, Symbol_map, CONSTELLATION);//测试通过

	/* 脉冲成形以及调制 */
	UpFIR(Symbol_map, len_symbol_map, Fs, Rb, pT_fft, len_pT, pT_order, N_1024, w_1024, CosCoeff, SinCoeff, Match_Data , Up_Spread);//测试通过,归一化后与matlab对比误差0.8%

	/*清零缓存*/
	cross_page_set0_float(Up_Spread , 1048576);

	/* 缓存空间 */
	Pwm_mod = (short *)&Up_Spread;

	/* PWM调制 */
	Overlap_Pwm(Match_Data, len_Match, Fs, PWM_Fs, Sawtooth, PWM_Fw, Pwm_lpf_fft, len_Pwm_lpf, Pwm_lpf_order, N_1024, w_1024, Pwm_mod );
	//结果存在缓存Up_Spread中

	/* 释放内存 */
	free(Crc_bits);
	free(Encode_symbol);
	free(Interleave_symbol);
	free(Send_symbol);
	free(Symbol_map);
	//free(Match_Data);
}


