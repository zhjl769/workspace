#ifdef _FUNCTION_H
#define _FUNCTION_H

	void system_config();//系统配置函数
	void pressure_init();//压力传感器初始化
	void get_pressure();//获取压力信息
	void get_voltage();//获取电压信息
	void get_temperature();//获取电压信息
	void Init_DS18B20();//初始化DS18B20
	short Read(void);   //读字节
	void Write(unsigned char cmd);//写字节
	void read_serial(void);//读序列号
	void send_serial();//发送序列号
	void get_noise();//获取电路噪声
	void variable_init();//变量初始化
	extern void Delay_diff_1us(volatile Uint32 *delay);//调用汇编延迟子函数
	void Revprepare();//接收数据重叠保留，AD数据符号化
	void FFTprepare(short *data, short *data_bu0, short data_N);//FFT准备函数
	void FFTprepare2(short *data, short *data_bu0, short N_FFT);
	void FFTprepare_FSK(short *data, short *data_bu0, short data_N);//FFT准备函数
	void qumo2(short *In, short *Out, short N);
	void dot(short* A,short* B,short* C,short len);//点乘子函数
	void dot1(short* A,short* B,short* C,short len);//点乘子函数
	void qujunzhi1(short *datain, short length);//去均值子函数
	void qujunzhi2(short *datain, short length);//去均值子函数
	void detect(short *envelopein, short len,volatile int *flag,volatile short *endpos);//检测函数
	void asksignal();//检测询问信号

	void FSKdetect();//检测FSK指令
	void Senddata0();//发射信号0

	void detect_prepare();//检测准备函数
	void detect_prepare_FSK();//检测准备函数
	void take_action1();//执行动作1
	void take_action2();//执行动作2
	void take_action3();//执行动作3
	void get_parameter();//获取参数信息
	void uart_config();//uart数据配置
	void detect_signal();//检测信号
	void receive_752_init();//串口初始化函数
	void System_initial();//系统初始化
	void McBSP_init();//McBSP初始化
	void Produceref();//产生参考信号
	void Producefilter();   /*产生滤波器系数*/
	void Producelfm();//产生发射信号
	void InitADC();//AD初始化
	void Syntimermod();//同步信标函数
	void Respondermod();//电触发功能函数
	
	interrupt void myhDma_isr(void);//DMA接收数据中断
	interrupt void dma0_Isr(void);//发射信号0中断

	interrupt void dma1_Isr(void);//发射信号1中断
	interrupt void timer1_Isr(void);//定时器1中断
	interrupt void timer0_Isr(void);//定时器1中断
	interrupt void int1_Isr(void);//串口接收中断
	interrupt void no_isr(void);//无中断函数

#endif




