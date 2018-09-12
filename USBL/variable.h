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

short RAMSES_code1[8] = {8,3,4,1,7,6,2,5};//RAMSES01 
short RAMSES_freq1[8] = {10490,11390,12100,12880,13700,14450,15200,16000}; 
short RAMSES_time1[8] = {50,50,50,50,50,50,50,50}; 

short RAMSES_code[8] = {6,8,3,7,1,2,5,4}; //RAMSES15
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

//11.5k
const short filter1[257] = {
95 , -317 , 52 , 318 , -211 , -244 , 352 , 97 , -437 , 108 , 434 , -335 , -321 , 535 , 96 , -650 , 214 , 628 , -554 , -435 , 842 , 74 , -987 , 406 , 911 , -909 , -575 , 1305 , 0 , -1463 , 722 , 1278 , -1436 , -717 , 1950 , -163 , -2082 , 1203 , 1710 , -2165 , -823 , 2784 , -457 , -2828 , 1881 , 2172 , -3110 , -850 , 3796 , -923 , -3667 , 2780 , 2615 , -4269 , -750 , 4958 , -1593 , -4551 , 3906 , 2985 , -5616 , -479 , 6219 , -2486 , -5417 , 5246 , 3225 , -7108 , 0 , 7513 , -3603 , -6196 , 6764 , 3281 , -8679 , 706 , 8764 , -4927 , -6819 , 8405 , 3112 , -10251 , 1642 , 9890 , -6415 , -7223 , 10094 , 2692 , -11735 , 2786 , 10811 , -8007 , -7360 , 11744 , 2018 , -13042 , 4098 , 11458 , -9625 , -7200 , 13263 , 1107 , -14086 , 5516 , 11780 , -11180 , -6737 , 14558 , 0 , -14800 , 6963 , 11746 , -12581 , -5989 , 15550 , -1242 , -15133 , 8353 , 11352 , -13742 , -4998 , 16172 , -2544 , -15060 , 9599 , 10621 , -14586 , -3824 , 16384 , -3824 , -14586 , 10621 , 9599 , -15060 , -2544 , 16172 , -4998 , -13742 , 11352 , 8353 , -15133 , -1242 , 15550 , -5989 , -12581 , 11746 , 6963 , -14800 , 0 , 14558 , -6737 , -11180 , 11780 , 5516 , -14086 , 1107 , 13263 , -7200 , -9625 , 11458 , 4098 , -13042 , 2018 , 11744 , -7360 , -8007 , 10811 , 2786 , -11735 , 2692 , 10094 , -7223 , -6415 , 9890 , 1642 , -10251 , 3112 , 8405 , -6819 , -4927 , 8764 , 706 , -8679 , 3281 , 6764 , -6196 , -3603 , 7513 , 0 , -7108 , 3225 , 5246 , -5417 , -2486 , 6219 , -479 , -5616 , 2985 , 3906 , -4551 , -1593 , 4958 , -750 , -4269 , 2615 , 2780 , -3667 , -923 , 3796 , -850 , -3110 , 2172 , 1881 , -2828 , -457 , 2784 , -823 , -2165 , 1710 , 1203 , -2082 , -163 , 1950 , -717 , -1436 , 1278 , 722 , -1463 , 0 , 1305 , -575 , -909 , 911 , 406 , -987 , 74 , 842 , -435 , -554 , 628 , 214 , -650 , 96 , 535 , -321 , -335 , 434 , 108 , -437 , 97 , 352 , -244 , -211 , 318 , 52 , -317 , 95};  
//13.5k
const short filter2[257] = {
95 , 206 , -326 , 132 , 211 , -375 , 179 , 217 , -437 , 241 , 221 , -514 , 321 , 222 , -607 , 424 , 214 , -716 , 554 , 194 , -842 , 715 , 156 , -980 , 911 , 94 , -1129 , 1145 , 0 , -1283 , 1418 , -132 , -1436 , 1730 , -309 , -1581 , 2082 , -537 , -1710 , 2469 , -823 , -1813 , 2888 , -1171 , -1881 , 3333 , -1585 , -1903 , 3796 , -2066 , -1869 , 4268 , -2615 , -1768 , 4736 , -3230 , -1593 , 5190 , -3906 , -1334 , 5616 , -4638 , -985 , 6001 , -5417 , -541 , 6329 , -6233 , 0 , 6588 , -7072 , 639 , 6764 , -7921 , 1375 , 6845 , -8764 , 2201 , 6819 , -9585 , 3112 , 6678 , -10365 , 4097 , 6415 , -11088 , 5143 , 6026 , -11735 , 6236 , 5508 , -12291 , 7360 , 4864 , -12740 , 8496 , 4098 , -13067 , 9625 , 3217 , -13263 , 10726 , 2231 , -13316 , 11780 , 1154 , -13223 , 12766 , 0 , -12978 , 13665 , -1212 , -12581 , 14459 , -2463 , -12038 , 15133 , -3732 , -11352 , 15671 , -4998 , -10535 , 16064 , -6238 , -9599 , 16304 , -7432 , -8559 , 16384 , -8559 , -7432 , 16304 , -9599 , -6238 , 16064 , -10535 , -4998 , 15671 , -11352 , -3732 , 15133 , -12038 , -2463 , 14459 , -12581 , -1212 , 13665 , -12978 , 0 , 12766 , -13223 , 1154 , 11780 , -13316 , 2231 , 10726 , -13263 , 3217 , 9625 , -13067 , 4098 , 8496 , -12740 , 4864 , 7360 , -12291 , 5508 , 6236 , -11735 , 6026 , 5143 , -11088 , 6415 , 4097 , -10365 , 6678 , 3112 , -9585 , 6819 , 2201 , -8764 , 6845 , 1375 , -7921 , 6764 , 639 , -7072 , 6588 , 0 , -6233 , 6329 , -541 , -5417 , 6001 , -985 , -4638 , 5616 , -1334 , -3906 , 5190 , -1593 , -3230 , 4736 , -1768 , -2615 , 4268 , -1869 , -2066 , 3796 , -1903 , -1585 , 3333 , -1881 , -1171 , 2888 , -1813 , -823 , 2469 , -1710 , -537 , 2082 , -1581 , -309 , 1730 , -1436 , -132 , 1418 , -1283 , 0 , 1145 , -1129 , 94 , 911 , -980 , 156 , 715 , -842 , 194 , 554 , -716 , 214 , 424 , -607 , 222 , 321 , -514 , 221 , 241 , -437 , 217 , 179 , -375 , 211 , 132 , -326 , 206 , 95};  

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
volatile short FLAG_RS232_REVOK;//���ڽ��������?

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

