/***************
//����ϵͳ���б���
****************/
#ifndef _VARIABLE_H
#define _VARIABLE_H

//��������
#define NFFT      2048  //FFT����
#define fs        40   //������40kHz
#define PI        3.1415926
#define NFFT_mingling 256

//��ⲿ�ֶ���
#define SYNC_TIME_STD    400                  //�����źų���
#define RevDatReadLen    NFFT-SYNC_TIME_STD   //����ͬ���źŵ���
#define BUF_LEN_MAX      RevDatReadLen        //���ջ���������
#define Rev_resave       SYNC_TIME_STD        //�������ݳ���
#define N_LPF1     33   //��������ͨ�˲�������
#define N_LPF2     33   //խ���˲������ͨ�˲�������
#define N_resample 41  //������ÿ�����

//�����ر���
#define Maikuan_l 5  //��ط���������
#define Maikuan_h 18 //��ط���������
#define SNR 3        //�ֲ�����Ȳ���

volatile short NOISE_TH_huadong;//������������
	     short noise_maxbuffer[2] = {0,0};
volatile short NOISE_TH_NEW;    //��������
volatile float NOISE_TH_HD;
volatile Uint32 Time_Delay; //ת���ӳ�
volatile Uint32 responder_Delay;//�ű�ģʽ��ת���ӳ�
volatile Uint32 responder_period;//�ű�ģʽ��ͬ������

#define Pressure_Delay 300000  //�ϴ�ѹ����Ϣ�̶��ӳٵ�λus

volatile short DMA_count;  //DMA�������ݼ�����

volatile int Pos_flag;   //��⵽�źű�־λ
volatile short endpos;   //�ź���Rev_envelope�г���λ��
volatile short sum_mean_save;//�ź���ؾ�ֵ
volatile short width_save;   //��ؿ��
volatile short endpos_xcorr; //�ź���Rev_envelope�г���λ��
float endpos_float;
volatile float SNR_HD_out;

short block2;
volatile short testpinlv_flag;
volatile short count_final_jiance;
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
//short templfm;
float templfm;

short RAMSES_code1[8] = {8,3,4,1,7,6,2,5};//RAMSES01
short RAMSES_time1[8] = {50,50,50,50,50,50,50,50};   

short RAMSES_code[8] = {3,4,7,5,2,8,1,6}; //RAMSES07
short RAMSES_freq[8] = {9640,10540,11250,12030,12850,13600,14350,15150};
short RAMSES_time[8] = {1250,1250,1250,1250,1250,1250,1250,1250}; 

#pragma DATA_SECTION(Send_sig,"send")
short Send_sig[10000];//�����ź�

//�ο��źź��˲���ϵ��
#pragma DATA_SECTION(X_ref,".ref")
short X_ref[2*NFFT];//�ο��ź�
extern short h_LPF1[N_LPF1];//��ͨ�˲���ϵ��1;��ذ�������˲���
extern short h_LPF2[N_LPF2];//��ͨ�˲���ϵ��2

#pragma DATA_SECTION(h_BPF1,"filter")
short h_BPF1[2*NFFT];//խ���˲���ϵ��1
#pragma DATA_SECTION(h_BPF2,"filter")
short h_BPF2[2*NFFT];//խ���˲���ϵ��2 

Uint16 count_timer;
Uint16 responder_flag;
//�˲�����������
short h_LPF1_db[N_LPF1+2];
short h_LPF2_db[N_LPF2+2];

//�˲��������־λ����
short oflag_LPF1;
short oflag_LPF2;
short oflag_BPF1;
short oflag_BPF2;

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
short Rev_envelope[4096]; //�뱾����������������

#pragma DATA_ALIGN(Rev_bp1,2)
#pragma DATA_SECTION(Rev_bp1,"bp1")
short Rev_bp1[NFFT];            //խ���˲���1���

#pragma DATA_ALIGN(Rev_bp2,2)
#pragma DATA_SECTION(Rev_bp2,"bp2")
short Rev_bp2[NFFT];            //խ���˲���2���

#pragma DATA_ALIGN(Rev_bp1_abs,2)
#pragma DATA_SECTION(Rev_bp1_abs,"bp1abs")
short Rev_bp1_abs[NFFT];        //խ���˲���1�������ֵ

#pragma DATA_ALIGN(Rev_bp2_abs,2)
#pragma DATA_SECTION(Rev_bp2_abs,"bp2abs")
short Rev_bp2_abs[NFFT];        //խ���˲���2�������ֵ

#pragma DATA_SECTION(Rev_bp1_envelope,"bp1envelope")
short Rev_bp1_envelope[NFFT];   //խ���˲���1�������

#pragma DATA_SECTION(Rev_bp2_envelope,"bp2envelope")
short Rev_bp2_envelope[NFFT];   //խ���˲���2�������

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

//9e3 13bit
const short filter1[257] = {
 95, -283, -194, 243, 291, -171, -375, 65, 437, 72, -463, -234, 442, 409, -361, -581, 214, 728, 0, -822, -273, 838, 587, -750, -911, 542, 1205, -210, -1422, -235, 1513, 763, -1436, -1324, 1160, 1853, -676, -2273, 0, 2508, 823, -2488, -1719, 2164, 2589, -1518, -3320, 570, 3796, 619, -3914, -1943, 3599, 3267, -2819, -4431, 1593, 5272, 0, -5643, -1825, 5435, 3701, -4593, -5417, 3132, 6756, -1143, -7518, -1209, 7549, 3699, -6764, -6063, 5165, 8020, -2848, -9313, 0, 9736, 3112, -9162, -6168, 7569, 8830, -5049, -10774, 1804, 11735, 1867, -11540, -5597, 10130, 8988, -7581, -11656, 4098, 13273, 0, -13611, -4309, 12568, 8383, -10192, -11780, 6675, 14114, -2342, -15099, -2381, 14586, 7013, -12581, -11067, 9254, 14105, -4917, -15789, 0, 15918, 4998, -14454, -9560, 11527, 13212, -7425, -15569, 2563, 16384, 2563, -15569, -7425, 13212, 11527, -9560, -14454, 4998, 15918, 0, -15789, -4917, 14105, 9254, -11067, -12581, 7013, 14586, -2381, -15099, -2342, 14114, 6675, -11780, -10192, 8383, 12568, -4309, -13611, 0, 13273, 4098, -11656, -7581, 8988, 10130, -5597, -11540, 1867, 11735, 1804, -10774, -5049, 8830, 7569, -6168, -9162, 3112, 9736, 0, -9313, -2848, 8020, 5165, -6063, -6764, 3699, 7549, -1209, -7518, -1143, 6756, 3132, -5417, -4593, 3701, 5435, -1825, -5643, 0, 5272, 1593, -4431, -2819, 3267, 3599, -1943, -3914, 619, 3796, 570, -3320, -1518, 2589, 2164, -1719, -2488, 823, 2508, 0, -2273, -676, 1853, 1160, -1324, -1436, 763, 1513, -235, -1422, -210, 1205, 542, -911, -750, 587, 838, -273, -822, 0, 728, 214, -581, -361, 409, 442, -234, -463, 72, 437, 65, -375, -171, 291, 243, -194, -283, 95};
//12.5e3
const short filter2[257] = {
307, -122, -234, 318, 0, -347, 279, 159, -437, 176, 344, -476, 0, 535, -434, -250, 693, -282, -554, 769, 0, -869, 706, 406, -1126, 457, 896, -1240, 0, 1390, -1125, -643, 1775, -717, -1396, 1921, 0, -2127, 1710, 972, -2664, 1069, 2068, -2828, 0, 3089, -2468, -1394, 3796, -1513, -2910, 3955, 0, -4269, 3391, 1903, -5154, 2043, 3906, -5279, 0, 5635, -4452, -2486, 6696, -2640, -5023, 6753, 0, -7139, 5612, 3118, -8361, 3281, 6214, -8316, 0, 8712, -6819, -3772, 10070, -3935, -7421, 9890, 0, -10275, 8010, 4413, -11735, 4568, 8580, -11390, 0, 11744, -9120, -5006, 13262, -5143, -9625, 12731, 0, -13032, 10085, 5516, -14561, 5627, 10493, -13832, 0, 14062, -10844, -5911, 15552, -5989, -11132, 14625, 0, -14769, 11352, 6168, -16172, 6208, 11501, -15060, 0, 15109, -11576, -6269, 16384, -6269, -11576, 15109, 0, -15060, 11501, 6208, -16172, 6168, 11352, -14769, 0, 14625, -11132, -5989, 15552, -5911, -10844, 14062, 0, -13832, 10493, 5627, -14561, 5516, 10085, -13032, 0, 12731, -9625, -5143, 13262, -5006, -9120, 11744, 0, -11390, 8580, 4568, -11735, 4413, 8010, -10275, 0, 9890, -7421, -3935, 10070, -3772, -6819, 8712, 0, -8316, 6214, 3281, -8361, 3118, 5612, -7139, 0, 6753, -5023, -2640, 6696, -2486, -4452, 5635, 0, -5279, 3906, 2043, -5154, 1903, 3391, -4269, 0, 3955, -2910, -1513, 3796, -1394, -2468, 3089, 0, -2828, 2068, 1069, -2664, 972, 1710, -2127, 0, 1921, -1396, -717, 1775, -643, -1125, 1390, 0, -1240, 896, 457, -1126, 406, 706, -869, 0, 769, -554, -282, 693, -250, -434, 535, 0, -476, 344, 176, -437, 159, 279, -347, 0, 318, -234, -122, 307};

short PC_DATA[12];//����PC����
short Send_PC_DATA[28];//����PC����

short DS18B20_flag;
short serial[8];   // �����кź����е�����
short serial_x[8]; // ��ȡ�����кŴ�ŵ�����
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
volatile short FLAG_RS232_REVOK;//���ڽ��������

volatile short FLAGSENDOVER;    //�����źŽ�����־λ
volatile Uint32 resend_delay;   //ת���ӳټ�����
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
volatile short tongbu_flag;//ͬ�������־λ
volatile short jiance_counter;//����źż�����

#endif

