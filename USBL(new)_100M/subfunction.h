/*
 * subfunction.h
 *
 *  Created on: 2016-12-21
 */

#ifndef SUBFUNCTION_H_
#define SUBFUNCTION_H_


//��������ֵ�ĺ���
short mean_zh(short *x, short nx, short ind)
{
	short i;
	long sum = 0;
	for(i=0; i<nx; i=i+ind)
	{
		sum = sum + x[i];
	}
	return (short)(sum*ind/nx);
}


void max_zh(short *max_val, short *max_num, short *x, short nx, short ind)
{
	short index, i;
	short max0 = 0;

	for(i=0; i<nx; i=i+ind)
	{
		if(x[i] > max0)
		{
			max0 = x[i];
			index = i;
		}
	}
	*max_val = max0;
	*max_num = index;
}


/*�����ο��ź�*/

void Produceref()
{
	unsigned long lfm_head,t_Ref;
	double t_lfm;
	float t_temp;
	int i;
	t_temp = ((float) Time_Ref)/1000;
	memset(Rev,0,sizeof(short)*NFFT);  //��λ�ź�
	if(f2_Ref >= f1_Ref)
	{
		t_Ref = ((unsigned long)(f2_Ref-f1_Ref))*1000;
		for(lfm_head = 0; lfm_head <Time_Ref*fs; lfm_head++)
		{
			t_lfm = (float) lfm_head/(((float) fs)*1000);//����Ƶ��100kHz
			Rev[lfm_head] = (short)(-100*(cos(2*PI*(f2_Ref*1000)*t_lfm - PI*t_Ref*t_lfm*t_lfm/t_temp)));//��ʼƵ��30kHz����ֹƵ��22kHz,20ms
		}
	}
	else
	{
		t_Ref = ((unsigned long)(f1_Ref-f2_Ref))*1000;
		for(lfm_head = 0; lfm_head <Time_Ref*fs; lfm_head++)
		{
			t_lfm = (float)lfm_head/(((float) fs)*1000);//����Ƶ��100kHz
			Rev[lfm_head] = (short)(-100*(cos(2*PI*(f2_Ref*1000)*t_lfm + PI*t_Ref*t_lfm*t_lfm/t_temp)));//��ʼƵ��30kHz����ֹƵ��22kHz,20ms
		}
	}

	for (i = 0; i < NFFT; i++)
	{
		X_ref[2*i] = Rev[i]>>4;//ż��ַ����4bit
		X_ref[2*i+1] = 0;
	}
	cfft_NOSCALE(X_ref,NFFT);  //�Ƿ����������
	cbrev(X_ref,X_ref,NFFT);
}


/*���������ź�
  �������ܣ����������źţ�ѯ���źźͲ����źŴ����CE2�ռ�
			�׵�ַ�ֱ�Ϊ0x400000��0x500000
 */
void Producelfm()
{
	volatile unsigned long lfm_head,t_Sig;
	volatile double t_lfm;
	float t_Sigtemp;
	t_Sigtemp = ((float) Time_Sig)/1000;
	if(f2_Sig >= f1_Sig)
	{
		t_Sig = ((unsigned long)(f2_Sig - f1_Sig))*1000;
		for(lfm_head = 0; lfm_head < Time_Sig*1000; lfm_head++)
		{
			t_lfm = (float)lfm_head/1000000.0;//����Ƶ��1MHz
			templfm = (short)(200*(cos(2*PI*(f1_Sig*1000)*t_lfm + PI*t_Sig*t_lfm*t_lfm/t_Sigtemp)));//��ʼƵ��f1_Sig����ֹƵ��f2_Sig,Time_Sig
			if(templfm < 0)
				Send_sig[lfm_head]=0x05;//GPIO2=1;GPIO1=0;GPIO0=1;
			else
				Send_sig[lfm_head]=0x03;//GPIO2=0;GPIO1=1;GPIO0=1;
		}
	}
	else
	{
		t_Sig = ((unsigned long)(f1_Sig - f2_Sig))*1000;
		for(lfm_head = 0; lfm_head < Time_Sig*1000; lfm_head++)
		{
			t_lfm = (float)lfm_head/1000000.0;//����Ƶ��1MHz
			templfm = (short)(200*(cos(2*PI*(f1_Sig*1000)*t_lfm - PI*t_Sig*t_lfm*t_lfm/t_Sigtemp)));//��ʼƵ��f1_Sig����ֹƵ��f2_Sig,Time_Sig
			if(templfm < 0)
				Send_sig[lfm_head]=0x05;//GPIO2=1;GPIO1=0;GPIO0=1;
			else
				Send_sig[lfm_head]=0x03;//GPIO2=0;GPIO1=1;GPIO0=1;
		}
	}

}

/*
 *������ʼ��
 */
void variable_init()
{
	FLAGSENDOVER=0;

	FLAG_RS232_REVOK = 0;
	Pos_flag = 0;

	endpos = 0;
	sum_mean_save = 0;
	width_save = 0;

	resend_delay = 0;
	DMAnum = 0;
	DMAdone = FALSE;

	timer1_cnt = 0;
	temp_ADC = 0;

	voltage_AD = 0;
	depth = 0.0;
	Flag_answer = 0;

	temperature = 0.0;
	block2 = 2;
	memset(count_final_jiance,0,12);
	testpinlv_flag = 0;
	release_flag = 0;//�ͷųɹ�״̬λ
	Tr_time = 0;//��ʼ���رշ��䲿�ֵ�Դ����ֵ

	memset(temp_dainei_temp,0,sizeof(short)*(NFFT_mingling));
	memset(Rev,0,sizeof(short)*NFFT);
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	memset(Rev_xcorr,0,sizeof(short)*2*NFFT);
	memset(Rev_xcorrout,0,sizeof(short)*NFFT);
	memset(Rev_save_filter_out,0,sizeof(short)*NFFT);
	memset(Rev_envelope,0,sizeof(short)*5500);

	/*
	memset(Rev_bp1,0,sizeof(short)*NFFT);
	memset(Rev_bp2,0,sizeof(short)*NFFT);
	memset(Rev_bp1_abs,0,sizeof(short)*NFFT);
	memset(Rev_bp2_abs,0,sizeof(short)*NFFT);
	memset(Rev_bp1_envelope,0,sizeof(short)*NFFT);
	memset(Rev_bp2_envelope,0,sizeof(short)*NFFT);
	 */
}

/*��ȡ��������	*/
void get_noise()
{
	long temp;
	short i;
	IFR1 = 0x0ffff;
	IER1 |= 0x0010;
	DMA_start(myhDma);
	while(DMAdone == FALSE);
	DMAdone = FALSE;
	DMA_stop(myhDma);

	IFR1 = 0x0ffff;
	IER1 &= 0x0ffef;
	temp = 0;
	for(i = 100;i<100+512;i++)
	{
		temp = temp + fabs(Rev1[i]);
	}
	noise_10ms = (short)(temp>>9);
}


/*��ȡѹ����Ϣ����
  �������ܣ�����DSP�ڲ�AD(10bit),������Ϊ100kHz,�ɼ�64������,ȡ��ֵ
            ��ϳ������Ϣ
			��ʱ��ֵ��Ϊԭֵ��һ�롣
 */
void pressure_init()
{
	volatile short temp_js,buff,f_send_uart,j_uart;
	volatile Uint32 ii;
	FLAG_yl_init = 0;
	j_uart = 0;
	temp_js = 4;
	f_send_uart = 0;
	//���ͳ�ʼ��ѹ������������
	if(j_uart < temp_js)
	{
		if((UART_LSR485 & (0x0060)) != 0)
		{
			UART_THR485 = init_rs485[j_uart];
			for(ii=100;ii>0;ii--);
			j_uart++;
		}
	}// end while
	IER0 |= 0x0008;
	IFR0 = 0x0ffff;//ʹ��int0�ж�
	//	timer1_cnt = 0;
	// 	IFR1 = 0x0ffff;
	// 	IER1 |= 0x0040;//ʹ�ܶ�ʱ��1�ж�
	//	TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
	while(FLAG_yl_init == 0)
	{
		ii = 0;
		while(ii < 250000)
		{
			ii++;
			if(FLAG_yl_init == 1)
			{
				break;
			}
		}
		if(ii == 250000)
		{
			memset(RS485_DATA,0,sizeof(short)*12);
			break;
		}
		//		if(timer1_cnt == 100000)
		//		{
		//			break;
		//		}
	}
	//��ʾѹ����������ʼ���ɹ�
	IER0 &= 0x0fff7;
	IFR0 = 0x0ffff;//�ر�int2�ж�
	//	TIMER_stop(mhTimer1);//�رն�ʱ��1
	//	IFR1 = 0x0ffff;
	//	IER1 &= 0x0ffbf;//�رն�ʱ��1�ж�
}
void get_pressure()
{
	volatile short temp_js,buff,f_send_uart,j_uart;
	volatile Uint32 ii;
	FLAG_yl = 0;
	j_uart = 0;
	temp_js = 5;
	f_send_uart = 0;
	//	timer1_cnt = 0;
	// 	IFR1 = 0x0ffff;
	// 	IER1 |= 0x0040;//ʹ�ܶ�ʱ��1�ж�
	//	TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
	//���ͳ�ʼ��ѹ������������
	if(j_uart < temp_js)
	{
		buff = UART_LSR485;
		f_send_uart = buff & (0x0060);
		if(f_send_uart!=0)
		{
			UART_THR485 = ask_rs485[j_uart];
			for(ii=100;ii>0;ii--);
			j_uart++;
		}
	}// end while
	IER0 |= 0x0008;
	IFR0 = 0x0ffff;//ʹ��int2�ж�
	while(FLAG_yl == 0)
	{
		ii = 0;
		while(ii < 350000)
		{
			ii++;
			if(FLAG_yl == 1)
			{
				break;
			}
		}
		if(ii == 350000)
		{
			memset(RS485_DATA,0,sizeof(short)*12);
			break;
		}
		//		if(timer1_cnt == 100000)
		//		{
		//			temp_yl_data = 0;
		//			IER0 &= 0x0fff7;
		//			IFR0 = 0x0ffff;//�ر�int2�ж�
		//			break;
		//		}
	}
	timer1_cnt = ii;
	temp_yl_data = (((Uint32)RS485_DATA[2])<<24) + (((Uint32)RS485_DATA[3])<<16) + (((Uint32)RS485_DATA[4])<<8) + ((Uint32)RS485_DATA[5]);
	if(temp_yl_data > 268435455)
	{
		temp_yl_data = 0;
	}
	//temp_yl_data��ֵ��λΪ��Pa����1 Pa=1e-5 bar=��1e-5��*10 m=1e-4 m
	//��λ����ȷ��ף����ӳ٣�long��FLAG_yl_data��100usʱ��,���������У�0.1m�����ӳ�0.1ms  ѹ������ʱ��Ϊ1.5m��Ӧ0.2ms
	//�˴�Ҳ�ɱ�FLAG_yl_data = (long)temp_yl_data/10;�����ӳٵ�usֵ��
	//��ʾѹ����������ȡ��Ϣ�ɹ�
	if(temp_yl_data > 0)
	{
		depth = (float)(temp_yl_data-0)/15000;  //(���Ƚ��õ���ѹ��ֵ������m=yl/10000���ٽ��������ת��Ϊʱ��ֵ=m/1.5����λ��ms��)
	}
	else
	{
		depth = 0.0;
	}

	//	TIMER_stop(mhTimer1);//�رն�ʱ��1
	//	IFR1 = 0x0ffff;
	//	IER1 &= 0x0ffbf;//�رն�ʱ��1�ж�
}


/*��ȡ��ѹ��Ϣ����
  �������ܣ�����DSP�ڲ�AD(10bit),������Ϊ10kHz,�ɼ�64������,ȡ��ֵ
 */
void get_voltage()
{
	long sum;
	short i = 0;
	for ( i = 0; i < 64; i++)
	{
		ADCCTL = 0x8000;	// ����ADת����ͨ��0
		do
		{
			temp_ADC=ADCDATA;
		}while(temp_ADC & 0x8000);//��ѯADCæ��־λ
		ADC_in[i] = temp_ADC & 0x0fff;
	}
	sum = 0;
	for(i = 0; i < 64; i++)
	{
		sum = sum + ADC_in[i];
	}
	voltage_AD = (short)(sum>>6);//���ѹ��ֵ
}


void get_parameter()//��ȡ������Ϣ
{
	NOISE_TH_NEW = PC_DATA[4]+256*PC_DATA[5];//���¼����������16bit
	if(NOISE_TH_NEW < 1000)
	{
		NOISE_TH_NEW = 100;
	}
	if(NOISE_TH_NEW > 20000)
	{
		NOISE_TH_NEW = 100;
	}

	Time_Delay = (Uint32)(PC_DATA[8]+256*PC_DATA[9])*1000;

	if((PC_DATA[6]>0) && (PC_DATA[6]<31))
	{
		responder_period = PC_DATA[6]*10;
	}
	get_voltage();//��ȡ��ѹ��Ϣ
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;//ʹ�ܶ�ʱ��1�ж�
	TIMER_start(mhTimer1);//��1��ʱ
	get_temperature();//��ȡ�¶�
	IODATA = 0x0000;
	TIMER_stop(mhTimer1);//�رն�ʱ��1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//�رն�ʱ��1�ж�
	get_pressure();//��ȡѹ����Ϣ
	get_noise();//��ȡ��ǰ����
}



/*�ص���������
  �������ܣ��ص�������������
			����������ת��Ϊ�з�����
 */
void Revprepare()
{
	int i=0,j=0;

	for (i = BUF_LEN_MAX; i < NFFT; i++)//BUF_LEN_MAX=NFFT-Rev_resave=2096
	{
		Rev[j] = Rev[i];//������Ԫ��������ݰ��Ƶ�ǰ��
		j++;
	}
	j = 0;
	if (DMAnum%2 == 1)
	{
		for(i = Rev_resave;i < NFFT;i++)
		{
			Rev[i] = Rev1[j];//���DMAnum%2==1����Rev1�����е������Ƶ�����Ԫ�ĺ��棬�����޷�����ת��Ϊ�з�����
			j++;
		}
	}
	else
	{
		for(i = Rev_resave;i < NFFT;i++)
		{
			Rev[i] = Rev2[j];//���DMAnum%2==0����Rev2�����е������Ƶ�����Ԫ�ĺ��棬�����޷�����ת��Ϊ�з�����
			j++;
		}
	}
}

/*FFT����Ԥ����
  �������ܣ������ݴ���FFT���浥Ԫ�У��鲿Ϊ0��ʵ������7bit
 */
void FFTprepare(short *data, short *data_bu0, short data_N)
{
	int i;
	for (i = 0; i < data_N; i++)
	{

		data_bu0[2*i] = data[i]>>7;//ż��ַ����8bit,,�����ݵ�����Ϊshortת��Ϊint�ͣ�//ͬʱ����256���FFT����֮ǰ��һ��������
		data_bu0[2*i+1] = 0;
	}
}


void FFTprepare2(short *data, short *data_bu0, short N_FFT)
{
	int i;
	for (i = 0; i < 2*N_FFT; i ++)
	{
		data_bu0[i] = 0;
	}
	for (i = 0; i < N_FFT; i ++)
	{
		data_bu0[2*i] = data[i];
	}
}


/*��˺���
  �������ܣ�������ˣ�����������������A��B�����C�����鳤��Ϊlen
  WYH             ʱ�򷴾��<--->Ƶ������ˡ�
 */
void dot(short* A,short* B,short* C,short len)
{
	short i;
	long temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
	for(i = 0; i < len; i++)
	{
		temp1 = A[2*i];//����2
		temp2 = A[2*i + 1];
		temp3 = B[2*i];
		temp4 = B[2*i + 1];

		temp5 = (long)((long)temp1*temp3);//(long)((long)temp1*temp3>>k)
		temp6 = (long)((long)temp2*temp4);
		temp7 = (long)((long)temp1*temp4);
		temp8 = (long)((long)temp2*temp3);

		C[2*i] = (short)((temp5 - temp6) >> 18);//(short)((temp5+temp6)>>18)
		C[2*i + 1] = (short)((temp7 + temp8) >> 18);//(short)((temp7-temp8)>>18)
	}
}

/*��˺���
  �������ܣ�������ˣ�����������������A��B�����C�����鳤��Ϊlen
 */
void dot1(short* A,short* B,short* C,short len)
{
	short i;
	long temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
	for(i = 0; i < len; i++)
	{
		temp1 = A[2*i];
		temp2 = A[2*i + 1];
		temp3 = B[2*i];
		temp4 = B[2*i + 1];

		temp5 = (long)((long)temp1*temp3);//(long)((long)temp1*temp3>>k)
		temp6 = (long)((long)temp2*temp4);
		temp7 = (long)((long)temp1*temp4);
		temp8 = (long)((long)temp2*temp3);

		C[2*i] = (short)((temp5 - temp6) >> 12);//(short)((temp5+temp6)>>2)
		C[2*i + 1] = (short)((temp7 + temp8) >> 12);//(short)((temp7-temp8)>>2)
	}
}


void qujunzhi1(short *datain, short length)
{
	int i;
	long sum = 0;
	short mean;

	for (i = 0; i < length; i ++)
	{
		sum = sum + datain[i];
	}
	mean = sum/length;
	for (i = 0; i < length; i ++)
	{
		if(datain[i] > mean)
			datain[i] = 10;
		else
			datain[i] = -10;
	}
}


void qujunzhi2(short *datain, short length)
{
	int i;
	long sum = 0;
	short mean;

	for (i = 0; i < length; i ++)
	{
		sum = sum + datain[i];
	}
	mean = sum/length;
	for (i = 0; i < length; i ++)
	{
		if(datain[i] > mean)
			datain[i] = -10;
		else
			datain[i] = 10;


	}
}

void qumo2(short *In, short *Out, short N)
{
	int i;
	long result1;
	long result2;


	for (i = 0; i < N; i++)
	{
		result1 = (long)In[2*i]*In[2*i];
		result2 = (long)In[2*i+1]*In[2*i+1];
		Out[i] = (short)((result1>>8)+(result2>>8));
	}

}

/*

void neicha(int y1, int y2, int y3, int xn, double tao_max)
    {

	 double ts, tao1, tao2, coswt, wt, w, A, B, fai, k;

	  xn = (double) (xn);    //xnΪ��ֵ��Ӧ�ĵ���
	  ts = 1.0 / fs;         //fs
	  tao1 = (double)((xn - 1)) / fs;  //��ֵǰһ����
	  tao2 = xn / fs;        //��ֵ��Ӧ�ĵ�
	  coswt = (double) (y1 + y3) / (2 * y2);
	  wt = acos(coswt);
	  w = wt * fs;           //t=1/fs;
	  A = (y1 * sin(w * tao2) - y2 * sin(w * tao1)) / sin(w * ts);
	  B = (y2 * cos(w * tao1) - y1 * cos(w * tao2)) / sin(w * ts);
	  fai = atan2(B, A);
	  k = ceil((w * tao1 - fai) / PI);
	  tao_max = (k * PI + fai) / w;

    }

 */

/*���׼������
  �������ܣ����׼���������ص�������FFT�任
 */
void detect_prepare()
{
	Revprepare();   //0.075ms//�ص���������
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	FFTprepare(Rev,Rev_FFT,NFFT);//0.103ms
	cfft_NOSCALE(Rev_FFT,NFFT);  //0.862ms//FFT�任
	cbrev(Rev_FFT,Rev_FFT,NFFT);  //0.038ms //λ�������
	memset(Rev_FFT,0,20);
	//	memset(Rev_FFT+4076,0,20);
}



/*������
  �������ܣ�����Ϊ��ط���磬���ؼ��ɹ���־λ�ͼ��λ��
			���ɹ���־λ��1
 */
void detect(short *envelopein, short len,volatile int *flag,volatile short *endpos)
{
	short i,j,jishu=0;
	volatile short pos = 0;
	short Flag_sig = 0, flagg = 0;
	short startpos = 0, pos_begin =0, width = 0;
	short width_low = 0, width_high = 0;

	short Max_val = 0, Max_threshold = 0, Max_pos = 0;
	//	long  sum = 0;
	short sum_mean = 0;
	float neicha_a;
	float neicha_b;

	short point_count_hou = 0;
	short point_count_qian = 0;

	short ave[8] = {0,0,0,0,0,0,0,0};
	short max_ave1,ave_num = 0;
	long  sum_feng_hou = 0;
	short sum_mean_feng_hou = 0;
	short half_envelope_max = 0;
	//	short half23_envelope_max =0;

	//�жϾ������� ��ʼ���������ط����
	for(i = 1308; i < len; i++)
	{
		if(envelopein[i] > NOISE_TH_NEW)
		{
			Flag_sig = 1;
			startpos = i;//��¼���ھ������޵�λ�ã�������startpos��
			break;
		}
	}

	//��������������ֵ
	if(Flag_sig)
	{
		Flag_sig = 0;//���������ھ������ޣ�Flag_sig��־λ����
		for(i = startpos;i < len+200; i++)//200�㱣�����䣬��ֹ��len��������ط�
		{
			if(flagg == 0)
			{
				if(envelopein[i] >= NOISE_TH_NEW)
				{
					flagg = 1;		//������ھ������ޣ�flagg��1
					pos_begin = i;  //���ھ������޵ĵ㸳ֵΪpos_begin
					Max_val = 0;
					for(j = i;j < i + Maikuan_h; j++)
					{
						if(Max_val < envelopein[j])
						{
							Max_val = envelopein[j];
						}
					}               //�ҵ����ھ������޵ĵ����3*band�е����ֵ����ֵΪ Max_val
					Max_threshold = (short)Max_val*0.5;
					if((envelopein[i] < Max_threshold)) //�����ǰ�����ֵ�ĵ��ֵС�ڷ�ֵ��һ�룬
					{
						flagg = 0;
					}
				}  //��ȡ���߳�������޵ĵ�һ�㡢
			}
			else   //�ҵ���һ�����ڷ�ֵ���ޡ���ǰ������i��ȡ������������޵ĵ�һ�㡣
			{
				//if((envelopein[i] <= Max_threshold) || (Max_threshold < NOISE_TH_NEW))
				if((envelopein[i] <= Max_threshold))      //Max_thresholdΪ����ֵ��һ���ֵ
				{
					flagg = 0;
					for(j = pos_begin;j >= startpos; j--) //pos_beginΪ������������޵ĵ�һ�㡣
					{
						if(envelopein[j] <= Max_threshold)
						{
							pos_begin = j;                //�ѵ�������ֵһ��ĵ��ҵ��������µ�pos_begin
							break;
						}
					}

					//��pos_begin��i֮�������ֵ��λ�ã�
					//��pos_begin��i֮���ҵ�һ���յ��λ�ã�
					Max_pos = pos_begin;
					for(j = pos_begin; j < i; j++)
					{
						if((envelopein[Max_pos] < envelopein[j]) && (envelopein[j] > envelopein[j+1]))
						{
							Max_pos = j;
							break;
						}
					}

					half_envelope_max = (envelopein[Max_pos]/2);
					//half23_envelope_max = (envelopein[Max_pos]*2)/3;

					//����������
					for(j = Max_pos;j > Max_pos-12; j--)
					{
						if(envelopein[j] < half_envelope_max)
						{
							width_low = j;
							break;
						}
						else
						{
							width_low = pos_begin;
						}
					}
					for(j = Max_pos;j < Max_pos+12;j++)
					{
						if(envelopein[j] < half_envelope_max)
						{
							width_high = j;
							break;
						}
						else
						{
							width_high = i;
						}
					}
					width = width_high - width_low;

					//�����ǰ�����
					ave[0] = mean_zh(&envelopein[Max_pos - 112], 100, 2)*5;//����5
					ave[1] = mean_zh(&envelopein[Max_pos - 212], 100, 2)*5;
					ave[2] = mean_zh(&envelopein[Max_pos - 312], 100, 2)*5;
					ave[3] = mean_zh(&envelopein[Max_pos - 412], 100, 2)*5;
					ave[4] = mean_zh(&envelopein[Max_pos - 512], 100, 2)*5;
					ave[5] = mean_zh(&envelopein[Max_pos - 612], 100, 2)*5;
					ave[6] = mean_zh(&envelopein[Max_pos - 712], 100, 2)*5;
					ave[7] = mean_zh(&envelopein[Max_pos - 812], 100, 2)*5;
					max_zh(&max_ave1, &ave_num, ave, 8, 1);//�ҵ�8����ǰ���������ֵ�͵���

					//�����������
					sum_feng_hou = 0;
					sum_mean_feng_hou = 0;
					for(j= (Theory_wide>>1);j<512+(Theory_wide>>1);j++)////����ۼ�5.12ms������
					{
						sum_feng_hou = sum_feng_hou + envelopein[Max_pos + j] ;//���ֵ��512�����ֵ
					}
					sum_mean_feng_hou = (sum_feng_hou>>9)*2; //�ֲ�����ȵ�2����ȡƽ��ֵ�������ƶ�9λ��������512

					jishu = jishu + 1;
					sum_mean_save = 0;//SD���洢��ֵ
					width_save = 0;   //SD���洢��ֵ

					//����ǰα��������α������
					point_count_qian = 0;
					for(j = Max_pos-516; j < Max_pos-15; j++)
					{
						if (envelopein[j] > half_envelope_max)
						{
							point_count_qian = point_count_qian+1;
						}
					}

					point_count_hou = 0;
					for(j=Max_pos+(Theory_wide>>1);j<Max_pos+507;j++)
					{
						if (envelopein[j] > half_envelope_max)
						{
							point_count_hou = point_count_hou+1;
						}
					}

					//�ж��Ƿ�Ϊ��������ط�
					if((envelopein[Max_pos] > sum_mean_feng_hou) && (envelopein[Max_pos] > max_ave1) && (width > Maikuan_l) && (width < Maikuan_h) && (point_count_qian < Weifeng_qian) && (point_count_hou < Weifeng_hou) )
					{
						//asm(" bclr XF ");
						pos = 1;
						Tr_time = 0;
						IODATA &= 0xbf;    //�������䲿�ֵ�Դ�����������ֵ
						Flag_answer = 1;         //��⵽�ź�1
						sum_mean_save = sum_mean;//SD���洢��ֵ
						width_save = width;      //SD���洢��ֵ

						for(i = Max_pos - (Theory_wide>>1);i < Max_pos + (Theory_wide>>1); i++)//�����������ڽ���ȷ�����ֵ
						{
							if(envelopein[Max_pos] < Rev_envelope[i])
							{
								Max_pos = i;
							}
						}

						//�ڲ庯��78.9us
						neicha_a=(float)((float)(envelopein[Max_pos-1])+(float)(envelopein[Max_pos+1])-2*(float)(envelopein[Max_pos]))/2;
						if(neicha_a==0)
						{
							endpos_float=(float)Max_pos;
						}
						else
						{
							neicha_b=(float)((float)(envelopein[Max_pos])-(float)(envelopein[Max_pos-1]))-(float)(neicha_a*(float)(Max_pos*2-1));
							endpos_float=(-1)*neicha_b/2/neicha_a;
						}
						*endpos = Max_pos;
						//       *endpos =  endpos_float;
						resend_delay = Time_Delay - 41381 - (float)((float)((5500.0 - endpos_float))*10);	//��λ����1us,�����ڲ�
						//asm(" bset XF ");
						break;

					}//endif �ֲ�����Ⱥ���������
				}//endif��ǰ������������
			}//endif �������޾���ֵ
		}//endfor������ط�
	}//endif���ھ�������
	else
	{
		pos=0;
	}//endif���ڶ�����

	*flag = pos;//return pos
}



/*���ѯ���ź�
  �������ܣ����ѯ���źţ������⵽������Pos_flag1��־λ��endpos1���λ��
 */
void asksignal()
{
	int i,j;
	volatile short k;
	volatile long sum;
	short noise_max1,ave_num = 0;
	short point_count_zong,point_count_zong_th= 0;

	//	asm(" bclr XF ");//XF�ܽ��õ�
	dot(X_ref,Rev_FFT,Rev_xcorr,NFFT);//6.803ms//��ο��ź�1���
	cifft_NOSCALE(Rev_xcorr,NFFT);//1.731ms//������Ҷ�任
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);//0.075ms
	memset(Rev_xcorr,0,20);//  why?
	//	memset(Rev_xcorr+4076,0,20);
	j = 0;
	for(i = BUF_LEN_MAX; i  < NFFT; i++) //BUF_LEN_MAX-���ջ���������,ΪNFFT-�����źų���=2096
	{
		Rev_xcorrout[j] = Rev_xcorrout[i];
		j++;
	}//����ؽ���ص�����
	for(i = Rev_resave; i  < NFFT; i++)   //i = Rev_resave=2000
	{
		Rev_xcorrout[i] = abs(Rev_xcorr[2*i]);//Rev_xcorr��i����2
	}//0.036ms//���ʵ���Ľ��������ؽ������
	memset(h_LPF1_db,0,sizeof(short)*(N_LPF1 + 2));//0.00006ms
	oflag_LPF1 = fir2(Rev_xcorrout,h_LPF1,Rev_save_filter_out,h_LPF1_db,NFFT,N_LPF1);//2.122ms
	//���������ݽ����ص��������������ݸ�������ʱ1.101ms
	j = 0;
	for(i = 2096; i < 5500; i++)//2096���㿪ʼ��Ų��2096����Ŀռ�
	{
		Rev_envelope[j] = Rev_envelope[i];
		j++;
	}
	//������ݸ���
	j = 3404;//��3404�㿪ʼ���£��ܸ��µ���Ϊ2096
	for(i = Rev_resave; i < NFFT; i++)  //i = Rev_resave��Ϊ�����źų��ȣ�2000
	{
		Rev_envelope[j] = Rev_save_filter_out[i];
		j++;
	}
	//�������ѧϰ���õ���������ֵ��

	sum = 0;
	for(k = 2048;k < NFFT; k++)
	{
		sum = sum + Rev_save_filter_out[k];
	}
	sum = sum>>11;
	NOISE_TH_huadong = NOISE_TH_huadong*0.99 + (float)(sum*0.01);

	//��ȡ�������ֵ���õ����������ޡ�
	max_zh(&noise_max1, &ave_num, Rev_save_filter_out , NFFT, 2);
	noise_maxbuffer[0] = noise_max1;

	if(noise_maxbuffer[0] > noise_maxbuffer[1])
	{
		if((noise_maxbuffer[0]>>1) > NOISE_TH_huadong)
		{
			NOISE_TH_NEW = (noise_maxbuffer[0]>>1);
		}
		else
		{
			NOISE_TH_NEW = NOISE_TH_huadong;
		}
		point_count_zong_th = (noise_maxbuffer[0]>>1);
	}
	else
	{
		if((noise_maxbuffer[1]>>1) > NOISE_TH_huadong)
		{
			NOISE_TH_NEW = (noise_maxbuffer[1]>>1);
		}
		else
		{
			NOISE_TH_NEW = NOISE_TH_huadong;
		}
		point_count_zong_th = (noise_maxbuffer[1]>>1);
	}


	point_count_zong = 0;
	for(j=1308;j<3404;j=j+7)   //j=1308������ϴε���ؽ������2096���㣬����ֵΪ7�����������һ�룬
	{
		if (Rev_envelope[j] > point_count_zong_th)
		{
			point_count_zong = point_count_zong+1;
		}
	}

	noise_maxbuffer[1] = noise_maxbuffer[0];

	//	asm(" bset XF ");//XF�ܽ��ø�

	if (DMA_count > 1)
	{
		if(point_count_zong <37) //����;��/��������������ֵһ�룩
		{
			detect(Rev_envelope,3404,&Pos_flag,&endpos);//��⺯���������100����ط壬���ʱ�䷶ΧΪ0.056ms~2.85ms  BUF_LEN_MAX
		}
		point_count_zong = 0;
	}
}



/*����ź�	�������ܣ�����źų���
 */
void detect_signal()
{
	while((Flag_answer==0) && (FLAG_RS232_REVOK == 0) && ((IODATA & 0x0010) != 0x0010))//û�м�⵽�źŻ��ߴ�������������ⲿ��Դ����
	{
		if (DMAdone == TRUE)
		{
			//			asm(" bclr XF ");//XF�ܽ��õ�
			DMAdone = FALSE;
			while(DMA_FGETH(myhDma,DMACCR,ENDPROG))//��ȡDMA�Ĵ����ֶ�ֵ
			{
			}
			if (DMAnum%2 == 1)
			{
				DMA_RSETH(myhDma,DMACSSAU,RevsrcAddrHi);//��������DMA��ַ
				DMA_RSETH(myhDma,DMACSSAL,RevsrcAddrLo);
				DMA_RSETH(myhDma,DMACDSAU,Revdst2AddrHi);
				DMA_RSETH(myhDma,DMACDSAL,Revdst2AddrLo);
			}
			if (DMAnum%2 == 0)
			{
				DMA_RSETH(myhDma,DMACSSAU,RevsrcAddrHi);//�������DMA��ַ
				DMA_RSETH(myhDma,DMACSSAL,RevsrcAddrLo);
				DMA_RSETH(myhDma,DMACDSAU,Revdst1AddrHi);
				DMA_RSETH(myhDma,DMACDSAL,Revdst1AddrLo);
			}
			DMA_RSETH(myhDma,DMACEN, BUF_LEN_MAX);
			DMA_RSETH(myhDma,DMACFN, 1);
			/* Set programmation bit to 1, ENDPROG = 1) */
			DMA_FSETH(myhDma,DMACCR,ENDPROG,1);//��������DMA����ʼ��һ֡����
			detect_prepare();//��ⶨλ�ź�Ԥ����/3.5328ms-100MHz
			asksignal();//��ⶨλ�ź�//15.66ms-100MHz
			if(Flag_answer == 0)
			{
				//asm(" bset XF ");
				memcpy(Rev_save,Rev,NFFT);//������һ�����ݣ���������
				Tr_time++; //�����Դ�رռ����Լ�
				//asm(" bclr XF ");
			}
			if((Tr_time >= (Delay_Sig*60*100000)/(RevDatReadLen)) && (Delay_Sig != 0))
			{
				IODATA |= 0x0040;   //����������Delay_Sig���Ӻ�û�м�⵽�źź�رշ��䲿�ֵ�Դ
				Tr_time = 0;
			}
			if(DMA_count < 2)
			{
				DMA_count = DMA_count +1;
			}
			//          asm(" bset XF ");//XF�ܽ��ø�
		}//end if(DMAdone==TRUE)
	}    //end while((Flag_answer==0)&&(FLAG_RS232_REVOK==0))
	do
	{
		;
	}while(DMAdone==FALSE);
} //����ƵΪ100MHzʱ���ӻ�ȡ��������һ�ɼ�����������ɸ��������ݵĴ�����ʱ7.759ms(�Ż��ȼ�2)�����Ż���Ϊ18.589ms





/*�����ź�1
  �������ܣ������ź�1������DMA0ͨ�����䣬��ʱ��1Ϊͬ���¼�
 */
void Senddata0()
{
	//	IODATA = 0x0000;//�������䲿�ֵĵ�Դ
	IFR1 = 0x0ffff;
	IER1 |= 0x0004;
	DMA_FSETH(hDmaSend0,DMACCR,ENDPROG,1);//��ENDPROG��1
	DMA_start(hDmaSend0);//��ʼ��������
	TIMER_start(mhTimer1);
	do{
		;
	}while(FLAGSENDOVER == 0);//�������
	FLAGSENDOVER = 0;
	IFR1 = 0x0ffff;
	IER1 &= 0x0fffb;//����DMA1���ڷ���ѯ���ź�
	// 	IODATA = 0x0040;//�رշ��䲿�ֵĵ�Դ
}

/*ִ�ж���1
  ������ܣ���⵽�ź?��ִ�ж���1
 */
void take_action1()
{
	short i;
	long temp;
	//����ת���ӳ�
	//	resend_delay = Time_Delay - 41760 - (Uint32)((float)((5500 - endpos_float))*10);	//��λ����1us,�����ڲ�
	//	resend_delay = Time_Delay - 41760 - (Uint32)((float)((5500 - endpos))*10);       //�λ���?us,���ڲ�

	if(resend_delay > 0)
	{
		// �ӳ�
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//����DMA1��
		TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
		while(1)
		{
			if( resend_delay == timer1_cnt )
			{
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
				break;
			}
		}
	}

	Senddata0();//�����ź�1
	//	asm(" bset XF ");
	// ѹ������200ms��
	get_pressure();
	if(FLAG_yl == 0)
	{
		resend_delay = 200000 - 46790 - (Uint32)(0.134*timer1_cnt) + (Uint32)(depth*200);//��λ1us;׼ȷ���ݡ�
	}
	else if(FLAG_yl == 1)
	{
		resend_delay = 200000 - 46800 - (Uint32)(0.134*timer1_cnt) + (Uint32)(depth*200);//��λ1us
	}

	if(resend_delay > 0)
	{
		// �ӳ�
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
		TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
				break;
			}
		}
	}

	//Senddata0();//�����ź�1

	resend_delay = 50000;
	if(resend_delay > 0)
	{
		// �ӳ�
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
		TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
				break;
			}
		}
	}

	//�򿪶�ʱ������������¶Ⱥ͵�ص�ѹʱ�䣻�Ӵ˴���SD������104ms��
	timer1_cnt = 0;

	IFR1 = 0x0ffff;
	IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
	TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
	get_temperature();
	IODATA = 0x0000;
	get_voltage();
	temp = 0;
	for(i = 100;i<100+1024;i++)
	{
		temp = temp + fabs(Rev_save[i]);
	}
	noise_10ms = (short)(temp>>10);

	uart_config();
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;   //��ط����ֵ��8λ
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//��ط����ֵ��8λ
	SD_write[30] = (sum_mean_save) & 0x0ff;   //��ط��ֵ��8λ
	SD_write[31] = (sum_mean_save>>8) & 0x0ff;//��ط��ֵ��8λ
	SD_write[32] = (width_save) & 0x0ff;      //��ط��ȵ�8λ
	SD_write[33] = (width_save>>8) & 0x0ff;   //��ط��ȸ�8λ
	SD_write[34] = 0;                         //��ָ?
	SD_write[37] = (NOISE_TH_NEW) & 0x0ff;    //��ط�Ļ�����������	��8λ
	SD_write[38] = (NOISE_TH_NEW>>8) & 0x0ff; //��ط�Ļ�����������	��8λ

	TIMER_stop(mhTimer1);//�رն�ʱ��1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�

	if(SD_status == 1)
	{
		SD_WriteData(SectorCounter,SD_write);//����ȡ��Ϣд��SD��������
		SectorCounter = SectorCounter + 1;
	}

	resend_delay = 350000 - timer1_cnt - 50000;//������Ҫ�޸ģ���ѹ���������ӳ�ʱ���й�
	if(resend_delay > 0)
	{
		// ��?
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
		TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
				break;
			}
		}
	}
	//	asm(" bclr XF ");
}


/*ִ�ж���2
  �������ܣ���⵽�ź�2��ִ�ж���2
 */
void take_action2()
{
	short i;
	long temp;
	short temp_read;
	Senddata0();//�����ź�1

	IODOR = 0x0bf;
	IODATA |= 0x0080;       //GPIO7 = 1�������ͷ����ͷ�
	resend_delay = 25000000;//�����ͷ�ʱ��:25s,�ͷ����ͷ�ʱ����24s��
	//resend_delay = 100000;
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;
	TIMER_start(mhTimer1);//�򪶨ʱ�?��ʱ
	while(1)
	{
		temp_read = IODATA & 0x0040;
		if(temp_read == 0x0040)
		{
			release_flag = 1;
			break;
		}
		if(timer1_cnt == resend_delay)
		{
			release_flag = 0;
			break;

		}
	}
	TIMER_stop(mhTimer1);//�رըʱ�1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;
	IODOR = 0x0bf;
	IODATA = 0x0000;//GPIO7 = 0��ֹͣ�����ͷ����ͷ�
	Senddata0();    //�����ź�

	if(release_flag == 1)
	{
		resend_delay = 190000;	//��λ����1us
	}
	else
	{
		resend_delay = 290000;	//��λ����1us
	}
	if(resend_delay > 0)
	{
		// �ӳ�
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;       //����DMA1���ڷ���ѯ���ź�
		TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;     //����DMA1���ڷ���ѯ���ź�
				break;
			}
		}
	}
	Senddata0();//�����ź�1

	resend_delay = 50000;
	if(resend_delay > 0)
	{
		// �ӳ�
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
		TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
				break;
			}
		}
	}
	get_pressure();//ѹ����ȡ����
	//�򿪶��������������¶Ⱥ͵�ص�ѹʱ�?
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
	TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
	get_temperature();

	IODATA = 0x0000;
	get_voltage();
	temp = 0;
	for(i = 100;i<100+512;i++)
	{
		temp = temp + fabs(Rev_save[i]);
	}
	noise_10ms = (short)(temp>>9);

	uart_config();
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;//��ط����ֵ?λ
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//��ط����ֵ��8λ
	SD_write[30] = (sum_mean_save) & 0x0ff;//��ط��ֵ��8λ
	SD_write[31] = (sum_mean_save>>8) & 0x0ff;//�ط��ֵ��8λ
	SD_write[32] = (width_save) & 0x0ff;//��ط��ȵ�8λ
	SD_write[33] = (width_save>>8) & 0x0ff;//��ط��ȸ�8λ
	SD_write[34] = 1;//�ͷ�ָ��
	SD_write[35] = 1;//�ͷ�ָ��
	TIMER_stop(mhTimer1);//�رն�ʱ��1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
	if(SD_status == 1)
	{
		SD_WriteData(SectorCounter,SD_write);//���ȡ�Ϣд��SD������
		SectorCounter = SectorCounter + 1;
	}
	resend_delay = 350000 - timer1_cnt - 50000;//������Ҫ�޸ģ���ѹ���������ӳ�ʱ���й�
	if(resend_delay > 0)
	{
		// ��ʱ
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
		TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
				break;
			}
		}
	}
}
/*ִ�ж���3
  �������ܣ���⵽�ź�3��ִ�ж���3
 */
void take_action3()
{
	short i;
	long temp;
	//����ת���ӳ�
	resend_delay = 75000;	//��λ����1us
	if(resend_delay > 0)
	{
		// �ӳ�
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
		TIMER_start(mhTimer1);//�򿪶�ʱ��1ʱ
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
				break;
			}
		}
	}

	Senddata0();//�����ź�1

	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;//ʹ�ܶ�1�ж�
	TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
	get_voltage();

	TIMER_stop(mhTimer1);//�رն�ʱ��1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//�رն�ʱ��1�ж�

	resend_delay = 175000 + (Uint32)((float)voltage_AD/1024*3.3*8*10000) - timer1_cnt;

	if(resend_delay > 0)
	{
		// �ӳ�
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
		TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
				break;
			}
		}
	}
	Senddata0();//�����ź�1

	resend_delay = 50000;
	if(resend_delay > 0)
	{
		// �ӳ�
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
		TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
				break;
			}
		}
	}
	get_pressure();//ѹ����ȡ
	//�򿪶�ʱ������������¶Ⱥ͵�ص�ѹʱ
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
	TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
	get_temperature();
	IODATA = 0x0000;
	get_voltage();
	temp = 0;
	for(i = 100;i<100+1024;i++)
	{
		temp = temp + fabs(Rev_save[i]);
	}
	noise_10ms = (short)(temp>>10);
	uart_config();
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;//��ط����ֵ��8λ
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//��ط����ֵ��8λ
	SD_write[30] = (sum_mean_save) & 0x0ff;//�����ֵ�??
	SD_write[31] = (sum_mean_save>>8) & 0x0ff;//��ط��ֵ��8?
	SD_write[32] = (width_save) & 0x0ff;//��ط��ȵ�8λ
	SD_write[33] = (width_save>>8) & 0x0ff;//��ط��ȸ�8λ
	SD_write[34] = 2;//����ָ��
	TIMER_stop(mhTimer1);//�رն�ʱ��1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
	if(SD_status == 1)
	{
		SD_WriteData(SectorCounter,SD_write);//����ȡ��Ϣд��SD��������
		SectorCounter = SectorCounter + 1;
	}
	resend_delay = 2291000 - timer1_cnt - 50000;//������Ҫ޸ģ���ѹ���������ӳ�ʱ���?
	if(resend_delay > 0)
	{
		// �ӳ�
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
		TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
				break;
			}
		}
	}
}


/*
ϵͳ����
 */
void system_config()
{
	//	IODOR=0xff;
	IODATA=0x00;
	IER0=0x0000;
	IFR0=0xFFFF;
	IER1=0x0011;//ʹ��DMAC2����ж?
	IFR1=0xFFFF;
	variable_init();
	//	asm(" bclr intm ");
	while(DMA_FGETH(myhDma,DMACCR,ENDPROG))
	{
		;
	}
	DMA_RSETH(myhDma,DMACSSAU,RevsrcAddrHi);
	DMA_RSETH(myhDma,DMACSSAL,RevsrcAddrLo);
	DMA_RSETH(myhDma,DMACDSAU,Revdst1AddrHi);
	DMA_RSETH(myhDma,DMACDSAL,Revdst1AddrLo);
	DMA_RSETH(myhDma,DMACEN, BUF_LEN_MAX);
	DMA_RSETH(myhDma,DMACFN, 1);
	DMA_FSETH(myhDma,DMACCR,ENDPROG,1);//��������DMA
	DMA_count = 0; //DMA�������㡣
	//	DMA_start(myhDma);//DMA��ʼ�ɼ�
}

/*�紥������*/
void Respondermod()
{
	while(1)
	{
		if(responder_flag == 1)
		{
			if(count_timer == 0)
			{
				responder_flag = 0;
				timer1_cnt = 0;
				IRQ_enable(IRQ_EVT_TINT1);//����TINT1��1us��ʱ
				IRQ_disable(IRQ_EVT_INT0);//�ر��ⲿ�ж�0
				IRQ_clear(IRQ_IEMASK_ALL);//������еı�־λ
				TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
				while(timer1_cnt < 3);
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IRQ_disable(IRQ_EVT_TINT1);//�ر�TINT1��1us��ʱ
				IRQ_clear(IRQ_IEMASK_ALL);//������еı�־λ
				count_timer ++;
				IRQ_enable(IRQ_EVT_INT0);//�����ⲿ�ж�0
				break;
			}
			else
			{
				responder_flag = 0;
				timer1_cnt = 0;
				IRQ_disable(IRQ_EVT_INT0);//�ر��ⲿ�ж�0
				IRQ_enable(IRQ_EVT_TINT1);//����TINT1��1us��ʱ
				IRQ_clear(IRQ_IEMASK_ALL);//������еı�־λ
				TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
				while(timer1_cnt < responder_Delay);
				TIMER_stop(mhTimer1);//�رն�ʱ��1
				IRQ_disable(IRQ_EVT_TINT1);//�ر�TINT1��1us��ʱ
				IRQ_clear(IRQ_IEMASK_ALL);//������еı�־λ
				count_timer = 0;
			}
		}

		Senddata0();//�����ź�1
		resend_delay = 500000;
		if(resend_delay > 0)
		{
			timer1_cnt = 0;
			TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
			while(timer1_cnt < resend_delay);
			TIMER_stop(mhTimer1);//�رն�ʱ��1
			IRQ_disable(IRQ_EVT_TINT1);//�ر�TINT1��1us��ʱ
		}
		IRQ_enable(IRQ_EVT_INT0);//�����ⲿ�ж�0
		IRQ_clear(IRQ_IEMASK_ALL);//������еı�־λ
		if(FLAG_RS232_REVOK == 1)//���յ������ж�
		{
			transponder_mode1 = 0;
			break;
		}
		if((transponder_mode1 ==1) && ((IODATA & 0x0010) != 16))//��⵽�ⲿ��Դ��ʧ������ѭ��
			break;
	}
}

#endif /* SUBFUNCTION_H_ */
