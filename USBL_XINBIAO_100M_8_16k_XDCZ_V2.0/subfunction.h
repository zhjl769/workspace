/*
 * subfunction.h
 *
 *  Created on: 2016-12-21
 */

#ifndef SUBFUNCTION_H_
#define SUBFUNCTION_H_
#include "function_declare.h"
#include "Send_Tx.h"
#include "Create_signal.h"
/*高速时钟计时器*/
void timer_high_clk(volatile Uint32 time,Uint16 clk)//time为预期计时时间单位us; clk为系统时钟频率单位MHz
{//使用前需开启定时器中断
	//CHIP_FSET(ST1_55,XF,1);//XF引脚置高
	//CHIP_FSET(ST1_55,XF,0);//XF引脚置低
	timer1_cnt = 0;
	volatile Uint16 a=0,b=0,temp_PRD=0,temp_PRSC=0;
	time = time*clk;//转换成时钟周期
	a = time>>15;//取商（移位除法，等效于除以32768）
	b = time-(((unsigned long)a)<<15);//取余
	b = b-1;//余数值预先减1，准备赋值给寄存器
	temp_PRD = TIMER_RGET(PRD1);//保存之前的寄存器
	temp_PRSC = TIMER_RGET(PRSC1);//保存之前的寄存器
	TIMER_RSET(PRD1,0x7FFFu);//0x7FFFu = 十进制32767
	TIMER_RSET(PRSC1,0x0000u);//高位清零
	TIMER_FSET(TCR1,TLB,1);//TLB置1，使得PRD、TDDR中的值装入TIM、PSC（确保定时器已停止才能置位！）
	TIMER_start(mhTimer1);
	while(timer1_cnt < a);
	TIMER_stop(mhTimer1);
	TIMER_RSET(PRD1,b);
	TIMER_FSET(TCR1,TLB,1);//TLB置1，使得PRD、TDDR中的值装入TIM、PSC（确保定时器已停止才能置位！）
	TIMER_start(mhTimer1);
	while(timer1_cnt < a+1);
	TIMER_stop(mhTimer1);
	TIMER_RSET(PRD1,temp_PRD);
	TIMER_RSET(PRSC1,temp_PRSC);//恢复程序，恢复PRD,PRSC寄存器
	TIMER_FSET(TCR1,TLB,1);//TLB置1，使得PRD、TDDR中的值装入TIM、PSC（确保定时器已停止才能置位！）
}

void variable_init()
{
	FLAGSENDOVER=0;
	//标志位复位
	FLAG_RS232_REVOK = 0;//串口
	Flag_CIF = 0;        //
	Flag_IIF = 0;        //
	FSK_flag = 0;        //FSK指令
	Flag_timer = 0;      //主站周期发射timer0计数器
	responder_flag = 0;  //电触发

	//相关峰检测
	Pos_flag = 0;	     //检测到相关峰标志
	endpos = 0;          //相关峰位置
	width_save = 0;      //相关峰宽度
	Xcorr_max_val = 0;
	resend_delay = 0;    //转发延时

	DMAnum = 0;          //AD采集中DMA计数器
	DMAdone = FALSE;
	timer0_cnt = 0;      //定时器0计数器
	timer1_cnt = 0;      //定时器1计数器
	count_timer = 0;     //同步命令计数器（无用）

	//传感器
	temp_ADC = 0;
	voltage_AD = 0;
	depth = 0.0;
	temperature = 0.0;

	block2 = 2;
	release_flag = 0;    //释放成功状态位

	memset(temp_dainei_temp,0,sizeof(short)*(NFFT_mingling));
	memset(Rev,0,sizeof(short)*NFFT);
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	memset(Rev_xcorr,0,sizeof(short)*2*NFFT);
	memset(Rev_xcorrout_temp,0,sizeof(short)*400);    //每个信号的Rev_xcorr中间保存结果
	memset(Rev_xcorrout,0,sizeof(short)*NFFT);
	memset(Rev_save_filter_out,0,sizeof(short)*NFFT);
	memset(Rev_envelope,0,sizeof(short)*4096);

	memset(Rev_xcorrout_temp2,0,sizeof(short)*400);    //每个信号的Rev_xcorr中间保存结果
	memset(Rev_envelope2,0,sizeof(short)*4096);

	memset(Rev_xcorrout_temp3,0,sizeof(short)*400);    //每个信号的Rev_xcorr中间保存结果
	memset(Rev_envelope3,0,sizeof(short)*4096);

	memset(Rev_xcorrout_temp4,0,sizeof(short)*400);    //每个信号的Rev_xcorr中间保存结果
	memset(Rev_envelope4,0,sizeof(short)*4096);

	//噪声门限复位
	/*	NOISE_TH_NEW = 100;
	NOISE_TH_NEW2 = 100;
	memset(noise_maxbuffer,0,sizeof(short)*2);
	memset(noise_maxbuffer2,0,sizeof(short)*2);*/

	// FSK指令检测内存复位
	memset(FSK_cmd_flag,0,sizeof(short)*12);       //单次FFT指令判断结果

	memset(count_FSK_cmd0,0,sizeof(short)*12);     //释放信号缓存空间
	memset(count_FSK_cmd1,0,sizeof(short)*12);     //释放信号缓存空间
	memset(count_FSK_cmd2,0,sizeof(short)*12);     //释放信号缓存空间
	memset(count_FSK_cmd3,0,sizeof(short)*12);     //释放信号缓存空间
	memset(count_FSK_cmd4,0,sizeof(short)*12);     //释放信号缓存空间
	memset(count_FSK_cmd5,0,sizeof(short)*12);     //释放信号缓存空间
	memset(count_FSK_cmd6,0,sizeof(short)*12);     //释放信号缓存空间
	memset(count_FSK_cmd7,0,sizeof(short)*12);     //释放信号缓存空间
	memset(count_FSK_cmd8,0,sizeof(short)*12);     //释放信号缓存空间
	memset(count_FSK_cmd9,0,sizeof(short)*12);     //释放信号缓存空间
	memset(count_FSK_cmd10,0,sizeof(short)*12);    //释放信号缓存空间
	memset(count_FSK_cmd11,0,sizeof(short)*12);    //释放信号缓存空间

	memset(sum_count_FSK_cmd1,0,sizeof(short)*14); //指令判断标志位

}

/*系统配置*/
void system_config()
{
	//	IODOR=0xff;
	IODATA=0x00;
	IER0=0x0000;
	IFR0=0xFFFF;
	IER1=0x0011;//使能DMAC2
	IFR1=0xFFFF;
	variable_init();
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
}


/*产生参考信号*/
void Produceref(short *X_ref, short RAMSES_ref_sig_offset)
{
	volatile unsigned long lfm_head;
	volatile double t_lfm;
	volatile double fai;
	volatile short q;
	int i;

	memset(X_ref,0,sizeof(short)* 2* NFFT);
	fai = 0;
	for(q = 0; q < 8; q++)
	{
		for(lfm_head = 0; lfm_head < RAMSES_time_rev[RAMSES_CODE[RAMSES_ref_sig_offset][q]-1]; lfm_head++)
		{
			t_lfm = lfm_head/40000.0; //采样频率40kHz
			templfm = (short)(-550*(sin(2*PI*RAMSES_freq_L[RAMSES_CODE[RAMSES_ref_sig_offset][q]-1]*t_lfm + fai)));
			X_ref[3695 + (q*50 + lfm_head)] = templfm;
		}
		t_lfm = (lfm_head)/40000.0;
		fai += 2*PI*RAMSES_freq_L[RAMSES_CODE[RAMSES_ref_sig_offset][q]-1]*t_lfm ;
	}

	for (i = 0; i < 400; i++)
	{
		X_ref[2*i] = X_ref[4095 - i];
		X_ref[4095 - i] = 0;
		X_ref[2*i+1] = 0;
	}

	cfft_NOSCALE(X_ref,NFFT);
	cbrev(X_ref,X_ref,NFFT);
}


/*产生发射信号  CIF;IIF*/
void Producelfm()
{
	volatile unsigned long lfm_head;
	volatile double t_lfm;
	for(lfm_head = 0; lfm_head < 10000; lfm_head++)
	{
		t_lfm = (float)lfm_head/1000000.0;//1MHz
		templfm = (short)(20000*(cos(2*PI*9000*t_lfm + PI*6000*t_lfm*t_lfm/0.01)));//30kHz,10ms

		if(templfm < 0)
		{
			Send_sig[lfm_head]=0x0005;//GPIO2=1;GPIO1=0;GPIO0=1;
		}
		else
		{
			Send_sig[lfm_head]=0x0003;//GPIO2=0;GPIO1=1;GPIO0=1;
		}
	}
}
void Produce_send_signal(short send_signal_offset)
{
	volatile unsigned long lfm_head;
	volatile double t_lfm;
	volatile double fai;
	volatile short q;
	int i;

	//定位信号10ms
	i = 0;
	fai = 0;
	for(q = 0; q < 8; q++)
	{
		for(lfm_head = 0; lfm_head < RAMSES_time_send[RAMSES_CODE[send_signal_offset][q]-1]; lfm_head++)
		{
			t_lfm = lfm_head/1000000.0;
			templfm = (short)(2000*(sin(2*PI*RAMSES_freq_L[RAMSES_CODE[send_signal_offset][q]-1]*t_lfm + fai)));

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
		fai = 2*PI*RAMSES_freq_L[RAMSES_CODE[send_signal_offset][q]-1]*t_lfm + fai;
	}
}


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

//产生双通道窄带滤波器系数
void Producefilter_Band(short ID1, short ID2)
{
	int i;
	for (i = 0; i < 2*NFFT; i ++)
	{
		h_BPF1[i] = 0;
	}
	for (i = 0; i < 257; i ++)
	{
		h_BPF1[2*i] = fir_B_XS[ID1][i]/1200;
	}
	cfft_NOSCALE(h_BPF1,NFFT);  //是否考虑溢出问题

	cbrev(h_BPF1,h_BPF1,NFFT);
	for (i = 0; i < 2*NFFT; i ++)
	{
		h_BPF2[i] = 0;
	}
	for (i = 0; i < 257; i ++)
	{
		h_BPF2[2*i] = fir_B_XS[ID2][i]/1200;
	}
	cfft_NOSCALE(h_BPF2,NFFT);  //是否考虑溢出问题
	cbrev(h_BPF2,h_BPF2,NFFT);
}



/*获取噪声量级*/
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
			将时延值改为原值的一半。 */
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
	//表示压力传感器初始化成功
	IER0 &= 0x0fff7;
	IFR0 = 0x0ffff;//关闭int2中断
}


void get_pressure()
{
	volatile short temp_js,buff,f_send_uart,j_uart;
	volatile Uint32 ii;
	FLAG_yl = 0;
	j_uart = 0;
	temp_js = 5;
	f_send_uart = 0;
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
	}
	timer1_cnt = ii;
	temp_yl_data = (((Uint32)RS485_DATA[2])<<24) + (((Uint32)RS485_DATA[3])<<16) + (((Uint32)RS485_DATA[4])<<8) + ((Uint32)RS485_DATA[5]);
	if(temp_yl_data > 268435455)
	{
		temp_yl_data = 0;
	}
	//temp_yl_data的值单位为：Pa；且1 Pa=1e-5 bar=（1e-5）*10 m=1e-4 m
	//单位是深度分米，即延迟（long）FLAG_yl_data个100us时间,后续计算中：0.1m代表延迟0.1ms  压力调制时间为1.5m对应0.2ms
	//此处也可以变换为：FLAG_yl_data = (long)temp_yl_data/10;代表延迟的us值。
	//表示压力传感器获取信息成功
	if(temp_yl_data > 0)
	{
		depth = (float)(temp_yl_data)/10000;//(首先将得到的压力值变成深度m=yl/10000；再将深度数据转换为时延值=m/1.5，单位：ms。)
	}
	else
	{
		depth = 0.0;
	}
}


/*获取电压信息函数
  函数功能：利用DSP内部AD(10bit),采样率为100kHz,采集64点数据,取均值 */
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
	/*
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
	 */
	get_voltage();   //获取电压信息
	get_pressure();  //压力获取函数，获取值为0则压力传感器连接有问题

	//获取温度；
	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;  //使能定时器1中断
	TIMER_start(mhTimer1);//打开1定时
	get_temperature();//读取温度
	IODATA = 0x0000;
	TIMER_stop(mhTimer1);//关闭定时器1
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//关闭定时器1中断
	//获取温度

	SPCR2_0 = 0x00c1;    //打开McBSP
	SPCR1_0 = 0x0001;
	get_noise();//获取当前噪声
	SPCR2_0 = 0x0000;    //关闭McBSP
	SPCR1_0 = 0x0000;
	uart_config();
}

void URAT_reply_func()
{
	short i;
	FLAG_RS232_REVOK = 0;      //串口接收标志清零
	get_parameter();
	uart_config();
	if(SD_status == 1)
	{
		SD_WriteData(SectorCounter,SD_write);//7672扇区为每次保留数据扇区
		SectorCounter = SectorCounter + 1;
	}
	for(i = 0; i < 28; i++)     //发送42字节信息
	{
		UART_THR232 = SD_write[i];
		do
		{
			buff = UART_LSR232;
			f_send_uart = buff & (0x0060);
		}while(f_send_uart == 0);//查询发送位是否清零
	}
}

/*重叠保留数据 将接收数据转化为有符号数*/
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
  函数功能：将数据存在FFT缓存单元中，虚部为0，实部右移7bit */
void FFTprepare(short *data, short *data_bu0, short data_N)
{
	int i;
	for (i = 0; i < data_N; i++)
	{
		data_bu0[2*i] = data[i] >> 8;//偶地址右移7bit
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
  函数功能：将数据存在FFT缓存单元中，虚部为0，实部右移7bit */
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
  WYH  时域反卷积<--->频域共轭相乘。 */
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

		C[2*i] = (short)((temp5 - temp6) >> 20);    //(short)((temp5+temp6)>>2)
		C[2*i + 1] = (short)((temp7 + temp8) >> 20);//(short)((temp7-temp8)>>2)
	}
}

/*点乘函数
  函数功能：复数点乘，输入两个复数数组A、B，输出C，数组长度为len */
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

		C[2*i] = (short)((temp5 - temp6) >> 12);    //(short)((temp5+temp6)>>2)
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
			检测成功标志位置1	 */
void detect(short *envelopein, short len,volatile int *flag,volatile short *endpos,short NOISE_TH_NEWtemp)
{
	short i,j,jishu=0;
	volatile short pos = 0;
	short Flag_sig = 0, flagg = 0;
	short startpos = 0, pos_begin =0, width = 0;
	short width_low = 0, width_high = 0;

	short Max_val = 0, Max_threshold = 0, Max_pos = 0;
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
		if(envelopein[i] > NOISE_TH_NEWtemp)
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
				if(envelopein[i] >= NOISE_TH_NEWtemp)
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
							pos_begin = j;                //pos_begin为左侧低限第一点
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

					//计算脉冲宽
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
					sum_mean_feng_hou = sum_feng_hou*2/200;                    //3为待定参数


					jishu = jishu + 1;
					Xcorr_max_val = 0;//SD卡存储该值
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
					if((envelopein[Max_pos] > 300) && (envelopein[Max_pos] > sum_mean_feng_hou) && (envelopein[Max_pos] > max_ave1) && (width > Maikuan_l) && (width < Maikuan_h) && (point_count_qian < 3) && (point_count_hou < 48)   )//相关峰满足局部信噪比和脉宽门  && (SNR_HD_out > 10)
					{
						//asm(" bclr XF ");
						pos = 1;
						//						Flag_CIF = 1;             //检测到信号1
						Flag_CIF_IIF = 1;         //检测到信号1
						Xcorr_max_val = envelopein[Max_pos]; //SD卡存储该值
						width_save = width;       //SD卡存储该值

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


/*  函数功能：检测询问信号，如果检测到，返回Pos_flag1标志位和endpos1检测位置*/
void asksignal_CIF(short *X_ref, short *Rev_xcorrout_temp, short *Rev_envelope ,volatile short *NOISE_TH_huadong_temp,  short *noise_maxbuffer, volatile short *NOISE_TH_NEW_temp)//
{
	int i,j;
	volatile long sum;
	volatile short k;
	short noise_max1,ave_num = 0;

	dot(X_ref,Rev_FFT,Rev_xcorr,NFFT);//与参考信号1点乘
	cifft_NOSCALE(Rev_xcorr,NFFT);    //反傅里叶变换
	cbrev(Rev_xcorr,Rev_xcorr,NFFT);
	memset(Rev_xcorr,0,20);
	//memset(Rev_xcorr+4076,0,20);

	j = 0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)	  //将相关结果重叠保留
	{
		Rev_xcorrout[j] = Rev_xcorrout_temp[j];
		j++;
	}

	for(i = Rev_resave; i < NFFT; i++)
	{
		Rev_xcorrout[i] = abs(Rev_xcorr[2*i]);
	}

	j = 0;
	for(i = BUF_LEN_MAX; i < NFFT; i++)	  //更新相关后的结果
	{
		Rev_xcorrout_temp[j] = Rev_xcorrout[i];
		j++;
	}

	memset(h_LPF1_db,0,sizeof(short)*(N_LPF1 + 2));
	oflag_LPF1 = fir2(Rev_xcorrout,h_LPF1,Rev_save_filter_out,h_LPF1_db,NFFT,N_LPF1);

	j = 0;
	for(i = 1648; i < 4096; i++)	      //将包络数据进行重叠保留
	{
		Rev_envelope[j] = Rev_envelope[i];
		j++;
	}

	j =2448;
	for(i = Rev_resave; i < NFFT; i++)    //完成数据更新
	{
		Rev_envelope[j] = Rev_save_filter_out[i];
		j++;
	}

	//---------------------------------------------------------------------------------------------------------
	//计算当前数据块的噪声门限  //完成噪声学习，得到滑动门限值。
	sum = 0;
	point_count_zong_th= 0;

	for(k = 0;k < 2048; k++)
	{
		sum = sum + Rev_save_filter_out[k];
	}
	sum = sum>>11;
	*NOISE_TH_huadong_temp = (*NOISE_TH_huadong_temp)*0.98 + (short)((long)sum*0.02);

	//获取噪声最大值，得到检测绝对门限。
	max_zh(&noise_max1, &ave_num, Rev_save_filter_out, 2048, 2);
	noise_maxbuffer[0] = noise_max1;

	if(noise_maxbuffer[0] > noise_maxbuffer[1])
	{
		if((noise_maxbuffer[0]*0.5) > *NOISE_TH_huadong_temp)
		{
			*NOISE_TH_NEW_temp = noise_maxbuffer[0]*0.5;
		}
		else
		{
			*NOISE_TH_NEW_temp = *NOISE_TH_huadong_temp;
		}
	}
	else
	{
		if((noise_maxbuffer[1]*0.5) > *NOISE_TH_huadong_temp)
		{
			*NOISE_TH_NEW_temp = noise_maxbuffer[1]*0.5;
		}
		else
		{
			*NOISE_TH_NEW_temp = *NOISE_TH_huadong_temp;
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
	noise_maxbuffer[1] = noise_maxbuffer[0];
	//---------------------------------------------------------------------------------------------------------
	//相关峰初判
	point_count_zong = 0;
	for(j=800;j<2448;j=j+4)
	{
		if (Rev_envelope[j] > point_count_zong_th)
		{
			point_count_zong = point_count_zong+1;
		}
	}

	if(point_count_zong < 11)
	{
		detect(Rev_envelope,2448,&Pos_flag,&endpos, *NOISE_TH_NEW_temp);//检测函数最大搜索100个相关峰，检测时间范围为
	}
	point_count_zong = 0;

}


/*检测准备函数
  函数功能：检测准备，包括重叠保留，FFT变换 */
void detect_prepare()
{
	Revprepare();   //重叠保留数据
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	FFTprepare(Rev,Rev_FFT,NFFT);
	cfft_NOSCALE(Rev_FFT,NFFT);  //FFT变换
	cbrev(Rev_FFT,Rev_FFT,NFFT);
	memset(Rev_FFT,0,20);
	//	memset(Rev_FFT+4076,0,20);
}

/*FSK检测准备函数
  函数功能：检测准备，包括重叠保留，FFT变换 */
void detect_prepare_FSK()
{
	Revprepare();  //重叠保留数据
	memset(Rev_FFT,0,sizeof(short)*2*NFFT);
	FFTprepare_FSK(Rev,Rev_FFT,NFFT);//
	cfft_NOSCALE(Rev_FFT,NFFT);      //
	cbrev(Rev_FFT,Rev_FFT,NFFT);     //
}


//定时器1 不同延时时间函数
void timer1_jishi(Uint32 resend_delay)
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

/*发射信号1
  函数功能：发射信号1，采用DMA0通道发射，定时器1为同步事件 */
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


void XDCZ_Tranmit_CIF()
{
	SPCR2_0 = 0x00c1;//打开McBSP
	SPCR1_0 = 0x0001;
	DMA_start(myhDma);
	system_config(); //系统重新配置
	DMA_stop(myhDma);
	Senddata0();     //发射CIF
	//设置避混响时间;单位us
	/*resend_delay = 100000;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}*/

	DMA_count = 0;   //完成最大检测时间计数
	SPCR2_0 = 0x00c1;//打开McBSP
	SPCR1_0 = 0x0001;
	DMA_start(myhDma);
}


void Tranmit_CIF()
{
	//	IER0 = 0x0010; //使能定时器0
	//	IFR0 = 0x0ffff;
	IER1 = 0x0001; //串口1中断
	IFR1 = 0x0ffff;

	//CHIP_FSET(ST1_55,XF,1);//XF引脚置高
	//	timer0_cnt = 0;
	//	TIMER_start(mhTimer0);
	//	do
	//	{
	//	}while(Flag_timer == 0);
	//	Flag_timer = 0;
	//IER0 &= 0xffef; //关闭定时器0
	//TIMER_stop(mhTimer0);
	//CHIP_FSET(ST1_55,XF,0);//XF引脚置低
	///////////////////////////////////////////
	//	IFR1 = 0x0ffff;
	//	IER1 |= 0x0040; //开启定时器1中断
	//	timer1_cnt = 0;
	//	timer_low_clk(2000000,clk_MHz);
	//	IFR1 = 0x0ffff;
	//	IER1 &= 0x0ffbf;//关闭定时器1中断
	///////////////////////////////////////////
	Senddata0();

}

void take_action_CIF_IIF()
{

	if((Flag_CIF==1) && (IIF_enable == 0))
	{
		resend_delay = Time_Delay - 51700 - (Uint32)((2*NFFT - endpos_float)*25);  //单位都是1us,三点内插
	}
	if((Flag_CIF==1) && (IIF_enable == 1))
	{
		resend_delay = Time_Delay - 51700 - (Uint32)((2*NFFT - endpos_float)*25);  //单位都是1us,三点内插
	}

	if(Flag_IIF==1)
	{
		resend_delay = Time_Delay - 51700 - (Uint32)((2*NFFT - endpos_float)*25);  //单位都是1us,三点内插
	}

	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}

	Senddata0();

	//设置避混响时间;单位us
	resend_delay = 1000000;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
}


/*执行动作1*/
void take_action1( short transmit_sig_val )
{
	short i;
	Uint32  temp;

	Senddata0();//发射信号1

	//计算信号的时间间隔;单位us
	temp = 0;
	for (i=0; i<transmit_sig_val; i++)
	{
		temp = temp + 10000;
	}

	resend_delay = 200000 + temp;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}

	Senddata0();

	resend_delay = 100000;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
}


/* 执行释放*/
void take_action2()
{
	//	short i;long temp;
	short temp_read;
	Senddata0();//发射信号1

	IODOR = 0x0bf;
	IODATA |= 0x0080;       //GPIO7 = 1，驱动释放器释放
	resend_delay = 25000000;//驱动释放时间:25s,释放器释放时间需24s。
	//	resend_delay = 300000;  //test
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
	TIMER_stop(mhTimer1);//关闭

	IFR1   = 0x0ffff;
	IER1  &= 0x0ffbf; //关闭定时器1中断
	IODOR  = 0x0bf;
	IODATA = 0x0000;  //GPIO7 = 0，停止驱动释放器释放
	Senddata0();

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
		timer1_jishi( resend_delay);
	}
	Senddata0();

	resend_delay = 500000;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
}


/* 电量上传*/
void take_action3()
{
	Senddata0();

	//获取电压，计算给过程时间
	timer1_cnt = 0;
	IFR1  = 0x0ffff;
	IER1 |= 0x0040;       //使能定1中断
	TIMER_start(mhTimer1);//打开定时器1定时
	get_voltage();
	TIMER_stop(mhTimer1); //关闭定时器1
	IFR1  = 0x0ffff;
	IER1 &= 0x0ffbf;      //关闭定时器1中断
	//获取电压结束，计算给过程时

	//规则：200ms+电压*10ms
	//DSP内部AD计算公式：V_ture = V_N*3.3/1024*(416k/51k)。其中：365k、51k电路板电阻分压。
	resend_delay = 200000 + (Uint32)((float)voltage_AD/1024*27*10000) - timer1_cnt;

	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
	Senddata0();//发射信号1

	//加入避混响时间
	resend_delay = 100000;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
}

/* 压力上传*/
void take_action4()
{
	Senddata0();

	timer1_cnt = 0;
	IFR1 = 0x0ffff;
	IER1 |= 0x0040;  //使能定1中断
	TIMER_start(mhTimer1);
	get_pressure();  //压力获取函数，获取值为0则压力传感器连接有问题

	TIMER_stop(mhTimer1);//关闭定时器1
	IFR1  = 0x0ffff;
	IER1 &= 0x0ffbf;    //关闭定时器1中断

	//200ms+深度m×0.1ms
	resend_delay = 200000 + (Uint32)((float)depth*0.1) - timer1_cnt;

	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
	Senddata0();

	//加入避混响时间
	resend_delay = 100000;
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
		main_node = 3;			//辅节点
		CIF_enable = 1;         //禁止CIF
		IIF_enable = 1;         //使能IIF
		XDCZ = 0;
		transmit_sig_val=1;     //两个IRF时间间隔
		take_action1( transmit_sig_val);
	}

	else if (sum_count_FSK_cmd1[1]>9)//禁止CIF
	{
		FSK_flag=1;
		transponder_mode = 1;
		main_node = 3;			//辅节点
		CIF_enable = 0;         //禁止CIF
		IIF_enable = 1;         //使能IIF
		XDCZ = 0;
		transmit_sig_val=2;     //两个IRF时间间隔
		take_action1( transmit_sig_val);
	}

	else if (sum_count_FSK_cmd1[2]>9)//禁止IIF
	{
		FSK_flag=1;
		transponder_mode = 1;
		main_node = 3;			//辅节点
		CIF_enable = 1;         //使能CIF
		IIF_enable = 0;         //禁止IIF
		XDCZ = 0;
		transmit_sig_val=3;     //两个IRF时间间隔
		take_action1( transmit_sig_val);
	}

	else if (sum_count_FSK_cmd1[3]>9)//配置为主站;周期1
	{
		FSK_flag=1;
		transponder_mode = 1;   //配置为水下模式
		main_node = 2;          //主站
		responder_period = 2000;  //配置CIF发射周期3秒
		CIF_enable = 0;         //禁止CIF
		IIF_enable = 0;         //禁止IIF
		XDCZ = 0;               //禁止相对测阵
		transmit_sig_val=4;     //两个IRF时间间隔
		take_action1( transmit_sig_val);
	}

	else if (sum_count_FSK_cmd1[4]>9)//配置为主站;周期2
	{
		FSK_flag=1;
		transponder_mode = 1;  //配置为水下模式
		main_node = 2;         //配置为主站
		responder_period = 3000;  //配置CIF发射周期4秒
		CIF_enable = 0;        //禁止CIF
		IIF_enable = 0;        //禁止IIF
		XDCZ = 0;              //禁止相对测阵
		transmit_sig_val=5;    //两个IRF时间间隔
		take_action1( transmit_sig_val);
	}

	else if (sum_count_FSK_cmd1[5]>9)//配置为主站;周期3
	{
		FSK_flag=1;
		transponder_mode = 1;  //配置为水下模式
		main_node = 2;         //配置为主站
		responder_period = 4000; //配置CIF发射周期5秒
		CIF_enable = 0;        //禁止CIF
		IIF_enable = 0;        //禁止IIF
		XDCZ = 0;              //禁止相对测阵
		transmit_sig_val=6;    //两个IRF时间间隔
		take_action1( transmit_sig_val);
	}

	else if (sum_count_FSK_cmd1[6]>9)//相对测阵;测阵次数1
	{
		FSK_flag=1;
		transponder_mode = 1;  //水下模式
		main_node = 1;         //相对测阵模式
		XDCZ_num = 50;         //配置测试次数；
		CIF_enable = 0;        //禁止CIF
		IIF_enable = 0;        //禁止IIF
		XDCZ = 1;              //禁止相对测阵
		transmit_sig_val = 7;  //两个IRF时间间隔
		take_action1( transmit_sig_val);
	}

	else if (sum_count_FSK_cmd1[7]>9)//相对测阵;测阵次数2
	{
		FSK_flag=1;
		transponder_mode = 1;  //配置为水下模式
		main_node = 1;         //相抵测阵模式
		XDCZ_num = 2;         //配置测试次数；
		CIF_enable = 0;        //禁止CIF
		IIF_enable = 0;        //禁止IIF
		XDCZ = 1;              //开启相对测阵
		transmit_sig_val = 8;  //两个IRF时间间隔
		take_action1( transmit_sig_val);
	}

	else if (sum_count_FSK_cmd1[8]>9)//相对测阵;测阵次数3
	{
		FSK_flag=1;
		transponder_mode = 1;  //配置为水下模式
		main_node = 1;         //相抵测阵模式
		XDCZ_num = 50;         //配置测试次数；
		CIF_enable = 0;        //禁止CIF
		IIF_enable = 0;        //禁止IIF
		XDCZ = 1;              //开启相对测阵
		transmit_sig_val = 9;  //两个IRF时间间隔
		take_action1( transmit_sig_val);
	}

	else if (sum_count_FSK_cmd1[9]>9) //释放指令
	{

		FSK_flag = 1;
		take_action2();
	}

	else if (sum_count_FSK_cmd1[10]>9)//获取电量
	{
		FSK_flag=1;
		take_action3();
	}

	else if (sum_count_FSK_cmd1[11]>9)//获取压力
	{
		FSK_flag=1;
		take_action4();
	}
	else if (sum_count_FSK_cmd1[12]>9)//costas
	{
		FSK_flag=1;
		costas_flag=1;
		transmit_sig_val = 10;  //两个IRF时间间隔
		take_action1( transmit_sig_val);
	}
	else if (sum_count_FSK_cmd1[13]>9)//lfm
	{
		FSK_flag=1;
		lfm_flag=1;
		transmit_sig_val = 11;  //两个IRF时间间隔
		take_action1( transmit_sig_val);
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
	}
	j=0;
	for(i = Rev_resave; i < NFFT; i++)
	{
		Rev_bp1[i] = abs(Rev_xcorr[2*i]);

	}
	memset(h_LPF2_db,0,sizeof(short)*(N_LPF2 + 2));
	oflag_LPF2 = fir2(Rev_bp1,h_LPF2,Rev_bp1_envelope,h_LPF2_db,NFFT,N_LPF2);
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
	}
	j=0;
	for(i = Rev_resave; i < NFFT; i++)
	{
		Rev_bp2[i] = abs(Rev_xcorr[2*i]);

	}
	memset(h_LPF2_db,0,sizeof(short)*(N_LPF2 + 2));
	oflag_LPF2 = fir2(Rev_bp2,h_LPF2,Rev_bp2_envelope,h_LPF2_db,NFFT,N_LPF2);
	qujunzhi2(Rev_bp2_envelope, NFFT);

	//降采样
	j = 0;
	for (i = (block2-1)*N_resample; i < block2*N_resample; i ++)
	{
		temp_dainei_temp[i] = (Rev_bp1_envelope[Rev_resave+40*j] + Rev_bp2_envelope[Rev_resave+40*j])/2;///64;
		j++;
	}

	//FFT检测频率
	if (block2 == 6)
	{
		FFTprepare2(temp_dainei_temp,temp_dainei_temp_fft, NFFT_mingling);
		cfft_NOSCALE(temp_dainei_temp_fft, NFFT_mingling);             //是否考虑溢出问题
		cbrev(temp_dainei_temp_fft,temp_dainei_temp_fft,NFFT_mingling);//可不可以指向一个数组
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
			temp_dainei_temp[i] = temp_dainei_temp[i+4*N_resample];
		}
		memset(sum_count_FSK_cmd1,0,14);
		memset(FSK_cmd_flag,0,14);

		//查表测频
		/*if ((f_code >= 60-2.5)&&(f_code < 60+2.5))
		{
			FSK_cmd_flag[0]=1;
		}
		else if ((f_code >= 65-2.5)&&(f_code < 65+2.5))
		{
			FSK_cmd_flag[1]=1;
		}*/
		if ((f_code >= 70-2.5)&&(f_code < 70+2.5))
		{
			FSK_cmd_flag[2]=1;
		}
		else if ((f_code >= 75-2.5)&&(f_code < 75+2.5))
		{
			FSK_cmd_flag[12]=1;
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
		/*else if ((f_code >= 100-2.5)&&(f_code < 100+2.5))
		{
			FSK_cmd_flag[8]=1;
		} */
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
		/*else if ((f_code >= 120-2.5)&&(f_code < 120+2.5))
		{
			FSK_cmd_flag[12]=1;
		}*/
		else if ((f_code >= 125-2.5)&&(f_code < 125+2.5))
		{
			FSK_cmd_flag[13]=1;
		}
		else if ((f_code >= 130-2.5)&&(f_code < 130+2.5))
		{
			FSK_cmd_flag[0]=1;
		}
		else if ((f_code >= 135-2.5)&&(f_code < 135+2.5))
		{
			FSK_cmd_flag[1]=1;
		}
		//数据搬移11次
		j=0;
		for (i = 1; i<12; i ++)
		{
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
			count_FSK_cmd12[j]     = count_FSK_cmd12[i];
			count_FSK_cmd13[j]     = count_FSK_cmd13[i];
			j++ ;
		}
		//更新1个数据
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
		count_FSK_cmd12[11]     = FSK_cmd_flag[12];
		count_FSK_cmd13[11]     = FSK_cmd_flag[13];

		//计算指令结果
		for (i = 0; i<12 ;i ++)
		{
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
			sum_count_FSK_cmd1[12]  = sum_count_FSK_cmd1[12]  + count_FSK_cmd12[i];
			sum_count_FSK_cmd1[13]  = sum_count_FSK_cmd1[13]  + count_FSK_cmd13[i];
		}
	}
	block2++;
}

//检测信号程序
void detect_signal()
{
	IER1 = 0x0011;    //使能串口int1中断
	IFR1 = 0x0ffff;
	SPCR2_0 = 0x00c1; //打开McBSP
	SPCR1_0 = 0x0001;

	while((Flag_CIF==0) && (Flag_IIF==0) && (FSK_flag==0) && (FLAG_RS232_REVOK==0) && (Flag_timer==0) && (responder_flag==0))
	{
		if (DMAdone == TRUE)
		{
			//asm(" bset XF ");
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
			DMA_FSETH(myhDma,DMACCR,ENDPROG,1);//重新配置DMA，开始下一帧传输

			detect_prepare();

			if(CIF_enable == 1)
			{
				asksignal_CIF(X_ref, Rev_xcorrout_temp, Rev_envelope, &NOISE_TH_huadong, noise_maxbuffer, &NOISE_TH_NEW);
				if (Flag_CIF_IIF==1)
				{
					Flag_CIF=1;
					Flag_CIF_IIF=0;
				}
			}

			if(IIF_enable == 1)
			{
				asksignal_CIF(X_ref2, Rev_xcorrout_temp2, Rev_envelope2, &NOISE_TH_huadong2, noise_maxbuffer2, &NOISE_TH_NEW2);
				if (Flag_CIF_IIF==1)
				{
					Flag_IIF=1;
					Flag_CIF_IIF=0;
				}
			}

			detect_prepare_FSK();  //检测FSK指令
			FSKdetect();           //检测FSK指令
			take_action_FSK();
			//////////////////////////////////////////////////
			if(main_node == 2)
			{
				DMA_count_zhuzhan++;
				if(DMA_count_zhuzhan == zhuzhan_detect_count)
				{
					break;
				}
			}//主站模式时，当计数达到规定时跳出检测
			//////////////////////////////////////////////////

			/*			if(Flag_CIF == 0)
			{
				memcpy(Rev_save,Rev,2*NFFT);//保留上一次数据，计算噪声
			}
			 */			//asm(" bclr XF ");
		}//end if(DMAdone==TRUE)
	}//end while

	do
	{
	}while(DMAdone == FALSE);

}


/*检测信号程序*/
void detect_signal_XDCZ()
{
	IER1 = 0x0011;    //使能串口int1中断
	IFR1 = 0x0ffff;

	while((Flag_CIF==0) && (FSK_flag==0) && (FLAG_RS232_REVOK==0) && (Flag_timer==0) && (responder_flag==0))
	{
		if (DMAdone == TRUE)
		{
			//asm(" bset XF ");
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
			DMA_FSETH(myhDma,DMACCR,ENDPROG,1);         //重新配置DMA，开始下一帧传输

			detect_prepare();

			if(sig1_flag==1)
			{
				asksignal_CIF(X_ref, Rev_xcorrout_temp, Rev_envelope, &NOISE_TH_huadong, noise_maxbuffer, &NOISE_TH_NEW);

				if (Flag_CIF_IIF==1)
				{
					sig1_flag = 0;
					Flag_CIF_IIF= 0;
					//低通滤波器长度为33；信号自身长度Rev_resave=400;检测envelope的起始位置为800
					if(XDCZ_work_mode==0) //记录信标1时延
						delay_sig1[XDCZ_CIF_count] =  (DMA_count-1)*41200 + (Uint32)((endpos_float-21-Rev_resave-800)*25);
					else if(XDCZ_work_mode==1)
						delay_sig1[XDCZ_CIF_count] = ((DMA_count-1)*41200 + (Uint32)((endpos_float-22-Rev_resave-800)*25)+10000-JCSY1);
					else
						delay_sig1[XDCZ_CIF_count]=1;
				}
			}

			if(sig2_flag==1)
			{
				asksignal_CIF(X_ref2, Rev_xcorrout_temp2, Rev_envelope2, &NOISE_TH_huadong2, noise_maxbuffer2, &NOISE_TH_NEW2);

				if (Flag_CIF_IIF==1)
				{
					sig2_flag = 0;
					Flag_CIF_IIF= 0;
					if(XDCZ_work_mode==0) //记录信标2时延
						delay_sig2[XDCZ_CIF_count] = (DMA_count-1)*41200 + (Uint32)((endpos_float-21-Rev_resave-800)*25);
					else if(XDCZ_work_mode==1)
						delay_sig2[XDCZ_CIF_count] = ((DMA_count-1)*41200 + (Uint32)((endpos_float-22-Rev_resave-800)*25)+10000-JCSY2);
					else
						delay_sig2[XDCZ_CIF_count]=1;
				}
			}

			if(sig3_flag==1)
			{
				asksignal_CIF(X_ref3, Rev_xcorrout_temp3, Rev_envelope3, &NOISE_TH_huadong3, noise_maxbuffer3, &NOISE_TH_NEW3);

				if (Flag_CIF_IIF==1)
				{
					sig3_flag = 0;
					Flag_CIF_IIF= 0;
					if(XDCZ_work_mode==0) //记录信标3时延
						delay_sig3[XDCZ_CIF_count] = (DMA_count-1)*41200 + (Uint32)((endpos_float-21-Rev_resave-800)*25);
					else if(XDCZ_work_mode==1)
						delay_sig3[XDCZ_CIF_count] = ((DMA_count-1)*41200 + (Uint32)((endpos_float-22-Rev_resave-800)*25)+10000-JCSY3);
					else
						delay_sig3[XDCZ_CIF_count]=1;
				}
			}

			if(sig4_flag == 1 )
			{
				asksignal_CIF(X_ref4, Rev_xcorrout_temp4, Rev_envelope4, &NOISE_TH_huadong4, noise_maxbuffer4, &NOISE_TH_NEW4);

				if (Flag_CIF_IIF==1)
				{
					//Flag_CIF = 1;
					sig4_flag = 0;
					Flag_CIF_IIF= 0;
					if(XDCZ_work_mode==0) //记录信标4时延
						delay_sig4[XDCZ_CIF_count] = (DMA_count-1)*41200 + (Uint32)((endpos_float-21-Rev_resave-800)*25);
					else if(XDCZ_work_mode==1)
						delay_sig4[XDCZ_CIF_count] = ((DMA_count-1)*41200 + (Uint32)((endpos_float-22-Rev_resave-800)*25)+10000-JCSY4);
					else
						delay_sig4[XDCZ_CIF_count]=1;
				}
			}
			//计算检测的DMA块长度
			if(DMA_count < XDCZ_max_DMA_N)
			{
				DMA_count = DMA_count + 1;
			}

			if(DMA_count == XDCZ_max_DMA_N-2)
			{
				Flag_CIF = 1;
				break;
			}

			//detect_prepare_FSK();//检测FSK指令
			//FSKdetect();         //检测FSK指令
			//take_action_FSK();

			//asm(" bclr XF ");

		}//end if(DMAdone==TRUE)
	}//end while

	do
	{
	}while(DMAdone == FALSE);

}


/*电触发功能*/
void Respondermod()
{
	responder_flag = 0;
	timer1_cnt = 0;
	IFR1  = 0x0ffff;
	IER1 |= 0x0040; //开启DMA1用于发射询问信号
	TCR1 &= 0x0fbef;

	while(timer1_cnt < responder_Delay- DMANum)
	{ ;
	}
	TCR1 |= 0x0c34;
	IFR1 = 0x0ffff;
	IER1 &= 0x0ffbf;//开启DMA1用于发射询问信号
	Senddata0();
	resend_delay = 50000;
	if(resend_delay > 0)
	{
		timer1_jishi( resend_delay);
	}
	IER0 |= 0x0014; //使能定时器0和外部中断0
	IFR0 = 0x0ffff;

}

//将相对测阵得到的时延值写入缓存
void SD_delay_W(Uint32 *delay_sig)
{
	short i,j;
	j=50;
	for (i=0;i<50;i++)
	{
		SD_write[j]   = (delay_sig[i]) & 0x0ff;
		SD_write[j+1] = (delay_sig[i]>>8 ) & 0x0ff;
		SD_write[j+2] = (delay_sig[i]>>16) & 0x0ff;
		SD_write[j+3] = (delay_sig[i]>>24) & 0x0ff;
		j = j+4;
	}
	if(SD_status == 1)
	{
		SD_WriteData(SectorCounter,SD_write);//将时延息写入SD扇区内
		SectorCounter = SectorCounter + 1;
	}
}


// 单载波通信；传输测量得到的时延；SD卡数据存储。
void Transmit_IRF_delay()
{
	short i;
	XDCZ_CIF_count = 0;  //相对测阵次数计数器
	//SectorCounter = SectorCounter + 1;

	SD_write[48]=1;  SD_write[49]=1;
	SD_delay_W( delay_sig1);

	SD_write[48]=2;  SD_write[49]=2;
	SD_delay_W( delay_sig2);

	SD_write[48]=3;  SD_write[49]=3;
	SD_delay_W( delay_sig3);

	SD_write[48]=4;  SD_write[49]=4;
	SD_delay_W( delay_sig4);

	for(i=0;i<3;i++)
	{
		Senddata0();
		timer1_jishi(200000);
	}

	PLL_config(&PLLconfig_Tx);	//切换到高速时钟，准备处理通信信号
	TIMER_RSET(PRD1,0x00c7u);	//修改定时器1的中断周期，准备通信信号的发送
	Produce_LFM();//需要在960kHz采样率下发送
	//Produce_long_LFM();//需要在960kHz采样率下发送
	Produce_CW();//需要在960kHz采样率下发送
	IRQ_enable(IRQ_EVT_DMAC1);
	IRQ_enable(IRQ_EVT_DMAC3);
	IRQ_enable(IRQ_EVT_TINT1);

	//-----------------------------------------------------------------
	//Input_bits_change(delay_sig1 , JCXBID1);					//数组转换
	Create_signal(Input_bits, 1350, Pwm_RLE, &len_Pwm_Mod);		//信号调制
	Send_LFM_CW_Tx(3 , 60);										//重复发3次,间隔60s
	//-----------------------------------------------------------------
	//Input_bits_change(delay_sig2 , JCXBID2);					//数组转换
	Create_signal(Input_bits, 1350, Pwm_RLE, &len_Pwm_Mod);		//信号调制
	Send_LFM_CW_Tx(3 , 60);										//重复发3次,间隔60s
	//-----------------------------------------------------------------
	//Input_bits_change(delay_sig3 , JCXBID3);					//数组转换
	Create_signal(Input_bits, 1350, Pwm_RLE, &len_Pwm_Mod);		//信号调制
	Send_LFM_CW_Tx(3 , 60);										//重复发3次,间隔60s
	//-----------------------------------------------------------------
	//Input_bits_change(delay_sig4 , JCXBID4);					//数组转换
	Create_signal(Input_bits, 1350, Pwm_RLE, &len_Pwm_Mod);		//信号调制
	Send_LFM_CW_Tx(3 , 60);										//重复发3次,间隔60s
	//-----------------------------------------------------------------
	PLL_config(&PLLconfig);	//切换回低速时钟
	TIMER_RSET(PRD1,0x0063u);	//改回定时器1的中断周期

	IRQ_disable(IRQ_EVT_DMAC1);
	IRQ_disable(IRQ_EVT_DMAC3);
	IRQ_disable(IRQ_EVT_TINT1);


	main_node = 3;
	CIF_enable= 1;
	IIF_enable= 1;
}



#endif /* SUBFUNCTION_H_ */
