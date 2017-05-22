/* ===================================================================================
 * 文件名：UpFIR_and_Pwm_Modulation.c
 * 日期：2016年10月12日
 * 作者：xiaop
 *
 * 描述：UpFIR_and_Pwm_Modulation.h和UpFIR_and_Pwm_Modulation.c这两个文件主要实现的功能是
 * 		完成脉冲成型、带通调制及PWM调制等功能
 * 版本：1.0
 * -----------------------------------------------------------------------------------
 * 修改内容: 修正overlap_save函数中的if判断语句，存在重复赋值情况导致死循环，有可能触发
 * 修改时间: 2017年3月7日
 * 修改人: xiaop
 * -----------------------------------------------------------------------------------
 * 修改内容: 修正PWM调制函数中二次生成信号存在静态变量重新赋值问题
 * 修改时间: 2017年3月15日
 * 修改人: xiaop
 * -----------------------------------------------------------------------------------
 * 修改内容: 使用重叠保留法完成PWM调制过程
 * 修改时间: 2017年3月18日
 * 修改人: xiaop
 * ===================================================================================
 */

/* ================================= 宏定义声明 ====================================== */
/* 系统模块声明 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Dsplib.h>
/* 数学库模块声明 */
//#include "DSPF_sp_fftSPxSP.h"
//#include "DSPF_sp_ifftSPxSP.h"
//#include "DSPF_sp_maxval.h"
#include "function_declare.h"
/* =============================== 全局变量声明 ======================================= */
//#pragma DATA_SECTION(input_seg,      ".input_seg");
//#pragma DATA_SECTION(input_seg_fft,  ".input_seg_fft");

//#pragma DATA_ALIGN(input_seg, 8);
//#pragma DATA_ALIGN(input_seg_fft, 8);

float input_seg[2*N_1024];
float input_seg_fft[2*N_1024];

#pragma DATA_SECTION(TEMP_DARAM, "temp");
short TEMP_DARAM[2048];//4KB空间

//#pragma DATA_SECTION(brev, ".brev");
//unsigned char brev[64] = {
//    0x0, 0x20, 0x10, 0x30, 0x8, 0x28, 0x18, 0x38,
//    0x4, 0x24, 0x14, 0x34, 0xc, 0x2c, 0x1c, 0x3c,
//    0x2, 0x22, 0x12, 0x32, 0xa, 0x2a, 0x1a, 0x3a,
//    0x6, 0x26, 0x16, 0x36, 0xe, 0x2e, 0x1e, 0x3e,
//    0x1, 0x21, 0x11, 0x31, 0x9, 0x29, 0x19, 0x39,
//    0x5, 0x25, 0x15, 0x35, 0xd, 0x2d, 0x1d, 0x3d,
//    0x3, 0x23, 0x13, 0x33, 0xb, 0x2b, 0x1b, 0x3b,
//    0x7, 0x27, 0x17, 0x37, 0xf, 0x2f, 0x1f, 0x3f
//};

/* ================================== 函数实现 ====================================== */
/* ================================ tw_gen 函数 ===================================== */
//void tw_gen (float *w, int n)
//{
//    int i, j, k;
//    double x_t, y_t, theta1, theta2, theta3;
//    const double PI = 3.141592654;
//
//    for (j = 1, k = 0; j <= n >> 2; j = j << 2)
//    {
//        for (i = 0; i < n >> 2; i += j)
//        {
//            theta1 = 2 * PI * i / n;
//            x_t = cos (theta1);
//            y_t = sin (theta1);
//            w[k] = (float) x_t;
//            w[k + 1] = (float) y_t;
//
//            theta2 = 4 * PI * i / n;
//            x_t = cos (theta2);
//            y_t = sin (theta2);
//            w[k + 2] = (float) x_t;
//            w[k + 3] = (float) y_t;
//
//            theta3 = 6 * PI * i / n;
//            x_t = cos (theta3);
//            y_t = sin (theta3);
//            w[k + 4] = (float) x_t;
//            w[k + 5] = (float) y_t;
//            k += 6;
//        }
//    }
//}

/* =========================== UpFIR 函数 ============================================ */
void UpFIR(float *Spread, int len_Spread,long int fss, int Rc,
		float *filter_fft, int len_filter, int filter_order, int N_fft, float *w,
		float *CosCoeff, float *SinCoeff,
		float *Match , float *A)
{
	volatile long int i;
	long int OVF = fss/Rc;
	//float *Up_Spread = (float *)calloc(OVF*len_Spread, sizeof(float));

	//	if(Up_Spread == NULL)
	//		printf("*Up_Spread is wrong!");

	for(i = 0; i < len_Spread; i++)
		A[OVF*i] = Spread[i];

	Overlap_Save(A, OVF*len_Spread, filter_fft, len_filter, filter_order, N_fft, A);

	//memset(Match, 0, OVF*len_Spread*sizeof(float));
	//////////////////////////////////////////////////
	cross_page_set0_float(Match , OVF*len_Spread);
	//////////////////////////////////////////////////
	for(i = 0; i < OVF*len_Spread/2; i++)
		Match[2*i] = A[2*i]*CosCoeff[i%4]-A[2*i+1]*SinCoeff[i%4];

	//free(Up_Spread);

}

/* ========================== Overlap_Save 函数 ====================================== */
void Overlap_Save(float *input,long int len_input, float *filter_fft, int len_filter, int filter_order, int N_fft, float *output)
{
	volatile long int i, j, k ,xi;
	//int N_min;
	int constellation = log2(N_fft);
	int len_seg = N_fft-(len_filter-1);
	int len_seg_cx = 2*len_seg;

	int Num_seg;
	int Num_input;
	int End_seg = len_input%len_seg_cx;

	volatile int Loop;
	int first_seg = len_seg_cx-filter_order;
	int last_seg = (len_input-first_seg)%len_seg_cx;

	float *t;

	//-------------------------------------------------------
	if((len_input/2+len_filter-1)%len_seg)
		Num_seg = (len_input/2+len_filter-1)/len_seg+1;
	else
		Num_seg = (len_input/2+len_filter-1)/len_seg;
	//-------------------------------------------------------


	//-------------------------------------------------------
	if(End_seg)
		Num_input = len_input/len_seg_cx+1;
	else
		Num_input = len_input/len_seg_cx;
	//-------------------------------------------------------


	//-------------------------------------------------------
	if(last_seg)
		Loop = (len_input-first_seg)/len_seg_cx+1;
	else
		Loop = (len_input-first_seg)/len_seg_cx;
	//-------------------------------------------------------

	//-------------------------------------------------------
	//	if(constellation%2)
	//		N_min = 2;
	//	else
	//		N_min = 4;

	//-------------------------------------------------------


	//-------------------------------------------------------
	t = calloc(2*(len_filter-1), sizeof(float));

	//	if(t == NULL)
	//		printf("*t is wrong!\n");

	//-------------------------------------------------------


	for(i = 0; i < Num_seg; i++)//Num_seg
	{
		xi = i*len_seg_cx;

		//-------------------------------------------------------------------------------------
		memset(input_seg, 0, 2*N_fft*sizeof(float));
		memcpy(input_seg, t, 2*(len_filter-1)*sizeof(float));

		if(i < Num_input-1)
			//memcpy(input_seg+2*(len_filter-1), input+xi, len_seg_cx*sizeof(float));
			////////////////////////////////////////////////////////////////////
			cross_page_copy_float(input_seg+2*(len_filter-1) , input+xi , len_seg_cx);
			////////////////////////////////////////////////////////////////////
		else if(i == Num_input-1)
		{
			if(!End_seg)
				//memcpy(input_seg+2*(len_filter-1), input+xi, len_seg_cx*sizeof(float));
				////////////////////////////////////////////////////////////////////
				cross_page_copy_float(input_seg+2*(len_filter-1) , input+xi , len_seg_cx);
				////////////////////////////////////////////////////////////////////
			else
			{
				//memcpy(input_seg+2*(len_filter-1), input+xi, End_seg*sizeof(float));
				////////////////////////////////////////////////////////////////////
				cross_page_copy_float(input_seg+2*(len_filter-1) , input+xi , End_seg);
				////////////////////////////////////////////////////////////////////
				memset(input_seg+2*(len_filter-1)+End_seg, 0, (len_seg_cx-End_seg)*sizeof(float));
			}
		}
		else if((Num_input < Num_seg)&&(i > Num_input-1))
			memset(input_seg+2*(len_filter-1), 0, len_seg_cx*sizeof(float));

		memcpy(t, input_seg+len_seg_cx, 2*(len_filter-1)*sizeof(float));
		//-------------------------------------------------------------------------------------

		//		DSPF_sp_fftSPxSP(N_fft, input_seg, w, input_seg_fft, brev, N_min, 0, N_fft);
		////////////////////////////////////////////////////////////////////////
		for(k=0;k<(N_fft*2);k++)
		{
			TEMP_DARAM[k] = (short)(input_seg[k]*1000);
		}
		cfft_NOSCALE(TEMP_DARAM,N_fft);//傅里叶变换
		cbrev(TEMP_DARAM,TEMP_DARAM,N_fft);
		for(k=0;k<(N_fft*2);k++)
		{
			input_seg_fft[k] = ((float)TEMP_DARAM[k])/15030.04;
		}
		////////////////////////////////////////////////////////////////////////
		for(j = 0; j < N_fft; j++)
		{
			input_seg[2*j] = input_seg_fft[2*j]*filter_fft[2*j]-input_seg_fft[2*j+1]*filter_fft[2*j+1];
			input_seg[2*j+1] = input_seg_fft[2*j+1]*filter_fft[2*j]+input_seg_fft[2*j]*filter_fft[2*j+1];
		}

		//		DSPF_sp_ifftSPxSP(N_fft, input_seg, w, input_seg_fft, brev, 4, 0, N_fft);
		///////////////////////////////////////////////////////////////////////
		for(k=0;k<(N_fft*2);k++)
		{
			TEMP_DARAM[k] = (short)(input_seg[k]*1000);
		}

		cifft_NOSCALE(TEMP_DARAM,N_fft);//傅里叶变换
		cbrev(TEMP_DARAM,TEMP_DARAM,N_fft);
		for(k=0;k<(N_fft*2);k++)
		{
			input_seg_fft[k] = ((float)TEMP_DARAM[k])/24832.86;
		}
		////////////////////////////////////////////////////////////////////////
		if(i == 0)
			memcpy(output, input_seg_fft+2*(len_filter-1)+filter_order, first_seg*sizeof(float));
		else if(i < Loop)
			//memcpy(output+xi-filter_order, input_seg_fft+2*(len_filter-1), len_seg_cx*sizeof(float));
			////////////////////////////////////////////////////////////////////
			cross_page_copy_float(output+xi-filter_order , input_seg_fft+2*(len_filter-1) , len_seg_cx);
			////////////////////////////////////////////////////////////////////
		else if(i == Loop)
		{
			if(last_seg)
				//memcpy(output+xi-filter_order, input_seg_fft+2*(len_filter-1), last_seg*sizeof(float));
				////////////////////////////////////////////////////////////////////
				cross_page_copy_float(output+xi-filter_order , input_seg_fft+2*(len_filter-1) , last_seg);
				////////////////////////////////////////////////////////////////////

			else
				//memcpy(output+xi-filter_order, input_seg_fft+2*(len_filter-1), len_seg_cx*sizeof(float));
				////////////////////////////////////////////////////////////////////
				cross_page_copy_float(output+xi-filter_order , input_seg_fft+2*(len_filter-1) , len_seg_cx);
				////////////////////////////////////////////////////////////////////
		}
	}

	free(t);
}

/* =========================== Pwm_Modulation 函数 =================================== */
void Overlap_Pwm(float *Match , long int len_Match,long int fss,long int Pwm_Fs,
		float *Sawtooth,long int Pwm_Fw,
		float *filter_fft, int len_filter, int filter_order, int N_fft, float *w,
		volatile short *Pwm_mod )
{

	/* 分配静态变量 */
	volatile long int i , j , k;
	//int N_min; /* 定义整型变量用于循环 */
	int OVF = Pwm_Fs/fss; /* 升采样倍数 */
	int constellation = log2(N_fft); /* 用于计算FFT的算法 */
	long int Lx = (long int)len_Match*(long int)OVF/2; /* 获取Pwm_data的长度 */
	int M = len_filter; /* 获取插值滤波器的长度 */
	int L = N_fft-(M-1); /* 数据分段大小 */
	int Num_seg = 0; /* 初始化分段数 */
	long int ccount = 0; /* 重叠保留部分数据计数器 */
	long int count = 0; /* PWM调制部分计数器 */
	// ---------------------------------------------------------

	/* 分配动态内存 */
	float *t = (float *)calloc(2*(len_filter-1), sizeof(float)); /* 重叠保留数据部分 */
	float *x = (float *)calloc(2*L, sizeof(float)); /* 数据更新部分 */
	float *refer = (float *)calloc(Pwm_Fs/Pwm_Fw, sizeof(float)); /* 分配空间用于三角波幅度改变 */

	//	if ((t==NULL) || (x==NULL) || (refer==NULL))
	//		printf("Memory allocation is failed in Overlap_Pwm!!!\n");
	// ---------------------------------------------------------

	/* 修正三角波幅度 */
	for(j = 0; j < Pwm_Fs/Pwm_Fw; j++)
	{
		refer[j] = Sawtooth[j]*17159/0.6;
	}
	// ---------------------------------------------------------

	/* 计算分段大小，实际上完成的是向上取整 */
	if((Lx+M-1)%L)
		Num_seg = (Lx+M-1)/L+1;
	else
		Num_seg = (Lx+M-1)/L;
	// ---------------------------------------------------------

	/* 选择FFT算法参数 */
	//	if(constellation%2)
	//		N_min = 2;
	//	else
	//		N_min = 4;
	// ---------------------------------------------------------

	for (i=0; i<Num_seg; i++)//Num_seg
	{
		/* 数据升采样，循环威武模式 */
		for (j=0; j<L; j++)
		{
			if ( ((ccount%OVF)==0) && (ccount < Lx) )
			{
				x[2*j] = Match[2*ccount/OVF];
				x[2*j+1] = Match[2*ccount/OVF+1];
			}
			else
			{
				x[2*j] = 0;
				x[2*j+1] = 0;
			}
			ccount += 1;
		}
		// ---------------------------------------------------------

		/* 重叠保留核心部分 */
		memset(input_seg, 0, 2*N_fft*sizeof(float)); /* 清空处理变量 */
		memcpy(input_seg, t, 2*filter_order*sizeof(float)); /* 获取保留部分数据 */
		memcpy(input_seg+2*filter_order, x, 2*L*sizeof(float)); /* 获取数据更新部分 */
		memcpy(t, input_seg+2*L, 2*filter_order*sizeof(float)); /* 更新重叠保留部分数据 */

		//		DSPF_sp_fftSPxSP(N_fft, input_seg, w, input_seg_fft, brev, N_min, 0, N_fft);
		////////////////////////////////////////////////////////////////////////
		for(k=0;k<(N_fft*2);k++)
		{
			TEMP_DARAM[k] = (short)(input_seg[k]*2000);
		}
		cfft_NOSCALE(TEMP_DARAM,N_fft);//傅里叶变换
		cbrev(TEMP_DARAM,TEMP_DARAM,N_fft);
		for(k=0;k<(N_fft*2);k++)
		{
			input_seg_fft[k] = ((float)TEMP_DARAM[k])/28943.54;
		}
		////////////////////////////////////////////////////////////////////////

		for(j = 0; j < N_fft; j++)
		{
			input_seg[2*j] = input_seg_fft[2*j]*filter_fft[2*j]-input_seg_fft[2*j+1]*filter_fft[2*j+1];
			input_seg[2*j+1] = input_seg_fft[2*j+1]*filter_fft[2*j]+input_seg_fft[2*j]*filter_fft[2*j+1];
		}

		//		DSPF_sp_ifftSPxSP(N_fft, input_seg, w, input_seg_fft, brev, 4, 0, N_fft);
		////////////////////////////////////////////////////////////////////////
		for(k=0;k<(N_fft*2);k++)
		{
			TEMP_DARAM[k] = (short)(input_seg[k]*12000);
		}
		cifft_NOSCALE(TEMP_DARAM,N_fft);//傅里叶变换
		cbrev(TEMP_DARAM,TEMP_DARAM,N_fft);
		for(k=0;k<(N_fft*2);k++)
		{
			input_seg_fft[k] = ((float)TEMP_DARAM[k]);//通过调整三角波的幅度直接比较，此处就不再归一化了
		}
		////////////////////////////////////////////////////////////////////////

		// ---------------------------------------------------------

		/* 循环模式进行PWM调制过程 */
		if (i==0)
		{
			memset(x, 0, 2*L*sizeof(float)); /* 清空数据更新部分，用作临时变量 */
			memcpy(x, input_seg_fft+2*(M-1)+filter_order, (2*L-filter_order)*sizeof(float)); /* 获取插值结果 */
			//////////////////////////////////////////////////////////////////////
			//cross_page_copy_float_dest(T , x , 2*L-filter_order);
			//////////////////////////////////////////////////////////////////////
			for (j=0; j<(L-filter_order/2); j++)
			{
				if (count >= Lx) /* bug修正版本 */
					break;
				// ---------------------------------------------------------

				if (x[2*j] < refer[count%10])
					Pwm_mod[count] = 0x03;
				//				  	Pwm_mod[count] = 0x10400000;
				else
					Pwm_mod[count] = 0x05;
				//					Pwm_mod[count] = 0x12000000;
				// ---------------------------------------------------------

				count += 1;
				// ---------------------------------------------------------
			}
			// ---------------------------------------------------------
		}
		else
		{
			memset(x, 0, 2*L*sizeof(float)); /* 清空数据更新部分，用作临时变量 */
			memcpy(x, input_seg_fft+2*(M-1), 2*L*sizeof(float)); /* 获取插值结果 */
			//////////////////////////////////////////////////////////////////////
			//cross_page_copy_float_dest(T+2*L-filter_order+(i-1)*2*L , x , 2*L);
			//////////////////////////////////////////////////////////////////////
			for (j=0; j<L; j++)
			{
				if (count >= Lx) /* bug修正版本 */
					break;
				// ---------------------------------------------------------

				if (x[2*j] < refer[count%10])
					Pwm_mod[count] = 0x03;
				//					Pwm_mod[count] = 0x10400000;
				else
					Pwm_mod[count] = 0x05;
				//					Pwm_mod[count] = 0x12000000;
				// ---------------------------------------------------------

				count += 1;
				// ---------------------------------------------------------
			} // end of for j
			// ---------------------------------------------------------
		} // end of if i
		// ---------------------------------------------------------
	} // end of for i
	// ---------------------------------------------------------

	/* 释放动态内存 */
	free(refer);
	free(x);
	free(t);
	// ---------------------------------------------------------
}
