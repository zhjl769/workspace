/*
 * main.c
 */
//时钟频率=CLK*MULT/(DIV+1)
//PLL_Config PLLConfig = {
//		0,		//IAI,PLL锁定,与进入Idle模式之前的过程相同
//		1,		//IOB,时钟发生器切换到旁路模式，并重新开始PLL的锁相过程
//		18,		//PLL MULT,PLL倍频值(2-31)	四倍频
//		0		//PLL DIV,PLL分频值(1-4)		二分频
//};
#include <math.h>
#define N	4096	//
#define DIV	50

short sig[N];
short isig[N*2];

short sig_0[N];
short isig_0[N*2];

short ixcorr[N*2];
short xcorr[N];

#pragma DATA_SECTION(sig,"sig_section");
#pragma DATA_SECTION(isig,"data_section");
#pragma DATA_SECTION(sig_0,"sig_section");
#pragma DATA_SECTION(isig_0,"data_section");
#pragma DATA_SECTION(ixcorr,"xcorr_section");
#pragma DATA_SECTION(xcorr,"xcorr_section");

long f1 = 20000;
long f2 = 30000;
long fs = 200000;
#define PI	3.1415926
#define t	0.02

float t1;
float t2;

void dot(short* A,short* B,short* C,short len)
{
	short i;
	long long temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
	for(i = 0; i < len; i++)
	{
		temp1 = A[2*i];//乘以2
		temp2 = A[2*i + 1];
		temp3 = B[2*i];
		temp4 = B[2*i + 1];

		temp5 = (long)((long)temp1*temp3);//(long)((long)temp1*temp3>>k)
		temp6 = (long)((long)temp2*temp4);
		temp7 = (long)((long)temp1*temp4);
		temp8 = (long)((long)temp2*temp3);

		C[2*i] = (short)((temp5 - temp6) >> 17);//(short)((temp5+temp6)>>18)
		C[2*i + 1] = (short)((temp7 + temp8) >> 17);//(short)((temp7-temp8)>>18)
	}
}

void cfft_NOSCALE(short *x, unsigned short nx);
void cbrev (short *x, short *y, unsigned short n);
void cifft_NOSCALE (short *x,  unsigned short nx);

int main(void) {
	
	int i;
//	 CSL_init();					//CSL库初始化
//	 PLL_config(&PLLConfig);		//初始化时钟
	t1=(float)1/fs;
	t2=(float)1/fs;
	float a1;
	float a2;
	float b1;
	float b2;
	for(i=(N-t*fs);i<N;i++)
	{
		a1=2*PI*f1*t1*i;
		b1=PI*10000*t1*i*t1*i*(1/t);
		sig[i]=(short)100*cos(a1+b1);
	}
	for(i=0;i<(N-t*fs);i++)
	{
		sig[i]=0;
	}
	for(i=(N-t*fs);i<N;i++)
	{
		a2=2*PI*f2*t2*i;
		b2=PI*10000*t2*i*t2*i*(1/t);
		sig_0[i]=(short)100*cos(a2-b2);
	}
	for(i=0;i<(N-t*fs);i++)
	{
		sig_0[i]=0;
	}

	/*
	short te=0;
	for(i=0;i<N/2;i++)
	{
		te=sig_0[i];
		sig_0[i]=sig_0[N-i];
		sig_0[N-i]=te;
	}
	for(i=0;i<N/2;i++)
	{
		te=sig[i];
		sig[i]=sig[N-i];
		sig[N-i]=te;
	}
*/
	for(i=0;i<N;i++)
	{
		isig[2*i]=sig[i]>>4;
		isig[2*i+1]=0;
		isig_0[2*i]=sig_0[i]>>4;
		isig_0[2*i+1]=0;
	}


	cfft_NOSCALE(isig,N);
	cbrev(isig,isig,N);

	cfft_NOSCALE(isig_0,N);
	cbrev(isig_0,isig_0,N);

	for(i=0;i<N;i++)
	{
		sig[i]=DIV*sqrt((isig[2*i+1]/DIV)*(isig[2*i+1]/DIV)+(isig[2*i+1]/DIV)*(isig[2*i+1]/DIV));
	}

	dot(isig,isig_0,ixcorr,N);

	cifft_NOSCALE(ixcorr,N);
	cbrev(ixcorr,ixcorr,N);


	for(i=0;i<N;i++)
	{
		xcorr[i]=ixcorr[2*i];
	}

	return 0;
}
