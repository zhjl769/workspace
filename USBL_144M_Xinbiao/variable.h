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

#define SYNC_TIME_STD    1000                   //�����źų���

#define RevDatReadLen    NFFT-SYNC_TIME_STD     //����ͬ���źŵ���
#define BUF_LEN_MAX      RevDatReadLen         //���ջ���������
#define Rev_resave       SYNC_TIME_STD        //�������ݳ���

#define N_LPF1    33   //��������ͨ�˲�������
#define N_LPF2    33   //խ���˲������ͨ�˲�������

#define N_resample 27 //������ÿ�����

//�����ر���
#define Maikuan_l 4  //��ط���������
#define Maikuan_h 16 //��ط���������
#define SNR 3        //�ֲ�����Ȳ���
volatile short NOISE_TH_NEW;  //��������
volatile float NOISE_TH_HD;
volatile Uint32 Time_Delay;//ת���ӳ�
volatile Uint32 responder_Delay;//�ű�ģʽ��ת���ӳ�

volatile Uint32 responder_period;//�ű�ģʽ��ͬ������



#define Pressure_Delay 300000  //�ϴ�ѹ����Ϣ�̶��ӳٵ�λus

volatile int Pos_flag;   //��⵽�źű�־λ
volatile short endpos;   //�ź���Rev_envelope�г���λ��
volatile short sum_mean_save;//�ź���ؾ�ֵ
volatile short width_save;//��ؿ��
volatile short endpos_xcorr;   //�ź���Rev_envelope�г���λ��
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


short code;           //Ƶ����
//SDRAM�����źű���
volatile unsigned long SDRAM_lfmhead;//SDRAM�洢ͷ
short templfm;
#pragma DATA_SECTION(Send_sig,"send")
short Send_sig[20000];//�����ź�


//�ο��źź��˲���ϵ��
#pragma DATA_SECTION(X_ref,".ref")
short X_ref[2*NFFT];//�ο��ź�
extern short h_LPF1[N_LPF1];//��ͨ�˲���ϵ��1
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
short Rev_envelope[NFFT]; //�뱾����������������

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

#pragma DATA_SECTION(temp_dainei_temp,"revsave")
short temp_dainei_temp[NFFT_mingling];

#pragma DATA_SECTION(temp_dainei_temp_fft,"revsave")
short temp_dainei_temp_fft[2*NFFT_mingling];

#pragma DATA_SECTION(temp_dainei_temp_fft_abs,"revsave")
short temp_dainei_temp_fft_abs[NFFT_mingling];

const short filter1[257] = {//9e3
24,-71,-49,61,73,-43,-94,16,109,18,-116,-58,110,102,-90,-145,54,182,0,-206,-68,209,147,-188,-228,136,301,-53,-355,-59,378,191,-359,-331,290,463,-169,-568,0,627,206,-622,-430,541,647,-380,-830,142,949,155,-979,-486,900,817,-705,-1108,398,1318,0,-1411,-456,1359,925,-1148,-1354,783,1689,-286,-1879,-302,1887,925,-1691,-1516,1291,2005,-712,-2328,0,2434,778,-2290,-1542,1892,2207,-1262,-2693,451,2934,467,-2885,-1399,2533,2247,-1895,-2914,1025,3318,0,-3403,-1077,3142,2096,-2548,-2945,1669,3528,-586,-3775,-595,3646,1753,-3145,-2767,2313,3526,-1229,-3947,0,3980,1249,-3613,-2390,2882,3303,-1856,-3892,641,4096,641,-3892,-1856,3303,2882,-2390,-3613,1249,3980,0,-3947,-1229,3526,2313,-2767,-3145,1753,3646,-595,-3775,-586,3528,1669,-2945,-2548,2096,3142,-1077,-3403,0,3318,1025,-2914,-1895,2247,2533,-1399,-2885,467,2934,451,-2693,-1262,2207,1892,-1542,-2290,778,2434,0,-2328,-712,2005,1291,-1516,-1691,925,1887,-302,-1879,-286,1689,783,-1354,-1148,925,1359,-456,-1411,0,1318,398,-1108,-705,817,900,-486,-979,155,949,142,-830,-380,647,541,-430,-622,206,627,0,-568,-169,463,290,-331,-359,191,378,-59,-355,-53,301,136,-228,-188,147,209,-68,-206,0,182,54,-145,-90,102,110,-58,-116,18,109,16,-94,-43,73,61,-49,-71,24};

const short filter2[257] = {//12.5e3
77,-30,-58,79,0,-87,70,40,-109,44,86,-119,0,134,-109,-62,173,-70,-138,192,0,-217,177,101,-282,114,224,-310,0,347,-281,-161,444,-179,-349,480,0,-532,428,243,-666,267,517,-707,0,772,-617,-348,949,-378,-728,989,0,-1067,848,476,-1288,511,977,-1320,0,1409,-1113,-621,1674,-660,-1256,1688,0,-1785,1403,780,-2090,820,1553,-2079,0,2178,-1705,-943,2518,-984,-1855,2472,0,-2569,2003,1103,-2934,1142,2145,-2848,0,2936,-2280,-1252,3316,-1286,-2406,3183,0,-3258,2521,1379,-3640,1407,2623,-3458,0,3515,-2711,-1478,3888,-1497,-2783,3656,0,-3692,2838,1542,-4043,1552,2875,-3765,0,3777,-2894,-1567,4096,-1567,-2894,3777,0,-3765,2875,1552,-4043,1542,2838,-3692,0,3656,-2783,-1497,3888,-1478,-2711,3515,0,-3458,2623,1407,-3640,1379,2521,-3258,0,3183,-2406,-1286,3316,-1252,-2280,2936,0,-2848,2145,1142,-2934,1103,2003,-2569,0,2472,-1855,-984,2518,-943,-1705,2178,0,-2079,1553,820,-2090,780,1403,-1785,0,1688,-1256,-660,1674,-621,-1113,1409,0,-1320,977,511,-1288,476,848,-1067,0,989,-728,-378,949,-348,-617,772,0,-707,517,267,-666,243,428,-532,0,480,-349,-179,444,-161,-281,347,0,-310,224,114,-282,101,177,-217,0,192,-138,-70,173,-62,-109,134,0,-119,86,44,-109,40,70,-87,0,79,-58,-30,77};


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
volatile short transponder_mode1;//ģʽ�л���־λ
volatile short tongbu_flag;//ͬ�������־λ
volatile short jiance_counter;//����źż�����

#endif

