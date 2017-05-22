/*
 * function_declare.h
 *
 *  Created on: 2017-3-22
 *      Author: Administrator
 */

#ifndef FUNCTION_DECLARE_H_
#define FUNCTION_DECLARE_H_
/* ============================== ��ҳ���㺯�� ================================== */
//void cross_page_set0_int(void *A , unsigned long int B);
void cross_page_set0_float(void *A , unsigned long int B);
void cross_page_copy_float_src(float *dest , float *src,unsigned long int B);
void cross_page_copy_float_dest(float *dest , float *src,unsigned long int B);
void cross_page_copy_float(float *dest , float *src,unsigned long int B);

/* ============================== Rsc_encoder ���� ================================== */
/* �˺����Ĺ������ļ�����������һ�£�input - ����bit��Ϣ��len_input - ������Ϣ���ȣ�out_put
 * - �������������bit��Ϣ��gen - ���ɾ���
 * ע��: ����������RSC����
 */
void Rsc_encoder(char *input, int len_input, char *output, short *gen);

/* ============================== rsc_transit ���� ================================== */
static void rsc_transit(int output_p[], int trans_p[], int input, int g[], int KK, int nn);

/* ============================== rsc_enc_bit ���� ================================== */
static int rsc_enc_bit(int state_out_p[], int input, int state_in, int g[], int KK, int nn);

/* ================================ rsc_tail ���� ==================================== */
static void rsc_tail(int	tail_p[], int g[], int max_states, int mm);

/* ============================ parity_counter ���� ================================== */
int parity_counter(int symbol, int length);

/* ================================ conv_encode ���� ================================== */
static void conv_encode(int output_p[], int input[], int out0[], int state0[], int out1[],
		int state1[], int tail[],int KK, int LL, int nn);

/* ================================== itob ���� ====================================== */
void itob(int	binvec_p[], int symbol, int length);

/* ================================== crc ���� ======================================= */
/* �˺����Ĺ������ļ�����������һ�£�input - ����ı�����Ϣ��length_inpuit - ע������������
 * �����ȣ�gen - CRCУ�����ʽ��length_gen - CRCУ�����ʽ�ĳ��ȣ�
 * ע�⣺�������Ϣʵ�ʳ�����length_input - length_gen + 1, ���������油length_gen - 1��
 * ���ٲ�������
 */
void crc(char* input, int length_input, char* output, char* gen, int length_gen);
/* ============================= Interleaver���� ===================================== */
/*
Input_code_bits:the coded input bits
Input_code_bits_length:the length of the coded input bits
Output_interleave_bits:the output interleave bits
Interleaver_index:the index of the interleave
 */
void Interleaver(char * Input_code_bits, int Input_code_bits_length, char * Output_interleave_bits, short * Interleaver_index);

/* ============================ Symbol_combination ���� ============================= */
/* Trains - ѵ�����У� length_Trains - ѵ�����еĳ��ȣ� Transmits - ��Ϣ���г��ȣ�
 * length_Transmits - ��Ϣ���еĳ��ȣ� Send_symbol - ���ͷ���
 */
void Symbol_combination(char * Trains, int length_Trains, char * Transmits, int length_Transmits, char * Send_symbol);

/* ================================ Symbol_Mapper ���� ================================ */
/* This function modulates the Input signal according to the constellation.
 * Input_bits:Input bits
 * input_length: length of the input bits
 * symbol_map:the output bits
 * constellation:BPSK/QPSK constellation */
void Symbol_Mapper(char * Input_bits, int input_length,float *symbol_map, int constellation);

/* =================================== �궨������ ===================================== */
/* FFT���� */
#define N_1024 (1024)

/* ==================================== ����ʵ�� ====================================== */

/* =================================== �������ӳ�ʼ�� ================================= */
/* tw_gen �������ڳ�ʼ��DSPF_sp_fftSPxSP������DSPF_sp_ifftSPxSP��������ĵ������ӣ�
 * n - fft��ifft�ĵ���
 */
//void tw_gen (float *w, int n);

/* ====================================== UpFIR ���� ================================== */
/* �˺�������������ͼ���ͨ�˲���Spread - �����͵���Ƶ���У����ţ���len_Spread - ��Ƶ���еĳ��ȣ�
 * Fs - ����Ƶ�ʣ�Rc - ��Ƭ���ʣ�filter_fft - �˲�����Ƶ����ʽ��len_filter - �˲������ȣ�
 * filter_order - �˲���������N_fft - fft����������ָ�ĵ����ص���������fft������w - fft��ifft
 * ����ĵ������ӣ�CosCoeff - ����ϵ��ʵ����SinCoeff - ����ϵ���鲿��Match - ��ͨ�����ź�
 */
void UpFIR(float *Spread, int len_Spread,long int fss, int Rc,
		   float *filter_fft, int len_filter, int filter_order, int N_fft, float *w,
		   float *CosCoeff, float *SinCoeff,
		   float *Match , float *A);

/* ==================================== Overlap_Save ���� ============================== */
/* �˺��������ص���������˲����ܣ� input - �����źţ� len_input - �����źų��ȣ� filter_fft -
 * �˲�����Ƶ����ʽ�� len_filter - �˲����ĳ��ȣ� filter_order - �˲��������� N_fft - fft������
 * w - �������ӣ� output - �˲�����ź�
 */
void Overlap_Save(float *input,long int len_input, float *filter_fft, int len_filter, int filter_order, int N_fft, float *output);

/* ==================================== Overlap_Pwm ���� ============================= */
/* �˺�����Ҫ��ɻ������ǲ��Ƚϵ�PWM���ƣ� Match - ��ͨ���ƺ���źţ� len_Match - ��ͨ�źų��ȣ�
 * Fs - ����Ƶ�ʣ� Pwm_Fs - PWM���ƵĲ���Ƶ�ʣ� Sawtooth - ���ǲ�����ϵ���� Pwm_Fw - ���ǲ���Ƶ��
 * filter_fft - �˲�����Ƶ����ʽ�� len_filter - �˲����ĳ��ȣ� filter_order - �˲���������
 * N_fft - fft������ w,wi - �������ӣ� Pwm_mod - PWM�����ź�
 * ע��: ������Ҫ�������ص��������Ļ���˼��Ӷ��������ڴ�ռ�����
 */
void Overlap_Pwm(float *Match , long int len_Match,long int fss,long int Pwm_Fs,float *Sawtooth,long int Pwm_Fw,
		float *filter_fft, int len_filter, int filter_order, int N_fft, float *w,
		volatile short *Pwm_mod );

/* ========================== Create_signal ���� ===================================== */
/* �˺����Ĺ�������������һ�£�����Input_bits - ��Դ�����ı�����Ϣ��len_input - ���������Ϣ
 * �ĳ��ȣ�Pwm_mod - PWM���ƺ�ķ����źŵĴ洢λ�ã�len_Pwm - Pwm���ƺ�ķ����źŵĳ���
 */
void Create_signal(char *Input_bits,long int len_input, volatile short *Pwm_mod, long int *len_Pwm_Mod);















#endif /* FUNCTION_DECLARE_H_ */
