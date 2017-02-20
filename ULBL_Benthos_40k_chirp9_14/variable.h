/***************
//¶¨ÒåÏµÍ³ËùÓÐ±äÁ¿
****************/
#ifndef _VARIABLE_H
#define _VARIABLE_H

//±äÁ¿¶¨Òå
#define NFFT      2048  //FFTµãÊý
#define fs        40    //²ÉÑùÂÊ40kHz
#define PI        3.1415926
#define NFFT_mingling 256

//¼ì²â²¿·Ö¶¨Òå
#define SYNC_TIME_STD    1000                 //±¾µØÐÅºÅ³¤¶È
#define Rev_resave       SYNC_TIME_STD        //±£ÁôÊý¾Ý³¤¶È
#define RevDatReadLen    NFFT-SYNC_TIME_STD   //±£ÁôÍ¬²½ÐÅºÅµãÊý
#define BUF_LEN_MAX      RevDatReadLen        //½ÓÊÕ»º´æÇø³¤¶È

#define N_LPF1     33   //Ïà¹ØÊä³öµÍÍ¨ÂË²¨Æ÷½×Êý
#define N_LPF2     33   //Õ­´øÂË²¨Êä³öµÍÍ¨ÂË²¨Æ÷½×Êý
#define N_resample 26   //½µ²ÉÑùÃ¿¿éµãÊý

//¼ì²âÏà¹Ø±äÁ¿
#define Maikuan_l 5  //Ïà¹Ø·åÂö¿íÏÂÏÞ
#define Maikuan_h 18 //Ïà¹Ø·åÂö¿íÉÏÏÞ
#define SNR 3        //¾Ö²¿ÐÅÔë±È²ÎÊý

volatile short NOISE_TH_huadong; //ÔëÉù»¬¶¯ÃÅÏÞ
	     short noise_maxbuffer[2] = {0,0};
volatile short NOISE_TH_NEW;     //¾ø¶ÔÃÅÏÞ
volatile float NOISE_TH_HD;
volatile Uint32 Time_Delay;      //×ª·¢ÑÓ³Ù
volatile Uint32 responder_Delay; //ÐÅ±êÄ£Ê½ÏÂ×ª·¢ÑÓ³Ù
volatile Uint32 responder_period;//ÐÅ±êÄ£Ê½ÏÂÍ¬²½ÖÜÆÚ

#define Pressure_Delay 300000 //ÉÏ´«Ñ¹Á¦ÐÅÏ¢¹Ì¶¨ÑÓ³Ùµ¥Î»us

volatile short DMA_count;     //DMA°áÒÆÊý¾Ý¼ÆÊý¡£

volatile int Pos_flag;        //¼ì²âµ½ÐÅºÅ±êÖ¾Î»
volatile short endpos;        //ÐÅºÅÔÚRev_envelopeÖÐ³öÏÖÎ»ÖÃ
volatile short sum_mean_save; //ÐÅºÅÏà¹Ø¾ùÖµ
volatile short width_save;    //Ïà¹Ø¿í¶È
volatile short endpos_xcorr;  //ÐÅºÅÔÚRev_envelopeÖÐ³öÏÖÎ»ÖÃ
float endpos_float;
volatile float SNR_HD_out;

short block2;
volatile short testpinlv_flag;
#pragma DATA_SECTION(count_final_jiance,"send")  
short count_final_jiance[12]; //ÊÍ·ÅÐÅºÅ»º´æ¿Õ¼ä 

short testpinlv_max;
short testpinlv_ind;
short x1,x2,x3;
short y1,y2,y3;
float a,b;
float x_max;
float f_code;
short code;   //ÆµÂÊÂë

//SDRAM²úÉúÐÅºÅ±äÁ¿
volatile unsigned long SDRAM_lfmhead;//SDRAM´æ´¢Í·
short templfm;

short RAMSES_code1[8] = {8,3,4,1,7,6,2,5};//RAMSES01
short RAMSES_time1[8] = {50,50,50,50,50,50,50,50};   

short RAMSES_code[8] = {3,4,7,5,2,8,1,6}; //RAMSES07
short RAMSES_freq[8] = {9640,10540,11250,12030,12850,13600,14350,15150};
short RAMSES_time[8] = {1250,1250,1250,1250,1250,1250,1250,1250}; 

#pragma DATA_SECTION(Send_sig,"send")
short Send_sig[25000];//·¢ÉäÐÅºÅ

//²Î¿¼ÐÅºÅºÍÂË²¨Æ÷ÏµÊý
#pragma DATA_SECTION(X_ref,".ref")
short X_ref[2*NFFT];//²Î¿¼ÐÅºÅ
extern short h_LPF1[N_LPF1];//µÍÍ¨ÂË²¨Æ÷ÏµÊý1;Ïà¹Ø°üÂç½øÐÐÂË²¨¡£
extern short h_LPF2[N_LPF2];//µÍÍ¨ÂË²¨Æ÷ÏµÊý2

#pragma DATA_SECTION(h_BPF1,"filter")
short h_BPF1[2*NFFT];//Õ­´øÂË²¨Æ÷ÏµÊý1
#pragma DATA_SECTION(h_BPF2,"filter")
short h_BPF2[2*NFFT];//Õ­´øÂË²¨Æ÷ÏµÊý2 

Uint16 count_timer;
Uint16 responder_flag;
//ÂË²¨Æ÷»º´æÊý×é
short h_LPF1_db[N_LPF1+2];
short h_LPF2_db[N_LPF2+2];

//ÂË²¨Æ÷Òç³ö±êÖ¾Î»¶¨Òå
short oflag_LPF1;
short oflag_LPF2;
short oflag_BPF1;
short oflag_BPF2;

//´®¿Ú´«ÊäÊý¾Ý¶¨Òå
short PC_init[28];
short buff;
short f_send_uart;
const short init_rs485[4] = {0x0fa,0x30,0x04,0x43};//Ñ¹Á¦´«¸ÐÆ÷³õÊ¼»¯Êý¾Ý
const short ask_rs485[5] = {0x0fa,0x4a,0x01,0x51,0x0a7};
//const short ask_rs485[5] = {0x0fa,0x4a,0x01,0x51,0x43};
volatile short FLAG_yl_init;//Ñ¹Á¦´«¸ÐÆ÷³õÊ¼»¯±êÖ¾Î»
volatile short FLAG_yl;//Ñ¹Á¦´«¸ÐÆ÷¶ÁÈ¡±êÖ¾Î»
volatile short release_flag;

//Êý×éÉùÃ÷
//²É¼¯Êý×éÉùÃ÷
#pragma DATA_SECTION(Rev1,"receive1")
short Rev1[BUF_LEN_MAX];  //²É¼¯Êý¾ÝÆ¹ÅÒ»º´æÇø1

#pragma DATA_SECTION(Rev2,"receive2")
short Rev2[BUF_LEN_MAX];  //Ã¿´Î²É¼¯Êý¾ÝÆ¹ÅÒ»º´æÇø2

#pragma DATA_SECTION(Rev,"NFFT")
short Rev[NFFT];      //ÖØµþ±£ÁôÔ­Ê¼Êý¾Ý

#pragma DATA_SECTION(Rev_FFT,"FFT")
short Rev_FFT[2*NFFT];     //FFTÊý¾Ý»º´æ

#pragma DATA_SECTION(Rev_xcorr,"xcorr")
short Rev_xcorr[2*NFFT];   //Ïà¹Ø½á¹û»º´æ

#pragma DATA_SECTION(Rev_xcorrout,"xcorrout")
short Rev_xcorrout[NFFT]; //Óë±¾µØÐòÁÐÏà¹ØÊä³ö

#pragma DATA_ALIGN(Rev_envelope,2)
#pragma DATA_SECTION(Rev_envelope,"envelope")
short Rev_envelope[4096]; //Óë±¾µØÐòÁÐÏà¹ØÊä³ö°üÂç

#pragma DATA_ALIGN(Rev_bp1,2)
#pragma DATA_SECTION(Rev_bp1,"bp1")
short Rev_bp1[NFFT];            //Õ­´øÂË²¨Æ÷1Êä³ö

#pragma DATA_ALIGN(Rev_bp2,2)
#pragma DATA_SECTION(Rev_bp2,"bp2")
short Rev_bp2[NFFT];            //Õ­´øÂË²¨Æ÷2Êä³ö

#pragma DATA_ALIGN(Rev_bp1_abs,2)
#pragma DATA_SECTION(Rev_bp1_abs,"bp1abs")
short Rev_bp1_abs[NFFT];        //Õ­´øÂË²¨Æ÷1Êä³ö¾ø¶ÔÖµ

#pragma DATA_ALIGN(Rev_bp2_abs,2)
#pragma DATA_SECTION(Rev_bp2_abs,"bp2abs")
short Rev_bp2_abs[NFFT];        //Õ­´øÂË²¨Æ÷2Êä³ö¾ø¶ÔÖµ

#pragma DATA_SECTION(Rev_bp1_envelope,"bp1envelope")
short Rev_bp1_envelope[NFFT];   //Õ­´øÂË²¨Æ÷1Êä³ö°üÂç

#pragma DATA_SECTION(Rev_bp2_envelope,"bp2envelope")
short Rev_bp2_envelope[NFFT];   //Õ­´øÂË²¨Æ÷2Êä³ö°üÂç

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

//9.5k 13bit
const short filter1[257] = {
-248 , 166 , 294 , -132 , -342 , 88 , 390 , -33 , -437 , -36 , 481 , 120 , -519 , -222 , 547 , 341 , -561 , -478 , 554 , 633 , -520 , -802 , 454 , 980 , -348 , -1162 , 198 , 1338 , 0 , -1500 , -249 , 1635 , 549 , -1730 , -896 , 1773 , 1287 , -1750 , -1710 , 1649 , 2155 , -1459 , -2606 , 1171 , 3044 , -781 , -3448 , 286 , 3796 , 310 , -4065 , -999 , 4231 , 1768 , -4273 , -2598 , 4170 , 3467 , -3906 , -4345 , 3471 , 5201 , -2858 , -6001 , 2069 , 6708 , -1111 , -7287 , 0 , 7703 , 1242 , -7923 , -2584 , 7921 , 3989 , -7675 , -5417 , 7170 , 6819 , -6402 , -8147 , 5373 , 9350 , -4097 , -10380 , 2596 , 11189 , -905 , -11735 , -936 , 11984 , 2878 , -11909 , -4864 , 11492 , 6835 , -10729 , -8728 , 9625 , 10479 , -8197 , -12027 , 6475 , 13316 , -4499 , -14297 , 2321 , 14926 , 0 , -15173 , -2399 , 15020 , 4806 , -14459 , -7147 , 13498 , 9352 , -12156 , -11352 , 10467 , 13084 , -8476 , -14492 , 6238 , 15532 , -3818 , -16169 , 1285 , 16384 , 1285 , -16169 , -3818 , 15532 , 6238 , -14492 , -8476 , 13084 , 10467 , -11352 , -12156 , 9352 , 13498 , -7147 , -14459 , 4806 , 15020 , -2399 , -15173 , 0 , 14926 , 2321 , -14297 , -4499 , 13316 , 6475 , -12027 , -8197 , 10479 , 9625 , -8728 , -10729 , 6835 , 11492 , -4864 , -11909 , 2878 , 11984 , -936 , -11735 , -905 , 11189 , 2596 , -10380 , -4097 , 9350 , 5373 , -8147 , -6402 , 6819 , 7170 , -5417 , -7675 , 3989 , 7921 , -2584 , -7923 , 1242 , 7703 , 0 , -7287 , -1111 , 6708 , 2069 , -6001 , -2858 , 5201 , 3471 , -4345 , -3906 , 3467 , 4170 , -2598 , -4273 , 1768 , 4231 , -999 , -4065 , 310 , 3796 , 286 , -3448 , -781 , 3044 , 1171 , -2606 , -1459 , 2155 , 1649 , -1710 , -1750 , 1287 , 1773 , -896 , -1730 , 549 , 1635 , -249 , -1500 , 0 , 1338 , 198 , -1162 , -348 , 980 , 454 , -802 , -520 , 633 , 554 , -478 , -561 , 341 , 547 , -222 , -519 , 120 , 481 , -36 , -437 , -33 , 390 , 88 , -342 , -132 , 294 , 166 , -248 };  
 //13k
const short filter2[257] = {
-248 , -50 , 314 , -243 , -111 , 371 , -232 , -188 , 437 , -209 , -286 , 509 , -169 , -409 , 584 , -102 , -561 , 656 , 0 , -742 , 716 , 147 , -950 , 750 , 348 , -1180 , 745 , 609 , -1422 , 683 , 935 , -1660 , 549 , 1324 , -1878 , 325 , 1771 , -2051 , 0 , 2263 , -2155 , -437 , 2781 , -2164 , -989 , 3303 , -2052 , -1653 , 3796 , -1795 , -2419 , 4228 , -1375 , -3267 , 4561 , -778 , -4170 , 4756 , 0 , -5091 , 4777 , 954 , -5988 , 4593 , 2069 , -6814 , 4175 , 3319 , -7518 , 3508 , 4665 , -8048 , 2584 , 6063 , -8357 , 1408 , 7455 , -8402 , 0 , 8783 , -8147 , -1609 , 9981 , -7569 , -3373 , 10985 , -6659 , -5236 , 11735 , -5419 , -7132 , 12177 , -3869 , -8988 , 12267 , -2046 , -10729 , 11974 , 0 , -12278 , 11282 , 2207 , -13564 , 10192 , 4499 , -14522 , 8723 , 6797 , -15099 , 6910 , 9015 , -15257 , 4806 , 11067 , -14973 , 2476 , 12873 , -14244 , 0 , 14360 , -13084 , -2538 , 15469 , -11527 , -5047 , 16153 , -9622 , -7437 , 16384 , -7437 , -9622 , 16153 , -5047 , -11527 , 15469 , -2538 , -13084 , 14360 , 0 , -14244 , 12873 , 2476 , -14973 , 11067 , 4806 , -15257 , 9015 , 6910 , -15099 , 6797 , 8723 , -14522 , 4499 , 10192 , -13564 , 2207 , 11282 , -12278 , 0 , 11974 , -10729 , -2046 , 12267 , -8988 , -3869 , 12177 , -7132 , -5419 , 11735 , -5236 , -6659 , 10985 , -3373 , -7569 , 9981 , -1609 , -8147 , 8783 , 0 , -8402 , 7455 , 1408 , -8357 , 6063 , 2584 , -8048 , 4665 , 3508 , -7518 , 3319 , 4175 , -6814 , 2069 , 4593 , -5988 , 954 , 4777 , -5091 , 0 , 4756 , -4170 , -778 , 4561 , -3267 , -1375 , 4228 , -2419 , -1795 , 3796 , -1653 , -2052 , 3303 , -989 , -2164 , 2781 , -437 , -2155 , 2263 , 0 , -2051 , 1771 , 325 , -1878 , 1324 , 549 , -1660 , 935 , 683 , -1422 , 609 , 745 , -1180 , 348 , 750 , -950 , 147 , 716 , -742 , 0 , 656 , -561 , -102 , 584 , -409 , -169 , 509 , -286 , -209 , 437 , -188 , -232 , 371 , -111 , -243 , 314 , -50 , -248};  
short PC_DATA[12];//½ÓÊÕPCÊý¾Ý
short Send_PC_DATA[28];//·¢ËÍPCÊý¾Ý

short DS18B20_flag;
short serial[8];   // ¶ÁÐòÁÐºÅº¯ÊýÖÐµÄÊý×é
short serial_x[8]; // ¶ÁÈ¡µÄÐòÁÐºÅ´æ·ÅµÄÊý×é
float temperature; // ·µ»ØµÄÕæÊµÎÂ¶ÈÖµ


//DMAÅäÖÃ±äÁ¿
volatile short DMAdone;//DMA´«Êä½áÊø±êÖ¾
volatile Uint32 DMAnum;//DMA½ÓÊÕ¼ÆÊýÆ÷
volatile Uint16 RevsrcAddrHi,RevsrcAddrLo;
volatile Uint16 Revdst1AddrHi,Revdst1AddrLo;
volatile Uint16 Revdst2AddrHi,Revdst2AddrLo;//DMA½ÓÊÕµØÖ·
volatile Uint16 sendsrc0AddrHi, sendsrc0AddrLo;
volatile Uint16 senddst0AddrHi, senddst0AddrLo;
volatile Uint16 sendsrc1AddrHi, sendsrc1AddrLo;
volatile Uint16 senddst1AddrHi, senddst1AddrLo;//·¢ÉäDMAµØÖ·

//¶¨Òå¾ä±ú
DMA_Handle myhDma, hDmaSend0,hDmaSend1;
TIMER_Handle mhTimer0,mhTimer1;
MMC_Handle mmc1;
MMC_CardObj *card, cardalloc;
MMC_CardIdObj *cid, cardid;

Uint16 count, retVal;
Uint16 rca;
Uint16 cardtype;
Uint32 SectorCounter;
Uint16 SecCounter[256];//×°ÈëÉÈÇøºÅ
Uint16 SD_write[256];//SD¿¨Ð´ÈëÊý¾Ý
Uint16 SD_read[256];//SD¿¨¶ÁÈ¡Êý¾Ý
short SD_test;
short SD_status;

volatile short Flag_timer;
volatile short FLAG_OK;
volatile short Flag_answer;//¼ì²âÐÅºÅ·µ»ØÖµ
volatile short FLAG_RS232_REVOK;//´®¿Ú½ÓÊÕÃüÁî±ê»

volatile short FLAGSENDOVER;    //·¢ÉäÐÅºÅ½áÊø±êÖ¾Î»
volatile Uint32 resend_delay;   //×ª·¢ÑÓ³Ù¼ÆÊýÆ÷
volatile Uint32 timer0_cnt;      //¶¨Ê±Æ÷100ms¼ÆÊýÆ÷
volatile Uint32 timer1_cnt;      //¶¨Ê±Æ÷1us¼ÆÊýÆ÷


//Ñ¹Á¦´«¸ÐÆ÷¡¢ÄÚ²¿µçÑ¹²É¼¯±äÁ¿
short RS485_DATA[12];
Uint32 temp_yl_data;
short temp_ADC;//AD½á¹û»º´æ
short ADC_in[64];//ÄÚ²¿AD½á¹û»º´æÊý×é

short voltage_AD; //µçÔ´µçÑ¹Á¿»¯Öµ
float depth;//Ñ¹Á¦´«¸ÐÆ÷ÄâºÏÉî¶È

short noise_10ms;//²É¼¯10msÔëÉùÁ¿»¯Öµ
volatile short transponder_mode;//Ó¦´ðÄ£Ê½
volatile short tongbu_flag;//Í¬²½ÃüÁî±êÖ¾Î»
volatile short jiance_counter;//¼ì²âÐÅºÅ¼ÆÊýÆ÷

#endif

