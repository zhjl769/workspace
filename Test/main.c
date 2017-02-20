/*
 * main.c
 */

#include <msp430f6638.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Variable.h"
#include "Other.h"
#include "Init.h"
#include "ADC12.h"
#include "DMA.h"
#include "Xcorr.h"
#include "DSPLib.h"


#define CPU_F ((double)20000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))
//������ʱ��������ֱ�ӵ���delay_ms (x),xΪ��Ҫ��ʱ��ʱ��


/* Select the global Q value and include the Qmath header file. */
//#define GLOBAL_Q    12
//#include "QmathLib.h"
msp_max_q15_params MAXparams;

DSPLIB_DATA(Fir_coeffs, 4)
_q15 Fir_coeffs[FIR_COEFF_LENGTH] = {
		_Q15(-0.00122631),
		_Q15(-0.00110810), _Q15(-0.00096266), _Q15(-0.00048341), _Q15(0.00071389),
		_Q15(0.00302998), _Q15(0.00680972), _Q15(0.01227014), _Q15(0.01944104),
		_Q15(0.02812891), _Q15(0.03791185), _Q15(0.04816823), _Q15(0.05813698),
		_Q15(0.06700193), _Q15(0.07398872), _Q15(0.07846077), _Q15(0.08000000),
		_Q15(0.07846077), _Q15(0.07398872), _Q15(0.06700193), _Q15(0.05813698),
		_Q15(0.04816823), _Q15(0.03791185), _Q15(0.02812891), _Q15(0.01944104),
		_Q15(0.01227014), _Q15(0.00680972), _Q15(0.00302998), _Q15(0.00071389),
		_Q15(-0.00048341), _Q15(-0.00096266), _Q15(-0.00110810), _Q15(-0.00122631),
		_Q15(0.00000000)
};

/* Initialize the Fir parameter structure. */
msp_fir_q15_params FirParams =  {
		Dxcor + FIR_COEFF_ORDER,
		FIR_COEFF_LENGTH,
		Fir_coeffs
};


/* Initialize the fft parameter structure. */
msp_fft_q15_params FFTParams =  {
		256,
		1,
		msp_cmplx_twiddle_table_256_q15
};


/* Initialize the max parameter structure.
msp_max_q15_params MAXParams =  {
		256,
};
 */

/*
int X_ref1[26] = {1,8,-13,5,22,-40,16,42,
		-75,34,54,-100,50,50,-100,54,34,
		-75,42,16,-40,22,5,-13,8,1};
 */

const int X_ref1[40] = {0,-8,-7,6,18,6,-24,-29,10,50,27,-44,-69,
		0,81,61,-45,-100,-25,86,86,-25,-100,-45,61,81,
		0,-69,-44,27,50,10,-29,-24,6,18,6,-7,-8,0};

int _system_pre_init(void)  //����������ʱ�������ڴ˺���
{
	/* Insert your low-level initializations here */
	WDTCTL = WDTPW + WDTHOLD; // Stop Watchdog timer
	/*==================================*/
	/* Choose if segment initialization */
	/* should be done or not. */
	/* Return: 0 to omit initialization */
	/* 1 to run initialization */
	/*==================================*/
	return (1);
}

void main( ) //������
{
	WDTCTL = WDTPW + WDTHOLD; //�ؿ��Ź�
	IO_Init();//IO�ڳ�ʼ��
	Init_CLK();//ʱ�ӳ�ʼ��
	//	P1OUT &= ~BIT0;//�����ϵ�
	//	Build_rev(f1, fs, X_ref, Dref);
	//	P1OUT |= BIT0;//�����µ�
	Init_ADC();//ADC12��ʼ��
	//UART1_Init(); //����UART1��ʼ������
	//	P1OUT &= ~BIT0;//�����ϵ�
	//	P1OUT |= BIT0;//�����µ�
	Init_DMA();//DMA��ʼ��
	ADC12_Start();//AD��ʼ��
	_EINT(); //����C�������ڲ�����ʹ���ж�
	//	_DINT(); //����C�������ڲ�������ֹ�ж�
	DMAdone = 0; Block = 1;
	/* Initialize the parameter structure. */
	MAXparams.length = 256;
	//_BIS_SR(LPM1_bits); //����C�������ڲ���״̬�Ĵ���ĳλ��λ�ĺ�����
	//	__bis_SR_register(LPM1_bits);
	//	LPM_bits=SCG0+CPUOFF��
	// SCG0=0x0040������LPM1�͹��Ĺ���ģʽ
	// CPUOFF=0x0010 �ر�CPU�����������������ж�
	//_NOP(); //����C�������ڲ��ղ�������
	// ���봦��ѭ��
	while(1)
	{
		unsigned int i, j;
		if(DMAdone != 0)
		{
			//P1OUT &= ~BIT0;//�����ϵ�

			xcorr(Di, Rev, Dref, X_ref1, Fout);

			j = 0; i = Dxcor - 1;
			while( i < Dfir)
			{
				Xout[j++] = Xout[i++];
			}

			j = 0; i = Dfir - Dxcor;
			while( i < Dfir)
			{
				Xout[i++] = Fout[j++];
			}

			msp_fir_q15(&FirParams, Xout, Fout);
			/*
			 * �˲���֮��Ľ����ǰ32��������Ƕ�ʧ�ģ������ĵ���Ϊ40�㣬
			 * ��ôʵ���˲����ĵ������ظ���ֻ��8���㣬�������ݴ���ǰ8���޳���
			 * ʵ�ʵ��Ե�ʱ���֣�ÿ�θ���40��������ݣ�ʵ���ظ�������Ϊ33���㣬
			 * ��ô�ں����Ĵ�����158�����У�ǰ33�����������Ҫ�޳���
			 */
			//			msp_mean_q15(Dxcor, 79, 2, Fout, Fout);
			msp_Fang_q15(Dxcor + FIR_COEFF_ORDER,Fout,Fout);
			Savefft_q15(Dxcor + FIR_COEFF_ORDER, 25, Fout, FFTout);
			if(Block > 9)
			{
				Block = 1;
				FFTsave_out_q15(25, FFTout, Fout);
				msp_fft_fixed_q15(&FFTParams, FFTout);
				DATAabs_q15(256,FFTout,FFTout);
				memset(FFTout, 0, 2);//�޳�FFT�е�ǰ�����㡣
				msp_max_q15(&MAXparams, FFTout, &q15MaxVector, &uint16MaxIndex);
				Neicha_q15(&uint16MaxIndex, &MaxIndex_out, FFTout);
				MaxIndex_out = MaxIndex_out/510*1000;
				/*
				 * ע�⣺�Դ���FFT������������ĵ���Ӧ�ó���2������ʵ��ֵ��������ʽʵ��Ϊ��
				 * MaxIndex_out = MaxIndex_out/��255*2��*1000;
				 */
				FFTsave_in_q15(25, Fout, FFTout);
				//				Last_result = 0;
				if((MaxIndex_out >= 89.5) & (MaxIndex_out <= 92.5))
				{
					Last_result = 1;
					Result_out++;
				}
				else
					Last_result = 0;

				if(Result_out > 0 & Last_result != 0)
					Result_out++;
				else
					Result_out = 0;
				if(Result_out > 10)
				{
					P1OUT |= BIT0;//�����µ�
					delay_ms(3000);
					P1OUT &= ~BIT0;//�����ϵ�
					Result_out = 0;
				}
				__no_operation();
			}
			//			P1OUT |= BIT0;//�����µ�
			DMAdone = 0;
			//ADC12_Start();
		}
		//temperature_get();
	}
}


