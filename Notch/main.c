#include "msp430f6638.h"
#include "function.h"
#include "type.h"
#include "table.h"
#include "init.h"

#define COMMAND_LEN 26	//串口指令长度
int notchIteraStep=0;   //notch滤波器迭代参数
int sinTabStep = 0;		//正弦表步长
int wc=0;				//滤波器参数wc
int ws=0;				//滤波器参数ws
int err=0;				//残差
const int*sinAdd = table;			//滤波器信号1
const int*cosAdd = table + 0x800;	//滤波器信号2
unsigned char uartBuffer[COMMAND_LEN-1] = {0};	//串口命令
unsigned char uartCnt = 0;						//串口数据计数
unsigned char uartCommand = 0;					//是否收到串口命令串口命令
unsigned char uartFlag = 0;						//串口标志
unsigned char spiBuffer[COMMAND_LEN] = {0};		//spi缓冲
unsigned char spiBuffer1[COMMAND_LEN-2] = {0};	//
#define COMM "Communication "					//串口发送"通讯中"指令字符
#define SUCCESS "Success\n"						//串口发送"成功"指令字符
#define FAULT "Fault\n"							//串口发送"失败"指令字符
uint sum;										//没用到
int resFilter;									//当前滤波器的输出
uchar stateChange;								//是否检测到信号
long widthCnt;									//检测到的信号点数
uint noiseCnt;									//检测到的噪声点数
uint offCnt;									//没用到
uint offProCnt;									//没用到
uint results;									//没用到
int envelAveOut;            					//包络平均输出
int envelOut;									//包络输出
uint ADCtemp;									//ADC采样值
uint ADC_temp[1024]={0};						//没用到
int ADCin;										//ADC采样值转为有符号数
int output[1024] = {0};							//没用到
uint out_count = 0;								//没用到
#define ADDRBONDAR (table + 0x2000)				//addr_bound？地址门限，过该值，正弦表重置
#define SINADDRORG (table)						//没用到
#define COSADDRORG (table + 0x800)				//没用到

void main()
{
	int i,j = 0;
	int checkFlag = 0;			//
	WDTCTL = WDTPW + WDTHOLD;		//关闭看门狗
	OMAP_POWER_OFF;				//初始时OMAP关闭
	TRANS_POWER_OFF;				//初始时关闭
	MOTO_POWER_OFF;				//初始时关闭
	MOTO_RELEASE_OFF;				//初始时关闭
	initClock();					//初始化时钟
	initPort();					//初始化端口
	initVar();					//初始化变量
	initADC();					//初始化ADC
	initUart();					//初始化串口
	initSpi();					//初始化SPI
	__bis_SR_register(GIE);		//使能全局中断

	//一直进行检测处理
	while(1)
	{
		if(uartCommand)
		{	//检测收到串口命令
			uartCommand = 0;		//串口命令清零，方便下一次进行串口命令检测
			spiBuffer[0] = 0x5A;	//spi buffer[0]设置
			for(j = 1;j < COMMAND_LEN;j++)
			{
				spiBuffer[j] = uartBuffer[j-1];		//将串口数据赋值给spi buffer
			}
			spiSend(spiBuffer,COMMAND_LEN);			//串口数据通过spi发出
			uartSend(COMM,14);						//串口发送，通讯中
			checkFlag = dspTo430();					//检测dsp给430返回数据
			if(!checkFlag)
			{	//检测标志为0，通讯成功
				uartSend(SUCCESS,8);				//串口发出，成功
			}
			else
			{	//检测标志不为0，通讯失败
				uartSend(FAULT,6);					//串口发出，失败
			}
			uartSend(spiBuffer1,(COMMAND_LEN-2));	//串口发送数据
		}
		if( stateChange == 1)
		{	//检测到信号
			stateChange = 0;
		}
		else
		{	//没有检测到信号
			if( ADC12IFG & BIT1 )				//ADC中断标志位
			{
				ADCtemp = ADC12MEM1;				//ADC采数传出
				ADCin = (int)(ADCtemp - 0x800); 	//无符号数变成有符号数
				ADCin = ADCin *15;					//ADC采样值*15
				if(!notchFilter(ADCin))				//进行north滤波并判断是否检测到信号
					// output[out_count] = resFilter;
					// ADC_temp[out_count] = ADCtemp;
					// out_count++;
					// if(out_count == 1024)
					// out_count = 0;
				{	//检测到信号点状态
					noiseCnt = 0;				//噪声点数清零
					widthCnt += 1;				//检测信号点数加一
					if(widthCnt >= 80000)		//如果检测到80000个信号点数，确定检测到信号
					{
						powerOnAct();			//检测到信号之后进行的操作
					}
				}
				else
				{	//未检测到信号点
					noiseCnt += 1;				//噪声点数加一
					if(noiseCnt >= 20)			//连续20次检测到噪声点
					{
						noiseCnt = 0;			//噪声点数清零
						widthCnt = 0;			//信号点数清零
					}
				}
			}//if(uartCommand),else,if( ADC12IFG & BIT1 )
		}//if(uartCommand),else
	}//if(uartCommand)
}//main


//串口发送数据
#pragma vector=USCI_A1_VECTOR
__interrupt void uartIsr(void)
{
  unsigned char temp = 0;
   temp = UCA1RXBUF;
   if((temp == 0x5A) && (uartFlag == 0))
   {
     uartFlag = 1;
     uartCnt = 0;
     uartCommand = 0;
   }
   else if(uartFlag == 1)
   {
     uartBuffer[uartCnt] = temp;
     uartCnt++;
     if(uartCnt == (COMMAND_LEN-1))
     {
       uartCnt = 0;
       uartCommand = 1;
       uartFlag = 0;
     }
   }
}

//检测到信号
void powerOnAct()
{
	stateChange = 1;		//状态改变为1，即检测到信号
	ADC12CTL0 &= ~ADC12ON;	//中断ADC采样
	OMAP_POWER_ON;			//打开OMAP
	TRANS_POWER_ON;			//开始传输
	noiseCnt = 0;			//噪声检测点清零
	widthCnt = 0;			//信号检测点清零
}


//notch滤波器函数
int notchFilter( int ADCin)
{
	//计算本时刻，即k时刻的输出信号
	//resFilter=wc*coswk+ws*sinwk
	MPYS = wc;  			//wc赋值给硬件乘法器
	OP2 = *cosAdd;			//wc*coswk
	MACS = ws;				//ws赋值给乘加器
	OP2 = *sinAdd;			//wc*coswk +ws*sinwk
	resFilter = RESHI;		//结果的高字节，将wc*coswk +ws*sinwk结果赋值给resFilter，即此时滤波器的输出

	//计算此时刻，即k时刻的残差
	//err(t)=x(t)-resFilter
	err = (ADCin) - resFilter;

	//计算下一时刻，即k+1时刻的滤波器参数wc(k+1)与k时刻滤波器参数wc(k)之差
	//err*cos(w*k)*step
	MPYS = err;
	OP2 = *cosAdd;
	cosAdd = cosAdd + sinTabStep;
	//更新正余弦表
	if( cosAdd >= ADDRBONDAR)
		cosAdd = cosAdd - 0x2000;
	//计算下一时刻，即k+1时刻校正后的滤波器参数wc(k+1)=wc(k)+u*err(k)*cos(w*k)
	MPYS = RESHI;
  	OP2 = notchIteraStep;
  	wc = wc + RESHI;

	//计算下一时刻，即k+1时刻的滤波器参数wk(k+1)与k时刻滤波器参数wk(k)之差
	//err*sin(w*k)*step
  	MPYS = err;
  	OP2 = *sinAdd;
  	sinAdd = sinAdd + sinTabStep;
  	//更新正余弦表
  	if( sinAdd >= ADDRBONDAR)
  		sinAdd = sinAdd - 0x2000;
	//计算下一时刻，即k+1时刻校正后的滤波器参数wk(k+1)=wk(k)+u*err(k)*sin(w*k)
  	MPYS = RESHI;
  	OP2 = notchIteraStep;
  	ws = ws + RESHI;

  	MPYS = wc;
  	OP2 = wc;		//wc*wc
  	MACS = ws;
  	OP2 = ws;		//wc*wc+ws*ws

  	envelOut = RESHI;	//得到该时刻的包络幅度

  	envelOut = envelOut - envelAveOut;
  	envelOut = envelOut >> 3;	//右移3位，除以8
  	envelAveOut = envelAveOut + envelOut;
  	if((envelAveOut) >= 3)
  		return 0;
  	else
  		return -1;
 }

//spi发送数据
void spiSend(unsigned char*data,int len)
{
  unsigned char i = 0;
  unsigned char temp = 0;
  unsigned int j=0;
  for (i = 0;i < len;i++)
  {
    while(!(UCB1IFG&UCTXIFG));
    UCB1TXBUF = data[i];
    while(!(UCB1IFG&UCRXIFG));
    if((i>=2) && (i<=len-1))
    {
      spiBuffer1[i-2] = UCB1RXBUF;
    }
    else
    {
      temp = UCB1RXBUF;
    }
    for(j=60000;j>0;j--);
  }
}

//dsp发送给430的数据
int dspTo430(void)
{
  int i,j=0;
  for(i = 0; i < COMMAND_LEN-2;i++)
  {
    if(spiBuffer[i+1] == spiBuffer1[i])
      j++;
  }
  if( j == (COMMAND_LEN-2))
    return 0;
  else
    return -1;
}

//串口发送数据
void uartSend(unsigned char* data,int len)
{
  unsigned char i = 0;
  for (i = 0;i < len;i++)
  {
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = data[i];
  }
}





/********************************************************/
/* 变量值初始化 */
void initVar()
{
  sinTabStep = 0x800;			//正弦表的步进数
  notchIteraStep = 1000;		//notch滤波器的迭代参数
  ws = *(sinAdd + sinTabStep);	//初始化滤波器ws
  wc = *(cosAdd + sinTabStep);	//初始化滤波器wc
  err = 0;						//滤波器的残差
  resFilter = 0;				//当前时刻滤波器的输出
//  sinAdd = table;
 // cosAdd = (table + 0x1000);
  stateChange = 0;				//状态改变
  widthCnt = 0;					//检测到的信号点数
  noiseCnt = 0;					//检测到的噪声点数
  offCnt = 0;					//
  offProCnt = 0;				//
  envelAveOut = 0;				//包络的平均输出
}
