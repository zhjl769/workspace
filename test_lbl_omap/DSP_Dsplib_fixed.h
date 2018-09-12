/*

DSPLIB����⺯������������ͷ�ļ�


*/

#ifndef _DSP_DSPLIB_FIXED_H_
#define _DSP_DSPLIB_FIXED_H_	
//�������Ҫ�Ķ���⺯���������ڴ˴�ͷ�ļ����������

int d2i(double d);
short d2s(double d);
int gen_twiddle_fft32x32(int *w,int n,double scale);
int gen_twiddle_fft16x32(short *w,int n);
void DSP_fir_gen(short * x, short * h, short * r, int nh, int nr);
void DSP_fft32x32(int * w,int nx,int * x,int * y);
void DSP_fft16x32(short * w,int nx,int * x,int * y);
void DSP_ifft32x32(int * w,int nx,int * x,int * y);
#endif
