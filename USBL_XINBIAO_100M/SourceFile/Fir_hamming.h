/*
 * Fir_hamming.h
 *
 *  Created on: 2017-3-2
 *      Author: Administrator
 */

#ifndef FIR_HAMMING_H_
#define FIR_HAMMING_H_

/*n:�˲����Ľ���, Ҫ��n����Ϊż��
band:�˲�������1��2��3��4�ֱ��Ӧ1��ͨ��2��ͨ��3��ͨ��4����
fln:ͨ���±߽�Ƶ��
fhn:ͨ���ϱ߽�Ƶ��
fs:����Ƶ��
h:����Ϊn+1,����˲�����ϵ��*/


void firwin(int n, int band,long int fln,long int fhn,long int Fs,DATA *h)
{
  /*Ҫ��n����Ϊż��*/
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
          h[i] = ((sin(wc1*s / Fs) / (PI*s))*(0.54 - 0.46*cos(2 * i*PI / (n - 1))))*10000;//��ͨ,���ڳ���=����+1����Ϊn+1
          h[n - i] = h[i];
        }
      h[n / 2] = wc1 / PI/Fs*10000;//nΪż��ʱ�������м�ֵϵ��
      break;
    }
    case 2:
    {
      for (i = 0; i <= n2; i++)
        {
          s = i - delay;
          h[i] = (sin(PI*s) - sin(wc1*s / Fs)) / (PI*s);//��ͨ-//��
          h[i] = h[i] * (0.54 - 0.46*cos(2 * i*PI / (n - 1)))*10000;
          h[n - i] = h[i];
        }
      h[n / 2] = (1.0 - wc1 / PI/Fs)*10000;//��
      break;
    }

    case 3:
    {
      for (i = 0; i < n2; i++)
        {
          s = i - delay;
          h[i] = (sin(wc2*s / Fs) - sin(wc1*s / Fs)) / (PI*s);//��ͨ-//��
          h[i] = h[i] * (0.54 - 0.46*cos(2 * i*PI / (n - 1)))*10000;
          h[n - i] = h[i];
        }
      h[n / 2] = (wc2 - wc1) / PI/Fs*10000;//��
      break;
    }
    case 4:
    {
      for (i = 0; i <= n2; i++)
        {
          s = i - delay;
          h[i] = (sin(wc1*s / Fs) + sin(PI*s) - sin(wc2*s / Fs)) / (PI*s);//����-//��������
          h[i] = h[i] * (0.54 - 0.46*cos(2 * i*PI / (n - 1)))*10000;
          h[n - i] = h[i];
        }
      h[n / 2] = (wc1 + PI - wc2) / PI/Fs*10000;
      break;
    }
    }
}








#endif /* FIR_HAMMING_H_ */
