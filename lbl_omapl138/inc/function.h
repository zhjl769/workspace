#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <string.h>
#include <math.h>
#include <c6x.h>
#include "user_evmomapl138_header.h"


extern char Time_over;
extern int Max_Buffer;
extern char ddr2_cmd[1024];
extern const char arm_syn_flag;
extern const char arm_syn_type;
extern const char arm_syn_cyc;

extern char syn_type;
extern char syn_cyc;

extern struct Para Parameter_sig;
extern const char arm_parameter_flag;
extern const char arm_parameter_snr_max;
extern const char arm_parameter_abs_snr_threshold;
extern const char arm_parameter_rel_snr_threshold;
extern const char arm_parameter_rel2_snr_threshold;
extern const char arm_parameter_min_pulse_width;
extern const char arm_parameter_max_pulse_width;
extern const char arm_parameter_w_before;
extern const char arm_parameter_w_all;
extern const char arm_parameter_w_after;
extern const char arm_parameter_close_time;
extern const char arm_parameter_th_noise;

extern char sys_gain;
extern char Work_mode;
extern int gain_db[2];
extern double Turnaround[4];
extern char Start_flag;

extern const char arm_work_mode_flag;
extern const char arm_work_mode;
extern const char arm_sys_gain;
extern const char arm_beacon_flag;
extern const char arm_beacon_turnaround[4];
extern const char arm_start_flag;

extern char Syn_flag;
extern char Rx_flag;
extern char Pp_flag;
extern int Block_num;
extern int Close_num;
extern int send_num;

extern short buffer1[IN_LENGTH];
extern short buffer2[IN_LENGTH];
extern short Fifo1[3*IN_LENGTH];
extern int Correlation1[IN_LENGTH];
extern int Correlation2[IN_LENGTH];
extern int Correlation3[IN_LENGTH];
extern int Correlation4[IN_LENGTH];
extern int Envelop1[4*IN_LENGTH];
extern int Envelop2[4*IN_LENGTH];
extern int Envelop3[4*IN_LENGTH];
extern int Envelop4[4*IN_LENGTH];
extern struct Sig Sig1;
extern struct Sig Sig2;
extern struct Sig Sig3;
extern struct Sig Sig4;
extern double ddr2_delay[4];
extern int ddr2_corr_pos[4];
extern double ddr2_envelop_max[4];
extern int ddr2_real_pulse_width[4];
extern double ddr2_noise_std[4];
extern int ddr2_snr_out[4];
extern char ddr2_beacon_status[4];
extern char ddr2_beacon_detection_flag[4];
extern char ddr2_beacon_detection_flag_num[4];
extern struct Para ddr2_parameter;


void evm_init_timer(timer_regs_t *timer_x, uint32_t user_timer);
void evm_init_uart(uart_regs_t *uart);
int d2i(double d);
int gen_twiddle_fft32x32(int *w,int n,double scale);
void DSP_fft32x32(int * w,int nx,int * x,int * y);
void DSP_ifft32x32(int * w,int nx,int * x,int * y);

#endif  



