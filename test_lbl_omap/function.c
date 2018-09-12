#include "function.h"

void gen_sig(short *ref_fft, int sig_flag, short *chip_l, int refsig_len, double *f, double (*code)[8], int *w, int *temp_buffer1, int *temp_buffer2)
{
	int j = 0, k = 0, m = 0, n = 0, c = 0;
	double f0 = 0, fs = 40000.0, pha = 0;
	
	memset(temp_buffer1, 0, NFFT*8);
	if(sig_flag >= 0 && sig_flag <= 23)
	{
		for(j=0;j<8;j++)
		{
			c = *(*(code+sig_flag)+j);
			f0 = *(f+c);
			m = n;
			n += *chip_l;

			for(k=m;k<n;k++)
			{
				*(temp_buffer1+2*(refsig_len-k-1)) = (int)(32767*sin(2*PI*f0*(k-m)/fs+pha));
			}
			pha += f0*(*chip_l)/fs*2*PI;
		}
	}

	DSP_fft32x32(w, NFFT, temp_buffer1, temp_buffer2);
	for(j=0;j<2*NFFT;j++)
		*(ref_fft+j) = (short)(*(temp_buffer2+j)>>6);
}

void gen_transmit_sig(unsigned short *trans_signal, int *trans_signal_len, int trans_signal_flag, double *f, double (*code)[8])
{
	int j = 0, k = 0, m = 0, n = 0, c = 0, chip_l = 1250, p = 0, q = 0;
	double f0 = 0, fs = 1000000.0, pha = 0,sig;
	
	memset(trans_signal, 0, 40000);
	if(trans_signal_flag >= 0 && trans_signal_flag <= 23)
	{
		for(j=0;j<8;j++)
		{
			c = *(*(code+trans_signal_flag)+j);
			f0 = *(f+c);
			m = n;
			n += chip_l;

			for(k=m;k<n;k++)
			{
				sig = sin(2*PI*f0*(k-m)/fs+pha);
				if(sig >= 0)
					*(trans_signal+p) = (*(trans_signal+p)>>1) | 0x8000;
				else
					*(trans_signal+p) = (*(trans_signal+p)>>1);
				q++;
				if(q >= 16)
				{
					q = 0;
					p++;
				}
			}
			pha += f0*chip_l/fs*2*PI;
		}
	}
	*trans_signal_len = p+1;
}
//............¸ü¸ÄÍ¬²½ÅäÖÃº¯Êı.............//
void syn_config(char type, char cyc)
{
	if(type == 1)
	{	//ÄÚÍ¬²½
	    
		GPIO_BANK45->OUT_DATA &= 0xFB7FFFFF;//GPIO5[7][10]
		GPIO_BANK23->OUT_DATA |= 0x10000000;//GPIO3[12]=1
		GPIO_BANK23->OUT_DATA &= 0xDFFFFFFF;//gpio3[13]=0 
		evm_init_timer(TMR1, 10);
		while(1)
		{
			if(Time_over == 1)
			{
				Time_over = 0;
				break;
			}
		}
       	GPIO_BANK23->OUT_DATA |= 0x20000000;//GPIO3[13]=1
		GPIO_BANK23->OUT_DATA &= 0xEFFFFFFF;//gpio3[12]=0

		if(cyc==1 || cyc==9 || cyc==17 || cyc==25)
			GPIO_BANK45->OUT_DATA = 0x00000000;//GPIO5[7][10][11] µÍÈıÎ» 
        else if(cyc==2 || cyc==10  || cyc==18 || cyc==26)
			GPIO_BANK45->OUT_DATA = 0x00800000;
	    else if(cyc==3 || cyc==11  || cyc==19 || cyc==27)
			GPIO_BANK45->OUT_DATA = 0x04000000;
 		else if(cyc==4 || cyc==12  || cyc==20 || cyc==28)
			GPIO_BANK45->OUT_DATA = 0x04800000;
        else if(cyc==5 || cyc==13  || cyc==21 || cyc==29)
			GPIO_BANK45->OUT_DATA = 0x08000000;
		else if(cyc==6 || cyc==14  || cyc==22 || cyc==30)
			GPIO_BANK45->OUT_DATA = 0x08800000;
		else if(cyc==7 || cyc==15  || cyc==23 || cyc==31)
			GPIO_BANK45->OUT_DATA = 0x0C000000;
 		else if(cyc==8 || cyc==16  || cyc==24 || cyc==32)
			GPIO_BANK45->OUT_DATA = 0x0C800000;

		evm_init_timer(TMR1, 10);
		while(1)
		{
			if(Time_over == 1)
			{
				Time_over = 0;
				break;
			}
		}

        GPIO_BANK23->OUT_DATA |= 0x30000000;//GPIO3[12]=1 3[13]=1

		if(cyc>=1 && cyc <=8)
			GPIO_BANK45->OUT_DATA = 0x00000000;//GPIO5[7][10]
        else if(cyc>=9 && cyc <=16)
			GPIO_BANK45->OUT_DATA = 0x00800000;
	    else if(cyc>=17 && cyc <=24)
			GPIO_BANK45->OUT_DATA = 0x04000000;
 		else if(cyc>=25 && cyc <=32)
			GPIO_BANK45->OUT_DATA = 0x04800000;


		evm_init_timer(TMR1, 10);
		while(1)
		{
			if(Time_over == 1)
			{
				Time_over = 0;
				break;
			}
		}

        GPIO_BANK23->OUT_DATA &= 0xCFFFFFFF;//GPIO3[12]=0 3[13]=0 ÍË³öÅäÖÃ ÔËĞĞÄ£Ê½
		Max_Buffer = (int)(cyc*1000/12.8) - 5; //Áô³ö5msµÄÓàÁ¿½øĞĞ³ÌĞòµÄÔÙÅäÖÃ
	}

	else
	if(type == 2)//ÍâÍ¬²½
	{ 
	    GPIO_BANK45->OUT_DATA |= 0x00800000;//GPIO5[7]=1 
		GPIO_BANK45->OUT_DATA &= 0xFBFFFFFF;//GPIO5[10]=0

		GPIO_BANK23->OUT_DATA |= 0x10000000;//GPIO3[12]=1
		GPIO_BANK23->OUT_DATA &= 0xDFFFFFFF;//gpio3[13]=0 
		evm_init_timer(TMR1, 10);
		while(1)
		{
			if(Time_over == 1)
			{
				Time_over = 0;
				break;
			}
		}


        GPIO_BANK23->OUT_DATA &= 0xCFFFFFFF;//GPIO3[12]=0 3[13]=0 ÍË³öÅäÖÃ ÔËĞĞÄ£Ê½
		Max_Buffer = (int)(cyc*1000/12.8) - 5; //Áô³ö5msµÄÓàÁ¿½øĞĞ³ÌĞòµÄÔÙÅäÖÃ
		
	}	

	else if(type == 3)
	{	//½øĞĞÍ¬²½
 	    
		GPIO_BANK45->OUT_DATA &= 0xFF7FFFFF;//GPIO5[7]=0 GPIO5[10]=1
	    GPIO_BANK45->OUT_DATA |= 0x04000000;//GPIO5[7]=0 GPIO5[10]=1
		GPIO_BANK23->OUT_DATA |= 0x10000000;//GPIO3[12]=1
		GPIO_BANK23->OUT_DATA &= 0xDFFFFFFF;//gpio3[13]=0 
		evm_init_timer(TMR1, 10);
		while(1)
		{
			if(Time_over == 1)
			{
				Time_over = 0;
				break;
			}
		}
       	GPIO_BANK23->OUT_DATA |= 0x20000000;//GPIO3[13]=1
		GPIO_BANK23->OUT_DATA &= 0xEFFFFFFF;//gpio3[12]=0

		if(cyc==1 || cyc==9 || cyc==17 || cyc==25)
			GPIO_BANK45->OUT_DATA = 0x00000000;//GPIO5[7][10][11] µÍÈıÎ» 
        else if(cyc==2 || cyc==10  || cyc==18 || cyc==26)
			GPIO_BANK45->OUT_DATA = 0x00800000;
	    else if(cyc==3 || cyc==11  || cyc==19 || cyc==27)
			GPIO_BANK45->OUT_DATA = 0x04000000;
 		else if(cyc==4 || cyc==12  || cyc==20 || cyc==28)
			GPIO_BANK45->OUT_DATA = 0x04800000;
        else if(cyc==5 || cyc==13  || cyc==21 || cyc==29)
			GPIO_BANK45->OUT_DATA = 0x08000000;
		else if(cyc==6 || cyc==14  || cyc==22 || cyc==30)
			GPIO_BANK45->OUT_DATA = 0x08800000;
		else if(cyc==7 || cyc==15  || cyc==23 || cyc==31)
			GPIO_BANK45->OUT_DATA = 0x0C000000;
 		else if(cyc==8 || cyc==16  || cyc==24 || cyc==32)
			GPIO_BANK45->OUT_DATA = 0x0C800000;

		evm_init_timer(TMR1, 10);
		while(1)
		{
			if(Time_over == 1)
			{
				Time_over = 0;
				break;
			}
		}

        GPIO_BANK23->OUT_DATA |= 0x30000000;//GPIO3[12]=1 3[13]=1

		if(cyc>=1 && cyc <=8)
			GPIO_BANK45->OUT_DATA = 0x00000000;//GPIO5[7][10]
        else if(cyc>=9 && cyc <=16)
			GPIO_BANK45->OUT_DATA = 0x00800000;
	    else if(cyc>=17 && cyc <=24)
			GPIO_BANK45->OUT_DATA = 0x04000000;
 		else if(cyc>=25 && cyc <=32)
			GPIO_BANK45->OUT_DATA = 0x04800000;


		evm_init_timer(TMR1, 10);
		while(1)
		{
			if(Time_over == 1)
			{
				Time_over = 0;
				break;
			}
		}

        GPIO_BANK23->OUT_DATA &= 0xCFFFFFFF;//GPIO3[12]=0 3[13]=0 ÍË³öÅäÖÃ ÔËĞĞÄ£Ê½
		Max_Buffer = (int)(cyc*1000/12.8) - 5; //Áô³ö5msµÄÓàÁ¿½øĞĞ³ÌĞòµÄÔÙÅäÖÃ
	}
}
//²ÎÊıÏÂ´«
void configure_para()
{									
		if(ddr2_cmd[arm_syn_flag] == 1) 	//Í¬²½ÅäÖÃ
		{		
			ddr2_cmd[arm_syn_flag] = 0;
			syn_type = ddr2_cmd[arm_syn_type];		//ÄÚ1//Íâ2//½ø3
			syn_cyc = ddr2_cmd[arm_syn_cyc];		//ÖÜÆÚ
			syn_config(syn_type, syn_cyc); 			//ÅäÖÃCPLD
		}
		if(ddr2_cmd[arm_parameter_flag] == 1)       //¼ì²â²ÎÊıÅäÖÃ
		{		
			ddr2_cmd[arm_parameter_flag] = 0;		//¿É·ñ¸ü¸ÄÎª½á¹¹Ìå´«Êä
			Parameter_sig.snr_max = *(double *)&ddr2_cmd[arm_parameter_snr_max];
			Parameter_sig.snr_abso = *(int *)&ddr2_cmd[arm_parameter_abs_snr_threshold];
			Parameter_sig.snr_rela = *(int *)&ddr2_cmd[arm_parameter_rel_snr_threshold];
			Parameter_sig.snr_rela2 = *(int *)&ddr2_cmd[arm_parameter_rel2_snr_threshold];
			Parameter_sig.Maikuan_l = *(int *)&ddr2_cmd[arm_parameter_min_pulse_width];
			Parameter_sig.Maikuan_h = *(int *)&ddr2_cmd[arm_parameter_max_pulse_width];
			Parameter_sig.w_before = *(int *)&ddr2_cmd[arm_parameter_w_before];
			Parameter_sig.w_all = *(int *)&ddr2_cmd[arm_parameter_w_all];
			Parameter_sig.w_after = *(int *)&ddr2_cmd[arm_parameter_w_after];
			Parameter_sig.close_time = *(int *)&ddr2_cmd[arm_parameter_close_time];
			Parameter_sig.th_noise = *(int *)&ddr2_cmd[arm_parameter_th_noise];
			
			Close_num = Parameter_sig.close_time*1000/12800;      //±Ü»ìÏìµÄ¿éÊı
		}
		if(ddr2_cmd[arm_work_mode_flag] == 1) //Ä£Ê½ÓĞÁ½ÖÖ£º1¡¢Ó¦´ğÄ£Ê½=1   2¡¢Í¬²½Ä£Ê½=0
		{     
			ddr2_cmd[arm_work_mode_flag] = 0;
			Work_mode = ddr2_cmd[arm_work_mode];
			sys_gain = ddr2_cmd[arm_sys_gain];
		}
		if(ddr2_cmd[arm_beacon_flag] == 1)	//ËÄ¸öĞÅ±ê×ª·¢Ê±ÑÓÅäÖÃ
		{		
			ddr2_cmd[arm_beacon_flag] = 0;
			Turnaround[0] = *(double *)&ddr2_cmd[arm_beacon_turnaround[0]]; 
			Turnaround[1] = *(double *)&ddr2_cmd[arm_beacon_turnaround[1]];
			Turnaround[2] = *(double *)&ddr2_cmd[arm_beacon_turnaround[2]];
			Turnaround[3] = *(double *)&ddr2_cmd[arm_beacon_turnaround[3]];
		}

		Start_flag = ddr2_cmd[arm_start_flag];

}

void init_global_var(void)
{
	Syn_flag = 0;
	Rx_flag=0;
	Pp_flag = 0;
	Block_num = 0;
    
	send_num = 0;
	Time_over = 0;

	memset(buffer1,0,sizeof(buffer1));
	memset(buffer2,0,sizeof(buffer2));	
	memset(Fifo1,0,sizeof(Fifo1));
	memset(Correlation1,0,sizeof(Correlation1));
	memset(Correlation2,0,sizeof(Correlation2));
	memset(Correlation3,0,sizeof(Correlation3));
	memset(Correlation4,0,sizeof(Correlation4));
	memset(Envelop1,0,sizeof(Envelop1));
	memset(Envelop2,0,sizeof(Envelop2));
	memset(Envelop3,0,sizeof(Envelop3));
	memset(Envelop4,0,sizeof(Envelop4));
	memset(&Sig1, 0, sizeof(Sig1));
	memset(&Sig2, 0, sizeof(Sig2));
	memset(&Sig3, 0, sizeof(Sig3));
	memset(&Sig4, 0, sizeof(Sig4));
}


//fifo¯Êı
void fifo(short *fifo_buffer, short *input_buffer, int n)
{
	int j;
	memcpy(fifo_buffer, fifo_buffer+n, 2*n*sizeof(short));	
	for(j = 0; j < n; j++){
		fifo_buffer[2*n+j] = input_buffer[j];	
	}
}

int d2i(double d)
{
    if (d >=  2147483647.0) 
    	return (int)0x7FFFFFFF;
    if (d <= -2147483648.0) 
    	return (int)0x80000000;
    return (int)d;
}

//²úÉúFFTÊ¹ÓÃµÄµûĞÎÒò×Ó
int gen_twiddle_fft32x32(int *w, int n, double scale)
{
    int i, j, k, s=0, t;

    for (j = 1, k = 0; j < n >> 2; j = j << 2, s++){
        for (i = t=0; i < n >> 2; i += j, t++){
            w[k +  5] = d2i(scale * cos(6.0 * PI * i / n));
            w[k +  4] = d2i(scale * sin(6.0 * PI * i / n));

            w[k +  3] = d2i(scale * cos(4.0 * PI * i / n));
            w[k +  2] = d2i(scale * sin(4.0 * PI * i / n));

            w[k +  1] = d2i(scale * cos(2.0 * PI * i / n));
            w[k +  0] = d2i(scale * sin(2.0 * PI * i / n));

            k += 6;
        }
    }

    return k;
}

//²úÉú²Î¿¼ĞÅºÅµÄÆµÓòÖµ
void get_fft(int *sig_fft, short *sig, int n_sig, int *w, int n_w, int *temp_buffer)
{	
    int j;
    int m = 2*n_w;
    
    memset(temp_buffer, 0, n_w*8);					
	for(j=0;j<n_sig;j++)
	{
		temp_buffer[2*j] = (int)sig[j];
		temp_buffer[2*j+1] = 0;
	}	
	DSP_fft32x32(w, n_w, temp_buffer, sig_fft);
	for(j=0;j<m;j++){
		sig_fft[j] = sig_fft[j]>>6;
	}
}

//Ïà¹ØÔËËã
void get_xcorr(int *correlation, int *sig_fft, short *ref_fft, int n, int *w, int *temp_1, int *temp_2)
{
	int j;
	int m = n/2;

	for(j=0;j<n;j++)
	{
		temp_1[2*j]   = (sig_fft[2*j]*ref_fft[2*j]-sig_fft[2*j+1]*ref_fft[2*j+1])>>9;
		temp_1[2*j+1] = (sig_fft[2*j]*ref_fft[2*j+1]+sig_fft[2*j+1]*ref_fft[2*j])>>9;
	}
	DSP_ifft32x32(w, n, temp_1, temp_2);	

	memcpy(correlation, correlation+m, m*4);
	
	for(j=0;j<m;j++)
		correlation[m+j] = temp_2[n+2*j]; 
	
}

//»ñµÃĞÅºÅ°üÂç
void get_envelop(int *envelop, int *correlation, int *coef_fft, int n, int *w, int *temp_1, int *temp_2)
{
	int j;
	int m = 2*n, m2 = n/2;

	memset(temp_1, 0, n*8);

	for(j=0;j<n;j++)
		temp_1[j*2] = abs(correlation[j]>>10);
	
	DSP_fft32x32(w, n, temp_1, temp_2);
	for(j=0;j<m;j++)
		temp_2[j]=temp_2[j]>>6;                 
	
	for(j=0;j<n;j++)
	{
		temp_1[2*j]   = (temp_2[2*j]*coef_fft[2*j]-temp_2[2*j+1]*coef_fft[2*j+1])>>8;
		temp_1[2*j+1] = (temp_2[2*j]*coef_fft[2*j+1]+temp_2[2*j+1]*coef_fft[2*j])>>8;
	}
	DSP_ifft32x32(w, n, temp_1, temp_2);
	memcpy(envelop, envelop+m2, m2*3*sizeof(int)); //Ã¿´Î¸üĞÂ512µãÊı¾İ
	for(j=0;j<m2;j++)
		envelop[3*m2+j]=temp_2[n+2*j]; 
}

void max(int *max_val, int *max_num, int *x, int nx, int ind)
{
	int index, i;
	int max0 = 0;
	
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


int mean(int *x, int nx, int ind) //ÇóÔëÉù¾ùÖµµÄº¯Êı
{
	int i,sum = 0;
	for(i=0; i<nx; i=i+ind)
	{
		sum = sum + x[i];
	}
	return (int)(sum*ind/nx);
}

int find_num(int *x, int nx, int abso, int ind)
{	
	int i,num = 0;
	for(i=0; i<nx; i=i+ind)
	{
		if(x[i] > abso)
			num++;
	}
	return num;
}


void Tcp_Info(void)
{
	ddr2_delay[0] = Sig1.delay;
	ddr2_delay[1] = Sig2.delay;
	ddr2_delay[2] = Sig3.delay;
	ddr2_delay[3] = Sig4.delay;
	
	ddr2_corr_pos[0] = Sig1.corr_pos;
	ddr2_corr_pos[1] = Sig2.corr_pos;
	ddr2_corr_pos[2] = Sig3.corr_pos;
	ddr2_corr_pos[3] = Sig4.corr_pos;

	ddr2_envelop_max[0] = Sig1.envelop_max;
	ddr2_envelop_max[1] = Sig1.envelop_max;
	ddr2_envelop_max[2] = Sig2.envelop_max;
	ddr2_envelop_max[3] = Sig3.envelop_max;

	ddr2_real_pulse_width[0] = Sig1.wid;
	ddr2_real_pulse_width[1] = Sig2.wid;
	ddr2_real_pulse_width[2] = Sig3.wid;
	ddr2_real_pulse_width[3] = Sig4.wid;

	ddr2_noise_std[0] = Sig1.noise_std;
	ddr2_noise_std[1] = Sig2.noise_std;
	ddr2_noise_std[2] = Sig3.noise_std;
	ddr2_noise_std[3] = Sig4.noise_std;

	ddr2_snr_out[0] = Sig1.snr;
	ddr2_snr_out[1] = Sig2.snr;
	ddr2_snr_out[2] = Sig3.snr;
	ddr2_snr_out[3] = Sig4.snr;

	ddr2_beacon_status[0] = Sig1.flag;
	ddr2_beacon_status[1] = Sig2.flag;
	ddr2_beacon_status[2] = Sig3.flag;
	ddr2_beacon_status[3] = Sig4.flag;

	memcpy(&ddr2_parameter, &Parameter_sig, sizeof(Parameter_sig)); //
}

void detection(struct Sig *sig, int *envelop, struct Para *parameter, int block_num, int close_num)
{
	int th_max1 = 0;
	int max_val = 0;
	int max_num = 0;
	int max_ave1 = 0;
	int max_ave2 = 0;
	int ave_num = 0;
	int wid = 0;
	int k,j,m;
	int ave[8] = {0};
	int noise_l = 100;
	int l = 40;
	int weifeng_num = 0, weifeng_ind = 4, weifeng_before = 0, weifeng_after = 0;
	int start_num, end_num;

	if(block_num == close_num)
	{
		sig->th_abso = mean(envelop, 3*IN_LENGTH, 2)*parameter->snr_abso; //È¡Ç°Èı¸öbuffer 
		max(&sig->th_max[0], &sig->th_num[0], envelop+2*IN_LENGTH, IN_LENGTH, 1);
		sig->th_max[1] = sig->th_max[2] = sig->th_max[0];
		sig->th_num[1] = sig->th_num[2] = sig->th_num[0];
	}
	if(block_num > close_num)
	{
		sig->th_abso = (int)(sig->th_abso*(1 - 1.0/noise_l) + (double)mean(&envelop[2*IN_LENGTH], IN_LENGTH, 2)/noise_l);
		if(sig->th_abso > parameter->th_noise*1790)//ADÇ°¶ËµÄ×îĞ¡ÃÅÏŞ£¬1790000Îª1VĞÅºÅ¶ÔÓ¦µÄ°üÂçÖµ
			sig->th_abso = parameter->th_noise;

		sig->th_max[2] = sig->th_max[1];
		sig->th_max[1] = sig->th_max[0];
		sig->th_num[2] = sig->th_num[1];
		sig->th_num[1] = sig->th_num[0];
		max(&sig->th_max[0], &sig->th_num[0], envelop+2*IN_LENGTH, IN_LENGTH, 1);
		if(sig->th_max[0] > sig->th_max[1])
			th_max1 = sig->th_max[0];
		else
			th_max1 = sig->th_max[1];

		th_max1 = (int)(th_max1*parameter->snr_max);
		weifeng_num = find_num(envelop, 4*IN_LENGTH, th_max1, weifeng_ind); //ËÑË÷Çø¼äÄÚËùÓĞ¿ÉÄÜ·åÖµµÄÊıÁ¿
		weifeng_num *= weifeng_ind;
		if(weifeng_num >= parameter->w_all)
			return;
			
		
		start_num = 2*IN_LENGTH;
		end_num =  3*IN_LENGTH;
		k = start_num;
		for(j=start_num; j<end_num; j=j+2)
		{
			if(j <= k)
				continue;

			if(envelop[j] > th_max1)
			{
				max_val = envelop[j];
				max_num = j;
				for(k=j; k<end_num+50; k++)//ÏòºóËÑË÷·åÖµ
				{
					if(envelop[k] > max_val)
					{
						max_val = envelop[k];
						max_num = k;
					}
					else if(envelop[k] <= 2*max_val/3)
					{
						for(m=max_num; m>max_num-50; m--)
						{
							if(envelop[m] <= max_val/2)
							{
								wid = k - m; //´ø¿í
								ave[0] = mean(envelop+m-parameter->Maikuan_l-1*l, l, 2)*parameter->snr_rela; //Ç°1msµÄ¾àÀëËÑË÷ ÇøÓòĞÅÔë±È µ÷ÆµĞÅºÅÏà¹Ø»ìµş
								ave[1] = mean(envelop+m-parameter->Maikuan_l-2*l, l, 2)*parameter->snr_rela;
								ave[2] = mean(envelop+m-parameter->Maikuan_l-3*l, l, 2)*parameter->snr_rela;
								ave[3] = mean(envelop+m-parameter->Maikuan_l-4*l, l, 2)*parameter->snr_rela;
								ave[4] = mean(envelop+m-parameter->Maikuan_l-5*l, l, 2)*parameter->snr_rela;
								ave[5] = mean(envelop+m-parameter->Maikuan_l-6*l, l, 2)*parameter->snr_rela;
								ave[6] = mean(envelop+m-parameter->Maikuan_l-7*l, l, 2)*parameter->snr_rela;
								ave[7] = mean(envelop+m-parameter->Maikuan_l-8*l, l, 2)*parameter->snr_rela;
								max(&max_ave1, &ave_num, ave, 8, 1);
								max_ave2 = mean(envelop+k+parameter->Maikuan_l, l, 2)*parameter->snr_rela2; //´ÓÖ÷·åµÚÒ»¸öÁãµãÏòºó1ms
								weifeng_before = find_num(envelop+m-parameter->Maikuan_l-10*l, 10*l, max_val/2, weifeng_ind);
								weifeng_before *= weifeng_ind;
								weifeng_after  = find_num(envelop+k+parameter->Maikuan_l, 5*l, max_val/2, weifeng_ind);
								weifeng_after *= weifeng_ind;
								if(max_val > max_ave1 && max_val > max_ave2 && wid > parameter->Maikuan_l && wid < parameter->Maikuan_h && weifeng_after < parameter->w_after && weifeng_before < parameter->w_before)
								{
									sig->snr = parameter->snr_rela*max_val/max_ave1;//ÏÔÊ¾ËùÓĞ¼ÆËãµÄ²ÎÊı 
									sig->flag = 1;
									sig->corr_pos = max_num;
									sig->envelop_max = max_val;
									sig->wid = wid;
									return; 
								}
								else
									break;
							}
						}
						break;	
					}
				}		
			}
		}
	}
}

void get_delay(int y1, int y2, int y3, struct Sig *sig, int block_num, double turnaround, char work_mode)
{
	double xn,ts,tao1,tao2,y,angle,w,A,B,fai,k,tao_max;

    xn = (double)(sig->corr_pos+(block_num-4)*IN_LENGTH-64-408+1); //Êı×ÖÂË²¨ÑÓ³Ù64µã ĞÅºÅ³¤¶ÈÎª408
	ts = 1.0/40000;
	tao1 = (xn-1)/40000;
	tao2 = xn/40000;
	y = (double)(y1+y3)/(2*y2);
	angle = acos(y);
	w = angle*40000;
	A = (y1*sin(w*tao2) - y2*sin(w*tao1))/sin(w*ts);
	B = (y2*cos(w*tao1) - y1*cos(w*tao2))/sin(w*ts);
    fai= atan2(B,A);
	k = ceil((w*tao1-fai)/PI);
	tao_max = (k*PI+fai)/w;
		
	if(work_mode == 1){
    	sig->delay = (tao_max - turnaround + 0.01 + digital_delay)/2;		//Ó¦´ğÄ£Ê½0.01¶ÔÓ¦ÓÚ·¢ÉäĞÅºÅµÄÊ±¼ä
	}else
	if(work_mode == 0){
		sig->delay = tao_max - turnaround + digital_delay;				//Í¬²½Ä£Ê½
	}else
		sig->delay = -1;
}

 






