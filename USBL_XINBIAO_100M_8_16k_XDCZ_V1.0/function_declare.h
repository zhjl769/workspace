/*
 * function_declare.h
 *
 *  Created on: 2017-3-22
 *      Author: Administrator
 */

#ifndef FUNCTION_DECLARE_H_
#define FUNCTION_DECLARE_H_
/* ============================== 跨页清零函数 ================================== */
//void cross_page_set0_int(void *A , unsigned long int B);
void cross_page_set0_float(void *A , unsigned long int B);
void cross_page_copy_float_src(float *dest , float *src,unsigned long int B);
void cross_page_copy_float_dest(float *dest , float *src,unsigned long int B);
void cross_page_copy_float(float *dest , float *src,unsigned long int B);

/* ============================== Rsc_encoder 函数 ================================== */
/* 此函数的功能与文件的描述保持一致；input - 输入bit信息；len_input - 输入信息长度；out_put
 * - 卷积码编码后的输出bit信息；gen - 生成矩阵；
 * 注意: 这里仅仅完成RSC编码
 */
void Rsc_encoder(char *input, int len_input, char *output, short *gen);

/* ============================== rsc_transit 函数 ================================== */
static void rsc_transit(int output_p[], int trans_p[], int input, int g[], int KK, int nn);

/* ============================== rsc_enc_bit 函数 ================================== */
static int rsc_enc_bit(int state_out_p[], int input, int state_in, int g[], int KK, int nn);

/* ================================ rsc_tail 函数 ==================================== */
static void rsc_tail(int	tail_p[], int g[], int max_states, int mm);

/* ============================ parity_counter 函数 ================================== */
int parity_counter(int symbol, int length);

/* ================================ conv_encode 函数 ================================== */
static void conv_encode(int output_p[], int input[], int out0[], int state0[], int out1[],
		int state1[], int tail[],int KK, int LL, int nn);

/* ================================== itob 函数 ====================================== */
void itob(int	binvec_p[], int symbol, int length);

/* ================================== crc 函数 ======================================= */
/* 此函数的功能与文件的描述保持一致；input - 输入的比特信息；length_inpuit - 注意这是最终输
 * 出长度；gen - CRC校验多项式；length_gen - CRC校验多项式的长度；
 * 注意：输入的信息实际长度是length_input - length_gen + 1, 输入矩阵后面补length_gen - 1个
 * 零再参与运算
 */
void crc(char* input, int length_input, char* output, char* gen, int length_gen);
/* ============================= Interleaver函数 ===================================== */
/*
Input_code_bits:the coded input bits
Input_code_bits_length:the length of the coded input bits
Output_interleave_bits:the output interleave bits
Interleaver_index:the index of the interleave
 */
void Interleaver(char * Input_code_bits, int Input_code_bits_length, char * Output_interleave_bits, short * Interleaver_index);

/* ============================ Symbol_combination 函数 ============================= */
/* Trains - 训练序列； length_Trains - 训练序列的长度； Transmits - 信息序列长度；
 * length_Transmits - 信息序列的长度； Send_symbol - 发送符号
 */
void Symbol_combination(char * Trains, int length_Trains, char * Transmits, int length_Transmits, char * Send_symbol);

/* ================================ Symbol_Mapper 函数 ================================ */
/* This function modulates the Input signal according to the constellation.
 * Input_bits:Input bits
 * input_length: length of the input bits
 * symbol_map:the output bits
 * constellation:BPSK/QPSK constellation */
void Symbol_Mapper(char * Input_bits, int input_length,float *symbol_map, int constellation);

/* =================================== 宏定义声明 ===================================== */
/* FFT点数 */
#define N_1024 (1024)

/* ==================================== 函数实现 ====================================== */

/* =================================== 蝶形因子初始化 ================================= */
/* tw_gen 函数用于初始化DSPF_sp_fftSPxSP函数及DSPF_sp_ifftSPxSP函数所需的蝶形因子；
 * n - fft或ifft的点数
 */
//void tw_gen (float *w, int n);

/* ====================================== UpFIR 函数 ================================== */
/* 此函数用于脉冲成型及带通滤波；Spread - 待发送的扩频序列（符号）；len_Spread - 扩频序列的长度；
 * Fs - 采样频率；Rc - 码片速率；filter_fft - 滤波器的频域形式；len_filter - 滤波器长度；
 * filter_order - 滤波器阶数；N_fft - fft点数，这里指的的是重叠保留法的fft点数；w - fft和ifft
 * 所需的蝶形因子；CosCoeff - 调试系数实部；SinCoeff - 调制系数虚部；Match - 带通调制信号
 */
void UpFIR(float *Spread, int len_Spread,long int fss, int Rc,
		   float *filter_fft, int len_filter, int filter_order, int N_fft, float *w,
		   float *CosCoeff, float *SinCoeff,
		   float *Match , float *A);

/* ==================================== Overlap_Save 函数 ============================== */
/* 此函数采用重叠保留完成滤波功能； input - 输入信号； len_input - 输入信号长度； filter_fft -
 * 滤波器的频域形式； len_filter - 滤波器的长度； filter_order - 滤波器阶数； N_fft - fft点数；
 * w - 蝶形因子； output - 滤波输出信号
 */
void Overlap_Save(float *input,long int len_input, float *filter_fft, int len_filter, int filter_order, int N_fft, float *output);

/* ==================================== Overlap_Pwm 函数 ============================= */
/* 此函数主要完成基于三角波比较的PWM调制； Match - 带通调制后的信号； len_Match - 带通信号长度；
 * Fs - 采样频率； Pwm_Fs - PWM调制的采样频率； Sawtooth - 三角波波形系数； Pwm_Fw - 三角波基频；
 * filter_fft - 滤波器的频域形式； len_filter - 滤波器的长度； filter_order - 滤波器阶数；
 * N_fft - fft点数； w,wi - 蝶形因子； Pwm_mod - PWM调制信号
 * 注意: 这里主要采用了重叠保留法的基本思想从而避免大块内存占用情况
 */
void Overlap_Pwm(float *Match , long int len_Match,long int fss,long int Pwm_Fs,float *Sawtooth,long int Pwm_Fw,
		float *filter_fft, int len_filter, int filter_order, int N_fft, float *w,
		volatile short *Pwm_mod );

/* ========================== Create_signal 函数 ===================================== */
/* 此函数的功能与描述保持一致，其中Input_bits - 信源编码后的比特信息；len_input - 输入比特信息
 * 的长度；Pwm_mod - PWM调制后的发送信号的存储位置；len_Pwm - Pwm调制后的发送信号的长度
 */
void Create_signal(char *Input_bits,long int len_input, volatile short *Pwm_mod, long int *len_Pwm_Mod);















#endif /* FUNCTION_DECLARE_H_ */
