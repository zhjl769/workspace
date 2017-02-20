#ifdef _FUNCTION_H
#define _FUNCTION_H

	void system_config();//ϵͳ���ú���
	void pressure_init();//ѹ����������ʼ��
	void get_pressure();//��ȡѹ����Ϣ
	void get_voltage();//��ȡ��ѹ��Ϣ
	void get_temperature();//��ȡ��ѹ��Ϣ
	void Init_DS18B20();//��ʼ��DS18B20
	short Read(void);   //���ֽ�
	void Write(unsigned char cmd);//д�ֽ�
	void read_serial(void);//�����к�
	void send_serial();//�������к�
	void get_noise();//��ȡ��·����
	void variable_init();//������ʼ��
	extern void Delay_diff_1us(volatile Uint32 *delay);//���û���ӳ��Ӻ���
	void Revprepare();//���������ص�������AD���ݷ��Ż�
	void FFTprepare(short *data, short *data_bu0, short data_N);//FFT׼������
	void FFTprepare2(short *data, short *data_bu0, short N_FFT);
	void FFTprepare_FSK(short *data, short *data_bu0, short data_N);//FFT׼������
	void qumo2(short *In, short *Out, short N);
	void dot(short* A,short* B,short* C,short len);//����Ӻ���
	void dot1(short* A,short* B,short* C,short len);//����Ӻ���
	void qujunzhi1(short *datain, short length);//ȥ��ֵ�Ӻ���
	void qujunzhi2(short *datain, short length);//ȥ��ֵ�Ӻ���
	void detect(short *envelopein, short len,volatile int *flag,volatile short *endpos);//��⺯��
	void asksignal();//���ѯ���ź�

	void FSKdetect();//���FSKָ��
	void Senddata0();//�����ź�0

	void detect_prepare();//���׼������
	void detect_prepare_FSK();//���׼������
	void take_action1();//ִ�ж���1
	void take_action2();//ִ�ж���2
	void take_action3();//ִ�ж���3
	void get_parameter();//��ȡ������Ϣ
	void uart_config();//uart��������
	void detect_signal();//����ź�
	void receive_752_init();//���ڳ�ʼ������
	void System_initial();//ϵͳ��ʼ��
	void McBSP_init();//McBSP��ʼ��
	void Produceref();//�����ο��ź�
	void Producefilter();   /*�����˲���ϵ��*/
	void Producelfm();//���������ź�
	void InitADC();//AD��ʼ��
	void Syntimermod();//ͬ���ű꺯��
	void Respondermod();//�紥�����ܺ���
	
	interrupt void myhDma_isr(void);//DMA���������ж�
	interrupt void dma0_Isr(void);//�����ź�0�ж�

	interrupt void dma1_Isr(void);//�����ź�1�ж�
	interrupt void timer1_Isr(void);//��ʱ��1�ж�
	interrupt void timer0_Isr(void);//��ʱ��1�ж�
	interrupt void int1_Isr(void);//���ڽ����ж�
	interrupt void no_isr(void);//���жϺ���

#endif




