/***************
//����ϵͳ���б���
 ****************/
#ifndef _VARIABLE_H
#define _VARIABLE_H

//��������
#define NFFT      4096  //FFT����
#define fs        100   //������100kHz
#define PI        3.14159265
#define NFFT_mingling 256

//�����źŲ��ֱ�������
#define Time_Sig      20  //�����ź�����,��λ��ms
#define f1_Sig        21  //�����ź�����Ƶ��,��λ��kHz
#define f2_Sig        33  //�����ź�����Ƶ��,��λ��kHz
#define Delay_Sig     5   //���䵥Ԫ��û�м�⵽ѯ���źŵĹر�ʱ��,Ϊ0��ʾ�رմ˹��ܣ���λ��min

//ѯ���źŲ��ֱ�������
#define Time_Ref      20  //ѯ���ź�����,��λ��ms
#define f1_Ref        30  //ѯ���ź�����Ƶ��,��λ��kHz
#define f2_Ref        22  //ѯ���ź�����Ƶ��,��λ��kHz

//��ⲿ�ֶ���
#define SYNC_TIME_STD    Time_Ref*fs           //�����źų���
#define RevDatReadLen    (NFFT-SYNC_TIME_STD)   //����ͬ���źŵ���
#define BUF_LEN_MAX      RevDatReadLen        //���ջ���������
#define Rev_resave       SYNC_TIME_STD        //�������ݳ���
#define Theory_wide      14               //���������С��fs/B,BΪ�źŴ���,�ڼ���ֵ�������о�����
#define N_LPF1     65   //��������ͨ�˲�������
#define N_LPF2     33   //խ���˲������ͨ�˲�������
#define N_resample 41  //������ÿ�����

//�����ر���
#define Maikuan_l 5  //��ط���������
#define Maikuan_h 65 //��ط��������ޣ�ȡֵΪ���������3��
//#define SNR 3        //�ֲ�����Ȳ���
#define Weifeng_qian 5  //��α������ֵ
#define Weifeng_hou 250  //��α������ֵ

volatile short NOISE_TH_huadong;//������������
short noise_maxbuffer[2] = {0,0};
volatile short NOISE_TH_NEW;    //��������
volatile float NOISE_TH_HD;
volatile Uint32 Time_Delay; //ת���ӳ�
volatile Uint32 responder_Delay;//�ű�ģʽ��ת���ӳ�
volatile Uint32 Tr_time;//�ű�ģʽ�¹رշ��䲿�ֵ�Դ�ļ���
volatile Uint32 responder_period;//�ű�ģʽ��ͬ������

#define Pressure_Delay 300000  //�ϴ�ѹ����Ϣ�̶��ӳٵ�λus

volatile short DMA_count;  //DMA�������ݼ�����

volatile int Pos_flag;   //��⵽�źű�־λ
volatile short endpos;   //�ź���Rev_envelope�г���λ��
volatile short sum_mean_save;//�ź���ؾ�ֵ
volatile short width_save;   //��ؿ��
//volatile short endpos_xcorr; //�ź���Rev_envelope�г���λ��
float endpos_float;
volatile float SNR_HD_out;
float Max_pos_float;

short block2;
volatile short testpinlv_flag;
#pragma DATA_SECTION(count_final_jiance,"send")  
short count_final_jiance[12]; //�ͷ��źŻ���ռ�

short testpinlv_max;
short testpinlv_ind;
short x1,x2,x3;
short y1,y2,y3;
float a,b;
float x_max;
float f_code;
short code;   //Ƶ����

//SDRAM�����źű���
volatile unsigned long SDRAM_lfmhead;//SDRAM�洢ͷ
short templfm;


/***********************************************************

short RAMSES_code1[8] = {8,3,4,1,7,6,2,5};//RAMSES01
short RAMSES_time1[8] = {50,50,50,50,50,50,50,50};   

short RAMSES_code[8] = {6,8,3,7,2,1,5,4}; //RAMSES15
short RAMSES_freq[8] = {9640,10540,11250,12030,12850,13600,14350,15150};
short RAMSES_time[8] = {1250,1250,1250,1250,1250,1250,1250,1250}; 

 **********************************************************/

#pragma DATA_SECTION(Send_sig,"send")
short Send_sig[Time_Sig*1000];  //�����źŵ�����С

//�ο��źź��˲���ϵ��
#pragma DATA_SECTION(X_ref,".ref")
short X_ref[2*NFFT];//�ο��ź�
extern short h_LPF1[N_LPF1];//��ͨ�˲���ϵ��1;��ذ�������˲���
extern short h_LPF2[N_LPF2];//��ͨ�˲���ϵ��2
Uint16 count_timer;
Uint16 responder_flag;
//�˲�����������
short h_LPF1_db[N_LPF1+2];
short h_LPF2_db[N_LPF2+2];

//�˲��������־λ����
short oflag_LPF1;
short oflag_LPF2;

//���ڴ������ݶ���
short PC_init[28];
short buff;
short f_send_uart;
const short init_rs485[4] = {0x0fa,0x30,0x04,0x43};//ѹ����������ʼ������
const short ask_rs485[5] = {0x0fa,0x4a,0x01,0x51,0x0a7};
//const short ask_rs485[5] = {0x0fa,0x4a,0x01,0x51,0x43};
volatile short FLAG_yl_init;//ѹ����������ʼ����־λ
volatile short FLAG_yl;//ѹ����������ȡ��־λ
volatile short release_flag;

//��������
//�ɼ���������
#pragma DATA_SECTION(Rev1,"receive1")
short Rev1[BUF_LEN_MAX];  //�ɼ�����ƹ�һ�����1

#pragma DATA_SECTION(Rev2,"receive2")
short Rev2[BUF_LEN_MAX];  //ÿ�βɼ�����ƹ�һ�����2

#pragma DATA_SECTION(Rev,"NFFT")
short Rev[NFFT];      //�ص�����ԭʼ����

#pragma DATA_SECTION(Rev_FFT,"FFT")
short Rev_FFT[2*NFFT];     //FFT���ݻ���

#pragma DATA_SECTION(Rev_xcorr,"xcorr")
short Rev_xcorr[2*NFFT];   //��ؽ������

#pragma DATA_SECTION(Rev_xcorrout,"xcorrout")
short Rev_xcorrout[NFFT]; //�뱾������������

#pragma DATA_ALIGN(Rev_envelope,2)
#pragma DATA_SECTION(Rev_envelope,"envelope")
short Rev_envelope[5500]; //�뱾����������������

/***************************************
#pragma DATA_ALIGN(Rev_bp1,2)
#pragma DATA_SECTION(Rev_bp1,"bp1")
 ****************************************/
#pragma DATA_SECTION(Rev_save,"revsave")
short Rev_save[NFFT];

#pragma DATA_SECTION(Rev_save_filter_out,"revsave")
short Rev_save_filter_out[NFFT];

#pragma DATA_SECTION(temp_dainei_temp,"revsave")
short temp_dainei_temp[NFFT_mingling];

#pragma DATA_SECTION(temp_dainei_temp_fft,"revsave")
short temp_dainei_temp_fft[2*NFFT_mingling];

#pragma DATA_SECTION(temp_dainei_temp_fft_abs,"revsave")
short temp_dainei_temp_fft_abs[NFFT_mingling];

//9.5k
const short filter1[257] = {
		-248, 166, 294, -132, -342, 88, 390, -33, -437, -36, 481, 120, -519, -222, 547, 341, -561, -478, 554, 633, -520, -802, 454, 980, -348, -1162, 198, 1338, 0, -1500, -249, 1635, 549, -1730, -896, 1773, 1287, -1750, -1710, 1649, 2155, -1459, -2606, 1171, 3044, -781, -3448, 286, 3796, 310, -4065, -999, 4231, 1768, -4273, -2598, 4170, 3467, -3906, -4345, 3471, 5201, -2858, -6001, 2069, 6708, -1111, -7287, 0, 7703, 1242, -7923, -2584, 7921, 3989, -7675, -5417, 7170, 6819, -6402, -8147, 5373, 9350, -4097, -10380, 2596, 11189, -905, -11735, -936, 11984, 2878, -11909, -4864, 11492, 6835, -10729, -8728, 9625, 10479, -8197, -12027, 6475, 13316, -4499, -14297, 2321, 14926, 0, -15173, -2399, 15020, 4806, -14459, -7147, 13498, 9352, -12156, -11352, 10467, 13084, -8476, -14492, 6238, 15532, -3818, -16169, 1285, 16384, 1285, -16169, -3818, 15532, 6238, -14492, -8476, 13084, 10467, -11352, -12156, 9352, 13498, -7147, -14459, 4806, 15020, -2399, -15173, 0, 14926, 2321, -14297, -4499, 13316, 6475, -12027, -8197, 10479, 9625, -8728, -10729, 6835, 11492, -4864, -11909, 2878, 11984, -936, -11735, -905, 11189, 2596, -10380, -4097, 9350, 5373, -8147, -6402, 6819, 7170, -5417, -7675, 3989, 7921, -2584, -7923, 1242, 7703, 0, -7287, -1111, 6708, 2069, -6001, -2858, 5201, 3471, -4345, -3906, 3467, 4170, -2598, -4273, 1768, 4231, -999, -4065, 310, 3796, 286, -3448, -781, 3044, 1171, -2606, -1459, 2155, 1649, -1710, -1750, 1287, 1773, -896, -1730, 549, 1635, -249, -1500, 0, 1338, 198, -1162, -348, 980, 454, -802, -520, 633, 554, -478, -561, 341, 547, -222, -519, 120, 481, -36, -437, -33, 390, 88, -342, -132, 294, 166, -248};
//13k
const short filter2[257] = {
		-248, -50, 314, -243, -111, 371, -232, -188, 437, -209, -286, 509, -169, -409, 584, -102, -561, 656, 0, -742, 716, 147, -950, 750, 348, -1180, 745, 609, -1422, 683, 935, -1660, 549, 1324, -1878, 325, 1771, -2051, 0, 2263, -2155, -437, 2781, -2164, -989, 3303, -2052, -1653, 3796, -1795, -2419, 4228, -1375, -3267, 4561, -778, -4170, 4756, 0, -5091, 4777, 954, -5988, 4593, 2069, -6814, 4175, 3319, -7518, 3508, 4665, -8048, 2584, 6063, -8357, 1408, 7455, -8402, 0, 8783, -8147, -1609, 9981, -7569, -3373, 10985, -6659, -5236, 11735, -5419, -7132, 12177, -3869, -8988, 12267, -2046, -10729, 11974, 0, -12278, 11282, 2207, -13564, 10192, 4499, -14522, 8723, 6797, -15099, 6910, 9015, -15257, 4806, 11067, -14973, 2476, 12873, -14244, 0, 14360, -13084, -2538, 15469, -11527, -5047, 16153, -9622, -7437, 16384, -7437, -9622, 16153, -5047, -11527, 15469, -2538, -13084, 14360, 0, -14244, 12873, 2476, -14973, 11067, 4806, -15257, 9015, 6910, -15099, 6797, 8723, -14522, 4499, 10192, -13564, 2207, 11282, -12278, 0, 11974, -10729, -2046, 12267, -8988, -3869, 12177, -7132, -5419, 11735, -5236, -6659, 10985, -3373, -7569, 9981, -1609, -8147, 8783, 0, -8402, 7455, 1408, -8357, 6063, 2584, -8048, 4665, 3508, -7518, 3319, 4175, -6814, 2069, 4593, -5988, 954, 4777, -5091, 0, 4756, -4170, -778, 4561, -3267, -1375, 4228, -2419, -1795, 3796, -1653, -2052, 3303, -989, -2164, 2781, -437, -2155, 2263, 0, -2051, 1771, 325, -1878, 1324, 549, -1660, 935, 683, -1422, 609, 745, -1180, 348, 750, -950, 147, 716, -742, 0, 656, -561, -102, 584, -409, -169, 509, -286, -209, 437, -188, -232, 371, -111, -243, 314, -50, -248};

short PC_DATA[12];//����PC����
short Send_PC_DATA[28];//����PC����

short DS18B20_flag;
short serial[8];   // �����кź����е�����
short serial_x[8]; // ��ȡ�����кŴ�ŵ�
float temperature; // ���ص���ʵ�¶�ֵ


//DMA���ñ���
volatile short DMAdone;//DMA���������־
volatile Uint32 DMAnum;//DMA���ռ�����
volatile Uint16 RevsrcAddrHi,RevsrcAddrLo;
volatile Uint16 Revdst1AddrHi,Revdst1AddrLo;
volatile Uint16 Revdst2AddrHi,Revdst2AddrLo;//DMA���յ�ַ
volatile Uint16 sendsrc0AddrHi, sendsrc0AddrLo;
volatile Uint16 senddst0AddrHi, senddst0AddrLo;
volatile Uint16 sendsrc1AddrHi, sendsrc1AddrLo;
volatile Uint16 senddst1AddrHi, senddst1AddrLo;//����DMA��ַ

//������
DMA_Handle myhDma, hDmaSend0,hDmaSend1;
TIMER_Handle mhTimer0,mhTimer1;
MMC_Handle mmc1;
MMC_CardObj *card, cardalloc;
MMC_CardIdObj *cid, cardid;

Uint16 count, retVal;
Uint16 rca;
Uint16 cardtype;
Uint32 SectorCounter;
Uint16 SecCounter[256];//װ��������
Uint16 SD_write[256];//SD��д������
Uint16 SD_read[256];//SD����ȡ����
short SD_test;
short SD_status;

volatile short Flag_timer;
volatile short FLAG_OK;
volatile short Flag_answer;//����źŷ���ֵ
volatile short FLAG_RS232_REVOK;//���ڽ��������?

volatile short FLAGSENDOVER;    //�����źŽ�����־λ
volatile Uint32 resend_delay;   //ת���ӳټ�����
volatile Uint32 resend_delay1;   //ת���ӳټ�����
volatile Uint32 timer0_cnt;      //��ʱ��100ms������
volatile Uint32 timer1_cnt;      //��ʱ��1us������


//ѹ�����������ڲ���ѹ�ɼ�����
short RS485_DATA[12];
Uint32 temp_yl_data;
short temp_ADC;//AD�������
short ADC_in[64];//�ڲ�AD�����������

short voltage_AD; //��Դ��ѹ����ֵ
float depth;//ѹ��������������

short noise_10ms;//�ɼ�10ms��������ֵ
volatile short transponder_mode;//Ӧ��ģʽ
volatile short transponder_mode1;//ģʽ�л���־λ
volatile short tongbu_flag;//ͬ�������־λ
volatile short jiance_counter;//����źż�����

#endif
