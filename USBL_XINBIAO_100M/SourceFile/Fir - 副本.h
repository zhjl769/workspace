/*
 * Fir.h
 *
 *  Created on: 2017-2-27
 *      Author: Administrator
 */

#ifndef FIR_H_
#define FIR_H_

#include"math.h"
#include"stdio.h"

/*n:滤波器的阶数,n是奇数时可用来设计各种滤波器
band:滤波器类型1、2、3、4分别对应1低通，2高通，3带通，4带阻
fln:通带下边界频率
fhn:通带上边界频率
wn:窗函数的类型1-7分别对应：1矩形窗、2图基窗、3三角窗、4汉宁窗、5海明窗、6布莱克曼窗、7凯塞窗
h:长度为n+1,存放滤波器的系数
fs:采样频率*/

void firwin(int n, int band,double  fln,double fhn,int wn,short *h)
//void firwin(n, band, fln, fhn, wn, h)
//int n, band, wn;
//double fln, fhn, h[];
{
	int i, n2, mid;
	double s, pi, wc1, wc2, beta, delay, fs;
	fs = 40000;//采样率
	double window(int type, int n, int i, double beta);//窗函数的计算
	beta = 0.0;
	if (wn == 7)//只有凯塞窗需要输入系数beta
	{
		printf("input beta parameter of Kaiser window(2<beta<10)\n");
		scanf("%1f", &beta);
	}
	pi = 4.0*atan(1.0);//pi=PI;

	if ((n % 2) == 0)/*如果阶数n是偶数*/
	{
		n2 = n / 2 + 1;/**/
		mid = 1;//
	}
	else
	{
		n2 = n / 2;//n是奇数,则窗口长度为偶数
		mid = 0;//
	}
	delay = n / 2.0;
	wc1 = 2.0*pi*fln;//
	if (band >= 3) /*先判断用户输入的数据，如果band参数大于3*/
	{
		wc2 = 2.0*pi*fhn;
	}

	switch (band)
	{
	case 1:
	{	for (i = 0; i <= n2; i++)
	{
		s = i - delay;//
		h[i] = (sin(wc1*s / fs) / (pi*s))*window(wn, n + 1, i, beta);//低通,窗口长度=阶数+1，故为n+1
		h[n - i] = h[i];
	}
	if (mid == 1)	h[n / 2] = wc1 / pi/fs;//n为偶数时，修正中间值系数
	break;
	}
	case 2:
	{	for (i = 0; i <= n2; i++)
	{
		s = i - delay;
		h[i] = (sin(pi*s) - sin(wc1*s / fs)) / (pi*s);//高通-//对
		h[i] = h[i] * window(wn, n + 1, i, beta);
		h[n - i] = h[i];
	}
	if (mid == 1)	h[n / 2] = 1.0 - wc1 / pi/fs;//对
	break;
	}

	case 3:
	{	for (i = 0; i < n2; i++)
	{
		s = i - delay;
		h[i] = (sin(wc2*s / fs) - sin(wc1*s / fs)) / (pi*s);//带通-//对
		h[i] = h[i] * window(wn, n + 1, i, beta);
		h[n - i] = h[i];
	}
	if (mid == 1)	h[n / 2] = (wc2 - wc1) / pi/fs;//对
	break;
	}
	case 4:
	{	for (i = 0; i <= n2; i++)
	{
		s = i - delay;
		h[i] = (sin(wc1*s / fs) + sin(pi*s) - sin(wc2*s / fs)) / (pi*s);//带阻-//存在问题
		h[i] = h[i] * window(wn, n + 1, i, beta);
		h[n - i] = h[i];
	}
	if (mid == 1)	h[n / 2] = (wc1 + pi - wc2) / pi/fs;
	break;
	}
	}
}



/*返回窗函数的表达式
n:窗口长度
type:选择窗函数的类型
beta:生成凯塞窗的系数*/
static double window(int type,int n,int i,double beta)
//static double window(type, n, i, beta)
//int i, n, type;
//double beta;
{
	int k;
	double pi, w;
	double kaiser(int i, int n, double beta);

	pi = 4.0*atan(1.0);//pi=PI;
	w = 1.0;

	switch (type)
	{
	case 1:
	{	w = 1.0;//矩形窗
	break;
	}
	case 2:
	{	k = (n - 2) / 10;
	if (i <= k)
		w = 0.5*(1.0 - cos(i*pi / (k + 1)));//图基窗//存在问题
	break;
	}
	case 3:
	{	w = 1.0 - fabs(1.0 - 2 * i / (n - 1.0));//三角窗
	break;
	}
	case 4:
	{	w = 0.5*(1.0 - cos(2 * i*pi / (n - 1)));//汉宁窗
	break;
	}
	case 5:
	{	w = 0.54 - 0.46*cos(2 * i*pi / (n - 1));//海明窗
	break;
	}
	case 6:
	{	w = 0.42 - 0.5*cos(2 * i*pi / (n - 1)) + 0.08*cos(4 * i*pi / (n - 1));//布莱克曼窗
	break;
	}
	case 7:
	{	w = kaiser(i, n, beta);//凯塞窗
	break;
	}
	}
	return(w);
}


static double kaiser(int i,int n,double beta)//因凯塞窗的表达式复杂，调用贝塞尔窗表达式
//static double kaiser(i, n, beta)//因凯塞窗的表达式复杂，调用贝塞尔窗表达式
//int i, n;
//double beta;
{
	double a, w, a2, b1, b2, beta1;
	double bessel0(double x);
	b1 = bessel0(beta);
	a = 2.0*i / (double)(n - 1) - 1.0;
	a2 = a*a;
	beta1 = beta*sqrt(1.0 - a2);
	b2 = bessel0(beta1);
	w = b2 / b1;
	return(w);
}


static double bessel0(double x)
//static double bessel0(x)
//double x;
{
	int i;
	double d, y, d2, sum;
	y = x / 2.0;
	d = 1.0;
	sum = 1.0;
	for (i = 1; i <= 25; i++)
	{
		d = d*y / i;
		d2 = d*d;
		sum = sum + d2;
		if (d2 < sum*(1.0e-8)) break;
	}
	return(sum);
}


/*void main()//调用实例
{
	int n = 511;
	int band = 3;
	double fln = 0.1, fhn = 0.2;
	int wn = 1;
	double h[512];
	firwin(n, band, fln, fhn, wn, h);
}*/


#endif /* FIR_H_ */
