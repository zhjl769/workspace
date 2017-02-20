
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

//��ʱ��1 ��ͬ��ʱʱ�亯��
void timer1_jishi(Uint32 resend_delay)
{
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


/*�����ο��ź�*/
void Produceref()
{
	volatile unsigned long lfm_head;
	volatile double t_lfm;
	volatile double fai;
	volatile short q;  
	int i;
	memset(Rev,0,sizeof(short)*NFFT);
	fai = 0;
	for(q = 0; q < 8; q++)
	{
		for(lfm_head = 0; lfm_head < RAMSES_time1[RAMSES_code1[q]-1]; lfm_head++)
		{
			t_lfm = lfm_head/40000.0; //����Ƶ��40kHz
			templfm = (short)(-550*(sin(2*PI*RAMSES_freq1[RAMSES_code1[q]-1]*t_lfm + fai)));
			Rev[q*50 + lfm_head] = templfm;
		}
		t_lfm = (lfm_head)/40000.0;
		fai = 2*PI*RAMSES_freq1[RAMSES_code1[q]-1]*t_lfm + fai;
	}

	for(i = 0; i < 400; i++)
	{
		Rev[2047-i] = Rev[i];
	}    

	for(i = 0; i < 400; i++)
	{
		Rev[i] = Rev[1648+i];
		Rev[1648+i] = 0;
	}  

	memset(X_ref,0,sizeof(short)*2*NFFT);
	for (i = 0; i < NFFT; i++) 
	{
		X_ref[2*i] = Rev[i];
		X_ref[2*i+1] = 0;
	}
	cfft_NOSCALE(X_ref,NFFT); 
	cbrev(X_ref,X_ref,NFFT); 

}

void Produceref1()
{
	volatile unsigned long lfm_head;
	volatile double t_lfm;
	volatile double fai;
	volatile short q;  
	int i;

	memset(Rev,0,sizeof(short)*NFFT);

	fai = 0;
	for(q = 0; q < 8; q++)
	{
		for(lfm_head = 0; lfm_head < RAMSES_time1[RAMSES_code1[q]-1]; lfm_head++)
		{
			t_lfm = lfm_head/40000.0; //����Ƶ��40kHz
			templfm = (short)(-550*(sin(2*PI*RAMSES_freq1[RAMSES_code1[q]-1]*t_lfm + fai)));
			Rev[q*50 + lfm_head] = templfm;
		}
		t_lfm = (lfm_head)/40000.0;
		fai = 2*PI*RAMSES_freq1[RAMSES_code1[q]-1]*t_lfm + fai;
	}

	for(i = 0; i < 400; i++)
	{
		Rev[2047-i] = Rev[i];
	}    

	for(i = 0; i < 400; i++)
	{
		Rev[i] = Rev[1648+i];
		Rev[1648+i] = 0;
	}  

	memset(X_ref,0,sizeof(short)*2*NFFT);
	for (i = 0; i < NFFT; i++) 
	{
		X_ref[2*i] = Rev[i];
		X_ref[2*i+1] = 0;
	}
	cfft_NOSCALE(X_ref,NFFT); 
	cbrev(X_ref,X_ref,NFFT); 

}


/*
void Produceref()
{
	unsigned long lfm_head;
	double t_lfm;
	int i;
	memset(Rev,0,sizeof(short)*NFFT);
    //��λ�ź�25ms
    for(lfm_head = 1; lfm_head < 1001; lfm_head++)
    {
       t_lfm = (float)lfm_head/40000.0;//����Ƶ��100kHz
       Rev[lfm_head - 1] = (short)(-100*(cos(2*PI*14000*t_lfm - PI*5000*t_lfm*t_lfm/0.025)));//��ʼƵ��9kHz����ֹƵ��14kHz,25ms
    }

	for (i = 0; i < NFFT; i++)
	{
		X_ref[2*i] = Rev[i];//ż��ַ����7bit
		X_ref[2*i+1] = 0;
	}
	cfft_NOSCALE(X_ref,NFFT);  //�Ƿ����������
	cbrev(X_ref,X_ref,NFFT); 
}  
 */

/*�����˲���ϵ��*/
void Producefilter()
{
	int i;
	for (i = 0; i < 2*NFFT; i ++)
	{
		h_BPF1[i] = 0;
	}
	for (i = 0; i < 257; i ++)
	{
		h_BPF1[2*i] = filter1[i]/1200;
	}
	cfft_NOSCALE(h_BPF1,NFFT);  //�Ƿ����������
	cbrev(h_BPF1,h_BPF1,NFFT);

	for (i = 0; i < 2*NFFT; i ++)
	{
		h_BPF2[i] = 0;
	}
	for (i = 0; i < 257; i ++)
	{
		h_BPF2[2*i] = filter2[i]/1200;
	}
	cfft_NOSCALE(h_BPF2,NFFT);  //�Ƿ����������
	cbrev(h_BPF2,h_BPF2,NFFT); 
} 

/*���������ź�
  �������ܣ����������źţ�ѯ���źźͲ����źŴ����CE2�ռ�
			�׵�ַ�ֱ�Ϊ0x400000��0x500000
 */
void Producelfm()
{
	volatile unsigned long lfm_head;
	volatile double t_lfm;
	volatile double fai;
	volatile short q;
	int i;

	//��λ�ź�10ms
	i = 0;
	fai = 0;
	for(q = 0; q < 8; q++)
	{
		for(lfm_head = 0; lfm_head < RAMSES_time[RAMSES_code[q]-1]; lfm_head++)
		{
			t_lfm = lfm_head/1000000.0;
			templfm = (short)(2000*(sin(2*PI*RAMSES_freq[RAMSES_code[q]-1]*t_lfm + fai)));

			if(templfm < 0)
			{
				templfm=0x0003;//GPIO2=1;GPIO1=0;GPIO0=1;
			}
			else
			{
				templfm=0x0005;//GPIO2=0;GPIO1=1;GPIO0=1;
			}

			Send_sig [i] = templfm;
			i = i+1 ;
		}
		t_lfm = (lfm_head)/1000000.0;
		fai = 2*PI*RAMSES_freq[RAMSES_code[q]-1]*t_lfm + fai;
	}
}


void ProduceCIF()
{
	volatile unsigned long lfm_head;
	volatile double t_lfm;
	volatile double fai;
	volatile short q;
	int i;

	//��λ�ź�10ms
	i = 0;
	fai = 0;
	for(q = 0; q < 8; q++)
	{
		for(lfm_head = 0; lfm_head < RAMSES_time[RAMSES_code[q]-1]; lfm_head++)
		{
			t_lfm = lfm_head/1000000.0;
			templfm = (short)(2000*(sin(2*PI*RAMSES_freq[RAMSES_code[q]-1]*t_lfm + fai)));

			if(templfm < 0)
			{
				templfm=0x0003;//GPIO2=1;GPIO1=0;GPIO0=1;
			}
			else
			{
				templfm=0x0005;//GPIO2=0;GPIO1=1;GPIO0=1;
			}

			Send_sig [i] = templfm;
			i = i+1 ;
		}
		t_lfm = (lfm_head)/1000000.0;
		fai = 2*PI*RAMSES_freq[RAMSES_code[q]-1]*t_lfm + fai;
	}
}




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
	Flag_CIF = 0;

	temperature = 0.0;
	block2 = 2;
	memset(count_final_jiance,0,12);
	testpinlv_flag = 0;
	release_flag = 0;//�ͷųɹ�״̬λ

	memset(temp_dainei_temp,0,sizeof(short)*(NFFT_mingling));
	memset(Rev,0,sizeof(short)*NFFT);
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	memset(Rev_xcorr,0,sizeof(short)*2*NFFT);
	memset(Rev_xcorrout,0,sizeof(short)*NFFT);
	memset(Rev_save_filter_out,0,sizeof(short)*NFFT);
	memset(Rev_envelope,0,sizeof(short)*4096);

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
	while(j_uart < temp_js)
	{
		buff = UART_LSR485;
		f_send_uart = buff & (0x0060);
		if(f_send_uart!=0)
		{    
			UART_THR485 = init_rs485[j_uart];
			for(ii=100;ii>0;ii--);
			j_uart++;
		}
	}// end while
	IER0 |= 0x0008;
	IFR0 = 0x0ffff;//ʹ��int0�ж�
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
	} 
	//��ʾѹ����������ʼ���ɹ�
	IER0 &= 0x0fff7;
	IFR0 = 0x0ffff;//�ر�int2�ж� 
}
void get_pressure()
{
	volatile short temp_js,buff,f_send_uart,j_uart;
	volatile Uint32 ii;
	FLAG_yl = 0;
	j_uart = 0;
	temp_js = 5;
	f_send_uart = 0;
	//���ͳ�ʼ��ѹ������������
	while(j_uart < temp_js)
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
	} 
	timer1_cnt = ii;
	temp_yl_data = (((Uint32)RS485_DATA[2])<<24) + (((Uint32)RS485_DATA[3])<<16) + (((Uint32)RS485_DATA[4])<<8) + ((Uint32)RS485_DATA[5]);
	if(temp_yl_data > 268435455)
	{
		temp_yl_data = 0;
	}
	//temp_yl_data��ֵ��λΪ��Pa����1 Pa=1e-5 bar=��1e-5��*10 m=1e-4 m
	//��λ����ȷ��ף����ӳ٣�long��FLAG_yl_data��100usʱ��,���������У�0.1m�����ӳ�0.1ms  ѹ������ʱ��Ϊ1.5m��Ӧ0.2ms
	//�˴�Ҳ���Ա任Ϊ��FLAG_yl_data = (long)temp_yl_data/10;�����ӳٵ�usֵ��
	//��ʾѹ����������ȡ��Ϣ�ɹ�
	if(temp_yl_data > 0)
	{
		depth = (float)(temp_yl_data)/10000;//(���Ƚ��õ���ѹ��ֵ������m=yl/10000���ٽ��������ת��Ϊʱ��ֵ=m/1.5����λ��ms��)
	}
	else
	{
		depth = 0.0;
	}

}


/*��ȡ��ѹ��Ϣ����
  �������ܣ�����DSP�ڲ�AD(10bit),������Ϊ100kHz,�ɼ�64������,ȡ��ֵ
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
	/*
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
	 */
	get_voltage();   //��ȡ��ѹ��Ϣ
	get_pressure();  //ѹ����ȡ��������ȡֵΪ0��ѹ������������������
	//��ȡ�¶ȣ�
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;  //ʹ�ܶ�ʱ��1�ж�
	TIMER_start(mhTimer1);//��1��ʱ
	get_temperature();//��ȡ�¶�
	IODATA = 0x0000;
	TIMER_stop(mhTimer1);//�رն�ʱ��1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//�رն�ʱ��1�ж�
	//��ȡ�¶�
	SPCR2_0 = 0x00c1;    //��McBSP
	SPCR1_0 = 0x0001;
	get_noise();//��ȡ��ǰ����
	SPCR2_0 = 0x0000;    //�ر�McBSP
	SPCR1_0 = 0x0000;
	uart_config();
}

void URAT_reply_func()
{
	short i;
	FLAG_RS232_REVOK = 0;  //���ڽ��ձ�־����
	get_parameter();
	uart_config();	
	if(SD_status == 1)
	{
		SD_WriteData(7672,SD_write);//7672����Ϊÿ�α�����������
	}
	for(i = 0; i < 28; i++)//����42�ֽ���Ϣ
	{ 
		UART_THR232 = SD_write[i];
		do
		{
			buff = UART_LSR232;
			f_send_uart = buff & (0x0060);
		}while(f_send_uart == 0);//��ѯ����λ�Ƿ�����
	}
}

/*�ص��������� ����������ת��Ϊ�з�����*/
void Revprepare()
{
	int i=0,j=0;

	for (i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev[j] = Rev[i];//��������Ԫ��������ݰ��Ƶ�ǰ��
		j++;
	}
	j = 0;
	if (DMAnum%2 == 1)
	{
		for(i = Rev_resave;i < NFFT;i++)
		{
			Rev[i] = Rev1[j];//���DMAnum%2==1����Rev1�����е������Ƶ�������Ԫ�ĺ��棬�����޷�����ת��Ϊ�з�����
			j++;
		}
	}
	else
	{
		for(i = Rev_resave;i < NFFT;i++)
		{
			Rev[i] = Rev2[j];//���DMAnum%2==0����Rev2�����е������Ƶ�������Ԫ�ĺ��棬�����޷�����ת��Ϊ�з�����
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
		data_bu0[2*i] = data[i]>>8;//ż��ַ����7bit
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

/*FFT����Ԥ����
  �������ܣ������ݴ���FFT���浥Ԫ�У��鲿Ϊ0��ʵ������7bit
 */
void FFTprepare_FSK(short *data, short *data_bu0, short data_N)
{
	int i;
	for (i = 0; i < data_N; i++)
	{
		data_bu0[2*i] = data[i]>>12;//��ַ����7bit
		data_bu0[2*i+1] = 0;
	}
}

/*��˺���
  �������ܣ�������ˣ�����������������A��B�����C�����鳤��Ϊlen
  WYH  ʱ�򷴾���<--->Ƶ������ˡ�
 */
void dot(short* A,short* B,short* C,short len)
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

		C[2*i] = (short)((temp5 - temp6) >> 20);//(short)((temp5+temp6)>>2)
		C[2*i + 1] = (short)((temp7 + temp8) >> 20);//(short)((temp7-temp8)>>2)
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


/*������
  �������ܣ�����Ϊ��ط���磬���ؼ��ɹ���־λ�ͼ��λ��
			���ɹ���־λ��1	 */

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
	short half23_envelope_max = 0;

	//�жϾ������� ��ʼ���������ط����
	for(i = 800; i < len; i++)
	{
		if(envelopein[i] > NOISE_TH_NEW)
		{
			Flag_sig = 1;
			startpos = i;//��⵽�ˣ�Flag_sig��1,��λ�ø���startpos
			break;
		}
	}

	//��������������ֵ
	if(Flag_sig)  
	{
		Flag_sig = 0;//���������ھ������ޣ�Flag_sig��־λ����
		for(i = startpos;i < len+50; i++)//�ӿ�ʼ�����80���������ֵ
		{			
			if(flagg == 0)
			{
				if(envelopein[i] >= NOISE_TH_NEW)
				{
					flagg = 1;		//������ڷ�ֵ���ޣ�flagg��1
					pos_begin = i;
					Max_val = 0;
					for(j = i;j < i + 24; j++)
					{
						if(Max_val < envelopein[j])
						{
							Max_val = envelopein[j];
						}
					}
					Max_threshold = (short)Max_val*0.5;
					if((envelopein[i] < Max_threshold)) //��ȡ���߳�������޵ĵ�һ��
					{
						flagg = 0;
					}
				}
			}
			else
			{
				//if((envelopein[i] <= Max_threshold) || (Max_threshold < NOISE_TH_NEW))
				if((envelopein[i] <= Max_threshold))      //��ǰ������i��ȡ�Ҳ����������޵ĵ�һ�㡣
				{
					flagg = 0;
					for(j = pos_begin;j >= startpos; j--) //pos_beginΪ�Ҳ����������޵ĵ�һ�㡣
					{
						if(envelopein[j] <= Max_threshold)
						{
							pos_begin = j;                //pos_beginΪ������������޵ĵ�һ��
							break;
						}
					}

					//�ҵ����ֵλ��
					Max_pos = pos_begin;
					for(j = pos_begin; j < i; j++)
					{
						if(envelopein[Max_pos] < envelopein[j])
						{
							Max_pos = j;
						}
					}

					half_envelope_max = envelopein[Max_pos]/2;
					half23_envelope_max = envelopein[Max_pos]*2/3;

					//���������?
					for(j = Max_pos;j > pos_begin; j--)
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
					for(j = Max_pos;j < i;j++)
					{
						if(envelopein[j] < half23_envelope_max)
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
					ave[0] = mean_zh(&envelopein[Max_pos - 45 ], 40, 2)*4;
					ave[1] = mean_zh(&envelopein[Max_pos - 85 ], 40, 2)*4;
					ave[2] = mean_zh(&envelopein[Max_pos - 125], 40, 2)*4;
					ave[3] = mean_zh(&envelopein[Max_pos - 165], 40, 2)*4;
					ave[4] = mean_zh(&envelopein[Max_pos - 205], 40, 2)*4;
					ave[5] = mean_zh(&envelopein[Max_pos - 245], 40, 2)*4;
					ave[6] = mean_zh(&envelopein[Max_pos - 285], 40, 2)*4;
					ave[7] = mean_zh(&envelopein[Max_pos - 325], 40, 2)*4;
					max_zh(&max_ave1, &ave_num, ave, 8, 1);

					//�����������
					sum_feng_hou = 0;
					sum_mean_feng_hou = 0;
					for(j=5;j<205;j++)
					{
						sum_feng_hou = sum_feng_hou + envelopein[Max_pos + j] ;//���ֵǰ��100�����ֵ
					} 
					sum_mean_feng_hou = sum_feng_hou*2/200;  //3Ϊ��������//�ֲ������Ϊ���ھ�ֵ��3


					jishu = jishu + 1;
					sum_mean_save = 0;//SD���洢��ֵ
					width_save = 0;   //SD���洢��ֵ

					//�������о������龯
					point_count_qian = 0;
					for(j = Max_pos-200; j < Max_pos-5; j++)
					{
						if (envelopein[j] > half_envelope_max)
						{
							point_count_qian = point_count_qian+1;
						}
					}  

					point_count_hou = 0;
					for(j=Max_pos+5;j<Max_pos+200;j++)
					{
						if (envelopein[j] > half_envelope_max)
						{
							point_count_hou = point_count_hou+1;
						}
					}

					//�ж��Ƿ�Ϊ��������ط�
					//if((envelopein[Max_pos] > sum_mean) && (width > Maikuan_l) && (width < Maikuan_h) && (point_count_qian < 8) && (point_count_hou < 40)  && (SNR_HD_out > 8) )//��ط�����ֲ�����Ⱥ�������  && (SNR_HD_out > 10)
					if((envelopein[Max_pos] > sum_mean_feng_hou) && (envelopein[Max_pos] > max_ave1) && (width > Maikuan_l) && (width < Maikuan_h) && (point_count_qian < 3) && (point_count_hou < 40)   )//��ط�����ֲ�����Ⱥ�������  && (SNR_HD_out > 10)
					{
						//asm(" bclr XF ");
						pos = 1;
						Flag_CIF = 1;             //��⵽�ź�1
						Flag_CIF_IIF = 1;         //��⵽�ź�1
						sum_mean_save = sum_mean; //SD���洢��ֵ
						width_save = width;       //SD���洢��ֵ

						for(i = Max_pos - 5;i < Max_pos + 5; i++)
						{
							if(envelopein[Max_pos] < Rev_envelope[i])
							{
								Max_pos = i;
							}
						}

						//�ڲ庯��
						neicha_a=(float)((float)(envelopein[Max_pos-1])+(float)(envelopein[Max_pos+1])-2*(float)(envelopein[Max_pos]))/2;
						if(neicha_a==0)
							endpos_float=(float)Max_pos;
						else
						{
							neicha_b=(float)((float)(envelopein[Max_pos])-(float)(envelopein[Max_pos-1]))-(float)(neicha_a*(float)(Max_pos*2-1));
							endpos_float=(-1)*neicha_b/2/neicha_a;
						}
						*endpos = Max_pos;
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
void asksignal_CIF()
{	
	int i,j;
	volatile short k;
	volatile long sum;
	short noise_max1,ave_num = 0;
	short point_count_zong,point_count_zong_th= 0;

	//	asm(" bclr XF ");
	dot(X_ref,Rev_FFT,Rev_xcorr,NFFT);//0.348ms//��ο��ź�1���
	cifft_NOSCALE(Rev_xcorr,NFFT);//0.862ms//������Ҷ�任
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);//0.064ms
	memset(Rev_xcorr,0,20);
	//	memset(Rev_xcorr+4076,0,20);
	j = 0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev_xcorrout[j] = Rev_xcorrout[i];
		j++;		
	}//����ؽ���ص�����
	//	j=0;//0.100ms//0.040ms
	for(i = Rev_resave; i < NFFT; i++)
	{		 
		Rev_xcorrout[i] = abs(Rev_xcorr[2*i]);
		//		j++;
	}//0.036ms//���ʵ���Ľ��������ؽ������

	memset(h_LPF1_db,0,sizeof(short)*(N_LPF1 + 2));//0.0005ms//0.0005ms//0.0005ms
	oflag_LPF1 = fir2(Rev_xcorrout,h_LPF1,Rev_save_filter_out,h_LPF1_db,NFFT,N_LPF1);//1.37ms
	//���������ݽ����ص�����
	j = 0;
	for(i = 1648; i < 4096; i++)
	{
		Rev_envelope[j] = Rev_envelope[i];
		j++;
	}
	//������ݸ���
	j =2448;
	for(i = Rev_resave; i < NFFT; i++)  
	{
		Rev_envelope[j] = Rev_save_filter_out[i];
		j++;  
	}    
	//�������ѧϰ���õ���������ֵ��
	sum = 0;
	for(k = 0;k < 2048; k++)
	{
		sum = sum + Rev_save_filter_out[k];
	}
	sum = sum>>11;
	NOISE_TH_huadong = NOISE_TH_huadong*0.98 + (float)(sum*0.02);

	//��ȡ�������ֵ���õ����������ޡ�
	max_zh(&noise_max1, &ave_num, Rev_save_filter_out, 2048, 2);
	noise_maxbuffer[0] = noise_max1;

	if(noise_maxbuffer[0] > noise_maxbuffer[1])
	{
		if((noise_maxbuffer[0]*0.5) > NOISE_TH_huadong)
		{
			NOISE_TH_NEW = noise_maxbuffer[0]*0.5;  
		}
		else
		{
			NOISE_TH_NEW = NOISE_TH_huadong; 
		}
	}
	else 
	{
		if((noise_maxbuffer[1]*0.5) > NOISE_TH_huadong)
		{
			NOISE_TH_NEW = noise_maxbuffer[1]*0.5;  
		}
		else
		{
			NOISE_TH_NEW = NOISE_TH_huadong; 
		}
	}

	if(noise_maxbuffer[1] >noise_maxbuffer[0])
	{
		point_count_zong_th = noise_maxbuffer[1]/2;
	}
	else
	{
		point_count_zong_th = noise_maxbuffer[0]/2;
	}

	point_count_zong = 0;
	for(j=800;j<2448;j=j+4)
	{
		if (Rev_envelope[j] > point_count_zong_th)
		{
			point_count_zong = point_count_zong+1;
		}
	}
	noise_maxbuffer[1] = noise_maxbuffer[0];

	//	asm(" bset XF ");
	if (DMA_count > 1)
	{
		if(point_count_zong < 12)
		{
			detect(Rev_envelope,2448,&Pos_flag,&endpos);//��⺯���������100����ط壬���ʱ�䷶ΧΪ0.12ms~2ms
		}
		point_count_zong = 0;
	}
}


void asksignal_IIF()
{	
	int i,j;
	volatile short k;
	volatile long sum;
	short noise_max1,ave_num = 0;
	short point_count_zong,point_count_zong_th= 0;

	//	asm(" bclr XF ");
	dot(X_ref,Rev_FFT,Rev_xcorr_IIF,NFFT);  //��ο��ź�1���
	cifft_NOSCALE(Rev_xcorr_IIF,NFFT);      //������Ҷ�任
	cbrev(Rev_xcorr_IIF,Rev_xcorr_IIF,NFFT);//0.064ms
	memset(Rev_xcorr_IIF,0,20);
	j = 0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev_xcorrout_IIF[j] = Rev_xcorrout_IIF[i];
		j++;		
	}
	for(i = Rev_resave; i < NFFT; i++)
	{		 
		Rev_xcorrout_IIF[i] = abs(Rev_xcorr_IIF[2*i]);
	}

	memset(h_LPF1_db,0,sizeof(short)*(N_LPF1 + 2));//
	oflag_LPF1 = fir2(Rev_xcorrout_IIF,h_LPF1,Rev_save_filter_out_IIF,h_LPF1_db,NFFT,N_LPF1);//
	j = 0;
	for(i = 1648; i < 4096; i++)
	{
		Rev_envelope_IIF[j] = Rev_envelope_IIF[i];
		j++;
	}
	//������ݸ���
	j =2448;
	for(i = Rev_resave; i < NFFT; i++)  
	{
		Rev_envelope_IIF[j] = Rev_save_filter_out_IIF[i];
		j++;  
	}    
	//�������ѧϰ���õ���������ֵ��
	sum = 0;
	for(k = 0;k < 2048; k++)
	{
		sum = sum + Rev_save_filter_out_IIF[k];
	}
	sum = sum>>11;
	NOISE_TH_huadong_IIF = NOISE_TH_huadong_IIF*0.98 + (float)(sum*0.02);

	//��ȡ�������ֵ���õ����������ޡ�
	max_zh(&noise_max1, &ave_num, Rev_save_filter_out_IIF, 2048, 2);
	noise_maxbuffer_IIF[0] = noise_max1;

	if(noise_maxbuffer_IIF[0] > noise_maxbuffer_IIF[1])
	{
		if((noise_maxbuffer_IIF[0]*0.5) > NOISE_TH_huadong_IIF)
		{
			NOISE_TH_NEW_IIF = noise_maxbuffer_IIF[0]*0.5;  
		}
		else
		{
			NOISE_TH_NEW_IIF = NOISE_TH_huadong_IIF; 
		}
	}
	else 
	{
		if((noise_maxbuffer_IIF[1]*0.5) > NOISE_TH_huadong_IIF)
		{
			NOISE_TH_NEW_IIF = noise_maxbuffer_IIF[1]*0.5;  
		}
		else
		{
			NOISE_TH_NEW_IIF = NOISE_TH_huadong_IIF; 
		}
	}

	if(noise_maxbuffer_IIF[1] >noise_maxbuffer_IIF[0])
	{
		point_count_zong_th = noise_maxbuffer_IIF[1]/2;
	}
	else
	{
		point_count_zong_th = noise_maxbuffer_IIF[0]/2;
	}

	point_count_zong = 0;
	for(j=800;j<2448;j=j+4)
	{
		if (Rev_envelope_IIF[j] > point_count_zong_th)
		{
			point_count_zong = point_count_zong+1;
		}
	}
	noise_maxbuffer_IIF[1] = noise_maxbuffer_IIF[0];
	//	asm(" bset XF ");
	if (DMA_count > 1)
	{
		if(point_count_zong < 12)
		{
			detect(Rev_envelope_IIF,2448,&Pos_flag,&endpos);//���ʱ�䷶ΧΪ0.12ms~2ms
		}
		point_count_zong = 0;
	}
}  


/*�����ź�1
  �������ܣ������ź�1������DMA0ͨ�����䣬��ʱ��1Ϊͬ���¼�
 */
void Senddata0()
{
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
}

/*���׼������
  �������ܣ����׼���������ص�������FFT�任
 */
void detect_prepare()
{
	Revprepare();   //0.075ms//�ص���������
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	FFTprepare(Rev,Rev_FFT,NFFT);//0.103ms
	cfft_NOSCALE(Rev_FFT,NFFT);  //0.862ms//FFT�任
	cbrev(Rev_FFT,Rev_FFT,NFFT);  //0.038ms 
	memset(Rev_FFT,0,20);
	//	memset(Rev_FFT+4076,0,20);
}

/*FSK���׼������
  �������ܣ����׼���������ص�������FFT�任
 */
void detect_prepare_FSK()
{
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	FFTprepare_FSK(Rev,Rev_FFT,NFFT);//0.103ms
	cfft_NOSCALE(Rev_FFT,NFFT);  //0.862ms//FFT�任
	cbrev(Rev_FFT,Rev_FFT,NFFT);  //0.038ms 
}

void Tranmit_CIF()
{
	IER0 = 0x0010; //ʹ�ܶ�ʱ��0
	IFR0 = 0x0ffff;
	IER1 = 0x0001; //ʹ�ܶ�ʱ��0������1�ж�
	IFR1 = 0x0ffff;
	TIMER_start(mhTimer0);

	timer0_cnt = 0;
	do 
	{
		;
	}while(Flag_timer == 0);
	Flag_timer = 0;
	Senddata0();  //����CIF
}   

void take_action_CIF_IIF()
{
	resend_delay = Time_Delay - 51680 - (Uint32)((2*NFFT - endpos_float)*25);//��λ����1us,�����ڲ�
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}

	Senddata0();

	//���ñܻ���ʱ��;��λus
	resend_delay = 100000; 
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
}


/*ִ�ж���1*/
void take_action1( short transmit_sig_val )
{
	//	short i;
	//	long temp;
	/*
	//����ת���ӳ�
	resend_delay = Time_Delay - 51680 - (Uint32)((2*NFFT - endpos_float)*25);//��λ����1us,�����ڲ�
	if(resend_delay > 0)
	{
        timer1_jishi( resend_delay);
	}
	 */
	Senddata0();//�����ź�1

	//�����źŵ�ʱ����;��λus
	resend_delay = 200000 + transmit_sig_val*10000;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}

	Senddata0();//�����ź�1

	resend_delay = 50000;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}

	resend_delay = 350000 - timer1_cnt - 50000;//������Ҫ�޸ģ���ѹ���������ӳ�ʱ���й�
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
}


/* ִ���ͷ�*/
void take_action2()
{
	//	short i;
	//	long temp;
	short temp_read;
	Senddata0();//�����ź�1

	IODOR = 0x0bf;
	IODATA |= 0x0080;       //GPIO7 = 1�������ͷ����ͷ�
	resend_delay = 25000000;//�����ͷ�ʱ��:25s,�ͷ����ͷ�ʱ����24s��
	//resend_delay = 100000;
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;
	TIMER_start(mhTimer1);
	while(1)
	{
		temp_read = IODATA & 0x0040;
		if(temp_read == 0x0040)
		{
			release_flag = 1;
			FSK_flag=1;
			break;
		}
		if(timer1_cnt == resend_delay)
		{
			release_flag = 0;
			break;
		}
	}
	TIMER_stop(mhTimer1);//�ر�

	IFR1   = 0x0ffff;
	IER1  &= 0x0ffbf; //�رն�ʱ��1�ж�
	IODOR  = 0x0bf;
	IODATA = 0x0000;  //GPIO7 = 0��ֹͣ�����ͷ����ͷ�
	Senddata0();      //�����ź�

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
		timer1_jishi( resend_delay);
	}
	Senddata0();      //�����ź�

	resend_delay = 50000;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
	/*
	resend_delay = 350000 - timer1_cnt - 50000;//������Ҫ�޸ģ���ѹ���������ӳ�ʱ���й�
	if(resend_delay > 0)
	{
        timer1_jishi( resend_delay);
	}
	 */
} 


/* �����ϴ�*/
void take_action3()
{
	//����ת���ӳ�
	Senddata0();          //�����ź�1

	//��ȡ��ѹ�����������ʱ��
	timer1_cnt = 0;
	IFR1  = 0x0ffff;
	IER1 |= 0x0040;       //ʹ�ܶ�1�ж�
	TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ 
	get_voltage();
	TIMER_stop(mhTimer1); //�رն�ʱ��1
	IFR1  = 0x0ffff;
	IER1 &= 0x0ffbf;      //�رն�ʱ��1�ж�
	//��ȡ��ѹ���������������ʱ
	//����200ms+��ѹ*10ms
	resend_delay = 200000 + (Uint32)((float)voltage_AD/1024*3.3*10000) - timer1_cnt;

	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
	Senddata0();//�����ź�1

	//����ܻ���ʱ��
	resend_delay = 50000;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
}  

/* ѹ���ϴ�*/
void take_action4()
{
	Senddata0();    //�����ź�1
	//
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040; //ʹ�ܶ�1�ж�
	TIMER_start(mhTimer1);//�򿪶�ʱ��1��ʱ
	get_pressure();  //ѹ����ȡ��������ȡֵΪ0��ѹ������������������

	TIMER_stop(mhTimer1);//�رն�ʱ��1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//�رն�ʱ��1�ж�

	//200ms+���m��0.1ms
	resend_delay = 200000 + (Uint32)((float)depth*0.1) - timer1_cnt;

	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
	Senddata0();//�����ź�1

	//����ܻ���ʱ��
	resend_delay = 50000;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
}   



void take_action_FSK()
{

	if (sum_count_FSK_cmd1[0]>9)
	{ 
		FSK_flag=1;
		transponder_mode = 1;
		main_node = 1;
		take_action1( transmit_sig_val);   
	}

	else if (sum_count_FSK_cmd1[1]>9)//��ֹCIF
	{
		FSK_flag=1;
		transponder_mode = 1; 
		main_node = 0;
		main_station = 0; //���ڵ�
		CIF_enable = 0;   //��ֹCIF
		IIF_enable = 1;   //ʹ��IIF
		XDCZ = 0;
		transmit_sig_val=2; //����IRFʱ����
		take_action1( transmit_sig_val);     
	}

	else if (sum_count_FSK_cmd1[2]>9)//��ֹIIF
	{
		FSK_flag=1;
		transponder_mode = 1; 
		main_node = 0;
		main_station = 0; //���ڵ�
		CIF_enable = 1;   //ʹ��CIF
		IIF_enable = 0;   //��ֹIIF
		XDCZ = 0;
		transmit_sig_val=3; //����IRFʱ����
		take_action1( transmit_sig_val);     
	}

	else if (sum_count_FSK_cmd1[3]>9)//����Ϊ��վ;����1
	{
		FSK_flag=1;
		transponder_mode = 1;//����Ϊˮ��ģʽ
		main_node = 0;       //����Ϊ��վ
		main_station = 1;    //���ڵ�
		responder_period=3;  //����CIF��������3��
		CIF_enable = 0;      //��ֹCIF
		IIF_enable = 0;      //��ֹIIF
		XDCZ = 0;            //��ֹ��Բ���
		transmit_sig_val=4;  //����IRFʱ����
		take_action1( transmit_sig_val);     
	}

	else if (sum_count_FSK_cmd1[4]>9)//����Ϊ��վ;����2
	{
		FSK_flag=1;
		transponder_mode = 1;//����Ϊˮ��ģʽ
		main_node = 0;       //����Ϊ��վ
		main_station = 1;    //���ڵ�
		responder_period=4;  //����CIF��������4��
		CIF_enable = 0;      //��ֹCIF
		IIF_enable = 0;      //��ֹIIF
		XDCZ = 0;            //��ֹ��Բ���
		transmit_sig_val=4;  //����IRFʱ����
		take_action1( transmit_sig_val);     
	}

	else if (sum_count_FSK_cmd1[5]>9)//����Ϊ��վ;����3
	{
		FSK_flag=1;
		transponder_mode = 1;//����Ϊˮ��ģʽ
		main_node = 0;       //����Ϊ��վ
		main_station = 1;    //���ڵ�
		responder_period=5;  //����CIF��������3��
		CIF_enable = 0;      //��ֹCIF
		IIF_enable = 0;      //��ֹIIF
		XDCZ = 0;            //��ֹ��Բ���
		transmit_sig_val=6;  //����IRFʱ����
		take_action1( transmit_sig_val);     
	}

	else if (sum_count_FSK_cmd1[6]>9)//��Բ���;�������1
	{
		FSK_flag=1;
		transponder_mode = 1;//ˮ��ģʽ
		main_node = 1;       //��ֲ���ģʽ
		main_station = 3;    //���ڵ�ģʽ
		XDCZ_num = 8;        //���ò��Դ���8��
		CIF_enable = 0;      //��ֹCIF
		IIF_enable = 0;      //��ֹIIF
		XDCZ = 1;            //��ֹ��Բ���
		transmit_sig_val = 7;  //����IRFʱ����
		take_action1( transmit_sig_val);     
	}

	else if (sum_count_FSK_cmd1[7]>9)//��Բ���;�������2
	{
		FSK_flag=1;
		transponder_mode = 1;//����Ϊˮ��ģʽ
		main_node = 1;       //��ֲ���ģʽ
		main_station = 3;    //���ڵ�
		XDCZ_num = 16;       //���ò��Դ���16��
		CIF_enable = 0;      //��ֹCIF
		IIF_enable = 0;      //��ֹIIF
		XDCZ = 1;            //��ֹ��Բ���
		transmit_sig_val = 8;//����IRFʱ����
		take_action1( transmit_sig_val);     
	}

	else if (sum_count_FSK_cmd1[8]>9)//��Բ���;�������3
	{
		FSK_flag=1;
		transponder_mode = 1;//����Ϊˮ��ģʽ
		main_node = 1;       //��ֲ���ģʽ
		main_station = 3;    //���ڵ�
		XDCZ_num = 32;       //���ò�Դ��?2��
		CIF_enable = 0;      //��ֹCIF
		IIF_enable = 0;      //��ֹIIF
		XDCZ = 1;            //��ֹ��Բ���
		transmit_sig_val = 9;//����IRFʱ����
		take_action1( transmit_sig_val);     
	}

	else if (sum_count_FSK_cmd1[9]>9)//�ͷ�ָ��
	{

		take_action2(); 
	}

	else if (sum_count_FSK_cmd1[10]>9)//��ȡ����
	{
		//FSK_flag=1;
		take_action3(); 
	}

	else if (sum_count_FSK_cmd1[11]>9)//��ȡѹ��
	{
		//FSK_flag=1;
		take_action4(); 
	}

}



void FSKdetect()
{
	short i,j,sum_count_final_jiance;
	sum_count_final_jiance = 0;  

	//խ���˲�1
	dot1(h_BPF1,Rev_FFT,Rev_xcorr,NFFT);
	cifft_NOSCALE(Rev_xcorr,NFFT);
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);

	j = 0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev_bp1[j] = Rev_bp1[i];
		j++;		
	}//������ص�����
	j=0;
	for(i = Rev_resave; i < NFFT; i++)
	{		 
		Rev_bp1[i] = abs(Rev_xcorr[2*i]);
		//	j++;
	}
	memset(h_LPF2_db,0,sizeof(short)*(N_LPF2 + 2));
	oflag_LPF2 = fir2(Rev_bp1,h_LPF2,Rev_bp1_envelope,h_LPF2_db,NFFT,N_LPF2);
	//���ϳ���ִ��ʱ��2.68ms
	qujunzhi1(Rev_bp1_envelope, NFFT);
	//խ���˲�2
	dot1(h_BPF2,Rev_FFT,Rev_xcorr,NFFT);
	cifft_NOSCALE(Rev_xcorr,NFFT);
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);

	j = 0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev_bp2[j] = Rev_bp2[i];
		j++;		
	}//������ص���
	j=0;
	for(i = Rev_resave; i < NFFT; i++)
	{		 
		Rev_bp2[i] = abs(Rev_xcorr[2*i]);
		//	j++;
	}

	memset(h_LPF2_db,0,sizeof(short)*(N_LPF2 + 2));
	oflag_LPF2 = fir2(Rev_bp2,h_LPF2,Rev_bp2_envelope,h_LPF2_db,NFFT,N_LPF2);
	//���ϳ���ִ��ʱ��2.68ms
	qujunzhi2(Rev_bp2_envelope, NFFT);

	//������
	j = 0;
	for (i = (block2-1)*N_resample; i < block2*N_resample; i ++)
	{
		temp_dainei_temp[i] = (Rev_bp1_envelope[Rev_resave+40*j] + Rev_bp2_envelope[Rev_resave+40*j])/2;///64;
		j++;
	}

	if (block2 == 6) //FFT���Ƶ��
	{
		FFTprepare2(temp_dainei_temp,temp_dainei_temp_fft, NFFT_mingling);
		cfft_NOSCALE(temp_dainei_temp_fft, NFFT_mingling);  //�Ƿ����������
		cbrev(temp_dainei_temp_fft,temp_dainei_temp_fft,NFFT_mingling);  //�ɲ�����ָ��һ������
		qumo2(temp_dainei_temp_fft, temp_dainei_temp_fft_abs, NFFT_mingling);
		testpinlv_max=0;
		testpinlv_ind=0;
		maxvec(temp_dainei_temp_fft_abs, NFFT_mingling/2, &testpinlv_max, &testpinlv_ind);
		//�ڲ庯��
		x2 = testpinlv_ind;
		x1 = x2 - 1;
		x3 = x2 + 1;
		y1 = temp_dainei_temp_fft_abs[x1];
		y2 = temp_dainei_temp_fft_abs[x2];
		y3 = temp_dainei_temp_fft_abs[x3];
		a = (float)(y1 + y3 - 2*y2)/2;
		b = (float)(y3 - y1 - 4*a*x2)/2;
		if (a == 0)
			x_max = (float)x2;
		else
			x_max = (-1)*b/(2*a);

		f_code = x_max/255*1000;
		block2 = 2;
		//����5,6��ᵽ��1,2��
		for (i = (block2-2)*N_resample; i < block2*N_resample; i ++)
		{
			temp_dainei_temp[i] = temp_dainei_temp[i+4*N_resample];
		}
		memset(sum_count_FSK_cmd1,0,12);
		memset(FSK_cmd_flag,0,12);
		//�����Ƶ
		if ((f_code >= 60-2.5)&&(f_code < 60+2.5))
		{
			testpinlv_flag = 1;FSK_cmd_flag[0]=1;
		}
		else if ((f_code >= 65-2.5)&&(f_code < 65+2.5))
		{
			FSK_cmd_flag[1]=1;
		}
		else if ((f_code >= 70-2.5)&&(f_code < 70+2.5))
		{
			FSK_cmd_flag[2]=1;
		}
		else if ((f_code >= 75-2.5)&&(f_code < 75+2.5))
		{
			FSK_cmd_flag[3]=1;
		}
		else if ((f_code >= 80-2.5)&&(f_code < 80+2.5))
		{
			FSK_cmd_flag[4]=1;
		} 
		else if ((f_code >= 85-2.5)&&(f_code < 85+2.5))
		{
			FSK_cmd_flag[5]=1;
		}  
		else if ((f_code >= 90-2.5)&&(f_code < 90+2.5))
		{
			FSK_cmd_flag[6]=1;
		}  
		else if ((f_code >= 95-2.5)&&(f_code < 95+2.5))
		{
			FSK_cmd_flag[7]=1;
		}  
		else if ((f_code >= 100-2.5)&&(f_code < 100+2.5))
		{
			FSK_cmd_flag[8]=1;
		}  
		else if ((f_code >= 105-2.5)&&(f_code < 105+2.5))
		{
			FSK_cmd_flag[9]=1;
		}  
		else if ((f_code >= 110-2.5)&&(f_code < 110+2.5))
		{
			FSK_cmd_flag[10]=1;
		}  
		else if ((f_code >= 115-2.5)&&(f_code < 115+2.5))
		{
			FSK_cmd_flag[11]=1;
		}
		//���ݰ���11��
		j=0;
		for (i = 1; i<12; i ++)
		{
			count_final_jiance[j] = count_final_jiance[i] ;
			count_FSK_cmd0[j]     = count_FSK_cmd0[i];
			count_FSK_cmd1[j]     = count_FSK_cmd1[i];
			count_FSK_cmd2[j]     = count_FSK_cmd2[i];
			count_FSK_cmd3[j]     = count_FSK_cmd3[i];
			count_FSK_cmd4[j]     = count_FSK_cmd4[i];
			count_FSK_cmd5[j]     = count_FSK_cmd5[i];
			count_FSK_cmd6[j]     = count_FSK_cmd6[i];
			count_FSK_cmd7[j]     = count_FSK_cmd7[i];
			count_FSK_cmd8[j]     = count_FSK_cmd8[i];
			count_FSK_cmd9[j]     = count_FSK_cmd9[i];
			count_FSK_cmd10[j]     = count_FSK_cmd10[i];
			count_FSK_cmd11[j]     = count_FSK_cmd11[i];
			j++ ;
		} 
		//����1������
		count_final_jiance[11] = testpinlv_flag;
		count_FSK_cmd0[11]     = FSK_cmd_flag[0];
		count_FSK_cmd1[11]     = FSK_cmd_flag[1];
		count_FSK_cmd2[11]     = FSK_cmd_flag[2];
		count_FSK_cmd3[11]     = FSK_cmd_flag[3];
		count_FSK_cmd4[11]     = FSK_cmd_flag[4];
		count_FSK_cmd5[11]     = FSK_cmd_flag[5];
		count_FSK_cmd6[11]     = FSK_cmd_flag[6];
		count_FSK_cmd7[11]     = FSK_cmd_flag[7];
		count_FSK_cmd8[11]     = FSK_cmd_flag[8];
		count_FSK_cmd9[11]     = FSK_cmd_flag[9];
		count_FSK_cmd10[11]     = FSK_cmd_flag[10];
		count_FSK_cmd11[11]     = FSK_cmd_flag[11];

		//����ָ����
		for (i = 0; i<12 ;i ++)
		{
			sum_count_final_jiance = sum_count_final_jiance + count_final_jiance[i];
			sum_count_FSK_cmd1[0]  = sum_count_FSK_cmd1[0]  + count_FSK_cmd0[i];
			sum_count_FSK_cmd1[1]  = sum_count_FSK_cmd1[1]  + count_FSK_cmd1[i];
			sum_count_FSK_cmd1[2]  = sum_count_FSK_cmd1[2]  + count_FSK_cmd2[i];
			sum_count_FSK_cmd1[3]  = sum_count_FSK_cmd1[3]  + count_FSK_cmd3[i];
			sum_count_FSK_cmd1[4]  = sum_count_FSK_cmd1[4]  + count_FSK_cmd4[i];
			sum_count_FSK_cmd1[5]  = sum_count_FSK_cmd1[5]  + count_FSK_cmd5[i];
			sum_count_FSK_cmd1[6]  = sum_count_FSK_cmd1[6]  + count_FSK_cmd6[i];
			sum_count_FSK_cmd1[7]  = sum_count_FSK_cmd1[7]  + count_FSK_cmd7[i];
			sum_count_FSK_cmd1[8]  = sum_count_FSK_cmd1[8]  + count_FSK_cmd8[i];
			sum_count_FSK_cmd1[9]  = sum_count_FSK_cmd1[9]  + count_FSK_cmd9[i];
			sum_count_FSK_cmd1[10]  = sum_count_FSK_cmd1[10]  + count_FSK_cmd10[i];
			sum_count_FSK_cmd1[11]  = sum_count_FSK_cmd1[11]  + count_FSK_cmd11[i];
		}
	}
	block2++;
}

/*����źų���*/
void detect_signal()
{
	//	IER0 = 0x0000;    //ʹ��DMA4������int1�ж�
	//	IFR0 = 0x0ffff;
	IER1 = 0x0011;    //ʹ�ܴ���int1�ж�
	IFR1 = 0x0ffff;
	SPCR2_0 = 0x00c1; //��McBSP
	SPCR1_0 = 0x0001;

	Flag_timer = 0;
	Flag_CIF = 0;

	while((Flag_CIF == 0) && (FSK_flag == 0) && (FLAG_RS232_REVOK == 0) && (Flag_timer == 0) && (responder_flag == 0))
	{
		if (DMAdone == TRUE)
		{
			asm(" bset XF ");
			DMAdone = FALSE;
			while(DMA_FGETH(myhDma,DMACCR,ENDPROG))
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
				DMA_RSETH(myhDma,DMACSSAU,RevsrcAddrHi);//��������DMA��ַ
				DMA_RSETH(myhDma,DMACSSAL,RevsrcAddrLo);
				DMA_RSETH(myhDma,DMACDSAU,Revdst1AddrHi);
				DMA_RSETH(myhDma,DMACDSAL,Revdst1AddrLo);
			}
			DMA_RSETH(myhDma,DMACEN, BUF_LEN_MAX);
			DMA_RSETH(myhDma,DMACFN, 1);
			DMA_FSETH(myhDma,DMACCR,ENDPROG,1);//�������DMA����ʼ��һ֡����

			detect_prepare();      //1.08ms

			if(CIF_enable == 1)
			{
				asksignal_CIF();   //��ⶨλ�ź�1 //2.8ms~4.68ms
			}
			if(IIF_enable == 1)
			{
				Produceref1();     //�����ο��ź�
				asksignal_IIF();   //��ⶨλ�ź�2 //2.8ms~4.68ms
			}  

			if( XDCZ == 1)
			{
				Produceref1();     //�����ο��ź�
				asksignal_CIF();   //��ⶨλ�ź�1 //2.8ms~4.68ms
				Produceref1();     //�����ο��ź�
				asksignal_IIF();   //��ⶨλ�ź�2 //2.8ms~4.68ms
				Produceref1();     //�����ο��ź�
				asksignal_CIF();   //��ⶨλ�ź�1 //2.8ms~4.68ms
				Produceref1();     //�����ο��ź�
				asksignal_IIF();   //��ⶨλ�ź�2 //2.8ms~4.68ms

			}

			detect_prepare_FSK();  //���FSKָ��/1.08ms
			FSKdetect();           //���FSKָ��//5.7ms~6.4ms
			take_action_FSK();

			if(Flag_CIF == 0)
			{
				//asm(" bset XF ");
				memcpy(Rev_save,Rev,2*NFFT);//������һ�����ݣ���������
				//asm(" bclr XF ");
			}

			if(DMA_count < 2)
			{
				DMA_count = DMA_count + 1;
			}
			asm(" bclr XF ");

		}//end if(DMAdone==TRUE)
	}    //end while((Flag_answer==0)&&(FLAG_RS232_REVOK==0))
	do
	{
	}while(DMAdone == FALSE);

}



/*ϵͳ����*/
void system_config()
{
	//	IODOR=0xff;
	IODATA=0x00;
	IER0=0x0000;
	IFR0=0xFFFF;
	IER1=0x0011;//ʹ��DMAC2
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
	DMA_count = 0; //DMA������
}


/*�紥������*/
void Respondermod()
{	
	while(1)
	{
		if(responder_flag == 1)
		{
			responder_flag = 0;
			timer1_cnt = 0;
			IFR1  = 0x0ffff;
			IER1 |= 0x0040;//����DMA1���ڷ���ѯ���ź�
			TCR1 &= 0x0fbef;

			while(timer1_cnt < (responder_Delay - DMANum))
			{ ;
			}
			TCR1 |= 0x0c34;
			IFR1 = 0x0ffff;
			IER1 &= 0x0ffbf;//����DMA1���ڷ���ѯ���ź�
			Senddata0();
			resend_delay = 500000;
			if(resend_delay > 0)
			{
				timer1_jishi( resend_delay);
			}
		}
		IER0 |= 0x0014;    //ʹ�ܶ�ʱ��0���ⲿ�ж�0
		IFR0 = 0x0ffff;
		DMA_start(myhDma);
		detect_signal();  //ֻ���FSK
		DMA_stop(myhDma);
		DMANum = BUF_LEN_MAX - ((DMANum & 0x0fff) >> 1);
		//DMANum����ȥ����4λ�������õ������ⲿ��������DMAĿ�ĵ�ַ����Ϊ��������Ϊ16λ������2�õ���ǰ����������
		DMANum = DMANum * 25 + 12;

		if(FLAG_RS232_REVOK == 1)//���յ������ж�
			break;
	}
}
