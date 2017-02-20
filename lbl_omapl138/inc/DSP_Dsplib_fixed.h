/*

DSPLIB定点库函数的连接声明头文件


*/

#ifndef _DSP_DSPLIB_FIXED_H_
#define _DSP_DSPLIB_FIXED_H_	
//如果有需要的定点库函数，可以在此处头文件中添加声明

int d2i(double d);
short d2s(double d);
int gen_twiddle_fft32x32(int *w,int n,double scale);
int gen_twiddle_fft16x32(short *w,int n);
void DSP_fir_gen(short * x, short * h, short * r, int nh, int nr);
void DSP_fft32x32(int * w,int nx,int * x,int * y);
void DSP_fft16x32(short * w,int nx,int * x,int * y);
void DSP_ifft32x32(int * w,int nx,int * x,int * y);
#endif
