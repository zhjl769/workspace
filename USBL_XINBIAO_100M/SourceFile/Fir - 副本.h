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

/*n:�˲����Ľ���,n������ʱ��������Ƹ����˲���
band:�˲�������1��2��3��4�ֱ��Ӧ1��ͨ��2��ͨ��3��ͨ��4����
fln:ͨ���±߽�Ƶ��
fhn:ͨ���ϱ߽�Ƶ��
wn:������������1-7�ֱ��Ӧ��1���δ���2ͼ������3���Ǵ���4��������5��������6������������7������
h:����Ϊn+1,����˲�����ϵ��
fs:����Ƶ��*/

void firwin(int n, int band,double  fln,double fhn,int wn,short *h)
//void firwin(n, band, fln, fhn, wn, h)
//int n, band, wn;
//double fln, fhn, h[];
{
	int i, n2, mid;
	double s, pi, wc1, wc2, beta, delay, fs;
	fs = 40000;//������
	double window(int type, int n, int i, double beta);//�������ļ���
	beta = 0.0;
	if (wn == 7)//ֻ�п�������Ҫ����ϵ��beta
	{
		printf("input beta parameter of Kaiser window(2<beta<10)\n");
		scanf("%1f", &beta);
	}
	pi = 4.0*atan(1.0);//pi=PI;

	if ((n % 2) == 0)/*�������n��ż��*/
	{
		n2 = n / 2 + 1;/**/
		mid = 1;//
	}
	else
	{
		n2 = n / 2;//n������,�򴰿ڳ���Ϊż��
		mid = 0;//
	}
	delay = n / 2.0;
	wc1 = 2.0*pi*fln;//
	if (band >= 3) /*���ж��û���������ݣ����band��������3*/
	{
		wc2 = 2.0*pi*fhn;
	}

	switch (band)
	{
	case 1:
	{	for (i = 0; i <= n2; i++)
	{
		s = i - delay;//
		h[i] = (sin(wc1*s / fs) / (pi*s))*window(wn, n + 1, i, beta);//��ͨ,���ڳ���=����+1����Ϊn+1
		h[n - i] = h[i];
	}
	if (mid == 1)	h[n / 2] = wc1 / pi/fs;//nΪż��ʱ�������м�ֵϵ��
	break;
	}
	case 2:
	{	for (i = 0; i <= n2; i++)
	{
		s = i - delay;
		h[i] = (sin(pi*s) - sin(wc1*s / fs)) / (pi*s);//��ͨ-//��
		h[i] = h[i] * window(wn, n + 1, i, beta);
		h[n - i] = h[i];
	}
	if (mid == 1)	h[n / 2] = 1.0 - wc1 / pi/fs;//��
	break;
	}

	case 3:
	{	for (i = 0; i < n2; i++)
	{
		s = i - delay;
		h[i] = (sin(wc2*s / fs) - sin(wc1*s / fs)) / (pi*s);//��ͨ-//��
		h[i] = h[i] * window(wn, n + 1, i, beta);
		h[n - i] = h[i];
	}
	if (mid == 1)	h[n / 2] = (wc2 - wc1) / pi/fs;//��
	break;
	}
	case 4:
	{	for (i = 0; i <= n2; i++)
	{
		s = i - delay;
		h[i] = (sin(wc1*s / fs) + sin(pi*s) - sin(wc2*s / fs)) / (pi*s);//����-//��������
		h[i] = h[i] * window(wn, n + 1, i, beta);
		h[n - i] = h[i];
	}
	if (mid == 1)	h[n / 2] = (wc1 + pi - wc2) / pi/fs;
	break;
	}
	}
}



/*���ش������ı��ʽ
n:���ڳ���
type:ѡ�񴰺���������
beta:���ɿ�������ϵ��*/
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
	{	w = 1.0;//���δ�
	break;
	}
	case 2:
	{	k = (n - 2) / 10;
	if (i <= k)
		w = 0.5*(1.0 - cos(i*pi / (k + 1)));//ͼ����//��������
	break;
	}
	case 3:
	{	w = 1.0 - fabs(1.0 - 2 * i / (n - 1.0));//���Ǵ�
	break;
	}
	case 4:
	{	w = 0.5*(1.0 - cos(2 * i*pi / (n - 1)));//������
	break;
	}
	case 5:
	{	w = 0.54 - 0.46*cos(2 * i*pi / (n - 1));//������
	break;
	}
	case 6:
	{	w = 0.42 - 0.5*cos(2 * i*pi / (n - 1)) + 0.08*cos(4 * i*pi / (n - 1));//����������
	break;
	}
	case 7:
	{	w = kaiser(i, n, beta);//������
	break;
	}
	}
	return(w);
}


static double kaiser(int i,int n,double beta)//�������ı��ʽ���ӣ����ñ����������ʽ
//static double kaiser(i, n, beta)//�������ı��ʽ���ӣ����ñ����������ʽ
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


/*void main()//����ʵ��
{
	int n = 511;
	int band = 3;
	double fln = 0.1, fhn = 0.2;
	int wn = 1;
	double h[512];
	firwin(n, band, fln, fhn, wn, h);
}*/


#endif /* FIR_H_ */
