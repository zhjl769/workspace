#ifndef _Main_Head_H_
#define _Main_Head_H_

#include "c674xdspinit.h"
#include "global.h"


//�û�Ӳ������ʹ�õĺ�����user_evmommapl138.c�еĺ���������

//��һ���ֵĳ����ļ���"evmomapl138.c"�У�������һЩ�޸ģ������д�����еĺ���EVMOMAPL138_init(void)
//�������Ҫ�޸ģ�ֱ������Ϊevmomapl138_bsl.lib�е���Ӧ����������
void USFUNC_delay(uint32_t num);

void init_psc_user(void);

void init_clocks_user(void);

void evm_init_omapl138(void);			//Ӳ��ȫ�ֳ�ʼ����������û��GEL�ļ������

void evm_init_omapl138_DDR2(void);		//DDR2ʹ�ó�ʼ����������û��GEL�ļ������

void evm_init_uart(uart_regs_t *uart, uint32_t baud_rate);

void evm_init_mcbsp();

void init_PaRAM_event4 (CSL_Edma3ccRegs *edma3ccRegs);
void init_PaRAM_event74 (CSL_Edma3ccRegs *edma3ccRegs);
void init_PaRAM_event75 (CSL_Edma3ccRegs *edma3ccRegs);
void evm_init_edma_setup(CSL_Edma3ccRegs *edma3ccRegs);

void AIC23_readRegister(uint8_t in_reg_addr, uint16_t *dest_buffer);
void AIC23_writeRegister(uint8_t in_reg_addr, uint16_t in_data);
void evm_init_aic23();

void evm_init_gpio_extern();//����ͬ���ⴥ������
void DSP_interrupt_ARM();

void set_gp2_1();
void clear_gp2_1();

void evm_init_timer_send(timer_regs_t *timer_x, uint32_t user_timer);
void evm_init_timer(timer_regs_t *timer_x, uint32_t user_timer);

void AD_start();
void AD_stop();

void open_EDMA_E4(CSL_Edma3ccRegs *edma3ccRegs);
void close_EDMA_E4(CSL_Edma3ccRegs *edma3ccRegs);

void transmit(uint16_t *signal, int n);

//����function.c�е����к���
void gen_sig(short *ref_fft, int sig_flag, short *chip_l, int refsig_len, double *f, double (*code)[8], int *w, int *temp_buffer1, int *temp_buffer2);
void gen_transmit_sig(unsigned short *trans_signal, int *trans_signal_len, int trans_signal_flag, double *f, double (*code)[8]);
void syn_config(char type, char cyc);
void configure_para();
void init_global_var(void);
void fifo(short *fifo_buffer, short *input_buffer, int n);
int d2i(double d);
int gen_twiddle_fft32x32(int *w, int n, double scale);
void get_fft(int *ref_fft, short *ref_sig, int n_sig, int *w, int n_w, int *temp_buffer);
void get_xcorr(int *correlation, int *sig_fft, short *ref_fft, int n, int *w, int *temp_1, int *temp_2);
void get_envelop(int *envelop, int *correlation, int *coef_fft, int n, int *w, int *temp_1, int *temp_2);
void max(int *max_val, int *max_num, int *x, int nx, int ind);
int mean(int *x, int nx, int ind);
int find_num(int *x, int nx, int abso, int ind);
void Tcp_Info(void);
void detection(struct Sig *sig, int *envelop, struct Para *parameter, int block_num, int close_num);
void get_delay(int y1, int y2, int y3, struct Sig *sig, int block_num, double turnaround, char work_mode);


#endif

