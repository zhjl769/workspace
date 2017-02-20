/***************
//定义系统所有变量
****************/
#ifndef _VARIABLE_H
#define _VARIABLE_H

//变量定义
#define NFFT      2048  //FFT点数
#define fs        40   //采样率40kHz
#define PI        3.1415926
#define NFFT_mingling 256

//检测部分定义
#define SYNC_TIME_STD    400                  //本地信号长度
#define RevDatReadLen    NFFT-SYNC_TIME_STD   //保留同步信号点数
#define BUF_LEN_MAX      RevDatReadLen        //接收缓存区长度
#define Rev_resave       SYNC_TIME_STD        //保留数据长度
#define N_LPF1     33   //相关输出低通滤波器阶数
#define N_LPF2     33   //窄带滤波输出低通滤波器阶数
#define N_resample 41  //降采样每块点数

//检测相关变量
#define Maikuan_l 5  //相关峰脉宽下限
#define Maikuan_h 18 //相关峰脉宽上限
#define SNR 3        //局部信噪比参数

volatile short NOISE_TH_huadong;    //噪声滑动门限
	     short noise_maxbuffer[2] = {0,0};
         short NOISE_TH_huadong_IIF;//噪声滑动门限
         short noise_maxbuffer_IIF[2] = {0,0};

volatile short NOISE_TH_NEW;        //绝对门限
volatile float NOISE_TH_HD;
volatile short NOISE_TH_NEW_IIF;    //绝对门限
volatile float NOISE_TH_HD_IIF;

volatile Uint32 Time_Delay; //转发延迟
volatile Uint32 responder_Delay;//信标模式下转发延迟
volatile Uint32 responder_period;//信标模式下同步周期

#define Pressure_Delay 300000  //上传压力信息固定延迟单位us

volatile short DMA_count;  //DMA搬移数据计数。

volatile int Pos_flag;   //检测到信号标志位
volatile short endpos;   //信号在Rev_envelope中出现位置
volatile short sum_mean_save;//信号相关均值
volatile short width_save;   //相关宽度
volatile short endpos_xcorr; //信号在Rev_envelope中出现位置
float endpos_float;
volatile float SNR_HD_out;


volatile int DMANum;   //DMA搬运计数


short block2;
short cmd_indx={0};
volatile short testpinlv_flag;
         short FSK_cmd_flag[12]={0,0,0,0,0,0,0,0,0,0,0,0};
         short sum_count_FSK_cmd1[12]={0,0,0,0,0,0,0,0,0,0,0,0};

#pragma DATA_SECTION(count_final_jiance,"send") 
short count_final_jiance[12]; //释放信号缓存空间
short count_FSK_cmd0[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
short count_FSK_cmd1[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
short count_FSK_cmd2[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
short count_FSK_cmd3[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
short count_FSK_cmd4[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
short count_FSK_cmd5[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
short count_FSK_cmd6[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
short count_FSK_cmd7[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
short count_FSK_cmd8[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
short count_FSK_cmd9[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
short count_FSK_cmd10[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
short count_FSK_cmd11[12]={0,0,0,0,0,0,0,0,0,0,0,0};     //释放信号缓存空间
 

short testpinlv_max;
short testpinlv_ind;
short x1,x2,x3;
short y1,y2,y3;
float a,b;
float x_max;
float f_code;
short code;   //频率码

//SDRAM产生信号变量
volatile unsigned long SDRAM_lfmhead;//SDRAM存储头
short templfm;

short RAMSES_code1[8] = {8,3,4,1,7,6,2,5};//RAMSES01
short RAMSES_freq1[8] = {10490,11390,12100,12880,13700,14450,15200,16000};
short RAMSES_time1[8] = {50,50,50,50,50,50,50,50};   

short RAMSES_code[8] = {3,4,7,5,2,8,1,6}; //RAMSES07
short RAMSES_freq[8] = {9640,10540,11250,12030,12850,13600,14350,15150};
short RAMSES_time[8] = {1250,1250,1250,1250,1250,1250,1250,1250}; 

short RAMSES_code7[8] = {3,4,7,5,2,8,1,6}; //RAMSES07
short RAMSES_code8[8] = {3,4,7,5,2,8,1,6}; //RAMSES08
short RAMSES_code9[8] = {3,4,7,5,2,8,1,6}; //RAMSES09


#pragma DATA_SECTION(Send_sig,"send")
short Send_sig[10000];//发射信号


//参考信号和滤波器系数
#pragma DATA_SECTION(X_ref,".ref")
short X_ref[2*NFFT];        //参考信号
extern short h_LPF1[N_LPF1];//低通滤波器系数1;相关包络进行滤波。
extern short h_LPF2[N_LPF2];//低通滤波器系数2


#pragma DATA_SECTION(h_BPF1,"filter")
short h_BPF1[2*NFFT];  //窄带滤波器系数1
#pragma DATA_SECTION(h_BPF2,"filter")
short h_BPF2[2*NFFT];  //窄带滤波器系数2 

Uint16 count_timer;
Uint16 responder_flag;
//滤波器缓存数组
short h_LPF1_db[N_LPF1+2];
short h_LPF2_db[N_LPF2+2];

//滤波器溢出标志位定义
short oflag_LPF1;
short oflag_LPF2;
short oflag_BPF1;
short oflag_BPF2;

//串口传输数据定义
short PC_init[28];
short buff;
short f_send_uart;
const short init_rs485[4] = {0x0fa,0x30,0x04,0x43};//压力传感器初始化数据
const short ask_rs485[5]  = {0x0fa,0x4a,0x01,0x51,0x0a7};
//const short ask_rs485[5] = {0x0fa,0x4a,0x01,0x51,0x43};
volatile short FLAG_yl_init;//压力传感器初始化标志位
volatile short FLAG_yl;     //压力传感器读取标志位
volatile short release_flag;

//数组声明
//采集数组声明
#pragma DATA_SECTION(Rev1,"receive1")
short Rev1[BUF_LEN_MAX];  //采集数据乒乓缓存区1

#pragma DATA_SECTION(Rev2,"receive2")
short Rev2[BUF_LEN_MAX];  //每次采集数据乒乓缓存区2

#pragma DATA_SECTION(Rev,"NFFT")
short Rev[NFFT];      //重叠保留原始数据

#pragma DATA_SECTION(Rev_FFT,"FFT")
short Rev_FFT[2*NFFT];     //FFT数据缓存

#pragma DATA_SECTION(Rev_xcorr,"xcorr")
short Rev_xcorr[2*NFFT];   //相关结果缓存

#pragma DATA_SECTION(Rev_xcorrout,"xcorrout")
short Rev_xcorrout[NFFT]; //与本地序列相关输出

#pragma DATA_ALIGN(Rev_envelope,2)
#pragma DATA_SECTION(Rev_envelope,"envelope")
short Rev_envelope[4096]; //与本地序列相关输出包络

//Benthos release signal detection

#pragma DATA_ALIGN(Rev_bp1,2)
#pragma DATA_SECTION(Rev_bp1,"bp1")
short Rev_bp1[NFFT];            //窄带滤波器1输出

#pragma DATA_ALIGN(Rev_bp2,2)
#pragma DATA_SECTION(Rev_bp2,"bp2")
short Rev_bp2[NFFT];            //窄带滤波器2输出

#pragma DATA_ALIGN(Rev_bp1_abs,2)
#pragma DATA_SECTION(Rev_bp1_abs,"bp1abs")
short Rev_bp1_abs[NFFT];        //窄带滤波器1输出绝对值

#pragma DATA_ALIGN(Rev_bp2_abs,2)
#pragma DATA_SECTION(Rev_bp2_abs,"bp2abs")
short Rev_bp2_abs[NFFT];        //窄带滤波器2输出绝对值

#pragma DATA_SECTION(Rev_bp1_envelope,"bp1envelope")
short Rev_bp1_envelope[NFFT];   //窄带滤波器1输出包络

#pragma DATA_SECTION(Rev_bp2_envelope,"bp2envelope")
short Rev_bp2_envelope[NFFT];   //窄带滤波器2输出包络

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

//为IIF/信标2  检测分配空间
#pragma DATA_SECTION(Rev_xcorr_IIF,"revsave")
short Rev_xcorr_IIF[2*NFFT];

#pragma DATA_SECTION(Rev_xcorrout_IIF,"revsave")
short Rev_xcorrout_IIF[NFFT];

#pragma DATA_SECTION(Rev_save_filter_out_IIF,"revsave")
short Rev_save_filter_out_IIF[NFFT];

#pragma DATA_SECTION(Rev_envelope_IIF,"revsave")
short Rev_envelope_IIF[2*NFFT];

//为信标3    检测分配空间
#pragma DATA_SECTION(Rev_xcorr_xb3,"send")
short Rev_xcorr_xb3[2*NFFT];

#pragma DATA_SECTION(Rev_xcorrout_xb3,"send")
short Rev_xcorrout_xb3[NFFT];

#pragma DATA_SECTION(Rev_save_filter_out_xb3,"send")
short Rev_save_filter_out_xb3[NFFT];

#pragma DATA_SECTION(Rev_envelope_xb3,"send")
short Rev_envelope_xb3[2*NFFT];

//为信标4    检测分配空间
#pragma DATA_SECTION(Rev_xcorr_xb4,"send")
short Rev_xcorr_xb4[2*NFFT];

#pragma DATA_SECTION(Rev_xcorrout_xb4,"send")
short Rev_xcorrout_xb4[NFFT];

#pragma DATA_SECTION(Rev_save_filter_out_xb4,"send")
short Rev_save_filter_out_xb4[NFFT];

#pragma DATA_SECTION(Rev_envelope_xb4,"send")
short Rev_envelope_xb4[2*NFFT];


//11e3 13bit
const short filter1[257] = {
95 , 283 , -194 , -243 , 291 , 171 , -375 , -65 , 437 , -72 , -463 , 234 , 442 , -409 , -361 , 581 , 214 , -728 , 0 , 822 , -273 , -838 , 587 , 750 , -911 , -542 , 1205 , 210 , -1422 , 235 , 1513 , -763 , -1436 , 1324 , 1160 , -1853 , -676 , 2273 , 0 , -2508 , 823 , 2488 , -1719 , -2164 , 2589 , 1518 , -3320 , -570 , 3796 , -619 , -3914 , 1943 , 3599 , -3267 , -2819 , 4431 , 1593 , -5272 , 0 , 5643 , -1825 , -5435 , 3701 , 4593 , -5417 , -3132 , 6756 , 1143 , -7518 , 1209 , 7549 , -3699 , -6764 , 6063 , 5165 , -8020 , -2848 , 9313 , 0 , -9736 , 3112 , 9162 , -6168 , -7569 , 8830 , 5049 , -10774 , -1804 , 11735 , -1867 , -11540 , 5597 , 10130 , -8988 , -7581 , 11656 , 4098 , -13273 , 0 , 13611 , -4309 , -12568 , 8383 , 10192 , -11780 , -6675 , 14114 , 2342 , -15099 , 2381 , 14586 , -7013 , -12581 , 11067 , 9254 , -14105 , -4917 , 15789 , 0 , -15918 , 4998 , 14454 , -9560 , -11527 , 13212 , 7425 , -15569 , -2563 , 16384 , -2563 , -15569 , 7425 , 13212 , -11527 , -9560 , 14454 , 4998 , -15918 , 0 , 15789 , -4917 , -14105 , 9254 , 11067 , -12581 , -7013 , 14586 , 2381 , -15099 , 2342 , 14114 , -6675 , -11780 , 10192 , 8383 , -12568 , -4309 , 13611 , 0 , -13273 , 4098 , 11656 , -7581 , -8988 , 10130 , 5597 , -11540 , -1867 , 11735 , -1804 , -10774 , 5049 , 8830 , -7569 , -6168 , 9162 , 3112 , -9736 , 0 , 9313 , -2848 , -8020 , 5165 , 6063 , -6764 , -3699 , 7549 , 1209 , -7518 , 1143 , 6756 , -3132 , -5417 , 4593 , 3701 , -5435 , -1825 , 5643 , 0 , -5272 , 1593 , 4431 , -2819 , -3267 , 3599 , 1943 , -3914 , -619 , 3796 , -570 , -3320 , 1518 , 2589 , -2164 , -1719 , 2488 , 823 , -2508 , 0 , 2273 , -676 , -1853 , 1160 , 1324 , -1436 , -763 , 1513 , 235 , -1422 , 210 , 1205 , -542 , -911 , 750 , 587 , -838 , -273 , 822 , 0 , -728 , 214 , 581 , -361 , -409 , 442 , 234 , -463 , -72 , 437 , -65 , -375 , 171 , 291 , -243 , -194 , 283 , 95}; 
//13e3
const short filter2[257] = {
-248 , -50 , 314 , -243 , -111 , 371 , -232 , -188 , 437 , -209 , -286 , 509 , -169 , -409 , 584 , -102 , -561 , 656 , 0 , -742 , 716 , 147 , -950 , 750 , 348 , -1180 , 745 , 609 , -1422 , 683 , 935 , -1660 , 549 , 1324 , -1878 , 325 , 1771 , -2051 , 0 , 2263 , -2155 , -437 , 2781 , -2164 , -989 , 3303 , -2052 , -1653 , 3796 , -1795 , -2419 , 4228 , -1375 , -3267 , 4561 , -778 , -4170 , 4756 , 0 , -5091 , 4777 , 954 , -5988 , 4593 , 2069 , -6814 , 4175 , 3319 , -7518 , 3508 , 4665 , -8048 , 2584 , 6063 , -8357 , 1408 , 7455 , -8402 , 0 , 8783 , -8147 , -1609 , 9981 , -7569 , -3373 , 10985 , -6659 , -5236 , 11735 , -5419 , -7132 , 12177 , -3869 , -8988 , 12267 , -2046 , -10729 , 11974 , 0 , -12278 , 11282 , 2207 , -13564 , 10192 , 4499 , -14522 , 8723 , 6797 , -15099 , 6910 , 9015 , -15257 , 4806 , 11067 , -14973 , 2476 , 12873 , -14244 , 0 , 14360 , -13084 , -2538 , 15469 , -11527 , -5047 , 16153 , -9622 , -7437 , 16384 , -7437 , -9622 , 16153 , -5047 , -11527 , 15469 , -2538 , -13084 , 14360 , 0 , -14244 , 12873 , 2476 , -14973 , 11067 , 4806 , -15257 , 9015 , 6910 , -15099 , 6797 , 8723 , -14522 , 4499 , 10192 , -13564 , 2207 , 11282 , -12278 , 0 , 11974 , -10729 , -2046 , 12267 , -8988 , -3869 , 12177 , -7132 , -5419 , 11735 , -5236 , -6659 , 10985 , -3373 , -7569 , 9981 , -1609 , -8147 , 8783 , 0 , -8402 , 7455 , 1408 , -8357 , 6063 , 2584 , -8048 , 4665 , 3508 , -7518 , 3319 , 4175 , -6814 , 2069 , 4593 , -5988 , 954 , 4777 , -5091 , 0 , 4756 , -4170 , -778 , 4561 , -3267 , -1375 , 4228 , -2419 , -1795 , 3796 , -1653 , -2052 , 3303 , -989 , -2164 , 2781 , -437 , -2155 , 2263 , 0 , -2051 , 1771 , 325 , -1878 , 1324 , 549 , -1660 , 935 , 683 , -1422 , 609 , 745 , -1180 , 348 , 750 , -950 , 147 , 716 , -742 , 0 , 656 , -561 , -102 , 584 , -409 , -169 , 509 , -286 , -209 , 437 , -188 , -232 , 371 , -111 , -243 , 314 , -50 , -248};  

short PC_DATA[17];//接收PC数据
short Send_PC_DATA[28];//发送PC数据

short DS18B20_flag;
short serial[8];   // 读序列号函数中的数组
short serial_x[8]; // 读取的序列号存放的数组
float temperature; // 返回的真实温度值


//DMA配置变量
volatile short DMAdone;//DMA传输结束标志
volatile Uint32 DMAnum;//DMA接收计数器
volatile Uint16 RevsrcAddrHi,RevsrcAddrLo;
volatile Uint16 Revdst1AddrHi,Revdst1AddrLo;
volatile Uint16 Revdst2AddrHi,Revdst2AddrLo;//DMA接收地址
volatile Uint16 sendsrc0AddrHi, sendsrc0AddrLo;
volatile Uint16 senddst0AddrHi, senddst0AddrLo;
volatile Uint16 sendsrc1AddrHi, sendsrc1AddrLo;
volatile Uint16 senddst1AddrHi, senddst1AddrLo;//发射DMA地址

//定义句柄
DMA_Handle myhDma, hDmaSend0,hDmaSend1;
TIMER_Handle mhTimer0,mhTimer1;
MMC_Handle mmc1;
MMC_CardObj *card, cardalloc;
MMC_CardIdObj *cid, cardid;

Uint16 count, retVal;
Uint16 rca;
Uint16 cardtype;
Uint32 SectorCounter;
Uint16 SecCounter[256];//装入扇区号
Uint16 SD_write[256];//SD卡写入数据
Uint16 SD_read[256];//SD卡读取数据
short SD_test;
short SD_status;


//标志位
volatile short CIF_enable;
volatile short IIF_enable;
volatile short XDCZ;
volatile short XDCZ_num;
volatile short transmit_sig_val;
volatile short Flag_timer;
volatile short FLAG_OK;
volatile short Flag_CIF;        //检测CIF信号返回值
volatile short Flag_IIF;        //检测CIF信号返回值
volatile short Flag_CIF_IIF;  
volatile short FLAG_RS232_REVOK;//串口接收命令标

volatile short FLAGSENDOVER;    //发射信号结束标志位
volatile Uint32 resend_delay;   //转发延迟计数器
volatile Uint32 timer0_cnt;      //定时器100ms计数器
volatile Uint32 timer1_cnt;      //定时器1us计数器


//压力传感器、内部电压采集变量
short RS485_DATA[12];
Uint32 temp_yl_data;
short temp_ADC;//AD结果缓存
short ADC_in[64];//内部AD结果缓存数组

short voltage_AD; //电源电压量化值
float depth;//压力传感器拟合深度

short noise_10ms;//采集10ms噪声量化值
volatile short transponder_mode;//应答模式
volatile short main_node;      //相对测阵模式
volatile short main_station;   //正常工作模式
volatile short FSK_flag;       //FSK指令标志位
volatile short jiance_counter; //检测信号计数器

#endif

