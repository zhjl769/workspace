/*
 * Fir_hamming.h
 *
 *  Created on: 2017-3-2
 *      Author: Administrator
 */

#ifndef FIR_HAMMING_H_
#define FIR_HAMMING_H_

/*n:滤波器的阶数, 要求n必须为偶数
band:滤波器类型1、2、3、4分别对应1低通，2高通，3带通，4带阻
fln:通带下边界频率
fhn:通带上边界频率
fs:采样频率
h:长度为n+1,存放滤波器的系数*/


void firwin(int n, int band,long int fln,long int fhn,long int Fs,DATA *h)
{
  /*要求n必须为偶数*/
  int i, n2;
  double s, wc1, wc2,delay;
  n2 = n/2+1;
  delay = n / 2.0;
  wc1 = 2.0*PI*fln;//
  wc2 = 2.0*PI*fhn;


  switch (band)
    {
    case 1:
    {
      for (i = 0; i <= n2; i++)
        {
          s = i - delay;
          h[i] = ((sin(wc1*s / Fs) / (PI*s))*(0.54 - 0.46*cos(2 * i*PI / (n - 1))))*10000;//低通,窗口长度=阶数+1，故为n+1
          h[n - i] = h[i];
        }
      h[n / 2] = wc1 / PI/Fs*10000;//n为偶数时，修正中间值系数
      break;
    }
    case 2:
    {
      for (i = 0; i <= n2; i++)
        {
          s = i - delay;
          h[i] = (sin(PI*s) - sin(wc1*s / Fs)) / (PI*s);//高通-//对
          h[i] = h[i] * (0.54 - 0.46*cos(2 * i*PI / (n - 1)))*10000;
          h[n - i] = h[i];
        }
      h[n / 2] = (1.0 - wc1 / PI/Fs)*10000;//对
      break;
    }

    case 3:
    {
      for (i = 0; i < n2; i++)
        {
          s = i - delay;
          h[i] = (sin(wc2*s / Fs) - sin(wc1*s / Fs)) / (PI*s);//带通-//对
          h[i] = h[i] * (0.54 - 0.46*cos(2 * i*PI / (n - 1)))*10000;
          h[n - i] = h[i];
        }
      h[n / 2] = (wc2 - wc1) / PI/Fs*10000;//对
      break;
    }
    case 4:
    {
      for (i = 0; i <= n2; i++)
        {
          s = i - delay;
          h[i] = (sin(wc1*s / Fs) + sin(PI*s) - sin(wc2*s / Fs)) / (PI*s);//带阻-//存在问题
          h[i] = h[i] * (0.54 - 0.46*cos(2 * i*PI / (n - 1)))*10000;
          h[n - i] = h[i];
        }
      h[n / 2] = (wc1 + PI - wc2) / PI/Fs*10000;
      break;
    }
    }
}








#endif /* FIR_HAMMING_H_ */
