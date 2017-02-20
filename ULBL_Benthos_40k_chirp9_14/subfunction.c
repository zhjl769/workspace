
//ÇóÔëÉù¾ùÖµµÄº¯Êı
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
//Çó×î´óÖµº¯Êı
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

/*²úÉú²Î¿¼ĞÅºÅ//chirp16-11k²Î¿¼ĞÅºÅ¡£ÏÈ½«ĞÅºÅ·´×ª£¬ÔÚÆµÓòÄÚÏà³Ë*/
void Produceref()
{
	unsigned long lfm_head;
	double t_lfm;
	int i;
	memset(Rev,0,sizeof(short)*NFFT);
    //¶¨Î»ĞÅºÅ25ms
    for(lfm_head = 1; lfm_head < 1001; lfm_head++)
    {
       t_lfm = (float)lfm_head/40000.0;//²ÉÑùÆµÂÊ40kHz
       Rev[lfm_head - 1] = (short)(-130*(cos(2*PI*11000*t_lfm + PI*5000*t_lfm*t_lfm/0.025)));
    }
	for (i = 0; i < NFFT; i++)
	{
		X_ref[2*i] = Rev[i];  //Å¼µØÖ·ÓÒÒÆ7bit
		X_ref[2*i+1] = 0;
	}
	cfft_NOSCALE(X_ref,NFFT); //ÊÇ·ñ¿¼ÂÇÒç³öÎÊÌâ
	cbrev(X_ref,X_ref,NFFT); 
}    

/*²úÉúÂË²¨Æ÷ÏµÊı*/
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
	cfft_NOSCALE(h_BPF1,NFFT);  //ÊÇ·ñ¿¼ÂÇÒç³öÎÊÌâ
	cbrev(h_BPF1,h_BPF1,NFFT);
	 
	for (i = 0; i < 2*NFFT; i ++)
	{
		h_BPF2[i] = 0;
	}
	for (i = 0; i < 257; i ++)
	{
		h_BPF2[2*i] = filter2[i]/1200;
 	}
	cfft_NOSCALE(h_BPF2,NFFT);  //ÊÇ·ñ¿¼ÂÇÒç³öÎÊÌâ
	cbrev(h_BPF2,h_BPF2,NFFT); 
} 

/*²úÉú·¢ÉäĞÅºÅ  º¯Êı¹¦ÄÜ£º²úÉúÁ½¸öĞÅºÅ£¬Ñ¯ÎÊĞÅºÅºÍ²âÕóĞÅºÅ´æ·ÅÔÚCE2¿Õ¼ä	Ê×µØÖ··Ö±ğÎª0x400000ºÍ0x500000*/
void Producelfm()
{
   volatile unsigned long lfm_head;
   volatile double t_lfm;
   for(lfm_head = 0; lfm_head < 25000; lfm_head++)
   {
      t_lfm = (float)lfm_head/1000000.0;//²ÉÑùÆµÂÊ1MHz
      templfm = (short)(20000*(cos(2*PI*9000*t_lfm + PI*5000*t_lfm*t_lfm/0.025)));//ÆğÊ¼ÆµÂÊ9kHz£¬½ØÖ¹ÆµÂÊ14kHz,25ms
	  if(templfm < 0)
	  {
	      Send_sig[lfm_head]=0x05;//GPIO2=1;GPIO1=0;GPIO0=1;
      }
      else
      {
          Send_sig[lfm_head]=0x03;//GPIO2=0;GPIO1=1;GPIO0=1;
	  }
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
	Flag_answer = 0;
	
	temperature = 0.0;
	block2 = 3;
	memset(count_final_jiance,0,12);
	testpinlv_flag = 0;
	release_flag = 0;//ÊÍ·Å³É¹¦×´Ì¬Î»

	memset(temp_dainei_temp,0,sizeof(short)*(NFFT_mingling));
	memset(Rev,0,sizeof(short)*NFFT);
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	memset(Rev_xcorr,0,sizeof(short)*2*NFFT);
	memset(Rev_xcorrout,0,sizeof(short)*NFFT);
	memset(Rev_save_filter_out,0,sizeof(short)*NFFT);
	memset(Rev_envelope,0,sizeof(short)*4096);
}     

/*»ñÈ¡ÔëÉùÁ¿¼¶	*/
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

/*»ñÈ¡Ñ¹Á¦ĞÅÏ¢º¯Êı  º¯Êı¹¦ÄÜ£ºÀûÓÃDSPÄÚ²¿AD(10bit),²ÉÑùÂÊÎª100kHz,²É¼¯64µãÊı¾İ,È¡¾ùÖµ
            ÄâºÏ³ÉÉî¶ÈĞÅÏ¢	½«Ê±ÑÓÖµ¸ÄÎªÔ­ÖµµÄÒ»°ë¡£*/
void pressure_init()
{
	volatile short temp_js,buff,f_send_uart,j_uart;
	volatile Uint32 ii;
	FLAG_yl_init = 0;
	j_uart = 0;
	temp_js = 4;
	f_send_uart = 0;
	//·¢ËÍ³õÊ¼»¯Ñ¹Á¦´«¸ĞÆ÷Êı¾İ
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
	IFR0 = 0x0ffff;//Ê¹ÄÜint0ÖĞ¶Ï
//	timer1_cnt = 0;
// 	IFR1 = 0x0ffff;
// 	IER1 |= 0x0040;//Ê¹ÄÜ¶¨Ê±Æ÷1ÖĞ¶Ï
//	TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
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
	//±íÊ¾Ñ¹Á¦´«¸ĞÆ÷³õÊ¼»¯³É¹¦
	IER0 &= 0x0fff7;
	IFR0 = 0x0ffff;//¹Ø±Õint2ÖĞ¶Ï 
//	TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
//	IFR1 = 0x0ffff;
//	IER1 &= 0x0ffbf;//¹Ø±Õ¶¨Ê±Æ÷1ÖĞ¶Ï
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
// 	IER1 |= 0x0040;//Ê¹ÄÜ¶¨Ê±Æ÷1ÖĞ¶Ï
//	TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê± 
	//·¢ËÍ³õÊ¼»¯Ñ¹Á¦´«¸ĞÆ÷Êı¾İ
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
	IFR0 = 0x0ffff;//Ê¹ÄÜint2ÖĞ¶Ï
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
//			IFR0 = 0x0ffff;//¹Ø±Õint2ÖĞ¶Ï
//			break;
//		} 
	} 
	timer1_cnt = ii;
	temp_yl_data = (((Uint32)RS485_DATA[2])<<24) + (((Uint32)RS485_DATA[3])<<16) + (((Uint32)RS485_DATA[4])<<8) + ((Uint32)RS485_DATA[5]);
	if(temp_yl_data > 268435455)
	{
		temp_yl_data = 0;
	}
	//temp_yl_dataµÄÖµµ¥Î»Îª£ºPa£»ÇÒ1 Pa=1e-5 bar=£¨1e-5£©*10 m=1e-4 m
	//µ¥Î»ÊÇÉî¶È·ÖÃ×£¬¼´ÑÓ³Ù£¨long£©FLAG_yl_data¸ö100usÊ±¼ä,ºóĞø¼ÆËãÖĞ£º0.1m´ú±íÑÓ³Ù0.1ms  Ñ¹Á¦µ÷ÖÆÊ±¼äÎª1.5m¶ÔÓ¦0.2ms
	//´Ë´¦Ò²¿ÉÒÔ±ä»»Îª£ºFLAG_yl_data = (long)temp_yl_data/10;´ú±íÑÓ³ÙµÄusÖµ¡£
	//±íÊ¾Ñ¹Á¦´«¸ĞÆ÷»ñÈ¡ĞÅÏ¢³É¹¦
    if(temp_yl_data > 0)
	{
		depth = (float)(temp_yl_data-0)/15000;//(Ê×ÏÈ½«µÃµ½µÄÑ¹Á¦Öµ±ä³ÉÉî¶Èm=yl/10000£»ÔÙ½«Éî¶ÈÊı¾İ×ª»»ÎªÊ±ÑÓÖµ=m/1.5£¬µ¥Î»£ºms¡£)
	}
	else
	{
		depth = 0.0;
	}

//	TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
//	IFR1 = 0x0ffff;
//	IER1 &= 0x0ffbf;//¹Ø±Õ¶¨Ê±Æ÷1ÖĞ¶Ï 
}


/*»ñÈ¡µçÑ¹ĞÅÏ¢º¯Êı  º¯Êı¹¦ÄÜ£ºÀûÓÃDSPÄÚ²¿AD(10bit),²ÉÑùÂÊÎª100kHz,²É¼¯64µãÊı¾İ,È¡¾ùÖµ*/
void get_voltage()
{
	long sum;
	short i = 0;
	for ( i = 0; i < 64; i++)
	{
		ADCCTL = 0x8000;	// Æô¶¯AD×ª»»£¬Í¨µÀ0
		do
		{
		  temp_ADC=ADCDATA;
		}while(temp_ADC & 0x8000);//²éÑ¯ADCÃ¦±êÖ¾Î»
		ADC_in[i] = temp_ADC & 0x0fff;
	}
	sum = 0;
	for(i = 0; i < 64; i++)
	{
	  sum = sum + ADC_in[i];
	}
	voltage_AD = (short)(sum>>6);//ÇóµçÑ¹¾ùÖµ
}

//»ñÈ¡²ÎÊıĞÅÏ¢
void get_parameter()
{
	NOISE_TH_NEW = PC_DATA[4]+256*PC_DATA[5];//ÖØĞÂ¼ÆËã¾ø¶ÔÃÅÏŞ16bit
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
	get_voltage();//»ñÈ¡µçÑ¹ĞÅÏ¢
	timer1_cnt = 0;
 	IFR1 = 0x0ffff;
 	IER1 |= 0x0040;//Ê¹ÄÜ¶¨Ê±Æ÷1ÖĞ¶Ï
	TIMER_start(mhTimer1);//´ò¿ª1¶¨Ê±
    get_temperature();//¶ÁÈ¡ÎÂ¶È
	IODATA = 0x0000;
	TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//¹Ø±Õ¶¨Ê±Æ÷1ÖĞ¶Ï
    get_pressure();//»ñÈ¡Ñ¹Á¦ĞÅÏ¢
    get_noise();//»ñÈ¡µ±Ç°ÔëÉù
}

/*ÖØµş±£ÁôÊı¾İ  º¯Êı¹¦ÄÜ£ºÖØµş±£Áô½ÓÊÕÊı¾İ½«½ÓÊÕÊı¾İ×ª»¯ÎªÓĞ·ûºÅÊı*/
void Revprepare()
{
	int i=0,j=0;

	for (i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev[j] = Rev[i];//½«´¦Àíµ¥ÔªºóÃæµÄÊı¾İ°áÒÆµ½Ç°Ãæ
		j++;
	}
	j = 0;
	if (DMAnum%2 == 1)
	{
		for(i = Rev_resave;i < NFFT;i++)
		{
			Rev[i] = Rev1[j];//Èç¹ûDMAnum%2==1£¬½«Rev1Êı×éÖĞµÄÊı°áÒÆµ½´¦Àíµ¥ÔªµÄºóÃæ£¬²¢½«ÎŞ·ûºÅÊı×ª»¯ÎªÓĞ·ûºÅÊı
			j++;
		}
	}
	else
	{
		for(i = Rev_resave;i < NFFT;i++)
		{
			Rev[i] = Rev2[j];//Èç¹ûDMAnum%2==0£¬½«Rev2Êı×éÖĞµÄÊı°áÒÆµ½´¦Àí
			j++;
		}		
	}
}

/*FFTÊı¾İÔ¤´¦Àí  º¯Êı¹¦ÄÜ£º½«Êı¾İ´æÔÚFFT»º´æµ¥ÔªÖĞ£¬Ğé²¿Îª0£¬Êµ²¿ÓÒÒÆ7bit*/
void FFTprepare(short *data, short *data_bu0, short data_N)
{
	int i;
	for (i = 0; i < data_N; i++)
	{

		data_bu0[2*i] = data[i]>>7;//Å¼µØÖ·ÓÒÒÆ7bit
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

/*FFTÊı¾İÔ¤´¦Àí  º¯Êı¹¦ÄÜ£º½«Êı¾İ´æÔÚFFT»º´æµ¥ÔªÖĞ£¬Ğé²¿Îª0£¬Êµ²¿ÓÒÒÆ7bit*/
void FFTprepare_FSK(short *data, short *data_bu0, short data_N)
{
	int i;
	for (i = 0; i < data_N; i++)
	{
		data_bu0[2*i] = data[i]>>12;//¼µÖ·ÓÒÒÆ7bit
		data_bu0[2*i+1] = 0;
	}
}

/*µã³Ëº¯Êı  º¯Êı¹¦ÄÜ£º¸´Êıµã³Ë£¬ÊäÈëÁ½¸ö¸´ÊıÊı×éA¡¢B£¬Êä³öC£¬Êı×é³¤¶ÈÎªlen
  WYH             Ê±Óò·´¾í»ı<--->ÆµÓò¹²éîÏà³Ë¡£*/
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

/*µã³Ëº¯Êı  º¯Êı¹¦ÄÜ£º¸´Êıµã³Ë£¬ÊäÈëÁ½¸ö¸´ÊıÊı×éA¡¢B£¬Êä³öC£¬Êı×é³¤¶ÈÎªlen*/
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

/*¼ì²â³ÌĞò  º¯Êı¹¦ÄÜ£ºÊäÈëÎªÏà¹Ø·å°üÂç£¬·µ»Ø¼ì²â³É¹¦±êÖ¾Î»ºÍ¼ì²âÎ»ÖÃ¼ì²â³É¹¦±êÖ¾Î»ÖÃ1*/
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
	short half23_envelope_max =0;

	//ÅĞ¶Ï¾ø¶ÔÃÅÏŞ ¿ªÊ¼ËÑË÷¼ì²âÏà¹Ø·åÆğµã
	for(i = 2000; i < len; i++)
	{
		if(envelopein[i] > NOISE_TH_NEW)
		{
			Flag_sig = 1;
			startpos = i;//¼ì²âµ½ÁË£¬Flag_sigÖÃ1,½«Î»ÖÃ¸³¸østartpos
			break;
		}
	}

	//ÕÒËø¶¨Çø¼äµÄ×î´óÖµ
	if(Flag_sig)  
	{
		Flag_sig = 0;//ËÑË÷µ½´óÓÚ¾ø¶ÔÃÅÏŞ£¬Flag_sig±êÖ¾Î»ÇåÁã
		for(i = startpos;i < len+50; i++)//´Ó¿ªÊ¼µ½×îºó80µãËÑË÷×î´óÖµ
		{			
			if(flagg == 0)
			{
				if(envelopein[i] >= NOISE_TH_NEW)
				{
					flagg = 1;		//Èç¹û´óÓÚ·åÖµÃÅÏŞ£¬flaggÖÃ1
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
					if((envelopein[i] < Max_threshold)) //»ñÈ¡×ó²à¸ß³öÏà¶ÔÃÅÏŞµÄµÚÒ»µã
					{
						flagg = 0;
					}
				}
			}
			else
			{
				//if((envelopein[i] <= Max_threshold) || (Max_threshold < NOISE_TH_NEW))
				if((envelopein[i] <= Max_threshold))      //ÏòÇ°ËÑË÷£¬i»ñÈ¡ÓÒ²àµÍÓÚÏà¶ÔÃÅÏŞµÄµÚÒ»µã¡£
				{
					flagg = 0;
					for(j = pos_begin;j >= startpos; j--) //pos_beginÎªÓÒ²àµÍÓÚÏà¶ÔÃÅÏŞµÄµÚÒ»µã¡£
					{
						if(envelopein[j] <= Max_threshold)
						{
							pos_begin = j;                //pos_beginÎª×ó²àµÍÓÚÏà¶ÔÃÅÏŞµÄµÚÒ»µã
							break;
						}
					}

                   //ÕÒµ½×î´óÖµÎ»ÖÃ
					Max_pos = pos_begin;
					for(j = pos_begin; j < i; j++)
					{
						if(envelopein[Max_pos] < envelopein[j])
						{
							Max_pos = j;
						}
					}

					half_envelope_max = envelopein[Max_pos]/2;
                    half23_envelope_max = (envelopein[Max_pos]*2)/3;

                    //¼ÆËãÂö³å¿í¶È
					for(j = Max_pos;j > Max_pos-8 ; j--)
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
					for(j = Max_pos;j < Max_pos+8;j++)
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

					//¼ÆËã·åÇ°ĞÅÔë±È
                    ave[0] = mean_zh(&envelopein[Max_pos - 45 ], 40, 2)*5;
					ave[1] = mean_zh(&envelopein[Max_pos - 85 ], 40, 2)*5;
                    ave[2] = mean_zh(&envelopein[Max_pos - 125], 40, 2)*5;
					ave[3] = mean_zh(&envelopein[Max_pos - 165], 40, 2)*5;
                    ave[4] = mean_zh(&envelopein[Max_pos - 205], 40, 2)*5;
					ave[5] = mean_zh(&envelopein[Max_pos - 245], 40, 2)*5;
                    ave[6] = mean_zh(&envelopein[Max_pos - 285], 40, 2)*5;
					ave[7] = mean_zh(&envelopein[Max_pos - 325], 40, 2)*5;
					max_zh(&max_ave1, &ave_num, ave, 8, 1);

                    //¼ÆËã·åºóĞÅÔë±È
					sum_feng_hou = 0;
                    sum_mean_feng_hou = 0;
					for(j=5;j<205;j++)
					{
						sum_feng_hou = sum_feng_hou + envelopein[Max_pos + j] ;//×î´óÖµÇ°ºó100µãÇó¾ùÖµ
					} 
					sum_mean_feng_hou = sum_feng_hou*2/200;  //3Îª´ı¶¨²ÎÊı//¾Ö²¿ĞÅÔë±ÈÎª´óÓÚ¾ùÖµµÄ3


					jishu = jishu + 1;
					sum_mean_save = 0;//SD¿¨´æ´¢¸ÃÖµ
					width_save = 0;   //SD¿¨´æ´¢¸ÃÖµ
					
                    //¼ÓÈë¶à·åÅĞ¾ö¼õÉÙĞé¾¯
					point_count_qian = 0;
					for(j = Max_pos-208; j < Max_pos-8; j++)
					{
					   if (envelopein[j] > half_envelope_max)
					   {
					     point_count_qian = point_count_qian+1;
					   }
					}  

					point_count_hou = 0;
					for(j = Max_pos+8; j < Max_pos+208; j++)
					{
					   if (envelopein[j] > half_envelope_max)
					   {
					     point_count_hou = point_count_hou+1;
					   }
					}

					//ÅĞ¶ÏÊÇ·ñÎªÕæÕıµÄÏà¹Ø·å
					if((envelopein[Max_pos] > sum_mean_feng_hou) && (envelopein[Max_pos] > max_ave1) && (width > Maikuan_l) && (width < Maikuan_h) && (point_count_qian < 3) && (point_count_hou < 60)   )//Ïà¹Ø·åÂú×ã¾Ö²¿ĞÅÔë±ÈºÍÂö¿íÃÅ  && (SNR_HD_out > 10)
					{

						//asm(" bclr XF ");
						pos = 1;
						Flag_answer = 1;         //¼ì²âµ½ĞÅºÅ1
						sum_mean_save = sum_mean;//SD¿¨´æ´¢¸ÃÖµ
						width_save = width;      //SD¿¨´æ´¢¸ÃÖµ

						for(i = Max_pos - 5;i < Max_pos + 5; i++)
						{
							if(envelopein[Max_pos] < Rev_envelope[i])
							{
								Max_pos = i;
							}
						}
						
						//ÄÚ²åº¯Êı
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
						
					}//endif ¾Ö²¿ĞÅÔë±ÈºÍÂö¿íÌõ¼ş
				}//endifÏòÇ°ËÑË÷´óÓÚÃÅÏŞ
			}//endif ´óÓÚÃÅÏŞ¾ø¶ÔÖµ
		}//endforËÑË÷Ïà¹Ø·å
	}//endif´óÓÚ¾ø¶ÔÃÅÏŞ
	else
	{
		pos=0; 	
	}//endif´óÓÚ¶ÔÃÅÏŞ

	*flag = pos;//return pos	 
}

/*¼ì²âÑ¯ÎÊĞÅºÅ  º¯Êı¹¦ÄÜ£º¼ì²âÑ¯ÎÊĞÅºÅ£¬Èç¹û¼ì²âµ½£¬·µ»ØPos_flag1±êÖ¾Î»ºÍendpos1¼ì²âÎ»ÖÃ*/
void asksignal()
{	
	int i,j;
	volatile short k;
    volatile long sum;
    short noise_max1,ave_num = 0;
	short point_count_zong,point_count_zong_th= 0;

//	asm(" bclr XF ");
    //Óë²Î¿¼ĞÅºÅ1µã³Ë
	dot(X_ref,Rev_FFT,Rev_xcorr,NFFT);
    //·´¸µÀïÒ¶±ä»»
	cifft_NOSCALE(Rev_xcorr,NFFT);    
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);
	memset(Rev_xcorr,0,20);
//	memset(Rev_xcorr+4076,0,20);

    //½«Ïà¹Ø½á¹ûÖØµş±£Áô
	j = 0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev_xcorrout[j] = Rev_xcorrout[i];
		j++;		
	}
    //µã³ËÊµ²¿µÄ½á¹û´æÔÚÏà¹Ø½á¹ûºóÃæ
	for(i = Rev_resave; i < NFFT; i++)
	{		 
		Rev_xcorrout[i] = abs(Rev_xcorr[2*i]);
	}
	memset(h_LPF1_db,0,sizeof(short)*(N_LPF1 + 2));
	oflag_LPF1 = fir2(Rev_xcorrout,h_LPF1,Rev_save_filter_out,h_LPF1_db,NFFT,N_LPF1);
	//½«°üÂçÊı¾İ½øĞĞÖØµş±£Áô
	j = 0;
    for(i = 1048; i < 4096; i++)
	{
	    Rev_envelope[j] = Rev_envelope[i];  
		j++;
	}
	//Íê³ÉÊı¾İ¸üĞÂ
	j =3048;
	for(i = Rev_resave; i < NFFT; i++)  
	{
	    Rev_envelope[j] = Rev_save_filter_out[i];  
		j++;  
	}    
    //Íê³ÉÔëÉùÑ§Ï°£¬µÃµ½»¬¶¯ÃÅÏŞÖµ¡£
    sum = 0;
	for(k = 0;k < 2048; k++)
	{
		sum = sum + Rev_save_filter_out[k];
	}
    sum = sum>>11;
	NOISE_TH_huadong = NOISE_TH_huadong*0.97 + (float)(sum*0.03);
    
    //»ñÈ¡ÔëÉù×î´óÖµ£¬µÃµ½¼ì²â¾ø¶ÔÃÅÏŞ¡£    
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
	for(j=1600;j<3448;j=j+4)
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
		if(point_count_zong < 16)
		{
			detect(Rev_envelope,3048,&Pos_flag,&endpos);//¼ì²âº¯Êı×î´óËÑË÷100¸öÏà¹Ø·å£¬¼ì²âÊ±¼ä·¶Î§Îª0.12ms~2ms
		}
		point_count_zong = 0;
    }
}  

/*¼ì²â×¼±¸º¯Êı¡£ º¯Êı¹¦ÄÜ£º¼ì²â×¼±¸£¬°üÀ¨ÖØµş±£Áô£¬FFT±ä»»*/
void detect_prepare()
{
	Revprepare();   //0.075ms//ÖØµş±£ÁôÊı¾İ
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	FFTprepare(Rev,Rev_FFT,NFFT);//0.103ms
	cfft_NOSCALE(Rev_FFT,NFFT);  //0.862ms//FFT±ä»»
	cbrev(Rev_FFT,Rev_FFT,NFFT);  //0.038ms 
    memset(Rev_FFT,0,20);
}

/*FSK¼ì²â×¼±¸º¯Êı¡£ º¯Êı¹¦ÄÜ£º¼ì²â×¼±¸£¬°üÀ¨ÖØµş±£Áô£¬FFT±ä»»*/
void detect_prepare_FSK()
{
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	FFTprepare_FSK(Rev,Rev_FFT,NFFT);//0.103ms
	cfft_NOSCALE(Rev_FFT,NFFT);  //0.862ms//FFT±ä»»
	cbrev(Rev_FFT,Rev_FFT,NFFT);  //0.038ms 
}

void FSKdetect()
{
	short i,j,sum_count_final_jiance;
	sum_count_final_jiance = 0;

	//Õ­´øÂË²¨1
	dot1(h_BPF1,Rev_FFT,Rev_xcorr,NFFT);
	cifft_NOSCALE(Rev_xcorr,NFFT);
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);

	//½«½á¹ûÖØµş±£Áô
	j=0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev_bp1[j] = Rev_bp1[i];
		j++;		
	}
	j=0;
	for(i = Rev_resave; i < NFFT; i++)
	{		 
		Rev_bp1[i] = abs(Rev_xcorr[2*i]);
	}
	memset(h_LPF2_db,0,sizeof(short)*(N_LPF2 + 2));
	oflag_LPF2 = fir2(Rev_bp1,h_LPF2,Rev_bp1_envelope,h_LPF2_db,NFFT,N_LPF2);
	qujunzhi1(Rev_bp1_envelope, NFFT);

    //Õ­´øÂË²¨2
	dot1(h_BPF2,Rev_FFT,Rev_xcorr,NFFT);
	cifft_NOSCALE(Rev_xcorr,NFFT);
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);
	//½«½á¹ûÖØµş±£Áô
	j = 0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)
	{
		Rev_bp2[j] = Rev_bp2[i];
		j++;		
	}
	j=0;
	for(i = Rev_resave; i < NFFT; i++)
	{		 
		Rev_bp2[i] = abs(Rev_xcorr[2*i]);
	}
	memset(h_LPF2_db,0,sizeof(short)*(N_LPF2 + 2));
	oflag_LPF2 = fir2(Rev_bp2,h_LPF2,Rev_bp2_envelope,h_LPF2_db,NFFT,N_LPF2);
    qujunzhi2(Rev_bp2_envelope, NFFT);

	//½µ²ÉÑù ĞèÒªÈ·ÈÏ²ÎÊı£¡
	j = 0;
	for (i = (block2-1)*N_resample; i < block2*N_resample; i++)
	{
		temp_dainei_temp[i] = (Rev_bp1_envelope[Rev_resave+40*j] + Rev_bp2_envelope[Rev_resave+40*j])/2;
		j++;
	}
	
	if (block2 == 9) //FFT¼ì²âÆµÂÊ ĞèÒªÈ·ÈÏ²ÎÊı£¡
	{
		FFTprepare2(temp_dainei_temp,temp_dainei_temp_fft, NFFT_mingling);
		cfft_NOSCALE(temp_dainei_temp_fft, NFFT_mingling);  
		cbrev(temp_dainei_temp_fft,temp_dainei_temp_fft,NFFT_mingling);  
		qumo2(temp_dainei_temp_fft, temp_dainei_temp_fft_abs, NFFT_mingling);
		testpinlv_max=0;
		testpinlv_ind=0;
		maxvec(temp_dainei_temp_fft_abs, NFFT_mingling/2, &testpinlv_max, &testpinlv_ind);
		//ÄÚ²åº¯Êı
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
		block2 = 3;
		//½«µÚ7,8,9¿é°áµ½µÚ1,2,3¿é;ĞèÒªÈ·ÈÏ²ÎÊı£¡
		for (i = (block2-3)*N_resample; i < block2*N_resample; i ++)
		{
			temp_dainei_temp[i] = temp_dainei_temp[i+6*N_resample];
		}
		//²é±í²âÆµ 82.5+-1.5
		if ((f_code >= 81) && (f_code <= 84))
		{
		   testpinlv_flag = 1; 
		}
		else
		{
			testpinlv_flag = 0;
		}

        count_final_jiance[11] = testpinlv_flag;
		j=0;
		for (i = 1; i<12; i ++)
		{
		  count_final_jiance[j] = count_final_jiance[i] ;
		  j++ ;
		}
        
		for (i = 0; i<12 ;i ++)
		{
		   sum_count_final_jiance = sum_count_final_jiance + count_final_jiance[i];
		}  
		//ÅĞ¶ÏÊÍ·ÅĞÅºÅµÄÊıÁ¿£»ÊÇ·ñÖ´ĞĞÊÍ·ÅÖ¸Áî£»
		if(sum_count_final_jiance > 9)
		{
		   Flag_answer = 2;//ÊÍ·ÅÖ¸Áî
		}  
	}
	block2++;

}

/*¼ì²âĞÅºÅ	º¯Êı¹¦ÄÜ£º¼ì²âĞÅºÅ³ÌĞò
*/
void detect_signal()
{
	while((Flag_answer==0) && (FLAG_RS232_REVOK == 0))//Ã»ÓĞ¼ì²âµ½ĞÅºÅ»òÕß´®¿ÚÃüÁî
	{
		if (DMAdone == TRUE)
		{
			DMAdone = FALSE;
			while(DMA_FGETH(myhDma,DMACCR,ENDPROG))
			{
			}
			if (DMAnum%2 == 1)
			{
				DMA_RSETH(myhDma,DMACSSAU,RevsrcAddrHi);//ÖØĞÂÅäÖÃDMAµØÖ·
				DMA_RSETH(myhDma,DMACSSAL,RevsrcAddrLo);
				DMA_RSETH(myhDma,DMACDSAU,Revdst2AddrHi);
				DMA_RSETH(myhDma,DMACDSAL,Revdst2AddrLo);
			}
			if (DMAnum%2 == 0)
			{
				DMA_RSETH(myhDma,DMACSSAU,RevsrcAddrHi);//ÖØĞÂÅäÖÃDMAµØÖ·
				DMA_RSETH(myhDma,DMACSSAL,RevsrcAddrLo);
				DMA_RSETH(myhDma,DMACDSAU,Revdst1AddrHi);
				DMA_RSETH(myhDma,DMACDSAL,Revdst1AddrLo);
			}
		    DMA_RSETH(myhDma,DMACEN, BUF_LEN_MAX);
		    DMA_RSETH(myhDma,DMACFN, 1);
			/* Set programmation bit to 1, ENDPROG = 1) */
		    DMA_FSETH(myhDma,DMACCR,ENDPROG,1);         //ÖØĞÂÅäÖÃDMA£¬¿ªÊ¼ÏÂÒ»Ö¡´«Êä
    	   	asm(" bset XF ");
			detect_prepare();
			asksignal();
			detect_prepare_FSK();
			FSKdetect();
            if(Flag_answer == 0)
			{
//				asm(" bset XF ");
				memcpy(Rev_save,Rev,NFFT);//±£ÁôÉÏÒ»´ÎÊı¾İ£¬¼ÆËãÔëÉù
//				asm(" bclr XF ");
			}
			asm(" bclr XF ");
			if(DMA_count < 2)
			{
				DMA_count = DMA_count +1;
			}  
		}//end if(DMAdone==TRUE)
	}    //end while((Flag_answer==0)&&(FLAG_RS232_REVOK==0))
	do
	{
	;
	}while(DMAdone==FALSE);
}

/*·¢ÉäĞÅºÅ1£¬  º¯Êı¹¦ÄÜ£º·¢ÉäĞÅºÅ1£¬²ÉÓÃDMA0Í¨µÀ·¢Éä£¬¶¨Ê±Æ÷1ÎªÍ¬²½ÊÂ¼ş*/
void Senddata0()
{
 	IFR1 = 0x0ffff;
 	IER1 |= 0x0004;
	DMA_FSETH(hDmaSend0,DMACCR,ENDPROG,1);//½«ENDPROGÖÃ1
	DMA_start(hDmaSend0);//¿ªÊ¼·¢ËÍÊı¾İ
	TIMER_start(mhTimer1);			
	do{
	;
	}while(FLAGSENDOVER == 0);//·¢ËÍÍê±Ï
	FLAGSENDOVER = 0;
	IFR1 = 0x0ffff;
 	IER1 &= 0x0fffb;//¿ªÆôDMA1ÓÃÓÚäÑÎÊĞÅºÅ
}  

/*Ö´ĞĞ¶¯×÷1*/
void take_action1()
{
	short i;
	long temp;
	//¼ÆËã×ª·¢ÑÓ³Ù
	resend_delay = Time_Delay - 51690 - (Uint32)((2*NFFT - endpos_float)*25);	//µ¥Î»¶¼ÊÇ1us,ÈıµãÄÚ²å
//	resend_delay = Time_Delay - 51720 - (Uint32)((NFFT - endpos)*25);       //µ¥Î»¶¼ÊÇ1us,ÎŞÄÚ²å

	if(resend_delay > 0)
	{
		// ÑÓ³Ù	
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;//¿ªÆôDMA1Éä
		TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
				IFR1 = 0x0ffff;
	 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				break;
			}
		}
	}

	Senddata0();//·¢ÉäĞÅºÅ1

//	asm(" bset XF ");
// Ñ¹Á¦µ÷ÖÆ200ms£»
	get_pressure();
	if(FLAG_yl == 0)
	{
		resend_delay = 200000 - 46790 - (Uint32)(0.134*timer1_cnt) + (Uint32)(depth*200);//µ¥Î»1us;×¼È·Êı¾İ¡£
	}
	else if(FLAG_yl == 1)
	{
		resend_delay = 200000 - 46800 - (Uint32)(0.134*timer1_cnt) + (Uint32)(depth*200);//µ¥Î»1us
	}

	if(resend_delay > 0)
	{
		// ÑÓ³Ù	
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
		TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
				IFR1 = 0x0ffff;
	 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				break;
			}
		}
	}

	//Senddata0();//·¢ÉäĞÅºÅ1

	resend_delay = 50000;
	if(resend_delay > 0)
	{
		// ÑÓ³Ù	
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
		TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
				IFR1 = 0x0ffff;
	 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				break;
			}
		}
	}

	//´ò¿ª¶¨Ê±Æ÷£¬¼ÆËã²âÁ¿ÎÂ¶ÈºÍµç³ØµçÑ¹Ê±¼ä£»´Ó´Ë´¦µ½SD¿¨¶ÁÍê104ms¡£
	timer1_cnt = 0;

 	IFR1 = 0x0ffff;
 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
	TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
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
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;   //Ïà¹Ø·å×î´óÖµµÍ8Î»
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//Ïà¹Ø·å×î´óÖµ¸ß8Î»
	SD_write[30] = (sum_mean_save) & 0x0ff;   //Ïà¹Ø·å¾ùÖµ
	SD_write[31] = (sum_mean_save>>8) & 0x0ff;//Ïà¹Ø·å¾ùÖµ¸ß8Î»
	SD_write[32] = (width_save) & 0x0ff;      //Ïà¹Ø·å¿í¶ÈµÍ8Î»
	SD_write[33] = (width_save>>8) & 0x0ff;   //Ïà¹Ø·å¿í¶È¸ß8Î»
	SD_write[34] = 0;                         //ÎŞÖ¸Áî
	SD_write[37] = (NOISE_TH_NEW) & 0x0ff;    //Ïà¹Ø·åµÄ»¬¶¯¾ø¶ÔÃÅÏŞµÍ8Î»
	SD_write[38] = (NOISE_TH_NEW>>8) & 0x0ff; //Ïà¹Ø·åµÄ»¬¶¯¾ø¶ÔÃÅÏŞ¸ß8Î»

	TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;     //¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ

	if(SD_status == 1)
	{
		SD_WriteData(SectorCounter,SD_write); //½«»ñÈ¡ĞÅÏ¢Ğ´ÈëSD¿¨ÉÈÇøÄÚ
		SectorCounter = SectorCounter + 1;
	}

	resend_delay = 350000 - timer1_cnt - 50000;//²ÎÊıĞèÒªĞŞ¸Ä£¬ÓëÑ¹Á¦´«¸ĞÆ÷ÑÓ³ÙÊ±¼äÓĞ¹Ø
	if(resend_delay > 0)
	{
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
		TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
				IFR1 = 0x0ffff;
	 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				break;
			}
		}
	}
//	asm(" bclr XF ");
}


/*Ö´ĞĞ¶¯×÷2¡£ º¯Êı¹¦ÄÜ£º¼ì²âµ½ĞÅºÅ2£¬Ö´ĞĞ¶¯×÷2*/
void take_action2()
{
	short i;
	long temp;
	short temp_read;
	Senddata0();//·¢ÉäĞÅºÅ1

    IODOR = 0x0bf;
	IODATA |= 0x0080;       //GPIO7 = 1£¬Çı¶¯ÊÍ·ÅÆ÷ÊÍ·Å
	resend_delay = 25000000;//Çı¶¯ÊÍ·ÅÊ±¼ä:25s,ÊÍ·ÅÆ÷ÊÍ·ÅÊ±¼äĞè21s¡£
//    resend_delay = 100000;
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
			break;
		}
		if(timer1_cnt == resend_delay)
		{
			release_flag = 0;
			break;

		}
	}
	TIMER_stop(mhTimer1);
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;
	IODOR = 0x0bf;
	IODATA = 0x0000;//GPIO7 = 0£¬Í£Ö¹Çı¶¯ÊÍ·ÅÆ÷ÊÍ·Å
	Senddata0();    //·¢ÉäĞÅºÅ

	if(release_flag == 1)
	{
		resend_delay = 190000;	//µ¥Î»¶¼ÊÇ1us
	}
	else
	{
		resend_delay = 290000;	//µ¥Î»¶¼ÊÇ1us
	}
	if(resend_delay > 0)
	{
		// ÑÓ³Ù	
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;       //¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
		TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
				IFR1 = 0x0ffff;
	 			IER1 &= 0x0ffbf;     //¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				break;
			}
		}
	}
	Senddata0();//·¢ÉäĞÅºÅ1

	resend_delay = 50000;
	if(resend_delay > 0)
	{
		// ÑÓ³Ù	
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
		TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
				IFR1 = 0x0ffff;
	 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				break;
			}
		}
	}
	get_pressure();//Ñ¹Á¦»ñÈ¡º¯Êı
	//´ò¿ª¶¨Ê±Æ÷£¬¼ÆËã²âÁ¿ÎÂ¶ÈºÍµç³ØµçÑ¹Ê±¼ä
	timer1_cnt = 0;
 	IFR1 = 0x0ffff;
 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
	TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
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
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;//Ïà¹Ø·å×î´óÖµÍ8Î»
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//Ïà¹Ø·å×î´óÖµ¸ß8Î»
	SD_write[30] = (sum_mean_save) & 0x0ff;//Ïà¹Ø·å¾ùÖµµÍ8Î»
	SD_write[31] = (sum_mean_save>>8) & 0x0ff;//Ïà¹Ø·å¾ùÖµ¸ß8Î»
	SD_write[32] = (width_save) & 0x0ff;//Ïà¹Ø·å¿í¶ÈµÍ8Î»
	SD_write[33] = (width_save>>8) & 0x0ff;//Ïà¹Ø·å¿í¶È¸ß8Î»
	SD_write[34] = 1;//ÊÍ·ÅÖ¸Áî
	SD_write[35] = 1;//ÊÍ·ÅÖ¸Áî
	TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
	if(SD_status == 1)
	{
		SD_WriteData(SectorCounter,SD_write);//½«ñÈ¡ĞÏ¢Ğ´ÈëSDÉÈÇøÄÚ
		SectorCounter = SectorCounter + 1;
	}
	resend_delay = 350000 - timer1_cnt - 50000;//²ÎÊıĞèÒªĞŞ¸Ä£¬ÓëÑ¹Á¦´«¸ĞÆ÷ÑÓ³ÙÊ±¼äÓĞ¹Ø
	if(resend_delay > 0)
	{
		// ÑÓÊ±	
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
		TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
				IFR1 = 0x0ffff;
	 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				break;
			}
		}
	}
} 
/*Ö´ĞĞ¶¯×÷3  º¯Êı¹¦ÄÜ£º¼ì²âµ½ĞÅºÅ3£¬Ö´ĞĞ¶¯×÷3*/
void take_action3()
{
	short i;
	long temp;
	resend_delay = 75000;
	if(resend_delay > 0)
	{
		// ÑÓ³Ù	
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;        //¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
		TIMER_start(mhTimer1);
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);
				IFR1 = 0x0ffff;
	 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				break;
			}
		}
	}

	Senddata0();//·¢ÉäĞÅºÅ1
	timer1_cnt = 0;
 	IFR1 = 0x0ffff;
 	IER1 |= 0x0040;       //Ê¹ÄÜ¶¨1ÖĞ¶Ï
	TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê± 
    get_voltage();

	TIMER_stop(mhTimer1); //¹Ø±Õ¶¨Ê±Æ÷1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//¹Ø±Õ¶¨Ê±Æ÷1ÖĞ¶Ï

	resend_delay = 175000 + (Uint32)((float)voltage_AD/1024*3.3*8*10000) - timer1_cnt;

	if(resend_delay > 0)
	{
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
		TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
				IFR1 = 0x0ffff;
	 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				break;
			}
		}
	}
	Senddata0();//·¢ÉäĞÅºÅ1

	resend_delay = 50000;
	if(resend_delay > 0)
	{
		// ÑÓ³Ù	
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
		TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
				IFR1 = 0x0ffff;
	 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅÅ
				break;
			}
		}
	}
	get_pressure();
	//Ê±÷£¬¼ÆËã²âÁ¿ÎÂ¶ÈºÍµç³ØµçÑ¹Ê±
	timer1_cnt = 0;
 	IFR1 = 0x0ffff;
 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
	TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
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
	SD_write[28] = (Rev_envelope[endpos]) & 0x0ff;//Ïà¹Ø·å×î´óÖµµÍ8Î»
	SD_write[29] = (Rev_envelope[endpos]>>8) & 0x0ff;//Ïà¹Ø·å×î´óÖµ¸ß8Î»
	SD_write[30] = (sum_mean_save) & 0x0ff;   //Ïà¹Øå¾ùÖµµÍ8»
	SD_write[31] = (sum_mean_save>>8) & 0x0ff;//Ïà¹Ø·å¾ùÖµ¸ß8
	SD_write[32] = (width_save) & 0x0ff;   //Ïà¹Ø·å¿í¶ÈµÍ8Î»
	SD_write[33] = (width_save>>8) & 0x0ff;//Ïà¹Ø·å¿í¶È¸ß8Î»
	SD_write[34] = 2;//µçÁ¿Ö¸Áî
	TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
	if(SD_status == 1)
	{
		SD_WriteData(SectorCounter,SD_write);//½«»ñÈ¡ĞÅÏ¢Ğ´ÈëSD¿¨ÉÈÇøÄÚ
		SectorCounter = SectorCounter + 1;
	}
	resend_delay = 2291000 - timer1_cnt - 50000;//²ÎÊıĞèÒªĞŞ¸Ä£¬ÓëÑ¹Á¦´«¸ĞÆ÷ÑÓ³ÙÊ±¼äÓĞ
	if(resend_delay > 0)
	{
		// ÑÓ³Ù	
		timer1_cnt = 0;
	 	IFR1 = 0x0ffff;
	 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
		TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
		while(1)
		{
			if(timer1_cnt == resend_delay)
			{
				TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
				IFR1 = 0x0ffff;
	 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				break;
			}
		}
	} 
}  


/*ÏµÍ³ÅäÖÃ  */
void system_config()
{
//	IODOR=0xff;	
	IODATA=0x00;
	IER0=0x0000;
	IFR0=0xFFFF;
	IER1=0x0011;//Ê¹ÄÜDMAC2
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
	DMA_FSETH(myhDma,DMACCR,ENDPROG,1);//ÖØĞÂÅäÖÃDMA
	DMA_count = 0; //DMA¼ÆÊıÇåÁã¡£
//	DMA_start(myhDma);//DMA¿ªÊ¼²É¼¯
}

/*Í¬²½ĞÅ±êº¯Êı */
void Syntimermod()
{	
    short i;
    long temp;
	while(1)
	{
		get_pressure();
		FLAG_OK = 0;
		if(depth > 0.5)
		{
			FLAG_OK = 1;
		}
		while(FLAG_OK == 1)  //ÒÑÏÂË®
		{
			if(Flag_timer == 1)
			{
				timer1_cnt = 0;
			 	IFR1 = 0x0ffff;
			 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				TCR1 &= 0x0fbef;
				TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
				while(1)
				{
					if(timer1_cnt == responder_Delay)
					{
						TCR1 |= 0x0c34;
						TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
						IFR1 = 0x0ffff;
			 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
						break;
					}
				}
				Senddata0();//·¢ÉäĞÅºÅ1
				get_pressure();
				
				if(FLAG_yl == 0)
				{
					resend_delay = 200000 - 46796 - (Uint32)(0.134*timer1_cnt);
				}
				else if(FLAG_yl == 1)
				{
					resend_delay = 200000 - 21238 - (Uint32)(0.134*timer1_cnt) + (Uint32)(depth*200);
				}
				if(resend_delay > 0)//ÑÓ³Ù	
				{
					timer1_cnt = 0;
				 	IFR1 = 0x0ffff;
				 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
					TIMER_start(mhTimer1);
					while(1)
					{
						if(timer1_cnt == resend_delay)
						{
							TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
							IFR1 = 0x0ffff;
				 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
							break;
						}
					}
				}
				//Senddata0();//·¢ÉäĞÅºÅ1
				FLAG_OK = 0;
				resend_delay = 500000;
				if(resend_delay > 0) // ÑÓ³Ù
				{
					timer1_cnt = 0;
				 	IFR1 = 0x0ffff;
				 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
					TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
					while(1)
					{
						if(timer1_cnt == resend_delay)
						{
							TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
							IFR1 = 0x0ffff;
				 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
							break;
						}
					}
				}
				//´ò¿ª¶¨Ê±Æ÷£¬¼ÆËã²âÁ¿ÎÂ¶ÈºÍµç³ØµçÑ¹Ê±¼ä
				timer1_cnt = 0;
			 	IFR1 = 0x0ffff;
			 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
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
				SD_write[28] = 0;//Ïà¹Ø·å×î´óÖµµÍ8Î»
				SD_write[29] = 0;//Ïà¹Øî´óÖµ¸?Î»
				SD_write[30] = 0;//Ïà¹Ø·å¾ùÖµµÍ8Î»
				SD_write[31] = 0;//Ïà¹Ø·å¾ùÖµ¸ß8Î»
				SD_write[32] = 0;//Ïà¹Ø·å¿í¶ÈµÍ8Î»
				SD_write[33] = 0;//Ïà¹Ø·å¿í¶È¸ß8Î»
				SD_write[34] = 0;//ÎŞÖ¸Áî
				TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
				IFR1 = 0x0ffff;
				IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				if(SD_status == 1)
				{
					SD_WriteData(SectorCounter,SD_write);//½«»ñÈ¡ĞÅÏ¢Ğ´ÈëSD¿¨ÉÈÇøÄÚ
					SectorCounter = SectorCounter + 1;
				}
			}
		}
		Flag_timer = 0;
		if(FLAG_RS232_REVOK == 1)//½ÓÊÕµ½´®¿ÚÖĞ¶Ï
		{
			TCR0=0x1c34;//¹Ø±Õ¶¨Ê±Æ÷0£¬ÖØĞÂÍ¬²½
			timer0_cnt=0;
			break;
		}
	}
} 

/*µç´¥·¢¹¦ÄÜ*/
void Respondermod()
{	
	while(1)
	{
		if(responder_flag == 1)
		{
			responder_flag = 0;
			timer1_cnt = 0;
		 	IFR1 = 0x0ffff;
		 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
			TCR1 &= 0x0fbef;
			//TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
			while(1)
			{
				if(timer1_cnt == responder_Delay)
				{
					TCR1 |= 0x0c34;
					//TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
					IFR1 = 0x0ffff;
		 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
					break;
				}
			}
			Senddata0();//·¢ÉäĞÅºÅ1
			IER0 &= 0x0fffb;//0x0004;//¹Ø±ÕÍâ²¿ÖĞ¶Ï0
			IFR0 = 0x0ffff;
			resend_delay = 500000;
			if(resend_delay > 0)
			{
				timer1_cnt = 0;
			 	IFR1 = 0x0ffff;
			 	IER1 |= 0x0040;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
				TIMER_start(mhTimer1);//´ò¿ª¶¨Ê±Æ÷1¶¨Ê±
				while(1)
				{
					if(timer1_cnt == resend_delay)
					{
						TIMER_stop(mhTimer1);//¹Ø±Õ¶¨Ê±Æ÷1
						IFR1 = 0x0ffff;
			 			IER1 &= 0x0ffbf;//¿ªÆôDMA1ÓÃÓÚ·¢ÉäÑ¯ÎÊĞÅºÅ
						break;
					}
				}
			}
			IER0 |= 0x0004;//´ò¿ªÍâ²¿ÖĞ¶Ï0
			IFR0 = 0x0ffff;
			responder_flag = 0;
		}
		if(FLAG_RS232_REVOK == 1)//½ÓÊÕµ½´®¿ÚÖĞ¶Ï
		{
			break;
		}
	}
}
 


