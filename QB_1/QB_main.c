// 这个版本用于测试从2min的片外SRAM取出1s的数据，FFT结果是否正确。

			
#include "QB_main.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dsp_fft32x32.h"

#define AD_Buffer_num 0x1000
#define AD_big_Buffer_num  114688   //与CF_Write_Bytes大小一致；
#define File_num 1095 
//#define File_num 260          // 每张卡有260个文件
#define File_FATSec_num 55 //每个文件的大小，单位MB
#define File_Cell_num 7000    // 每128个单元为1MB，缓冲区单元个数，决定文件大小
#define NFFT 2048             // 1s信号FFT点数；
#define NF   200			  // 处理频段1~200Hz；
#define NFFT2MIN 2000         // 1s信号实际FFT点数；
#define TSIG 120              // 信号处理总长度；
#define T 1					  // 单次FFT时间；
//#define Data_trans_num 57344  //Ad_big_buffer到CF卡写buffer数据搬移；
#define Data_banyi_num 8192   //EDMA buffer到Ad_big_buffer数据搬移；
// 调试定义
//#define CFZERO        // 调试时CF卡罗经扇区写零
#define CFTIME        // 调试时测试CF卡buffer写时间
//#define SRAMTEST      // 调试片外SRAM;
//long sum1 = 0;        // 测试SRAM结果输出；

short Ad_Data_buffer0[AD_Buffer_num];//16扇区
short Ad_Data_buffer1[AD_Buffer_num];//16扇区
unsigned char Ad_big_buffer[AD_big_Buffer_num];  //14*16=224扇区
unsigned char Seri[]={'T','E','S','T'};       // 测试串口反馈
unsigned char PRD[]={'$','0','0','P','A','R','0','0','0','0','0','0','0','0','0','0','0','0','*','0','0','#'};        // 测试串口反馈
unsigned char PRD_C[]={'$','0','0','P','A','R','C','O','M','P','0','0','0','0','0','0','0','0','*','0','0','#'};  
unsigned char PRD_N[]={'$','0','0','P','A','R','N','E','G','Z','0','0','0','0','0','0','0','0','*','0','0','#'};  
unsigned char PWR[]={'!'};        // 测试串口反馈
unsigned char DET[]={'!'};        // 测试串口反馈
unsigned char FED[]={'!'};        // 测试串口反馈
unsigned char TST[]={'$','0','0','T','F','D','0','0','0','0','0','0', '0','0','0','0','*','5','6','#'};        // 测试串口反馈
unsigned char NO[]={'H','O','W','?'};         // 测试串口反馈
unsigned char OBJ[50];
unsigned char First[]={'?'};
extern unsigned char Command_buffer[];
int bytesSize1=120;    // 串口发送fifo个数60字节；
int SerCount=0;      // 串口每2min发送2040字节；
extern int Ser_OK;      // fifo头60字节发送完成标志；
int count0=17;            // 串口每2min发送17个包；
short ComInt=0;
short WorkInt=1;
int bytesSize=22;
int bytesSize2=1;
int bytesSize3=50;
int bytesSize4=20;

// FFT初始化
int P1s[NFFT*2];
int Vx1s[NFFT*2];
int Vy1s[NFFT*2];
int Vz1s[NFFT*2];

int FP1s[NFFT*2];
int FVx1s[NFFT*2];
int FVy1s[NFFT*2];
int FVz1s[NFFT*2];
int w[NFFT*2];
double sca=121.0;
//int nfft=2048;

#pragma DATA_ALIGN(P1s, 8);                            // 内存对齐；
//#pragma DATA_ALIGN(Vx1s, 8);
#pragma DATA_ALIGN(Vy1s, 8);
#pragma DATA_ALIGN(Vz1s, 8);
#pragma DATA_ALIGN(FP1s, 8);                            
#pragma DATA_ALIGN(FVx1s, 8);
#pragma DATA_ALIGN(FVy1s, 8);
#pragma DATA_ALIGN(FVz1s, 8);
#pragma DATA_ALIGN(w, 8);

double Sx[NF];
double Sy[NF];
double Sz[NF];
double Sp[NF];
double tempx=0,tempy=0,tempz=0,temp1=0,temp2=0,temp3=0,temp4=0;
long long A=0xAF;

short Sx16[NF];     // ！！！！实际是short型，这是为了matlab方便！！！！
short Sy16[NF];     // ！！！！实际是short型，这是为了matlab方便！！！！
short Sz16[NF];     // ！！！！实际是short型，这是为了matlab方便！！！！
int Sr16=0;      // ！！！！实际是short型，这是为了matlab方便！！！！
short Sp16[NF];   // 其实取log用的是1字节，最后要修改；
int Sx32[NF];
int Sy32[NF];
float theta=0.0;
//int *thetaP;

int count_theta[361];
int Target1, T1;
extern unsigned char Compass_buffer[]; // 罗经，占CF卡1个扇区；

unsigned int File_index = 0;   //文件号索引
unsigned int Cell_Num_index = 0;  //每个文件的单元索引
unsigned int Data_Sector_num;
unsigned int Compass_Sector_num;

unsigned int Data_Sector_num;
extern unsigned int DataAdd ;
extern unsigned int DIRAdd ;
extern unsigned char CF_Wsector_buffer[];
extern unsigned char CF_Wsectors_buffer[];
extern unsigned char Edma16_Int_flag;
extern unsigned char Edma13_Int_flag;
extern unsigned char Edma_Int_flag;
extern unsigned char Compass_Int_flag;
extern unsigned char Command_Int_flag;  // 串口命令标志位；

extern unsigned char Sf[];    // 17*120，测试传输数据；
extern int file_count;
extern unsigned char KELLER_buffer[];

short ReadyInt = 0;           // 2分钟数据完毕，信号处理标志位置位；
short S1Int = 0;              // 第一步PVz过门限；
short OBJ_Int=0;              // 通过负谱门限；
short SendInt=0;             // 发送目标信息标志位；

long DelayT=0x165A0BC00;    // 延计数器；
long N_Del=0;

// SRAM参数
#define NBUFFER 480           // 一个buffer中的一路信号的点数；
double NSIG=240000.0;        // 2min数据中的一路信号的点数；
short CountBuffer=0;          // 写SRAM计数器，一个buffer（15个扇区数据）计0~479；
short Count2Min  =1;          // 写SRAM计数器，2min数据500个buffer；

//相位修正数据
short phX[200]={
0,0,0,0,0,0,0,0,0,
91, 82, 74, 67, 61, 56, 52, 48, 44, 42, 39, 37, 35, 33, 31, 30, 28, 27, 26, 25,
24, 23, 22, 21, 20, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 14, 13,
13, 12, 12, 12, 12, 11, 11, 11, 10, 10, 10, 10, 9, 9, 9, 9, 9, 8, 8, 8,
8, 8, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5,
5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
short phY[200]={
0,0,0,0,0,0,0,0,0,
90, 81, 73, 67, 61, 56, 51, 48, 44, 41, 39, 36, 34, 32, 31, 29, 28, 27, 25, 24,
23, 22, 22, 21, 20, 20, 19, 18, 18, 17, 17, 16, 16, 15, 15, 15, 14, 14, 13, 13,
13, 12, 12, 12, 11, 11, 11, 11, 10, 10, 10, 10, 9, 9, 9, 9, 8, 8, 8, 8,
8, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5,
5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
short phZ[200]={
0,0,0,0,0,0,0,0,0,
89, 80, 72, 65, 60, 55, 50, 47, 43, 40, 38, 36, 34, 32, 30, 29, 28, 26, 25, 24,
23, 22, 21, 21, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12,
12, 12, 12, 11, 11, 11, 10, 10, 10, 10, 10, 9, 9, 9, 9, 8, 8, 8, 8, 8,
7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// 深度计初始化；
//unsigned char KELLER_TX1[4]= {0xFA,0x30,0x04,0x43};
unsigned char KELLER_TX11[4]={0xFA,0x30,0x04,0x43};
unsigned char KELLER_TX2[5]= {0xFA,0x49,0x01,0xA1,0xA7};
unsigned char KELLER_TX3[5]= {0xFA,0x49,0x04,0xA2,0x67};
//unsigned char KELLER_RX1[10];
unsigned char KELLER_P[9];  // 压力;
unsigned char KELLER_T[9];  // 温度；

// 声明函数
long SRAM_Test();             // 测试片外SRAM读写；
//void SRAM_Main();           // 2min数据写入片外SRAM；
void CFZERO_Test();           // CF卡中罗经扇区写零；
void CF_Main();               // 信号写入CF卡主程序；
void CF_Write();              //实际写CF卡扇区函数；
static int d2i(double d);
int gen_twiddle_fft32x32(int *w, int n, double scale);
void Command_Choose();
void ClearCommandBuffer();
void SerSf();
void Ser2MinC();
void OBJ2MinC();
char C2A(char C);
void SendData();
void CFOK();
void T2R(double DT1,int C2);
void T2R_C(float ComPass);
void T2R_N(short C1);
void R2T();
void R2T_C();
void R2T_N();
void SerOBJ();
void SendOBJ();
void SerOK();
void ClearOrderFIFO();
void ClearCompassFIFO();

int ii=0;
int M_P=0,M_Vx=0,M_Vy=0,M_Vz=0;
int N=0,NN=0,TT=0;               // 循环计数用；
long sumP=0,sumVx=0,sumVy=0,sumVz=0;
short CountS1=0;              // 垂直能流5次循环计数；
double PVzA=0.0,PVzB=0.0,PVzC=0.0,PVz=0,PVzR=0;
int P_temp=0,Vz_temp=0,PVz_temp=0;
double P_temp1=0,Vz_temp1=0;
short DT2=1,C1=0x0;   //负谱门限
//double DT1=1.93;      // 5层逻辑的门限；
double DT1=1.05;      // 5层逻辑的门限;
int C2=0x124F800;            // 水平能流门限，就是角度谱哦；124F800~500mv
short CC=0,CC1=0,CC2=0,CC3=0,CC4=0;   // 2分钟循环计数器，用作参考时间；

float CompassFix[500];
float ComPass=0;   // 罗经物理放置�
short CompassNum=0;

// unsigned char Sf[2040];    // 16*126Byte;
short SerP,SerX,SerY,SerZ;
char c1=0,c2=0,c3=0;

int i1=0,j1=0;
//多扇区写CF卡变量
char CF_write_count=0;	

// 测试用的无用变量开始；------------------------
short testint=0;  
int s=0,ss=0;
short s16=0,ss16=0,sss16=0;
int m1=0,m2=0;
long long t1=10,t11;
float FF=-1.2;
long long TL[5]={0,-25987,-9223372036854775800,345698541,9223372036854775807};
double the=0.0;
//int C2_Temp1=0,C2_Temp2=0;
//float DT1_Temp3=0;
	
// 测试用的无用变量结束；----------------------------

void main()
{
				
//*************************initial program***********************************//

	for(N_Del=1;N_Del<300000000;N_Del++)    // 延时10s
	{
	  asm(" nop ");
	}


	config_chip(); 
	config_gpio();
  // McBSP_initial();								 //initialize GPIO   
    enableGlobalINT();                           //enable Global interrupt
    enableNMI();     	                         //enable NMI interrupt
    config_Interrupt_Selector(14, TINT0);      //timer0 interrupt 10s  
    disableSpecificINT(14);
    config_Interrupt_Selector(15, TINT1);      //timer1 interrupt 1s中断
    disableSpecificINT(15);
    config_Interrupt_Selector(4, EXT_INT4);      //using external interrupt INT4  syn_int
    disableSpecificINT(4);
    config_Interrupt_Selector(5, EXT_INT5);      //using external interrupt INT5 LJ interrupt
    disableSpecificINT(5);
    config_Interrupt_Selector(6, EXT_INT6);      //using external interrupt INT6 
    disableSpecificINT(6);   
	config_Interrupt_Selector(7, EXT_INT7);      //using external interrupt INT7 
    disableSpecificINT(7); 
    config_Interrupt_Selector(8, EDMA_INT);      //using EDMA interrupt INT8 EDMA interrupt
    disableSpecificINT(8);
	config_Interrupt_Selector(9, IRQ_EVT_GPINT0);      //using GPINT0 interrupt 
    disableSpecificINT(9);
	config_Interrupt_Selector(13, RINT0);      //using RINT0 McBSP 0 receive interrupt
    disableSpecificINT(13);
	CHIP_CRSET(ICR,0xFFFFFFFF);
//*********************************************************************************************//
//=========================初始化754通道a=============================//
	#ifdef SRAMTEST		
		sum1 = SRAM_Test();         // 测试SRAM读写是否正确；
	#endif

	config_754a();
	config_754b();
	config_754c();
	config_754d();

	memset(Compass_buffer,0x0,512*sizeof(char));    // 这句话是罗经扇区清零，为什么以前会被屏蔽掉呢？
	Init_CF();
	enableSpecificINT(6);
	enableSpecificINT(7);     //深度计命令接收中断使能,先检测KELLER工作是否正常
    RS485_to_KELLER(KELLER_TX11,4);    //发送开启命令到KELLER
    for(N_Del=1;N_Del<4000000;N_Del++)
	   asm(" nop ");
    //if((KELLER_buffer[0]==0xFA)&&(KELLER_buffer[1]==0x30))      //判断回复命令是否正确
    //{ 
	//   int n;
	//   for(n=0;n<10;n++)
	//   { 
	//      KELLER_RX1[n]=KELLER_buffer[n];
	//   }
    //}

	SendData();  // 是能定时器，完成无同步器时的延时启动功能。

	#ifdef CFZERO                   // CF卡中的罗经扇区全部写零；
    	 CFZERO_Test();
	#endif

	check_Sync();   // 杨师弟编写的同步器程序；
   	CHIP_CRSET(ICR,0xFFFFFFFF);
	//Data_Sector_num = DataAdd;
	Data_Sector_num = file_count * File_FATSec_num * 128 * 16 + DataAdd;
	Compass_Sector_num = Data_Sector_num + 105000;
	McBSP_initial();								 //initialize GPIO   	
	Config_EnableEdma13(Ad_Data_buffer0,Ad_Data_buffer1);    // C语言中，数组名代表数组第一个元素的地址
	enableSpecificINT(8);
	
	ClearOrderFIFO();
	ClearCompassFIFO();
	//ClearCommandBuffer();
	enableSpecificINT(6);
    enableSpecificINT(5);    //使能LJ中断


	//enableSpecificINT(7);     //深度计命令接收中断使能
	//enableSpecificINT(5);     //罗经

	//RS485_to_KELLER(KELLER_TX1,4);    //发送开启命令到KELLER
	//for(N_Del=0;N_Del<60000000;N_Del++)
	//   asm(" nop ");
    //if(KELLER_buffer[1]==0x30)      //判断回复命令是否正确
	//{ 
	//   int n;
	//   for(n=0;n<10;n++)
	//   { 
	//      KELLER_RX1[n]=KELLER_buffer[n];
	//   }
	//}
	
	gen_twiddle_fft32x32(w, NFFT, 2147483647.);
    
	// 初始化串口传输数据包
    SerSf();
	SerOBJ();

	R232_To_sync(First,bytesSize2);    // 程序开始
	//========================
	if(file_count >= File_num)
		{
			 disableSpecificINT(8);
		     Config_DisableEdma13();
		}
	//========================
	
	while(1)
	{
		SerOK();
		CFOK();
		Command_Choose();
		SerOK();    //查询串口谱信息发送情况；
				
		//ReadyInt=1;
		if((ReadyInt==1) && (WorkInt==1))               // 信号处理开始；
		{
		 	//*(volatile unsigned int *)GPVAL=0x00000002;    // 测试端口GPIO_1置1
		 	ReadyInt=0;
		 	//CC++;
		 	CountS1=CountS1+1;

		 	sumP = 0;
			sumVz= 0;
		 	for(N=0;N<NSIG;N++)       // 去均值；
			{
			 	sumP  = sumP  +  *(int16 *)(SRAM_P  + N*2);
				sumVz = sumVz +  *(int16 *)(SRAM_VZ + N*2);
				CFOK();
			}

			M_P = sumP/NSIG; 
			M_Vz= sumVz/NSIG;     
	
			for(N=0;N<NSIG;N++)
			{
			 	*(int16 *)(SRAM_P + N*2) = *(int16 *)(SRAM_P + N*2) - M_P;       
				*(int16 *)(SRAM_VZ+ N*2) = *(int16 *)(SRAM_VZ+ N*2) - M_Vz;
				CFOK();
			}


			if (CountS1==1)
			//if (CountS1<3)
			{
			 	PVzA = 0.0;
			 	for(N=0;N<NSIG;N++)
				{
			 		P_temp  = *(int16 *)(SRAM_P + N*2);       
					Vz_temp = *(int16 *)(SRAM_VZ+ N*2);
					P_temp1=P_temp;
					Vz_temp1=Vz_temp;
					
					PVzA = PVzA + P_temp1 * Vz_temp1; 

					CFOK();
				}
				PVzA = abs(PVzA/NSIG);
				PVz = PVzA;
				//PVzA = 0.0;//记得删除/////////////////////////////////////////////////////////////////////
			}
			else if (CountS1<=5)
			{
			 	PVzC = 0.0;
			 	for(N=0;N<NSIG;N++)
				{
			 		P_temp  = *(int16 *)(SRAM_P + N*2);       
					Vz_temp = *(int16 *)(SRAM_VZ+ N*2);
					P_temp1=P_temp;
					Vz_temp1=Vz_temp;
					
					PVzC = PVzC + P_temp1 * Vz_temp1; 
					CFOK();
				}
				PVzC = abs(PVzC/NSIG);
				PVz = PVzC;

				PVzR=PVzC/PVzA;
				if (PVzR<=DT1)
				{
				 	if (CountS1<3)
						PVzB=PVzC;
					else
					{
						if ((PVzC<=PVzB)||((PVzC>PVzB)&&(CountS1==5)))
							CountS1=0;
						else
							PVzB=PVzC;
							PVzA=PVzC;
					}
				}
				else
				{
				 	CountS1=0;
					S1Int=1;
					Sf[2001]=0x31;
				}
			} // if(CountS1) END

			//S1Int=1;   // !!!!!!要去掉！！！！！
			CFOK();
			Command_Choose();
			//if (S1Int==1)//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			//{
		 		//gen_twiddle_fft32x32(w, NFFT, 2147483647.);      // 产生蝶形因子；

			/*for(N=0;N<120;N++)                  // 罗经修正值提取；去除未成功提取的-1值；
			{
				 if (CompassFix[N]==-1);
					;
			}*/
			for(N=0;N<120;N++)                  // 罗经修正值提取；每4个平均1次对应1s的罗经修正值；
			{
				 CompassFix[N]=(CompassFix[N*4]+CompassFix[N*4+1]+CompassFix[N*4+2]+CompassFix[N*4+3])/4.0+ComPass;
				 if(CompassFix[N]<0)
					CompassFix[N]=360+CompassFix[N];
				 else if(CompassFix[N]>360)
					CompassFix[N]=CompassFix[N]-360;
			}
				
			memset(Sx,0x0,NF*sizeof(long long));     // memset具体用法待确认；
			memset(Sy,0x0,NF*sizeof(long long));
			memset(Sz,0x0,NF*sizeof(long long));
			memset(Sp,0x0,NF*sizeof(long long));

			memset(count_theta,0x0,361*sizeof(int));
		 		
		 	sumVx=0;
			sumVy=0;
			CFOK();
			Command_Choose();
		 	for(N=0;N<NSIG;N++)         // 去均值；
			{
			 	sumVx = sumVx +  *(int16 *)(SRAM_VX + N*2);
				sumVy = sumVy +  *(int16 *)(SRAM_VY + N*2);
			}

			M_Vx = sumVx/NSIG; 
			M_Vy = sumVy/NSIG;  
				
			CFOK(); 
			Command_Choose();  
	
			for(N=0;N<NSIG;N++)
			{
			 	*(int16 *)(SRAM_VX + N*2) = *(int16 *)(SRAM_VX + N*2) - M_Vx;       
				*(int16 *)(SRAM_VY + N*2) = *(int16 *)(SRAM_VY + N*2) - M_Vy;
			}

			CFOK();
			Command_Choose();
			for(N=0;N<120;N++)          // 谱分析,120s数据，1s计算一次；
			{
			 	memset(P1s,0x0000,2*NFFT*sizeof(int));                 // 时域1s数据跏蓟�
				memset(Vx1s,0x0000,2*NFFT*sizeof(int));
				memset(Vy1s,0x0000,2*NFFT*sizeof(int));
				memset(Vz1s,0x0000,2*NFFT*sizeof(int));
		 	    
				memset(FP1s,0x0000,2*NFFT*sizeof(int));                // 频域1s数据初始化;
				memset(FVx1s,0x0000,2*NFFT*sizeof(int));
				memset(FVy1s,0x0000,2*NFFT*sizeof(int));
				memset(FVz1s,0x0000,2*NFFT*sizeof(int));
				 	
			 	for(NN=0;NN<NFFT2MIN;NN++)
				{
				 	P1s[NN*2]  = *(int16 *)(SRAM_P  + (NN+N*NFFT2MIN)*2);
				 	Vx1s[NN*2] = *(int16 *)(SRAM_VX + (NN+N*NFFT2MIN)*2);
					Vy1s[NN*2] = *(int16 *)(SRAM_VY + (NN+N*NFFT2MIN)*2);
					Vz1s[NN*2] = *(int16 *)(SRAM_VZ + (NN+N*NFFT2MIN)*2);
				}

				// FFT 
				CFOK(); 
				Command_Choose();                  
				DSP_fft32x32(w, NFFT, P1s, FP1s);     
				DSP_fft32x32(w, NFFT, Vx1s, FVx1s);
				DSP_fft32x32(w, NFFT, Vy1s, FVy1s);
				DSP_fft32x32(w, NFFT, Vz1s, FVz1s);

				CFOK();
				Command_Choose();
				for(TT=1;TT<=NF;TT++)             // 只留实部结果
				{
				 	temp1=(double)FP1s[2*TT]*(double)FVx1s[2*TT];
					temp2=(double)FP1s[2*TT+1]*(double)FVx1s[2*TT+1];
					temp3=(double)FVx1s[2*TT]*(double)FP1s[2*TT+1];
					temp4=(double)FP1s[2*TT]*(double)FVx1s[2*TT+1];

					tempx=(temp1+temp2)*cos((double)phX[TT-1]/180.0*PI)-(temp3-temp4)*sin((double)phX[TT-1]/180.0*PI);
					//Sx[TT-1]=Sx[TT-1]+temp1+temp2;
						
					temp1=(double)FP1s[2*TT]*(double)FVy1s[2*TT];
					temp2=(double)FP1s[2*TT+1]*(double)FVy1s[2*TT+1];
					temp3=(double)FVy1s[2*TT]*(double)FP1s[2*TT+1];
					temp4=(double)FP1s[2*TT]*(double)FVy1s[2*TT+1];

					tempy=(temp1+temp2)*cos((double)phY[TT-1]/180.0*PI)-(temp3-temp4)*sin((double)phY[TT-1]/180.0*PI);
					//Sy[TT-1]=Sy[TT-1]+temp1+temp2;

					Sx[TT-1]=Sx[TT-1] + tempx*cos(CompassFix[N]/180.0*PI) + tempy*sin(CompassFix[N]/180.0*PI);
					Sy[TT-1]=Sy[TT-1] + tempx*sin(CompassFix[N]/180.0*PI) - tempy*cos(CompassFix[N]/180.0*PI);

					temp1=(double)FP1s[2*TT]*(double)FVz1s[2*TT];
					temp2=(double)FP1s[2*TT+1]*(double)FVz1s[2*TT+1];
					temp3=(double)FVz1s[2*TT]*(double)FP1s[2*TT+1];
					temp4=(double)FP1s[2*TT]*(double)FVz1s[2*TT+1];

					tempz=(temp1+temp2)*cos((double)phZ[TT-1]/180.0*PI)-(temp3-temp4)*sin((double)phZ[TT-1]/180.0*PI);
					Sz[TT-1]=Sz[TT-1]+tempz;
						
					temp1=(double)FP1s[2*TT]*(double)FP1s[2*TT];
					temp2=(double)FP1s[2*TT+1]*(double)FP1s[2*TT+1];
					Sp[TT-1]=Sp[TT-1]+temp1+temp2;
					CFOK();
					Command_Choose();
				}
			} // for(120s) END

			// 将互谱往回归算，注意，未计算P！！！;
			// (X*2048)^2*120 = (X^2)*503316480;
			CFOK();
			Command_Choose();
			for(N=1;N<=NF;N++)             
			{
			 	Sx32[N-1]=(long long)Sx[N-1]>>28;
				Sy32[N-1]=(long long)Sy[N-1]>>28;
				 	
				if (Sx[N-1]>0)  //将互谱结果转化为16bit，方便上传；
				 	Sx16[N-1]=(short)sqrt((long long)Sx[N-1]>>28);
				else
					Sx16[N-1]=0-(short)sqrt(abs((long long)Sx[N-1]>>28));
				if (Sy[N-1]>0)
				 	Sy16[N-1]=(short)sqrt((long long)Sy[N-1]>>28);
				else
					Sy16[N-1]=0-(short)sqrt(abs((long long)Sy[N-1]>>28));
				if (Sz[N-1]>0)
				 	Sz16[N-1]=(short)sqrt((long long)Sz[N-1]>>28);
				else
					Sz16[N-1]=0-(short)sqrt(abs((long long)Sz[N-1]>>28));
				Sp16[N-1]=(short)sqrt((long long)Sp[N-1]>>28);

				CFOK();
				Command_Choose();

				if (N>=10)  // 角度谱只计算10~200Hz
				{
					 if ((Sz16[N-1] < C1) && (S1Int==1))  //！！！！要补上！！！！！这个结合门限在看看研究下啊，对比matlab，看看对不对啊；
					 {
					 	OBJ_Int=1;
					 	temp1=(double)Sx32[N-1]*(double)Sx32[N-1];
					 	temp2=(double)Sy32[N-1]*(double)Sy32[N-1];
					 	Sr16=sqrt(temp1+temp2);
					 	theta=atan2(Sy32[N-1],Sx32[N-1])*180/PI+0.5;       // 0.5是为了取整时等效实现四舍五入；
					 	if (theta<0)           // 实际上是按照360取mod；
					 	{
					  		theta=360+theta;
					 	}
					 	count_theta[(int)theta-1]=Sr16+count_theta[(int)theta-1];   //??????出现0度怎么办？？？？？
					 }
				//	mod();
				}
			}

            
			//R232_To_sync(Seri,bytesSize);
				
			S1Int=0;
			if (OBJ_Int==1)				
			{
				 Sf[2003]=0x31;
				 Target1=count_theta[0];
				 for(N=0;N<361;N++)                  // 找最大值；角度存于T1；
				 {
				 	 if (Target1<=count_theta[N])
					 {	
					 	 T1=N+1;         // 目标角度；
						 Target1=count_theta[N];
					 }
				 }
				 if (Target1>C2)
				 {
				 	 SendInt=1;
				 	 SendOBJ();   // 发送目标信息；//////////////////////////////////////////////////////////////////////
					 SendInt=0;
				 }
			} 
			CFOK();   
			Command_Choose();
			
			for(N=1;N<=4;N++)          //将互谱结果写入串口传输数据；
			{
				 SerP=12;   SerX=492;   SerY=972;   SerZ=1452;
				 for(TT=1;TT<=50;TT++)
				 {
				  	Sf[SerP+(N-1)*120]  =  Sp16[TT-1+(N-1)*50] & 0xFF;
				  	Sf[SerP+(N-1)*120+1]= (Sp16[TT-1+(N-1)*50]>>8) & 0xFF;
					SerP=SerP+2;

					Sf[SerX+(N-1)*120]  =  Sx16[TT-1+(N-1)*50] & 0xFF;
					Sf[SerX+(N-1)*120+1]= (Sx16[TT-1+(N-1)*50]>>8) & 0xFF;
					SerX=SerX+2;

					Sf[SerY+(N-1)*120]  =  Sy16[TT-1+(N-1)*50] & 0xFF;
					Sf[SerY+(N-1)*120+1]= (Sy16[TT-1+(N-1)*50]>>8) & 0xFF;
					SerY=SerY+2;

					Sf[SerZ+(N-1)*120]  =  Sz16[TT-1+(N-1)*50] & 0xFF;
					Sf[SerZ+(N-1)*120+1]= (Sz16[TT-1+(N-1)*50]>>8) & 0xFF;
					SerZ=SerZ+2;
				}
			}
			CFOK();
			Command_Choose();
			Ser2MinC();               // 将2分钟计算计数值写入串口数据；
			for(N=1;N<=17;N++)        // 校验数据；
			{
			 	c1=Sf[1+(N-1)*120];
			 	for(TT=2;TT<=115;TT++)        // 亦或计算；
				{
				 	c1=Sf[TT+(N-1)*120]^c1;
				}
				c2=c1 & 0x0F;           // 取低4位；
				c3=(c1>>4) & 0x0F;      // 取高4位；

				Sf[118+(N-1)*120]=C2A(c2);   // 校验位；
				Sf[117+(N-1)*120]=C2A(c3);
			}
			//SendOBJ();
			//if (SendInt==1)
			//{
			// 	SendOBJ();
			//	SendInt=0;
			//}
			CFOK();
			Command_Choose();
			SendData();
			
			N=0;
		//	*(volatile unsigned int *)GPVAL=0x00000000;    // 测试端口GPIO_1置0
			//} // if(S1Int) END;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		} // if(Ready) END 		   
	} // while END
} // main END
  

long SRAM_Test()
{
 	#define  NSRAM 16384                // 调试片外SRAM; 定义16k个int点，SRAM有1M个short点，点数上是64倍的关系
 	int ii,kk,jj; 
	int Ram1[NSRAM];                    // 定义16k个int点，SRAM有1M个short点，点数上是64倍的关系
	int *P1 = Ram1, *P_temp;
	long sum=0;
	
	P_temp = P1;
	for (jj=0; jj<NSRAM; jj++)
	{
	 	*P_temp++=0;
	}
	
	for (ii=0; ii<1024*1024; ii++)      // 写满一片SRAM，共2^20个short点
	{	  
	 	*(int16 *)(SRAM0_add0+ii*2) = 0xa5a5;
		*(int16 *)(SRAM1_add0+ii*2) = 0x5a5a;
	    //*P_temp++ = *(int16 *)(SRAM0_add0+ii*2) ;
	}

	P_temp = P1;
	ii = 0;
	for (kk=0; kk<1024*1024; kk++)
	{
		*P_temp++ = (*P_temp) +  *(int16 *)(SRAM0_add0+kk*2);
		ii++;
		if (ii == NSRAM)
		{
			ii=0;
			P_temp = P1;
		}
	}
	for (ii=0; ii<NSRAM; ii++)          // 对于跏贾�xa5a5，结果为0xfa5a500000;
		sum = sum + *P_temp++;          // 对于初始值0x5a5a，结果为0x5a5a00000;;
	
	return sum;
}

void CFZERO_Test()
{
 	int k;                              // CF卡罗经扇区写零用
    for(k=3840;k<=4095;k++)             // 注意序号索引是从0~n-1
	{
		Ad_Data_buffer0[k]=0x0;
		Ad_Data_buffer1[k]=0x4ccc;
	}
}


/*void SRAM_Main()
{
 	;
}*/

       
void CF_Main()
{
//	int Ad_buffer_index = 0;
	int Ad_Sector_num = 0;								
						
	if((Edma13_Int_flag == 1) && (Edma16_Int_flag == 0))
	{				
		int mm=0;
		CF_write_count++;
		RS485_to_KELLER(KELLER_TX2,5);    //发送读取压力的命令
		for(N_Del=0;N_Del<1400000;N_Del++)
	   		asm(" nop ");
		Cell_Num_index++;	      // 缓冲区索引
		Edma13_Int_flag = 0;
		Edma_Int_flag = 0;

		/* 这个位置以下添加函数SRAM_Main------------------------------------------------------------------------ */
		
		// SRAM_Main(); 别忘了另一个buffer也要这样；
		for(CountBuffer=0;CountBuffer<NBUFFER;CountBuffer++)
		{
		 	*(int16 *)(SRAM_P + (CountBuffer+(Count2Min-1)*NBUFFER)*2) = Ad_Data_buffer0[CountBuffer*8+0];
			*(int16 *)(SRAM_VX+ (CountBuffer+(Count2Min-1)*NBUFFER)*2) = Ad_Data_buffer0[CountBuffer*8+1];
			*(int16 *)(SRAM_VY+ (CountBuffer+(Count2Min-1)*NBUFFER)*2) = Ad_Data_buffer0[CountBuffer*8+2];
			*(int16 *)(SRAM_VZ+ (CountBuffer+(Count2Min-1)*NBUFFER)*2) = Ad_Data_buffer0[CountBuffer*8+3];			
		}
		Count2Min++;
		

		/* 这个位置以上添加函数SRAM_Main------------------------------------------------------------------------ */
	    for(;Ad_Sector_num<3840;Ad_Sector_num++)                // 15*256=3840,15个扇区的数据写入CF
		{
			Ad_big_buffer[((CF_write_count-1) * Data_banyi_num)+(Ad_Sector_num * 2)]= Ad_Data_buffer0[Ad_Sector_num];
			Ad_big_buffer[((CF_write_count-1) * Data_banyi_num)+(Ad_Sector_num * 2)+1]= Ad_Data_buffer0[Ad_Sector_num]>>8;
		}

		for(mm=0;mm<512;mm++)     // 罗经数据写CF卡；
		{
		 	Ad_big_buffer[(CF_write_count * Data_banyi_num)-512+mm] = Compass_buffer[mm];
		}

		//for(N_Del=0;N_Del<600000;N_Del++)
	   		//asm(" nop ");
		if(KELLER_buffer[1]==0x49)        //判断回复命令是否正确
		{
		 	for(mm=64;mm<73;mm++)
			{
	   			Ad_big_buffer[(CF_write_count * Data_banyi_num)-512+mm]=KELLER_buffer[mm-64];
				KELLER_P[mm-64]=KELLER_buffer[mm-64];
			}
			memset(KELLER_buffer,0x0,10*sizeof(char));
	    }

		RS485_to_KELLER(KELLER_TX3,5);    //发送开启命令到KELLER
		for(N_Del=0;N_Del<1400000;N_Del++)
	   		asm(" nop ");
    	if(KELLER_buffer[1]==0x49)      //判断回复命令是否正确
		{ 
	   		for(mm=73;mm<82;mm++)
			{
	   			Ad_big_buffer[(CF_write_count * Data_banyi_num)-512+mm]=KELLER_buffer[mm-73];
				KELLER_T[mm-73]=KELLER_buffer[mm-73];
			}
			memset(KELLER_buffer,0x0,10*sizeof(char));
		}
		                		               
		/*for(;Ad_buffer_index < (Ad_Sector_num * 256);Ad_buffer_index++)      // 1个扇区的罗经数据写入CF卡缓冲区
		{
			CF_Wsector_buffer[j] = Ad_Data_buffer0[Ad_buffer_index] & 0xFF;
			CF_Wsector_buffer[j+1] = (Ad_Data_buffer0[Ad_buffer_index] >> 8) & 0xFF;
			j=j+2;
		}*/

	

	//	CF_WriteSector(Compass_Sector_num);
	//	Compass_Sector_num++;
		
		if(Compass_buffer[10]==0x2E)    // 提取罗经值；
		{
		 	CompassFix[CompassNum]=Compass_buffer[9]-0x30+((float)Compass_buffer[11]-0x30)/10.0;
			CompassNum++;
		}
		else if(Compass_buffer[11]==0x2E)
		{
		 	CompassFix[CompassNum]=(Compass_buffer[9]-0x30)*10+Compass_buffer[10]-0x30+((float)Compass_buffer[12]-0x30)/10.0;
			CompassNum++;
		}
		else if(Compass_buffer[12]==0x2E)
		{
		 	CompassFix[CompassNum]=(Compass_buffer[9]-0x30)*100+(Compass_buffer[10]-0x30)*10+Compass_buffer[11]-0x30+((float)Compass_buffer[13]-0x30)/10.0;
			CompassNum++;
		}
		else
		{
		 	CompassFix[CompassNum]=0;  // 如若未能成功提取，置为-1；
			CompassNum++;
		} 
		
		if (Count2Min > 500)           // 整个功能(for+if)与2个for循环嵌套等效；
		{
		 	ReadyInt=1;                // 2分钟数据完毕，信号处理标志位置位；
			Count2Min=1;
			CompassNum=0;
			CC++;
		}					
	}
					
	else if((Edma16_Int_flag == 1) && (Edma13_Int_flag == 0))
	{
		int mm=0;
		CF_write_count++;
		RS485_to_KELLER(KELLER_TX2,5);    //发送读取压力的命令
		for(N_Del=0;N_Del<1400000;N_Del++)
	   		asm(" nop ");
		Cell_Num_index++;	      // 缓冲区索引
		Edma16_Int_flag = 0;
		Edma_Int_flag = 0;					
		
		/* 这个位置以下添加函数SRAM_Main------------------------------------------------------------------------ */
		
		// SRAM_Main(); 别忘了另一个buffer也要这样；
		for(CountBuffer=0;CountBuffer<NBUFFER;CountBuffer++)
		{
		 	*(int16 *)(SRAM_P + (CountBuffer+(Count2Min-1)*NBUFFER)*2) = Ad_Data_buffer1[CountBuffer*8+0];
			*(int16 *)(SRAM_VX+ (CountBuffer+(Count2Min-1)*NBUFFER)*2) = Ad_Data_buffer1[CountBuffer*8+1];
			*(int16 *)(SRAM_VY+ (CountBuffer+(Count2Min-1)*NBUFFER)*2) = Ad_Data_buffer1[CountBuffer*8+2];
			*(int16 *)(SRAM_VZ+ (CountBuffer+(Count2Min-1)*NBUFFER)*2) = Ad_Data_buffer1[CountBuffer*8+3];			
		}
		Count2Min++;
		/*if (Count2Min > 500)
		{
		 	ReadyInt=1;                // 2分钟数据完毕，信号处理标志位置位；
			Count2Min=1;
		}*/

		/* 这个位置以上添加函数SRAM_Main------------------------------------------------------------------------ */

		for(;Ad_Sector_num<3840;Ad_Sector_num++)                // 15*256=3840,15个扇区的数据写入CF
		{
			Ad_big_buffer[((CF_write_count-1) * Data_banyi_num)+(Ad_Sector_num * 2)]= Ad_Data_buffer1[Ad_Sector_num];
			Ad_big_buffer[((CF_write_count-1) * Data_banyi_num)+(Ad_Sector_num * 2)+1]= Ad_Data_buffer1[Ad_Sector_num]>>8;
		}
						
		for(mm=0;mm<512;mm++)
		{
		 	Ad_big_buffer[(CF_write_count * Data_banyi_num)-512+mm] = Compass_buffer[mm];
		}

		if(KELLER_buffer[1]==0x49)        //判断回复命令是否正确
		{
		 	for(mm=64;mm<73;mm++)
	   			Ad_big_buffer[(CF_write_count * Data_banyi_num)-512+mm]=KELLER_buffer[mm-64];

			memset(KELLER_buffer,0x0,10*sizeof(char));
	    }

		RS485_to_KELLER(KELLER_TX3,5);    //发送开启命令到KELLER
		for(N_Del=0;N_Del<1400000;N_Del++)
	   		asm(" nop ");
    	if(KELLER_buffer[1]==0x49)      //判断回复命令是否正确
		{ 
	   		for(mm=73;mm<82;mm++)
	   			Ad_big_buffer[(CF_write_count * Data_banyi_num)-512+mm]=KELLER_buffer[mm-73];

			memset(KELLER_buffer,0x0,10*sizeof(char));
		}
		/*for(;Ad_buffer_index < (Ad_Sector_num * 256);Ad_buffer_index++) 
		{
			CF_Wsector_buffer[j] = Ad_Data_buffer1[Ad_buffer_index] & 0xFF;
			CF_Wsector_buffer[j+1] = (Ad_Data_buffer1[Ad_buffer_index] >> 8) & 0xFF;
			j=j+2;
		}*/

	//	CF_WriteSector(Compass_Sector_num);
	//	Compass_Sector_num++;
		
		if(Compass_buffer[10]==0x2E)    // 提取罗经值；
		{
		 	CompassFix[CompassNum]=Compass_buffer[9]-0x30+(float)Compass_buffer[11]/10.0;
			CompassNum++;
		}
		else if(Compass_buffer[11]==0x2E)
		{
		 	CompassFix[CompassNum]=(Compass_buffer[9]-0x30)*10+Compass_buffer[10]-0x30+(float)Compass_buffer[12]/10.0;
			CompassNum++;
		}
		else if(Compass_buffer[12]==0x2E)
		{
		 	CompassFix[CompassNum]=(Compass_buffer[9]-0x30)*100+(Compass_buffer[10]-0x30)*10+Compass_buffer[11]-0x30+(float)Compass_buffer[13]/10.0;
			CompassNum++;
		}
		else
		{
		 	CompassFix[CompassNum]=0;  // 如若未能成功提取，置为-1；
			CompassNum++;
		} 
		
		if (Count2Min > 500)           // 整个功能(for+if)与2个for循环嵌套等效；
		{
		 	ReadyInt=1;                // 2分钟数据完毕，信号处理标志位置位；
			Count2Min=1;
			CompassNum=0;
			CC++;
		}			
	}
	
	else
	{
	 	Edma16_Int_flag = 0;
		Edma13_Int_flag = 0;
		Edma_Int_flag = 0;
	}
	//SerOK();
		
				
}

void CF_Write()
{   
    if(CF_write_count==14)
	{
		int Data_trans=0;
	 	CF_write_count=0;
		for(;Data_trans<AD_big_Buffer_num;Data_trans++)
		{  
			CF_Wsectors_buffer[Data_trans] = Ad_big_buffer[Data_trans];
		}
		CF_WriteSectors(Data_Sector_num);
	    Data_Sector_num+=224;
	}
	if(Cell_Num_index==File_Cell_num)
	{
		Cell_Num_index=0;
		Write_File_dir(file_count);//写文件的根目录
		file_count++;
		// File_index++;
		if(file_count >= File_num)
		{
			 disableSpecificINT(8);
		     Config_DisableEdma13();
		}
		Data_Sector_num = Data_Sector_num + 640;
//		Compass_Sector_num = Compass_Sector_num + 105640;
	}

}

static int d2i(double d)
{
    if (d >=  2147483647.0) return(int)0x7FFFFFFF;
    if (d <= -2147483648.0) return(int)0x80000000;
    return(int)d;
}


//产生FFT使用的蝶形因子
int gen_twiddle_fft32x32(int *w, int n, double scale)
{
    int i, j, k, s=0, t;

    for (j = 1, k = 0; j < n >> 2; j = j << 2, s++)
    {
        for (i = t=0; i < n >> 2; i += j, t++)
        {
            w[k + 5] = d2i(scale * cos(6.0 * PI * i / n));
            w[k + 4] = d2i(scale * sin(6.0 * PI * i / n));

            w[k + 3] = d2i(scale * cos(4.0 * PI * i / n));
            w[k + 2] = d2i(scale * sin(4.0 * PI * i / n));

            w[k + 1] = d2i(scale * cos(2.0 * PI * i / n));
            w[k + 0] = d2i(scale * sin(2.0 * PI * i / n));

            k += 6;
        }
    }

    return k;
}


void Command_Choose()
{
 	if(Command_Int_flag==1)
	{
 	 	switch (Command_buffer[3])
  	 	{
    	 case 'P' :
     	 if(Command_buffer[4]=='R')
		 {
		 	if(Command_buffer[6]=='C')//////////////////////////////////////////////////
			{
			 	ComInt=5;
				T2R_C(ComPass);
				R232_To_sync(PRD_C,bytesSize);
				ClearCommandBuffer();
				Command_Int_flag=0;
			}
			else if(Command_buffer[6]=='N')
			{
			 	ComInt=6;
				T2R_N(C1);
				R232_To_sync(PRD_N,bytesSize);
				ClearCommandBuffer();
				Command_Int_flag=0;
			}
		 	else
			{
		 	 	ComInt=1;
			 	T2R(DT1,C2);
			 	R232_To_sync(PRD,bytesSize);
			 	ClearCommandBuffer();
			 	Command_Int_flag=0;
			}
		 }
		 else if(Command_buffer[6]=='C')     // 罗经放置偏差设置
		 {
		  	ComInt=3;
			R2T_C();
			R232_To_sync(PWR,bytesSize2);
			ClearCommandBuffer();
			Command_Int_flag=0;
		 }
		 else if(Command_buffer[6]=='N')     // 负线谱设置
		 {
		  	ComInt=4;
			R2T_N();
			R232_To_sync(PWR,bytesSize2);
			ClearCommandBuffer();
			Command_Int_flag=0;
		 }
		 else
		 {
		 	ComInt=2;
			R2T();
			R232_To_sync(PWR,bytesSize2);
			ClearCommandBuffer();
			Command_Int_flag=0;
		 }
      	 break;
    
    	 case 'D' :
      	 WorkInt=1;
		 R232_To_sync(DET,bytesSize2);
		 ClearCommandBuffer();
		 Command_Int_flag=0;
      	 break;

		 case 'F' :
      	 WorkInt=0;
		 R232_To_sync(FED,bytesSize2);
		 ClearCommandBuffer();
		 Command_Int_flag=0;
      	 break;

		 case 'T' :
      	 ComInt=5;
		 R232_To_sync(TST,bytesSize4);
		 ClearCommandBuffer();
		 Command_Int_flag=0;
      	 break;

    	 default : 
    	 R232_To_sync(NO,bytesSize2);
		 ClearCommandBuffer();
		 Command_Int_flag=0;
    	 break;
  		}
	}
}

void ClearCommandBuffer()
{
 	int i;
	for(i=0; i<=22; i++)
	 	Command_buffer[i]=0;
}

void SerSf()
{
 	int N=0;   // 这是后加的，不加的话N也是计数器；
 	memset(Sf,0x30,2040*sizeof(char));
	Sf[10]='0'; 	Sf[11]='1';        // 数据包序号
	Sf[130]='0'; 	Sf[131]='2';
	Sf[250]='0'; 	Sf[251]='3';
	Sf[370]='0'; 	Sf[371]='4';
    Sf[490]='0'; 	Sf[491]='5';
	Sf[610]='0'; 	Sf[611]='6';
	Sf[730]='0'; 	Sf[731]='7';
	Sf[850]='0'; 	Sf[851]='8';
	Sf[970]='0'; 	Sf[971]='9';
	Sf[1090]='1'; 	Sf[1091]='0';
	Sf[1210]='1'; 	Sf[1211]='1';
	Sf[1330]='1'; 	Sf[1331]='2';
    Sf[1450]='1'; 	Sf[1451]='3';
	Sf[1570]='1'; 	Sf[1571]='4';
	Sf[1690]='1'; 	Sf[1691]='5';
	Sf[1810]='1'; 	Sf[1811]='6';
	Sf[1930]='1'; 	Sf[1931]='7';
	for(N=0;N<17;N++)
	{
	 	Sf[0+N*120]='$';
		Sf[3+N*120]='D';
		Sf[4+N*120]='A';
		Sf[5+N*120]='T';
		Sf[116+N*120]='*';
		Sf[119+N*120]='#';
	}
}

void Ser2MinC()  //关于CC的重复使用必须修改，而且需要进行检查，看有没有其他的CC有影响。另，停止时CC要求可以继续计数！！！！！！
{
 	int N=0,C2_Temp=0;  // 这是后加的，不加的话N也是计数器；
 	if(CC<10)
	{
		for(N=0;N<17;N++)
		{
		 	Sf[9+N*120]=CC+0x30;

			Sf[115+N*120]= (char)CountS1+0x30;   // 将5层逻辑序号加入2min的串口数据中
		}
	}
	else if(CC<100)
	{
	 	CC1=CC/10;
		CC2=CC-CC1*10;
		for(N=0;N<17;N++)
		{
		 	Sf[9+N*120]=CC2 +0x30;
			Sf[8+N*120]=CC1+0x30;

			Sf[115+N*120]= (char)CountS1+0x30;   // 将5层逻辑序号加入2min的串口数据中
		}
	}
	else if(CC<1000)
	{
	 	CC1=CC/100;
		CC2=(CC-CC1*100)/10;
		CC3=CC-CC1*100-CC2*10;
		for(N=0;N<17;N++)
		{
		 	Sf[9+N*120]=CC3 +0x30;
			Sf[8+N*120]=CC2+0x30;
			Sf[7+N*120]=CC1+0x30;

			Sf[115+N*120]= (char)CountS1+0x30;   // 将5层逻辑序号加入2min的串口数据中
		}
	}
	else
	{
	 	CC1=CC/1000;
		CC2=(CC-CC1*1000)/100;
		CC3=(CC-CC1*1000-CC2*100)/10;
		CC4=CC-CC1*1000-CC2*100-CC3*10;
		for(N=0;N<17;N++)
		{
		 	Sf[9+N*120]=CC4 +0x30;
			Sf[8+N*120]=CC3+0x30;
			Sf[7+N*120]=CC2+0x30;
			Sf[6+N*120]=CC1+0x30;

			Sf[115+N*120]= (char)CountS1+0x30;   // 将5层逻辑序号加入2min的串口数据中
		}
	}

	// 写第17个数据包；
	for(N=0;N<40;N++)     // 罗经
	{
	 	Sf[1932+N]=Compass_buffer[N];
	}
	Sf[1972]=',';
	for(N=0;N<9;N++)    // 压力，温度
	{
	 	Sf[1973+N]=KELLER_P[N];
		Sf[1982+N]=KELLER_T[N];

	}
	Sf[1991]=',';
	C2_Temp=((int)PVz>>28) & 0x0000000F;    // 垂直能流；
	Sf[1992]=C2A((char)C2_Temp);
	C2_Temp=((int)PVz>>24) & 0x0000000F;
	Sf[1993]=C2A((char)C2_Temp);
	C2_Temp=((int)PVz>>20) & 0x0000000F;
	Sf[1994]=C2A((char)C2_Temp);
	C2_Temp=((int)PVz>>16) & 0x0000000F;
	Sf[1995]=C2A((char)C2_Temp);
	C2_Temp=((int)PVz>>12) & 0x0000000F;
	Sf[1996]=C2A((char)C2_Temp);
	C2_Temp=((int)PVz>>8) & 0x0000000F;
	Sf[1997]=C2A((char)C2_Temp);
	C2_Temp=((int)PVz>>4) & 0x0000000F;
	Sf[1998]=C2A((char)C2_Temp);
	C2_Temp=(int)PVz & 0x0000000F;
	Sf[1999]=C2A((char)C2_Temp);
	Sf[2000]=',';
	Sf[2002]=',';
	Sf[2004]=',';
	Sf[2008]=',';

	if(file_count<10)
	{
	 	Sf[2007]=file_count+0x30;
	}
	else if(file_count<100)
	{
	 	CC1=file_count/10;
		CC2=file_count-CC1*10;
	 	Sf[2007]=CC2+0x30;
		Sf[2006]=CC1+0x30;
	}
	else
	{
	 	CC1=file_count/100;
		CC2=(file_count-CC1*100)/10;
		CC3=file_count-CC1*100-CC2*10;
		Sf[2007]=CC3+0x30;
		Sf[2006]=CC2+0x30;
		Sf[2005]=CC1+0x30;

	}	

}

void OBJ2MinC()  //关于CC的重复使用必须修改，而且需要进行检查，看有没有其他的CC有影响。另，停止时CC要求可以继续计数！！！！！！
{
 	int N=0;  // 这是后加的，不加的话N也是计数器；
 	if(CC<10)
	{
		for(N=0;N<16;N++)
		{
		 	OBJ[28]=CC+0x30;
		}
	}
	else if(CC<100)
	{
	 	CC1=CC/10;
		CC2=CC-CC1*10;
		for(N=0;N<16;N++)
		{
		 	OBJ[28]=CC2 +0x30;
			OBJ[27]=CC1+0x30;
		}
	}
	else if(CC<1000)
	{
	 	CC1=CC/100;
		CC2=(CC-CC1*100)/10;
		CC3=CC-CC1*100-CC2*10;
		for(N=0;N<16;N++)
		{
		 	OBJ[28]=CC3 +0x30;
			OBJ[27]=CC2+0x30;
			OBJ[26]=CC1+0x30;
		}
	}
	else
	{
	 	CC1=CC/1000;
		CC2=(CC-CC1*1000)/100;
		CC3=(CC-CC1*1000-CC2*100)/10;
		CC4=CC-CC1*1000-CC2*100-CC3*10;
		for(N=0;N<16;N++)
		{
		 	OBJ[28]=CC4 +0x30;
			OBJ[27]=CC3+0x30;
			OBJ[26]=CC2+0x30;
			OBJ[25]=CC1+0x30;
		}
	}
}

char C2A(char C)
{
 	if(C>9)
		C=C+55;
	else
		C=C+0x30;

	return C;
}

void SendData()
{
 	enableSpecificINT(15);

    *(unsigned volatile int *)TIMER1_CTRL=0x00000215;   // 关计数器
    *(unsigned volatile int *)TIMER1_PRD=0x8F0D180;     // 2s计数
    *(unsigned volatile int *)TIMER1_COUNT=0x0;         // 清零
    *(unsigned volatile int *)TIMER1_CTRL =0x000002D5;  // 开计数器；
}

void CFOK()
{
 	if(Edma_Int_flag==1)        // Buffer数据写入CF卡及SRAM；
	{
		 #ifdef CFTIME
	     *(volatile unsigned int *)GPVAL=0x00000002;    // 测试端口GPIO_1置1；
		 #endif

		 CF_Main();
		 CF_Write();

		 //SerOK();
		 //Command_Choose();

		 #ifdef CFTIME
		 *(volatile unsigned int *)GPVAL=0x00000000;    // 测试端口GPIO_1清0；
		 #endif
	}
}

void T2R(double DT1,int C2)            // 参数到串口；
{
 	int DT1_Temp1=0,C2_Temp=0,N=0;
	double DT1_Temp2=0 ;
	char c1=0,c2=0,c3=0;
	DT1_Temp1=(DT1*100-100)/10;
	DT1_Temp2=DT1*100 - 100;
	DT1_Temp2=(int)(DT1*100 - 100 - DT1_Temp1*10);
	PRD[6]=(char)DT1_Temp1+0x30;
	PRD[7]=(char)DT1_Temp2+0x30;

	C2_Temp=(C2>>28);
	C2_Temp=(C2>>28) & 0x0000000F;
	PRD[10]=C2A((char)C2_Temp);
	C2_Temp=(C2>>24) & 0x0000000F;
	PRD[11]=C2A((char)C2_Temp);
	C2_Temp=(C2>>20) & 0x0000000F;
	PRD[12]=C2A((char)C2_Temp);
	C2_Temp=(C2>>16) & 0x0000000F;
	PRD[13]=C2A((char)C2_Temp);
	C2_Temp=(C2>>12) & 0x0000000F;
	PRD[14]=C2A((char)C2_Temp);
	C2_Temp=(C2>>8) & 0x0000000F;
	PRD[15]=C2A((char)C2_Temp);
	C2_Temp=(C2>>4) & 0x0000000F;
	PRD[16]=C2A((char)C2_Temp);
	C2_Temp=C2 & 0x0000000F;
	PRD[17]=C2A((char)C2_Temp);

	c1=PRD[1];
	for(N=2;N<=17;N++)        // 校验数据；
	{
		c1=PRD[N]^c1;
	}
	c2=c1 & 0x0F;           // 取低4位；
	c3=(c1>>4) & 0x0F;      // 取高4位；

	PRD[20]=C2A(c2);   // 校验位；
	PRD[19]=C2A(c3);
	
}

void T2R_C(float ComPass)    //罗经偏置到串口
{
 	short temp1=0,temp2=0,temp3=0,N=0;
	char c1=0,c2=0,c3=0;
	temp1=(short)ComPass/100+0x30;
 	PRD_C[10]=(char)temp1;
	temp2=((short)ComPass-(temp1-0x30)*100)/10+0x30;
	PRD_C[11]=(char)temp2;
	temp3=(short)ComPass-(temp1-0x30)*100-(temp2-0x30)*10+0x30;
	PRD_C[12]=(char)temp3;

	c1=PRD_C[1];
	for(N=2;N<=17;N++)        // 校验数据；
	{
		c1=PRD_C[N]^c1;
	}
	c2=c1 & 0x0F;           // 取低4位；
	c3=(c1>>4) & 0x0F;      // 取高4位；

	PRD_C[20]=C2A(c2);   // Ｑ槲唬�
	PRD_C[19]=C2A(c3);
} 

void T2R_N(short C1)    //负线谱到串口
{
 	short temp1=0,temp2=0,N=0;
	char c1=0,c2=0,c3=0;
	temp1=0-C1;          //  最后是减法，因为是负谱，调试的时候是加法，要注意！！！！
	temp2=(temp1>>12) & 0x0000000F;
	PRD_N[10]=C2A((char)temp2);
	temp2=(temp1>>8) & 0x0000000F;
	PRD_N[11]=C2A((char)temp2);
	temp2=(temp1>>4) & 0x0000000F;
	PRD_N[12]=C2A((char)temp2);
	temp2=temp1 & 0x0000000F;
	PRD_N[13]=C2A((char)temp2);

	c1=PRD_N[1];
	for(N=2;N<=17;N++)        // 校验数据；
	{
		c1=PRD_N[N]^c1;
	}
	c2=c1 & 0x0F;           // 取低4位；
	c3=(c1>>4) & 0x0F;      // 取高4位；

	PRD_N[20]=C2A(c2);   // 校验位；
	PRD_N[19]=C2A(c3);
}

void R2T()            // 串口到参数；
{
 	int DT1_Temp1=0,DT1_Temp2=0,C2_Temp1=0,C2_Temp2=0,N=0,OK_Flag=1;

	DT1_Temp1=Command_buffer[6]-0x30;
	DT1_Temp2=Command_buffer[7]-0x30;
	DT1 = (100.0 + (double)DT1_Temp1*10.0 + (double)DT1_Temp2)/100.0;

	for(N=10;N<=17;N++)
	{
	 	if ((Command_buffer[N]>=65) && (Command_buffer[N]<=70))
	 	 	C2_Temp1=Command_buffer[N]-55;
		else if ((Command_buffer[N]>=0x30) && (Command_buffer[N]<=0x39))
			C2_Temp1=Command_buffer[N]-0x30;
		else
			OK_Flag=0;

		if (OK_Flag==1)
		{
		 	C2_Temp2 = C2_Temp2 + C2_Temp1*pow(16,(17-N));
		}
	}
	C2=C2_Temp2;
}

void R2T_C()       // 串口配置罗经偏置到参数
{
 	int DT1_Temp1=0,DT1_Temp2=0,DT1_Temp3=0;

	DT1_Temp1=Command_buffer[10]-0x30;
	DT1_Temp2=Command_buffer[11]-0x30;
	DT1_Temp3=Command_buffer[12]-0x30;
	ComPass = DT1_Temp1*100 + DT1_Temp2*10 + DT1_Temp3;
}

void R2T_N()            // 串口负线谱到参数；
{
 	int C1_Temp1=0,C1_Temp2=0,N=0,OK_Flag=1;

	for(N=10;N<=13;N++)
	{
	 	if ((Command_buffer[N]>=65) && (Command_buffer[N]<=70))
	 	 	C1_Temp1=Command_buffer[N]-55;
		else if ((Command_buffer[N]>=0x30) && (Command_buffer[N]<=0x39))
			C1_Temp1=Command_buffer[N]-0x30;
		else
			OK_Flag=0;

		if (OK_Flag==1)
		{
		 	C1_Temp2 = C1_Temp2 + C1_Temp1*pow(16,(13-N)); // 和C2门限幂次不一样，因为长度是4字节和2字节；
		}
	}
	C1=0-(short)C1_Temp2;        // 最后是减法，因为是负值哦，调试的时候用加法，要注意！！1
}

void SerOBJ()
{
 	memset(OBJ,0x30,50*sizeof(char));
	OBJ[0]='$';
	OBJ[3]='O';
	OBJ[4]='B';
	OBJ[5]='J';
	OBJ[21]='2';
	OBJ[22]='0';
	OBJ[23]='1';
	OBJ[24]='2';
	OBJ[35]='1';
	OBJ[36]=':';
	OBJ[40]=',';
	OBJ[46]='*';
	OBJ[49]='#';
}

void SendOBJ()
{
 	int t1=0,t2=0,t3=0;
	char c1=0,c2=0,c3=0;
 	t1=T1/100;
	t2=(T1-t1*100)/10;
	t3=T1-t1*100-t2*10;

	OBJ[37]=C2A((char)t1);
	OBJ[38]=C2A((char)t2);
	OBJ[39]=C2A((char)t3);

	OBJ2MinC();

	c1=OBJ[1];
	for(N=2;N<=45;N++)        // 校验数据；
	{
		c1=OBJ[N]^c1;
	}
	c2=c1 & 0x0F;           // 取低4位；
	c3=(c1>>4) & 0x0F;      // 取高4位；

	OBJ[48]=C2A(c2);   // 校验位；
	OBJ[47]=C2A(c3);

	R232_To_sync(OBJ,bytesSize3);
	for(N_Del=1;N_Del<2000000;N_Del++)
	   asm(" nop ");
	R232_To_sync(OBJ,bytesSize3);
	for(N_Del=1;N_Del<2000000;N_Del++)
	   asm(" nop ");
	R232_To_sync(OBJ,bytesSize3);
	for(N_Del=1;N_Del<2000000;N_Del++)
	   asm(" nop ");

	OBJ_Int=0;
}

void SerOK()
{
 	char FIFO_Temp=0;
	FIFO_Temp= *(unsigned  char *)(EMB_CE0+SYNC_A5+LSR_7);
    FIFO_Temp &= 0x20;
 	if( Ser_OK==1 )
 	{
 	 	int i,j;
     
    	for(i=bytesSize1;i>0;i--)
    	{	
     		*(unsigned char *)(EMB_CE0+SYNC_A5+THR_7) = Sf[SerCount];//写字节到754
			SerCount++;
			for(j=0;j<7500;j++)
	   			asm(" nop ");
	   		CFOK(); 	  
    	}

		Ser_OK=0;
 	 	
 	 	count0--;
    	if (count0==0)
    	{
     		disableSpecificINT(15);
			count0=17;
			SerCount=0;
			Sf[2001]=0x30;
			Sf[2003]=0x30;
    	}
 	 	
 	}
}

void ClearOrderFIFO()
{
 	int iiii=0,j=0;
	int temp;
 	for (;iiii<64;iiii++)
    {
    	for(j=0;j<100;j++)
	   		asm(" nop ");
		temp= *(unsigned  char *)(EMB_CE0+SYNC_A5+LSR_7);
		if(temp==0)
		  { iiii=64; }
    	temp=*(unsigned  char *)(EMB_CE0+SYNC_A5+RHR_7);
	    //Sync_Buffer_index++;
		       
    }
    iiii=0;
}


void ClearCompassFIFO()
{
 	int iiii=0,j=0;
	int temp;
 	for (;iiii<64;iiii++)
    {
    	for(j=0;j<100;j++)
	   		asm(" nop ");
		temp= *(unsigned  char *)(EMB_CE0+LJ_A4+LSR_7);
		if(temp==0)
		  { iiii=64; }
    	temp=*(unsigned  char *)(EMB_CE0+LJ_A4+RHR_7);
	    //Sync_Buffer_index++;
		       
    }
    iiii=0;
}

