
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


/*产生参考信号*//*
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
			t_lfm = lfm_head/40000.0; //40kHz
			templfm = (short)(-550*(sin(2*PI*RAMSES_freq[RAMSES_code1[q]-1]*t_lfm + fai)));
			Rev[q*50 + lfm_head] = templfm;
		}
		t_lfm = (lfm_head)/40000.0;
		fai = 2*PI*RAMSES_freq[RAMSES_code1[q]-1]*t_lfm + fai;
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
}*/

void Produceref()  
{
	unsigned long lfm_head;
	double t_lfm;
	int i;
	memset(Rev,0,sizeof(short)*NFFT);
    //定位信号10ms
    for(lfm_head = 1; lfm_head < 401; lfm_head++)
    {
       t_lfm = (float)lfm_head/40000.0;//40kHz
       Rev[lfm_head - 1] = (short)(-100*(cos(2*PI*22000*t_lfm - PI*8000*t_lfm*t_lfm/0.01)));//30kHz,10ms
    }

	for (i = 0; i < NFFT; i++)
	{
		X_ref[2*i] = Rev[i];//7bit
		X_ref[2*i+1] = 0;
	}
	  
	cfft_NOSCALE(X_ref,NFFT); 
	cbrev(X_ref,X_ref,NFFT);     

}   


/**/
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
	cfft_NOSCALE(h_BPF1,NFFT);  //是否考虑溢出问题
	cbrev(h_BPF1,h_BPF1,NFFT);
	 
	for (i = 0; i < 2*NFFT; i ++)
	{
		h_BPF2[i] = 0;
	}
	for (i = 0; i < 257; i ++)
	{
		h_BPF2[2*i] = filter2[i]/1200;
 	}
	cfft_NOSCALE(h_BPF2,NFFT);  //是否考虑溢出问题
	cbrev(h_BPF2,h_BPF2,NFFT); 
} 

/* */
void Producelfm()
{
	volatile unsigned long lfm_head;
	volatile double t_lfm;
	volatile double fai;
	volatile short q;
	int i;

    //6.2ms
	i = 0;
	fai = 0;
	for(q = 0; q < 6; q++)
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
    //creat signal pslue 7.2ms
	i=0;
    for(i = 0; i<3600; i++)
	{
	   Send_sig [3600 + i] = Send_sig [i];   
	}
}  

/*/*
void Producelfm()
{
   volatile unsigned long lfm_head;
   volatile double t_lfm;
   for(lfm_head = 0; lfm_head < 10000; lfm_head++)
   {
      t_lfm = (float)lfm_head/1000000.0;//1MHz
     // templfm = (short)(20000*(cos(2*PI*20000*t_lfm + PI*10000*t_lfm*t_lfm/0.01)));//30kHz,10ms
      templfm = (20000*(cos(2*PI*22000*t_lfm + PI*8000*t_lfm*t_lfm/0.01)));//0kHz,10ms
	  if(templfm < 0)
	  {
	      Send_sig[lfm_head]=0x0005;//GPIO2=1;GPIO1=0;GPIO0=1;
      }
      else
      {
          Send_sig[lfm_head]=0x0003;//GPIO2=0;GPIO1=1;GPIO0=1;
	  }
   }
} */


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
	testpinlv_flag = 0;
	count_final_jiance = 0;
	release_flag = 0;//释放成功状态位

	memset(temp_dainei_temp,0,sizeof(short)*(NFFT_mingling));
	memset(Rev,0,sizeof(short)*NFFT);
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	memset(Rev_xcorr,0,sizeof(short)*2*NFFT);
	memset(Rev_xcorrout,0,sizeof(short)*NFFT);
	memset(Rev_save_filter_out,0,sizeof(short)*NFFT);
	memset(Rev_envelope,0,sizeof(short)*4096);

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
	//单位是深度分��，即延迟（long）FLAG_yl_data个100us时间,后续计算中：0.1m代表延迟0.1ms  压力调制时间为1.5m对应0.2ms
	//此处也可以变换为：FLAG_yl_data = (long)temp_yl_data/10;代表延迟的us值。
	//表示压力传感器获取信息成功
    if(temp_yl_data > 0)
	{
		depth = (float)(temp_yl_data-0)/15000;//(首先将得到的压力值变成深度m=yl/10000；再将深度数据转换为时延值=m/1.5，单位：ms。)
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
  函数功能：利用DSP内部AD(10bit),采样率为100kHz,采集64点数据,取均值
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
 //   get_temperature();//读取温度
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

	for (i = BUF_LEN_MAX; i < NFFT; i++)
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

		data_bu0[2*i] = data[i]>>8;//偶地址右移7bit
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

/*FFT数据预处理
  函数功能：将数据存在FFT缓存单元中，虚部为0，实部右移7bit
*/
void FFTprepare_FSK(short *data, short *data_bu0, short data_N)
{
	int i;
	for (i = 0; i < data_N; i++)
	{
		data_bu0[2*i] = data[i]>>12;//嫉址右移7bit
		data_bu0[2*i+1] = 0;
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
	short half23_envelope_max = 0;

	//判断绝对门限 开始搜索检测相关峰起点
	for(i = 800; i < len; i++)
	{
		if(envelopein[i] > NOISE_TH_NEW)
		{
			Flag_sig = 1;
			startpos = i;//检测到了，Flag_sig置1,将位置赋给startpos
			break;
		}
	}

	//找锁定区间的最大值
	if(Flag_sig)  
	{
		Flag_sig = 0;//搜索到大于绝对门限，Flag_sig标志位清零
		for(i = startpos;i < len+50; i++)//从开始到最后80点搜索最大值
		{			
			if(flagg == 0)
			{
				if(envelopein[i] >= NOISE_TH_NEW)
				{
					flagg = 1;		//如果大于峰值门限，flagg置1
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
					if((envelopein[i] < Max_threshold)) //获取左侧高出相对门限的第一点
					{
						flagg = 0;
					}
				}
			}
			else
			{
				//if((envelopein[i] <= Max_threshold) || (Max_threshold < NOISE_TH_NEW))
				if((envelopein[i] <= Max_threshold))      //向前搜索，i获取右侧低于相对门限的第一点。
				{
					flagg = 0;
					for(j = pos_begin;j >= startpos; j--) //pos_begin为右侧低于相对门限的第一点。
					{
						if(envelopein[j] <= Max_threshold)
						{
							pos_begin = j;                //pos_begin为左侧低于相对门限的第一点
							break;
						}
					}

                   //找到最大值位置
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

                    //计算脉冲宽度
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

					//计算峰前信噪比
                    ave[0] = mean_zh(&envelopein[Max_pos - 45 ], 40, 2)*4;
					ave[1] = mean_zh(&envelopein[Max_pos - 85 ], 40, 2)*4;
                    ave[2] = mean_zh(&envelopein[Max_pos - 125], 40, 2)*4;
					ave[3] = mean_zh(&envelopein[Max_pos - 165], 40, 2)*4;
                    ave[4] = mean_zh(&envelopein[Max_pos - 205], 40, 2)*4;
					ave[5] = mean_zh(&envelopein[Max_pos - 245], 40, 2)*4;
                    ave[6] = mean_zh(&envelopein[Max_pos - 285], 40, 2)*4;
					ave[7] = mean_zh(&envelopein[Max_pos - 325], 40, 2)*4;
					max_zh(&max_ave1, &ave_num, ave, 8, 1);

                    //计算峰后信噪比
					sum_feng_hou = 0;
                    sum_mean_feng_hou = 0;
					for(j=5;j<205;j++)
					{
						sum_feng_hou = sum_feng_hou + envelopein[Max_pos + j] ;//最大值前后100点求均值
					} 
					sum_mean_feng_hou = sum_feng_hou*2/200;  //3为待定参数//局部信噪比为大于均值的3


					jishu = jishu + 1;
					sum_mean_save = 0;//SD卡存储该值
					width_save = 0;   //SD卡存储该值
					
                    //加入多峰判决减少虚警
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

					//判断是否为真正的相关峰
					//if((envelopein[Max_pos] > sum_mean) && (width > Maikuan_l) && (width < Maikuan_h) && (point_count_qian < 8) && (point_count_hou < 40)  && (SNR_HD_out > 8) )//相关峰满足局部信噪比和脉宽门  && (SNR_HD_out > 10)
					if((envelopein[Max_pos] > sum_mean_feng_hou) && (envelopein[Max_pos] > max_ave1) && (width > Maikuan_l) && (width < Maikuan_h) && (point_count_qian < 3) && (point_count_hou < 40)   )//相关峰满足局部信噪比和脉宽门  && (SNR_HD_out > 10)
					{
						//asm(" bclr XF ");
						pos = 1;
						Flag_answer = 1;         //检测到信号1
						sum_mean_save = sum_mean;//SD卡存储该值
						width_save = width;      //SD卡存储该值

						for(i = Max_pos - 5;i < Max_pos + 5; i++)
						{
							if(envelopein[Max_pos] < Rev_envelope[i])
							{
								Max_pos = i;
							}
						}
						
						//内插函数
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

//	asm(" bclr XF ");
	dot(X_ref,Rev_FFT,Rev_xcorr,NFFT);//0.348ms//与参考信号1点乘
	cifft_NOSCALE(Rev_xcorr,NFFT);//0.862ms//反傅里叶变换
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);//0.064ms
	memset(Rev_xcorr,0,20);
//	memset(Rev_xcorr+4076,0,20);
	j = 0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev_xcorrout[j] = Rev_xcorrout[i];
		j++;		
	}//将相关结果重叠保留
//	j=0;//0.100ms//0.040ms
	for(i = Rev_resave; i < NFFT; i++)
	{		 
		Rev_xcorrout[i] = abs(Rev_xcorr[2*i]);
//		j++;		
	}//0.036ms//点乘实部的结果存在相关结果后面

	memset(h_LPF1_db,0,sizeof(short)*(N_LPF1 + 2));//0.0005ms//0.0005ms//0.0005ms
	oflag_LPF1 = fir2(Rev_xcorrout,h_LPF1,Rev_save_filter_out,h_LPF1_db,NFFT,N_LPF1);//1.37ms
	//将包络数据进行重叠保留
	j = 0;
    for(i = 1648; i < 4096; i++)
	{
	    Rev_envelope[j] = Rev_envelope[i];  
		j++;
	}
	//完成数据更新
	j =2448;
	for(i = Rev_resave; i < NFFT; i++)  
	{
	    Rev_envelope[j] = Rev_save_filter_out[i];  
		j++;  
	}    
    //完成噪声学习，得到滑动门限值。
    sum = 0;
	for(k = 0;k < 2048; k++)
	{
		sum = sum + Rev_save_filter_out[k];
	}
    sum = sum>>11;
	NOISE_TH_huadong = NOISE_TH_huadong*0.98 + (float)(sum*0.02*2.5);
    
    //获取噪声最大值，得到检测绝对门限。
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
			detect(Rev_envelope,2448,&Pos_flag,&endpos);//检测函数最大搜索100个相关峰，检测时间范围为0.12ms~2ms
		}
		point_count_zong = 0;

    }
}

/*发射信号1
  函数功能：发射信号1，采用DMA0通道发射，定时器1为同步事件
*/
void Senddata0()
{
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
}

/*检测准备函数
  函数功能：检测准备，包括重叠保留，FFT变换
*/
void detect_prepare()
{
	Revprepare();   //0.075ms//重叠保留数据
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	FFTprepare(Rev,Rev_FFT,NFFT);//0.103ms
	cfft_NOSCALE(Rev_FFT,NFFT);  //0.862ms//FFT变换
	cbrev(Rev_FFT,Rev_FFT,NFFT);  //0.038ms 
    memset(Rev_FFT,0,20);
//	memset(Rev_FFT+4076,0,20); 
}

/*FSK检测准备函数
  函数功能：检测准备，包括重叠保留，FFT变换
*/
void detect_prepare_FSK()
{
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	FFTprepare_FSK(Rev,Rev_FFT,NFFT);//0.103ms
	cfft_NOSCALE(Rev_FFT,NFFT);  //0.862ms//FFT变换
	cbrev(Rev_FFT,Rev_FFT,NFFT);  //0.038ms 
}

/*执行动作1
  函数功埽杭觳獾叫藕�，执行动作1
*/
void take_action1()
{
	short i;
	long temp;
	//计算转发延迟
	resend_delay = Time_Delay - 51680 - (Uint32)((2*NFFT - endpos_float)*25);	//单位都是1us,三点内插
//	resend_delay = Time_Delay - 51720 - (Uint32)((NFFT - endpos)*25);       //单位都是1us,无内插

	if(resend_delay > 0)
	{
		// 延迟
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;//开启DMA1射
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

	//打开ㄊ逼鳎扑悴饬课露群偷绯氐缪故奔洌淮哟舜Φ絊D卡读完104ms。
	timer1_cnt = 0;

 	IFR1 = 0x0ffff;
 	IER1 |= 0x0040;//开启DMA1用于发射询问信号
	TIMER_start(mhTimer1);//打开定时器1定时
//	get_temperature();
	IODATA = 0x0000;
	get_voltage();
	temp = 0;
    for(i = 100;i<100+512;i++)
	{
		temp = temp + fabs(Rev_save[i]);
	}
	noise_10ms = (short)(temp>>9);

	uart_config();
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;   //相关峰最大值低8位
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//相关峰最大值高8位
	SD_write[30] = (sum_mean_save) & 0x0ff;   //相关峰均值?位
	SD_write[31] = (sum_mean_save>>8) & 0x0ff;//相关峰均值高8位
	SD_write[32] = (width_save) & 0x0ff;      //相关峰宽度低8位
	SD_write[33] = (width_save>>8) & 0x0ff;   //相关峰宽度高8位
	SD_write[34] = 0;                         //无指
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
		// 延�
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
	resend_delay = 20000000;//驱动释放时间:20s,释放器释放时间需16s。
    //resend_delay = 100000;
	timer1_cnt = 0;
 	IFR1 = 0x0ffff;
 	IER1 |= 0x0040;
	TIMER_start(mhTimer1);//
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
	//打开定时器，计算测量温度和电池电压时间
	timer1_cnt = 0;
 	IFR1 = 0x0ffff;
 	IER1 |= 0x0040;//开启DMA1用于发射询问信号
	TIMER_start(mhTimer1);//打开定时器1定时
//	get_temperature();
	
	IODATA = 0x0000;
	get_voltage();
	temp = 0;
    for(i = 100;i<100+512;i++)
	{
		temp = temp + fabs(Rev_save[i]);
	}
	noise_10ms = (short)(temp>>9);

	uart_config();
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;//相关峰最大值�位
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//相关峰最大值高8位
	SD_write[30] = (sum_mean_save) & 0x0ff;//相关峰均值低8位
	SD_write[31] = (sum_mean_save>>8) & 0x0ff;//相关峰均值高8位
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
				TIMER_stop(mhTimer1);//关斩ㄊ逼�
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
	get_pressure();//压力获取�
	//打开定时器，计算测量温度和电池电压时
	timer1_cnt = 0;
 	IFR1 = 0x0ffff;
 	IER1 |= 0x0040;//开启DMA1用于发射询问信号
	TIMER_start(mhTimer1);//打开定时器1定时
//	get_temperature();
	IODATA = 0x0000;
	get_voltage();
	temp = 0;
    for(i = 100;i<100+512;i++)
	{
		temp = temp + fabs(Rev_save[i]);
	}
	noise_10ms = (short)(temp>>9);
	uart_config();
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;//相关峰最大值低8位
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//相关峰最大值高8位
	SD_write[30] = (sum_mean_save) & 0x0ff;//相关寰档��
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
	resend_delay = 2291000 - timer1_cnt - 50000;//参数需要修改，与压力传感器延迟时间有
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

void FSKdetect()
{
	short i,j;
	//窄带滤波1
	dot1(h_BPF1,Rev_FFT,Rev_xcorr,NFFT);
	cifft_NOSCALE(Rev_xcorr,NFFT);
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);

	j = 0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev_bp1[j] = Rev_bp1[i];
		j++;		
	}//将结果重叠保留
	j=0;
	for(i = Rev_resave; i < NFFT; i++)
	{		 
		Rev_bp1[i] = abs(Rev_xcorr[2*i]);
	//	j++;		
	}
	memset(h_LPF2_db,0,sizeof(short)*(N_LPF2 + 2));
	oflag_LPF2 = fir2(Rev_bp1,h_LPF2,Rev_bp1_envelope,h_LPF2_db,NFFT,N_LPF2);
	//以上程序执行时间2.68ms
	qujunzhi1(Rev_bp1_envelope, NFFT);
    //窄带滤波2
	dot1(h_BPF2,Rev_FFT,Rev_xcorr,NFFT);
	cifft_NOSCALE(Rev_xcorr,NFFT);
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);

	j = 0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev_bp2[j] = Rev_bp2[i];
		j++;		
	}//将结果重叠Ａ
	j=0;
	for(i = Rev_resave; i < NFFT; i++)
	{		 
		Rev_bp2[i] = abs(Rev_xcorr[2*i]);
	//	j++;		
	}

	memset(h_LPF2_db,0,sizeof(short)*(N_LPF2 + 2));
	oflag_LPF2 = fir2(Rev_bp2,h_LPF2,Rev_bp2_envelope,h_LPF2_db,NFFT,N_LPF2);
	//以上程序执行时间2.68ms
    qujunzhi2(Rev_bp2_envelope, NFFT);

	//降采样
	j = 0;
	for (i = (block2-1)*N_resample; i < block2*N_resample; i ++)
	{
		temp_dainei_temp[i] = (Rev_bp1_envelope[Rev_resave+40*j] + Rev_bp2_envelope[Rev_resave+40*j])/2;///64;
		j++;
	}
	
	if (block2 == 6) //FFT检测频率
	{
		FFTprepare2(temp_dainei_temp,temp_dainei_temp_fft, NFFT_mingling);
		cfft_NOSCALE(temp_dainei_temp_fft, NFFT_mingling);  //是否考虑溢出问题
		cbrev(temp_dainei_temp_fft,temp_dainei_temp_fft,NFFT_mingling);  //可不可以指向一个数组
		qumo2(temp_dainei_temp_fft, temp_dainei_temp_fft_abs, NFFT_mingling);
		testpinlv_max=0;
		testpinlv_ind=0;
		maxvec(temp_dainei_temp_fft_abs, NFFT_mingling/2, &testpinlv_max, &testpinlv_ind);
		//内插函数
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
		//将第5,6块搬到第1,2块
		for (i = (block2-2)*N_resample; i < block2*N_resample; i ++)
		{
			temp_dainei_temp[i] = temp_dainei_temp[i+4*N_resample];///64;
		}
		//查表测频
		if ((f_code >= 82.5-1)&&(f_code <= 82.5+1))
		{
			if (testpinlv_flag == 1)
			{
				count_final_jiance++;
				if (count_final_jiance == 10)
				{
					Flag_answer = 2;//释放指令
				}
			}
			else
			{
				count_final_jiance = 0;
				testpinlv_flag = 1;
			}
		}
		else
		{
			testpinlv_flag = 0;
			count_final_jiance = 0;
		}
	}
	block2++;
//	if((code & 0x00ff) == 0x0a9)
//	{
//		Flag_answer = 4;//获取电量指令
//	}
}

/*检测信号	函数功能：检测信号程序
*/
void detect_signal()
{
	while((Flag_answer==0) && (FLAG_RS232_REVOK == 0))//没有检测到信号或者串口命令
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
				DMA_RSETH(myhDma,DMACSSAU,RevsrcAddrHi);//重新配置DMA地址
				DMA_RSETH(myhDma,DMACSSAL,RevsrcAddrLo);
				DMA_RSETH(myhDma,DMACDSAU,Revdst2AddrHi);
				DMA_RSETH(myhDma,DMACDSAL,Revdst2AddrLo);
			}
			if (DMAnum%2 == 0)
			{
				DMA_RSETH(myhDma,DMACSSAU,RevsrcAddrHi);//重新配置DMA地址
				DMA_RSETH(myhDma,DMACSSAL,RevsrcAddrLo);
				DMA_RSETH(myhDma,DMACDSAU,Revdst1AddrHi);
				DMA_RSETH(myhDma,DMACDSAL,Revdst1AddrLo);
			}
		    DMA_RSETH(myhDma,DMACEN, BUF_LEN_MAX);
		    DMA_RSETH(myhDma,DMACFN, 1);
			/* Set programmation bit to 1, ENDPROG = 1) */
		    DMA_FSETH(myhDma,DMACCR,ENDPROG,1);//重新配置DMA，开始下一帧传输
    	   	
			detect_prepare();//检庾急?/1.08ms
			asksignal();//检测定位信号//2.8ms~4.68ms
			//detect_prepare_FSK();//检庾急?/1.08ms
			//FSKdetect();//检测FSK指令//5.7ms~6.4ms
            if(Flag_answer == 0)
			{
				//asm(" bset XF ");
				memcpy(Rev_save,Rev,2*NFFT);//保留上一次数据，计算噪声
				//asm(" bclr XF ");
			}
			if(DMA_count < 2)
			{
				DMA_count = DMA_count +1;
			}
			asm(" bclr XF ");

		}//end if(DMAdone==TRUE)
	}    //end while((Flag_answer==0)&&(FLAG_RS232_REVOK==0))
	do
	{
	;
	}while(DMAdone==FALSE);
}



/*系统配置
  函数δ埽褐匦屡渲孟低巢问�
*/
void system_config()
{
//	IODOR=0xff;	
	IODATA=0x00;
	IER0=0x0000;
	IFR0=0xFFFF;
	IER1=0x0011;//使能DMAC2谥卸�
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

/*同步信标函数 */
void Syntimermod()
{	
    short i;
    long temp;
	while(1)
	{
		get_pressure();
		FLAG_OK = 1;
		if(depth > 0.5)
		{
			FLAG_OK = 1;
		}
		while(FLAG_OK == 1)  //已下水
		{
			if(Flag_timer == 1)
			{
				timer1_cnt = 0;
			 	IFR1 = 0x0ffff;
			 	IER1 |= 0x0040;//开启DMA1用于发射询问信号
				TCR1 &= 0x0fbef;
				TIMER_start(mhTimer1);//打开定时器1定时
				while(1)
				{
					if(timer1_cnt == responder_Delay)
					{
						TCR1 |= 0x0c34;
						TIMER_stop(mhTimer1);//关闭定时器1
						IFR1 = 0x0ffff;
			 			IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
						break;
					}
				}
				Senddata0();//





				
				if(FLAG_yl == 0)
				{
					resend_delay = 200000 - 46796 - (Uint32)(0.134*timer1_cnt);
				}
				else if(FLAG_yl == 1)
				{
					resend_delay = 200000 - 21238 - (Uint32)(0.134*timer1_cnt) + (Uint32)(depth*200);
				}
				if(resend_delay > 0)//延迟
				{
					timer1_cnt = 0;
				 	IFR1 = 0x0ffff;
				 	IER1 |= 0x0040;//开启DMA1用于发射询问信号
					TIMER_start(mhTimer1);
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
				FLAG_OK = 0;
				resend_delay = 500000;
				if(resend_delay > 0) // 延迟
				{
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
				//打开定时器，计算测量温度和电池电压时间
				timer1_cnt = 0;
			 	IFR1 = 0x0ffff;
			 	IER1 |= 0x0040;//开启DMA1用于发射询问信号
				TIMER_start(mhTimer1);//打开定时器1定时
//				get_temperature();
				IODATA = 0x0000;
				get_voltage();
				temp = 0;
			    for(i = 100;i<100+512;i++)
				{
					temp = temp + fabs(Rev_save[i]);
				}
				noise_10ms = (short)(temp>>9);
				uart_config();
				SD_write[28] = 0;//相关峰最大值低8位
				SD_write[29] = 0;//相关畲笾蹈?位
				SD_write[30] = 0;//相关峰均值低8位
				SD_write[31] = 0;//相关峰均值高8位
				SD_write[32] = 0;//相关峰宽度低8位
				SD_write[33] = 0;//相关峰宽度高8位
				SD_write[34] = 0;//无指令
				TIMER_stop(mhTimer1);//关闭定时器1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
				if(SD_status == 1)
				{
					SD_WriteData(SectorCounter,SD_write);//将获取信息写入SD卡扇区内
					SectorCounter = SectorCounter + 1;
				}
			}
		}
		Flag_timer = 0;
		if(FLAG_RS232_REVOK == 1)//接收到串口中断
		{
			TCR0=0x1c34;//关闭定时器0，重新同步
			timer0_cnt=0;
			break;
		}
	}
} 

/*电触发功能*/
void Respondermod()
{	
	while(1)
	{
		if(responder_flag == 1)
		{
			responder_flag = 0;
			timer1_cnt = 0;
		 	IFR1 = 0x0ffff;
		 	IER1 |= 0x0040;//开启DMA1用于发射询问信号
			TCR1 &= 0x0fbef;
			//TIMER_start(mhTimer1);//打开定时器1定时
			while(1)
			{
				if(timer1_cnt == responder_Delay)
				{
					TCR1 |= 0x0c34;
					//TIMER_stop(mhTimer1);//关闭定时器1
					IFR1 = 0x0ffff;
		 			IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
					break;
				}
			}
			Senddata0();//发射信号1
			IER0 &= 0x0fffb;//0x0004;//关闭外部中断0
			IFR0 = 0x0ffff;
			resend_delay = 500000;
			if(resend_delay > 0)
			{
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
			IER0 |= 0x0004;//打开外部中断0
			IFR0 = 0x0ffff;
			responder_flag = 0;
		}
		if(FLAG_RS232_REVOK == 1)//接收到串口中断
		{
			break;
		}
	}
}
 


