/*
 * subfunction.h
 *
 *  Created on: 2016-12-21
 */

#ifndef SUBFUNCTION_H_
#define SUBFUNCTION_H_


//求噪声均值的函数
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


/*产生参考信号*/

void Produceref()
{
	unsigned long lfm_head,t_Ref;
	double t_lfm;
	float t_temp;
	int i;
	t_temp = ((float) Time_Ref)/1000;
	memset(Rev,0,sizeof(short)*NFFT);  //定位信号
	if(f2_Ref >= f1_Ref)
	{
		t_Ref = ((unsigned long)(f2_Ref-f1_Ref))*1000;
		for(lfm_head = 0; lfm_head <Time_Ref*fs; lfm_head++)
		{
			t_lfm = (float) lfm_head/(((float) fs)*1000);//采样频率100kHz
			Rev[lfm_head] = (short)(-100*(cos(2*PI*(f2_Ref*1000)*t_lfm - PI*t_Ref*t_lfm*t_lfm/t_temp)));//起始频率30kHz，截止频率22kHz,20ms
		}
	}
	else
	{
		t_Ref = ((unsigned long)(f1_Ref-f2_Ref))*1000;
		for(lfm_head = 0; lfm_head <Time_Ref*fs; lfm_head++)
		{
			t_lfm = (float)lfm_head/(((float) fs)*1000);//采样频率100kHz
			Rev[lfm_head] = (short)(-100*(cos(2*PI*(f2_Ref*1000)*t_lfm + PI*t_Ref*t_lfm*t_lfm/t_temp)));//起始频率30kHz，截止频率22kHz,20ms
		}
	}

	for (i = 0; i < NFFT; i++)
	{
		X_ref[2*i] = Rev[i]>>4;//偶地址右移4bit
		X_ref[2*i+1] = 0;
	}
	cfft_NOSCALE(X_ref,NFFT);  //是否考虑溢出问题
	cbrev(X_ref,X_ref,NFFT);
}


/*产生发射信号
  函数功能：产生两个信号，询问信号和测阵信号存放在CE2空间
			首地址分别为0x400000和0x500000
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
			t_lfm = (float)lfm_head/1000000.0;//采样频率1MHz
			templfm = (short)(200*(cos(2*PI*(f1_Sig*1000)*t_lfm + PI*t_Sig*t_lfm*t_lfm/t_Sigtemp)));//起始频率f1_Sig，截止频率f2_Sig,Time_Sig
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
			t_lfm = (float)lfm_head/1000000.0;//采样频率1MHz
			templfm = (short)(200*(cos(2*PI*(f1_Sig*1000)*t_lfm - PI*t_Sig*t_lfm*t_lfm/t_Sigtemp)));//起始频率f1_Sig，截止频率f2_Sig,Time_Sig
			if(templfm < 0)
				Send_sig[lfm_head]=0x05;//GPIO2=1;GPIO1=0;GPIO0=1;
			else
				Send_sig[lfm_head]=0x03;//GPIO2=0;GPIO1=1;GPIO0=1;
		}
	}

}

/*
 *变量初始化
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
	release_flag = 0;//释放成功状态位
	Tr_time = 0;//初始化关闭发射部分电源计数值

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

/*获取噪声量级	*/
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


/*获取压力信息函数
  函数功能：利用DSP内部AD(10bit),采样率为100kHz,采集64点数据,取均值
            拟合成深度信息
			将时延值改为原值的一半。
 */
void pressure_init()
{
	volatile short temp_js,buff,f_send_uart,j_uart;
	volatile Uint32 ii;
	FLAG_yl_init = 0;
	j_uart = 0;
	temp_js = 4;
	f_send_uart = 0;
	//发送初始化压力传感器数据
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
	IFR0 = 0x0ffff;//使能int0中断
	//	timer1_cnt = 0;
	// 	IFR1 = 0x0ffff;
	// 	IER1 |= 0x0040;//使能定时器1中断
	//	TIMER_start(mhTimer1);//打开定时器1定时
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
	//表示压力传感器初始化成功
	IER0 &= 0x0fff7;
	IFR0 = 0x0ffff;//关闭int2中断
	//	TIMER_stop(mhTimer1);//关闭定时器1
	//	IFR1 = 0x0ffff;
	//	IER1 &= 0x0ffbf;//关闭定时器1中断
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
	// 	IER1 |= 0x0040;//使能定时器1中断
	//	TIMER_start(mhTimer1);//打开定时器1定时
	//发送初始化压力传感器数据
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
	IFR0 = 0x0ffff;//使能int2中断
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
		//			IFR0 = 0x0ffff;//关闭int2中断
		//			break;
		//		}
	}
	timer1_cnt = ii;
	temp_yl_data = (((Uint32)RS485_DATA[2])<<24) + (((Uint32)RS485_DATA[3])<<16) + (((Uint32)RS485_DATA[4])<<8) + ((Uint32)RS485_DATA[5]);
	if(temp_yl_data > 268435455)
	{
		temp_yl_data = 0;
	}
	//temp_yl_data的值单位为：Pa；且1 Pa=1e-5 bar=（1e-5）*10 m=1e-4 m
	//单位是深度分米，即延迟（long）FLAG_yl_data个100us时间,后续计算中：0.1m代表延迟0.1ms  压力调制时间为1.5m对应0.2ms
	//此处也可变FLAG_yl_data = (long)temp_yl_data/10;代表延迟的us值。
	//表示压力传感器获取信息成功
	if(temp_yl_data > 0)
	{
		depth = (float)(temp_yl_data-0)/15000;  //(首先将得到的压力值变成深度m=yl/10000；再将深度数据转换为时延值=m/1.5，单位：ms。)
	}
	else
	{
		depth = 0.0;
	}

	//	TIMER_stop(mhTimer1);//关闭定时器1
	//	IFR1 = 0x0ffff;
	//	IER1 &= 0x0ffbf;//关闭定时器1中断
}


/*获取电压信息函数
  函数功能：利用DSP内部AD(10bit),采样率为10kHz,采集64点数据,取均值
 */
void get_voltage()
{
	long sum;
	short i = 0;
	for ( i = 0; i < 64; i++)
	{
		ADCCTL = 0x8000;	// 启动AD转换，通道0
		do
		{
			temp_ADC=ADCDATA;
		}while(temp_ADC & 0x8000);//查询ADC忙标志位
		ADC_in[i] = temp_ADC & 0x0fff;
	}
	sum = 0;
	for(i = 0; i < 64; i++)
	{
		sum = sum + ADC_in[i];
	}
	voltage_AD = (short)(sum>>6);//求电压均值
}


void get_parameter()//获取参数信息
{
	NOISE_TH_NEW = PC_DATA[4]+256*PC_DATA[5];//重新计算绝对门限16bit
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
	get_voltage();//获取电压信息
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;//使能定时器1中断
	TIMER_start(mhTimer1);//打开1定时
	get_temperature();//读取温度
	IODATA = 0x0000;
	TIMER_stop(mhTimer1);//关闭定时器1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//关闭定时器1中断
	get_pressure();//获取压力信息
	get_noise();//获取当前噪声
}



/*重叠保留数据
  函数功能：重叠保留接收数据
			将接收数据转化为有符号数
 */
void Revprepare()
{
	int i=0,j=0;

	for (i = BUF_LEN_MAX; i < NFFT; i++)//BUF_LEN_MAX=NFFT-Rev_resave=2096
	{
		Rev[j] = Rev[i];//将处理单元后面的数据搬移到前面
		j++;
	}
	j = 0;
	if (DMAnum%2 == 1)
	{
		for(i = Rev_resave;i < NFFT;i++)
		{
			Rev[i] = Rev1[j];//如果DMAnum%2==1，将Rev1数组中的数搬移到处理单元的后面，并将无符号数转化为有符号数
			j++;
		}
	}
	else
	{
		for(i = Rev_resave;i < NFFT;i++)
		{
			Rev[i] = Rev2[j];//如果DMAnum%2==0，将Rev2数组中的数搬移到处理单元的后面，并将无符号数转化为有符号数
			j++;
		}
	}
}

/*FFT数据预处理
  函数功能：将数据存在FFT缓存单元中，虚部为0，实部右移7bit
 */
void FFTprepare(short *data, short *data_bu0, short data_N)
{
	int i;
	for (i = 0; i < data_N; i++)
	{

		data_bu0[2*i] = data[i]>>7;//偶地址右移8bit,,把数据的类型为short转换为int型，//同时除以256完成FFT计算之前归一化？？？
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


/*点乘函数
  函数功能：复数点乘，输入两个复数数组A、B，输出C，数组长度为len
  WYH             时域反卷积<--->频域共轭相乘。
 */
void dot(short* A,short* B,short* C,short len)
{
	short i;
	long temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
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

		C[2*i] = (short)((temp5 - temp6) >> 18);//(short)((temp5+temp6)>>18)
		C[2*i + 1] = (short)((temp7 + temp8) >> 18);//(short)((temp7-temp8)>>18)
	}
}

/*点乘函数
  函数功能：复数点乘，输入两个复数数组A、B，输出C，数组长度为len
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

	  xn = (double) (xn);    //xn为峰值对应的点数
	  ts = 1.0 / fs;         //fs
	  tao1 = (double)((xn - 1)) / fs;  //峰值前一个点
	  tao2 = xn / fs;        //峰值对应的点
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

/*检测准备函数
  函数功能：检测准备，包括重叠保留，FFT变换
 */
void detect_prepare()
{
	Revprepare();   //0.075ms//重叠保留数据
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	FFTprepare(Rev,Rev_FFT,NFFT);//0.103ms
	cfft_NOSCALE(Rev_FFT,NFFT);  //0.862ms//FFT变换
	cbrev(Rev_FFT,Rev_FFT,NFFT);  //0.038ms //位倒序操作
	memset(Rev_FFT,0,20);
	//	memset(Rev_FFT+4076,0,20);
}



/*检测程序
  函数功能：输入为相关峰包络，返回检测成功标志位和检测位置
			检测成功标志位置1
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

	//判断绝对门限 开始搜索检测相关峰起点
	for(i = 1308; i < len; i++)
	{
		if(envelopein[i] > NOISE_TH_NEW)
		{
			Flag_sig = 1;
			startpos = i;//记录大于绝对门限的位置，并赋给startpos。
			break;
		}
	}

	//找锁定区间的最大值
	if(Flag_sig)
	{
		Flag_sig = 0;//搜索到大于绝对门限，Flag_sig标志位清零
		for(i = startpos;i < len+200; i++)//200点保护区间，防止在len处出现相关峰
		{
			if(flagg == 0)
			{
				if(envelopein[i] >= NOISE_TH_NEW)
				{
					flagg = 1;		//如果大于绝对门限，flagg置1
					pos_begin = i;  //大于绝对门限的点赋值为pos_begin
					Max_val = 0;
					for(j = i;j < i + Maikuan_h; j++)
					{
						if(Max_val < envelopein[j])
						{
							Max_val = envelopein[j];
						}
					}               //找到大于绝对门限的点后面3*band中的最大值，赋值为 Max_val
					Max_threshold = (short)Max_val*0.5;
					if((envelopein[i] < Max_threshold)) //如果当前的最大值的点的值小于峰值的一半，
					{
						flagg = 0;
					}
				}  //获取左侧高出相对门限的第一点、
			}
			else   //找到第一个大于峰值门限、向前搜索，i获取左侧低于相对门限的第一点。
			{
				//if((envelopein[i] <= Max_threshold) || (Max_threshold < NOISE_TH_NEW))
				if((envelopein[i] <= Max_threshold))      //Max_threshold为最大峰值的一半的值
				{
					flagg = 0;
					for(j = pos_begin;j >= startpos; j--) //pos_begin为左侧低于相对门限的第一点。
					{
						if(envelopein[j] <= Max_threshold)
						{
							pos_begin = j;                //把低于最大峰值一半的点找到。并更新到pos_begin
							break;
						}
					}

					//在pos_begin与i之间找最大值的位置；
					//在pos_begin与i之间找第一个拐点的位置；
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

					//计算脉冲宽度
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

					//计算峰前信噪比
					ave[0] = mean_zh(&envelopein[Max_pos - 112], 100, 2)*5;//乘以5
					ave[1] = mean_zh(&envelopein[Max_pos - 212], 100, 2)*5;
					ave[2] = mean_zh(&envelopein[Max_pos - 312], 100, 2)*5;
					ave[3] = mean_zh(&envelopein[Max_pos - 412], 100, 2)*5;
					ave[4] = mean_zh(&envelopein[Max_pos - 512], 100, 2)*5;
					ave[5] = mean_zh(&envelopein[Max_pos - 612], 100, 2)*5;
					ave[6] = mean_zh(&envelopein[Max_pos - 712], 100, 2)*5;
					ave[7] = mean_zh(&envelopein[Max_pos - 812], 100, 2)*5;
					max_zh(&max_ave1, &ave_num, ave, 8, 1);//找到8个峰前信噪比最大的值和点数

					//计算峰后信噪比
					sum_feng_hou = 0;
					sum_mean_feng_hou = 0;
					for(j= (Theory_wide>>1);j<512+(Theory_wide>>1);j++)////向后累加5.12ms的噪声
					{
						sum_feng_hou = sum_feng_hou + envelopein[Max_pos + j] ;//最大值后512点求均值
					}
					sum_mean_feng_hou = (sum_feng_hou>>9)*2; //局部信噪比的2倍后取平均值，向右移动9位，即除以512

					jishu = jishu + 1;
					sum_mean_save = 0;//SD卡存储该值
					width_save = 0;   //SD卡存储该值

					//计算前伪峰数；后伪峰数。
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

					//判断是否为真正的相关峰
					if((envelopein[Max_pos] > sum_mean_feng_hou) && (envelopein[Max_pos] > max_ave1) && (width > Maikuan_l) && (width < Maikuan_h) && (point_count_qian < Weifeng_qian) && (point_count_hou < Weifeng_hou) )
					{
						//asm(" bclr XF ");
						pos = 1;
						Tr_time = 0;
						IODATA &= 0xbf;    //开启发射部分电源，并清零计数值
						Flag_answer = 1;         //检测到信号1
						sum_mean_save = sum_mean;//SD卡存储该值
						width_save = width;      //SD卡存储该值

						for(i = Max_pos - (Theory_wide>>1);i < Max_pos + (Theory_wide>>1); i++)//在理论脉宽内进行确认最大值
						{
							if(envelopein[Max_pos] < Rev_envelope[i])
							{
								Max_pos = i;
							}
						}

						//内插函数78.9us
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
						resend_delay = Time_Delay - 41381 - (float)((float)((5500.0 - endpos_float))*10);	//单位都是1us,三点内插
						//asm(" bset XF ");
						break;

					}//endif 局部信噪比和脉宽条件
				}//endif向前搜索大于门限
			}//endif 大于门限绝对值
		}//endfor搜索相关峰
	}//endif大于绝对门限
	else
	{
		pos=0;
	}//endif大于对门限

	*flag = pos;//return pos
}



/*检测询问信号
  函数功能：检测询问信号，如果检测到，返回Pos_flag1标志位和endpos1检测位置
 */
void asksignal()
{
	int i,j;
	volatile short k;
	volatile long sum;
	short noise_max1,ave_num = 0;
	short point_count_zong,point_count_zong_th= 0;

	//	asm(" bclr XF ");//XF管脚置低
	dot(X_ref,Rev_FFT,Rev_xcorr,NFFT);//6.803ms//与参考信号1点乘
	cifft_NOSCALE(Rev_xcorr,NFFT);//1.731ms//反傅里叶变换
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);//0.075ms
	memset(Rev_xcorr,0,20);//  why?
	//	memset(Rev_xcorr+4076,0,20);
	j = 0;
	for(i = BUF_LEN_MAX; i  < NFFT; i++) //BUF_LEN_MAX-接收缓存区长度,为NFFT-本地信号长度=2096
	{
		Rev_xcorrout[j] = Rev_xcorrout[i];
		j++;
	}//将相关结果重叠保留
	for(i = Rev_resave; i  < NFFT; i++)   //i = Rev_resave=2000
	{
		Rev_xcorrout[i] = abs(Rev_xcorr[2*i]);//Rev_xcorr的i乘以2
	}//0.036ms//点乘实部的结果存在相关结果后面
	memset(h_LPF1_db,0,sizeof(short)*(N_LPF1 + 2));//0.00006ms
	oflag_LPF1 = fir2(Rev_xcorrout,h_LPF1,Rev_save_filter_out,h_LPF1_db,NFFT,N_LPF1);//2.122ms
	//将包络数据进行重叠保留。加上数据更新总用时1.101ms
	j = 0;
	for(i = 2096; i < 5500; i++)//2096个点开始，挪出2096个点的空间
	{
		Rev_envelope[j] = Rev_envelope[i];
		j++;
	}
	//完成数据更新
	j = 3404;//从3404点开始更新，总更新点数为2096
	for(i = Rev_resave; i < NFFT; i++)  //i = Rev_resave，为本地信号长度，2000
	{
		Rev_envelope[j] = Rev_save_filter_out[i];
		j++;
	}
	//完成噪声学习，得到滑动门限值。

	sum = 0;
	for(k = 2048;k < NFFT; k++)
	{
		sum = sum + Rev_save_filter_out[k];
	}
	sum = sum>>11;
	NOISE_TH_huadong = NOISE_TH_huadong*0.99 + (float)(sum*0.01);

	//获取噪声最大值，得到检测绝对门限。
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
	for(j=1308;j<3404;j=j+7)   //j=1308，检测上次的相关结果，共2096个点，步进值为7，理论脉宽的一半，
	{
		if (Rev_envelope[j] > point_count_zong_th)
		{
			point_count_zong = point_count_zong+1;
		}
	}

	noise_maxbuffer[1] = noise_maxbuffer[0];

	//	asm(" bset XF ");//XF管脚置高

	if (DMA_count > 1)
	{
		if(point_count_zong <37) //最大多途数/步长（脉宽理论值一半）
		{
			detect(Rev_envelope,3404,&Pos_flag,&endpos);//检测函数最大搜索100个相关峰，检测时间范围为0.056ms~2.85ms  BUF_LEN_MAX
		}
		point_count_zong = 0;
	}
}



/*检测信号	函数功能：检测信号程序
 */
void detect_signal()
{
	while((Flag_answer==0) && (FLAG_RS232_REVOK == 0) && ((IODATA & 0x0010) != 0x0010))//没有检测到信号或者串口命令或则是外部电源存在
	{
		if (DMAdone == TRUE)
		{
			//			asm(" bclr XF ");//XF管脚置低
			DMAdone = FALSE;
			while(DMA_FGETH(myhDma,DMACCR,ENDPROG))//获取DMA寄存器字段值
			{
			}
			if (DMAnum%2 == 1)
			{
				DMA_RSETH(myhDma,DMACSSAU,RevsrcAddrHi);//重新配置DMA地址
				DMA_RSETH(myhDma,DMACSSAL,RevsrcAddrLo);
				DMA_RSETH(myhDma,DMACDSAU,Revdst2AddrHi);
				DMA_RSETH(myhDma,DMACDSAL,Revdst2AddrLo);
			}
			if (DMAnum%2 == 0)
			{
				DMA_RSETH(myhDma,DMACSSAU,RevsrcAddrHi);//重屡渲肈MA地址
				DMA_RSETH(myhDma,DMACSSAL,RevsrcAddrLo);
				DMA_RSETH(myhDma,DMACDSAU,Revdst1AddrHi);
				DMA_RSETH(myhDma,DMACDSAL,Revdst1AddrLo);
			}
			DMA_RSETH(myhDma,DMACEN, BUF_LEN_MAX);
			DMA_RSETH(myhDma,DMACFN, 1);
			/* Set programmation bit to 1, ENDPROG = 1) */
			DMA_FSETH(myhDma,DMACCR,ENDPROG,1);//重新配置DMA，开始下一帧传输
			detect_prepare();//检测定位信号预处理/3.5328ms-100MHz
			asksignal();//检测定位信号//15.66ms-100MHz
			if(Flag_answer == 0)
			{
				//asm(" bset XF ");
				memcpy(Rev_save,Rev,NFFT);//保留上一次数据，计算噪声
				Tr_time++; //发射电源关闭计数自加
				//asm(" bclr XF ");
			}
			if((Tr_time >= (Delay_Sig*60*100000)/(RevDatReadLen)) && (Delay_Sig != 0))
			{
				IODATA |= 0x0040;   //计数周期满Delay_Sig分钟后没有检测到信号后关闭发射部分电源
				Tr_time = 0;
			}
			if(DMA_count < 2)
			{
				DMA_count = DMA_count +1;
			}
			//          asm(" bset XF ");//XF管脚置高
		}//end if(DMAdone==TRUE)
	}    //end while((Flag_answer==0)&&(FLAG_RS232_REVOK==0))
	do
	{
		;
	}while(DMAdone==FALSE);
} //在主频为100MHz时，从获取到完整的一采集缓冲区后到完成该区的数据的处理，用时7.759ms(优化等级2)，不优化则为18.589ms





/*发射信号1
  函数功能：发射信号1，采用DMA0通道发射，定时器1为同步事件
 */
void Senddata0()
{
	//	IODATA = 0x0000;//开启发射部分的电源
	IFR1 = 0x0ffff;
	IER1 |= 0x0004;
	DMA_FSETH(hDmaSend0,DMACCR,ENDPROG,1);//将ENDPROG置1
	DMA_start(hDmaSend0);//开始发送数据
	TIMER_start(mhTimer1);
	do{
		;
	}while(FLAGSENDOVER == 0);//发送完毕
	FLAGSENDOVER = 0;
	IFR1 = 0x0ffff;
	IER1 &= 0x0fffb;//开启DMA1用于发射询问信号
	// 	IODATA = 0x0040;//关闭发射部分的电源
}

/*执行动作1
  函数功埽杭觳獾叫藕?，执行动作1
 */
void take_action1()
{
	short i;
	long temp;
	//计算转发延迟
	//	resend_delay = Time_Delay - 41760 - (Uint32)((float)((5500 - endpos_float))*10);	//单位都是1us,三点内插
	//	resend_delay = Time_Delay - 41760 - (Uint32)((float)((5500 - endpos))*10);       //ノ欢际?us,无内插

	if(resend_delay > 0)
	{
		// 延迟
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//开启DMA1射
		TIMER_start(mhTimer1);//打开定时器1定时
		while(1)
		{
			if( resend_delay == timer1_cnt )
			{
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
				break;
			}
		}
	}

	Senddata0();//发射信号1
	//	asm(" bset XF ");
	// 压力调制200ms；
	get_pressure();
	if(FLAG_yl == 0)
	{
		resend_delay = 200000 - 46790 - (Uint32)(0.134*timer1_cnt) + (Uint32)(depth*200);//单位1us;准确数据。
	}
	else if(FLAG_yl == 1)
	{
		resend_delay = 200000 - 46800 - (Uint32)(0.134*timer1_cnt) + (Uint32)(depth*200);//单位1us
	}

	if(resend_delay > 0)
	{
		// 延迟
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//开启DMA1用于发射询问信号
		TIMER_start(mhTimer1);//打开定时器1定时
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
				break;
			}
		}
	}

	//Senddata0();//发射信号1

	resend_delay = 50000;
	if(resend_delay > 0)
	{
		// 延迟
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//开启DMA1用于发射询问信号
		TIMER_start(mhTimer1);//打开定时器1定时
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
				break;
			}
		}
	}

	//打开定时器，计算测量温度和电池电压时间；从此处到SD卡读完104ms。
	timer1_cnt = 0;

	IFR1 = 0x0ffff;
	IER1 |= 0x0040;//开启DMA1用于发射询问信号
	TIMER_start(mhTimer1);//打开定时器1定时
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
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;   //相关峰最大值低8位
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//相关峰最大值高8位
	SD_write[30] = (sum_mean_save) & 0x0ff;   //相关峰均值低8位
	SD_write[31] = (sum_mean_save>>8) & 0x0ff;//相关峰均值高8位
	SD_write[32] = (width_save) & 0x0ff;      //相关峰宽度低8位
	SD_write[33] = (width_save>>8) & 0x0ff;   //相关峰宽度高8位
	SD_write[34] = 0;                         //无指?
	SD_write[37] = (NOISE_TH_NEW) & 0x0ff;    //相关峰的滑动绝对门限	低8位
	SD_write[38] = (NOISE_TH_NEW>>8) & 0x0ff; //相关峰的滑动绝对门限	高8位

	TIMER_stop(mhTimer1);//关闭定时器1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号

	if(SD_status == 1)
	{
		SD_WriteData(SectorCounter,SD_write);//将获取信息写入SD卡扇区内
		SectorCounter = SectorCounter + 1;
	}

	resend_delay = 350000 - timer1_cnt - 50000;//参数需要修改，与压力传感器延迟时间有关
	if(resend_delay > 0)
	{
		// 延?
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//开启DMA1用于发射询问信号
		TIMER_start(mhTimer1);//打开定时器1定时
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
				break;
			}
		}
	}
	//	asm(" bclr XF ");
}


/*执行动作2
  函数功能：检测到信号2，执行动作2
 */
void take_action2()
{
	short i;
	long temp;
	short temp_read;
	Senddata0();//发射信号1

	IODOR = 0x0bf;
	IODATA |= 0x0080;       //GPIO7 = 1，驱动释放器释放
	resend_delay = 25000000;//驱动释放时间:25s,释放器释放时间需24s。
	//resend_delay = 100000;
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;
	TIMER_start(mhTimer1);//打ㄊ逼?定时
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
	TIMER_stop(mhTimer1);//关闭ㄊ摈1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;
	IODOR = 0x0bf;
	IODATA = 0x0000;//GPIO7 = 0，停止驱动释放器释放
	Senddata0();    //发射信号

	if(release_flag == 1)
	{
		resend_delay = 190000;	//单位都是1us
	}
	else
	{
		resend_delay = 290000;	//单位都是1us
	}
	if(resend_delay > 0)
	{
		// 延迟
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;       //开启DMA1用于发射询问信号
		TIMER_start(mhTimer1);//打开定时器1定时
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;     //开启DMA1用于发射询问信号
				break;
			}
		}
	}
	Senddata0();//发射信号1

	resend_delay = 50000;
	if(resend_delay > 0)
	{
		// 延迟
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//开启DMA1用于发射询问信号
		TIMER_start(mhTimer1);//打开定时器1定时
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
				break;
			}
		}
	}
	get_pressure();//压力获取函数
	//打开定逼鳎扑悴饬课露群偷绯氐缪故奔?
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;//开启DMA1用于发射询问信号
	TIMER_start(mhTimer1);//打开定时器1定时
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
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;//相关峰最大值?位
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//相关峰最大值高8位
	SD_write[30] = (sum_mean_save) & 0x0ff;//相关峰均值低8位
	SD_write[31] = (sum_mean_save>>8) & 0x0ff;//喙胤妁值高8位
	SD_write[32] = (width_save) & 0x0ff;//相关峰宽度低8位
	SD_write[33] = (width_save>>8) & 0x0ff;//相关峰宽度高8位
	SD_write[34] = 1;//释放指令
	SD_write[35] = 1;//释放指令
	TIMER_stop(mhTimer1);//关闭定时器1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
	if(SD_status == 1)
	{
		SD_WriteData(SectorCounter,SD_write);//将袢⌒息写入SD扇区内
		SectorCounter = SectorCounter + 1;
	}
	resend_delay = 350000 - timer1_cnt - 50000;//参数需要修改，与压力传感器延迟时间有关
	if(resend_delay > 0)
	{
		// 延时
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//开启DMA1用于发射询问信号
		TIMER_start(mhTimer1);//打开定时器1定时
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
				break;
			}
		}
	}
}
/*执行动作3
  函数功能：检测到信号3，执行动作3
 */
void take_action3()
{
	short i;
	long temp;
	//计算转发延迟
	resend_delay = 75000;	//单位都是1us
	if(resend_delay > 0)
	{
		// 延迟
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//开启DMA1用于发射询问信号
		TIMER_start(mhTimer1);//打开定时器1时
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
				break;
			}
		}
	}

	Senddata0();//发射信号1

	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;//使能定1中断
	TIMER_start(mhTimer1);//打开定时器1定时
	get_voltage();

	TIMER_stop(mhTimer1);//关闭定时器1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//关闭定时器1中断

	resend_delay = 175000 + (Uint32)((float)voltage_AD/1024*3.3*8*10000) - timer1_cnt;

	if(resend_delay > 0)
	{
		// 延迟
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//开启DMA1用于发射询问信号
		TIMER_start(mhTimer1);//打开定时器1定时
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
				break;
			}
		}
	}
	Senddata0();//发射信号1

	resend_delay = 50000;
	if(resend_delay > 0)
	{
		// 延迟
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//开启DMA1用于发射询问信号
		TIMER_start(mhTimer1);//打开定时器1定时
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
				break;
			}
		}
	}
	get_pressure();//压力获取
	//打开定时器，计算测量温度和电池电压时
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;//开启DMA1用于发射询问信号
	TIMER_start(mhTimer1);//打开定时器1定时
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
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;//相关峰最大值低8位
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//相关峰最大值高8位
	SD_write[30] = (sum_mean_save) & 0x0ff;//相关寰档??
	SD_write[31] = (sum_mean_save>>8) & 0x0ff;//相关峰均值高8?
	SD_write[32] = (width_save) & 0x0ff;//相关峰宽度低8位
	SD_write[33] = (width_save>>8) & 0x0ff;//相关峰宽度高8位
	SD_write[34] = 2;//电量指令
	TIMER_stop(mhTimer1);//关闭定时器1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
	if(SD_status == 1)
	{
		SD_WriteData(SectorCounter,SD_write);//将获取信息写入SD卡扇区内
		SectorCounter = SectorCounter + 1;
	}
	resend_delay = 2291000 - timer1_cnt - 50000;//参数需要薷模胙沽Υ衅餮映偈奔溆?
	if(resend_delay > 0)
	{
		// 延迟
		timer1_cnt = 0;
		IFR1 = 0x0ffff;
		IER1 |= 0x0040;//开启DMA1用于发射询问信号
		TIMER_start(mhTimer1);//打开定时器1定时
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
				break;
			}
		}
	}
}


/*
系统配置
 */
void system_config()
{
	//	IODOR=0xff;
	IODATA=0x00;
	IER0=0x0000;
	IFR0=0xFFFF;
	IER1=0x0011;//使能DMAC2谥卸?
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
	DMA_FSETH(myhDma,DMACCR,ENDPROG,1);//重新配置DMA
	DMA_count = 0; //DMA计数清零。
	//	DMA_start(myhDma);//DMA开始采集
}

/*电触发功能*/
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
				IRQ_enable(IRQ_EVT_TINT1);//开启TINT1即1us定时
				IRQ_disable(IRQ_EVT_INT0);//关闭外部中断0
				IRQ_clear(IRQ_IEMASK_ALL);//清除所有的标志位
				TIMER_start(mhTimer1);//打开定时器1定时
				while(timer1_cnt < 3);
				TIMER_stop(mhTimer1);//关闭定时器1
				IRQ_disable(IRQ_EVT_TINT1);//关闭TINT1即1us定时
				IRQ_clear(IRQ_IEMASK_ALL);//清除所有的标志位
				count_timer ++;
				IRQ_enable(IRQ_EVT_INT0);//开启外部中断0
				break;
			}
			else
			{
				responder_flag = 0;
				timer1_cnt = 0;
				IRQ_disable(IRQ_EVT_INT0);//关闭外部中断0
				IRQ_enable(IRQ_EVT_TINT1);//开启TINT1即1us定时
				IRQ_clear(IRQ_IEMASK_ALL);//清除所有的标志位
				TIMER_start(mhTimer1);//打开定时器1定时
				while(timer1_cnt < responder_Delay);
				TIMER_stop(mhTimer1);//关闭定时器1
				IRQ_disable(IRQ_EVT_TINT1);//关闭TINT1即1us定时
				IRQ_clear(IRQ_IEMASK_ALL);//清除所有的标志位
				count_timer = 0;
			}
		}

		Senddata0();//发射信号1
		resend_delay = 500000;
		if(resend_delay > 0)
		{
			timer1_cnt = 0;
			TIMER_start(mhTimer1);//打开定时器1定时
			while(timer1_cnt < resend_delay);
			TIMER_stop(mhTimer1);//关闭定时器1
			IRQ_disable(IRQ_EVT_TINT1);//关闭TINT1即1us定时
		}
		IRQ_enable(IRQ_EVT_INT0);//开启外部中断0
		IRQ_clear(IRQ_IEMASK_ALL);//清除所有的标志位
		if(FLAG_RS232_REVOK == 1)//接收到串口中断
		{
			transponder_mode1 = 0;
			break;
		}
		if((transponder_mode1 ==1) && ((IODATA & 0x0010) != 16))//检测到外部电源丢失，跳出循环
			break;
	}
}

#endif /* SUBFUNCTION_H_ */
