
/*******************************************************************************
* FILENAME
*   c6xdspinit.c
*
* DESCRIPTION
*   c641x Header File
*
*******************************************************************************/
#include "c6xdspinit.h"

void config_chip()		
{ 
	*(volatile unsigned int *)CCFG=0x00000000;  // L2用作全SRAM
	/*************************************************************************/
	/*                         CONFIG EMIFA REGISTER                         */
	/*************************************************************************/
	/*void 函数主要用来配置SRAM,EMIFA的时钟为1/4=CPU时钟=150MHz*/
	/*读选通=写选通=9 读操作=写操作=1 读保持=写保持=1*/	 
	// EMIFA是64位数据总线接口；	   
	*(volatile unsigned int *)EMIFA_GCTL=0x5203C;           //
	*(volatile unsigned int *)EMIFA_CE0 =0x1251C911;        //SRAM0
	*(volatile unsigned int *)EMIFA_CE1 =0x1251C911;        //SRAM1
	*(volatile unsigned int *)EMIFA_CE2 =0xA53AD40F;        //CF
	*(volatile unsigned int *)EMIFA_CE3 =0x43E48F02;
	/*************************************************************************/
	/*                         CONFIG EMIFB REGISTER                         */
	/*************************************************************************/	 
    // EMIFB是16位数据总线接口； 
	*(volatile unsigned int *)EMIFB_GCTL=0x0001203C;   	 
	*(volatile unsigned int *)EMIFB_CE0 =0x43E48F02;   //16C754 
	*(volatile unsigned int *)EMIFB_CE1 =0x43E48F02;
	*(volatile unsigned int *)EMIFB_CE2 =0x43E48F02;  
	*(volatile unsigned int *)EMIFB_CE3 =0x43E48F02;                                         							
	
}



/*void config_gpio()// 不添加同步器时使用配置 0427；
{
	*(volatile unsigned int *)GPGC =0x00000000;
	*(volatile unsigned int *)GPEN =0x0000fff9;        // 电路图上是GPIO_1可用，所以使能，用作测试程序时间； 
	*(volatile unsigned int *)GPDIR=0x00009b09;        // 方向；
	*(volatile unsigned int *)GPVAL=0x00000000;        // 赋值，初始值拉低；
	*(volatile unsigned int *)GPDH =0x00000000;
	*(volatile unsigned int *)GPDL =0x00000000;
	*(volatile unsigned int *)GPHM =0x00000000;
	*(volatile unsigned int *)GPLM =0x00000000;
	*(volatile unsigned int *)GPPOL=0x00000000;
}*/

void config_gpio()//根据同步器要求修改；0427；
{
	*(volatile unsigned int *)GPGC =0x00000010;
	*(volatile unsigned int *)GPEN =0x0000FFFF; 
	*(volatile unsigned int *)GPDIR=0x00009B08;
//	*(volatile unsigned int *)GPVAL=0x00000000;
//	*(volatile unsigned int *)GPDH =0x00000004;
//	*(volatile unsigned int *)GPDL =0x00000004;
	*(volatile unsigned int *)GPHM =0x00000004;
	*(volatile unsigned int *)GPLM =0x00000000;
//	*(volatile unsigned int *)GPPOL=0x00000000;
}


void McBSP_initial(void)
{ /* configure DSP2 McBSP1 */
  *(unsigned volatile int *)McBSP0_SPCR=0x0;   //串口控制寄存器
   //bit 31~26	:0  reserved;
  //bit 25,24	:01 when emulator is halted,the mcbsp is halted;
  //bit 23		:1	frame synchron is generated;
  //bit 22		:1	sample generator is working;
  //bit 21,20	:00	xint is drived by copy;
  //bit 19		:0	synchronization error isn't detected;
  //bit 18		:0	read only;
  //bit 17		:0	read only;
  //bit 16		:0	transmit port is disable; !!!!!!!
  //bit 15		:0	digitial loop mode is disable;
  //bit 14,13	:01	right-justed and sign-extended in msb;
  //bit 12,11	:01	clock stop mode is disable; !!!!!!!
  //bit 10~8	:000reserved;
  //bit 7		:1	dx is enabled;
  //bit 6		:0	reserved;
  //bit 5,4		:00	don't care;
  //bit 3		:1	don't care;
  //bit 2		:0	read only;
  //bit 1		:0	read only;
  //bit	0		:0	receiver reset port is disable;  !!!!!!!!
  //0 01| 1 1 00 |0 0 0 0 |0 01 0|1 000 |1 0 00 |1 0 0 0  0x1a0288、
  //*(unsigned volatile int *)McBSP0_SRGR=0x7BB70218;      //采样率控制寄存器(6M\2K)是内部时钟给McBSP
  //*(unsigned volatile int *)McBSP0_SRGR=0x51F30209;       // 采样率控制寄存器(1M\2K)CPLD给McBSP；
  *(unsigned volatile int *)McBSP0_SRGR=0x59C30201;        //采样率控制寄存器(5M\2K)CPLD给McBSP

  //*(unsigned volatile int *)McBSP0_SRGR=0x712602FF;      //采样率控制寄存器(581K\2K)
  //*(unsigned volatile int *)McBSP0_SRGR=0x71F40296;      //采样率控制寄存器(1M\2K)
  // 0010|0000|0001|0000|0000|0000|0001|1110
  //bit 31 :0  
  //bit 30 :1 CLKS下降沿产生信号CLKG、FSG
  //bit 29 :1 采样时钟由cpu时钟决定|0：由CLKS PIN 决定
  //bit 28 :1|0 transmit frame_sync signal(FSX) is generate by every DXR-to-XSR copy|1: driven by the sampal_rata generate frame_sync signal (FSG)
  //bit 27-16:3000(1101 0000 0100) 16+1 frame period value when frame_sync signal become active
  //bit 15-8 :2(0000 0010)     +1 frame wride value 
  //bit 7-0  :0x19(0001 1001)    分频大小
  
 *(unsigned volatile int *)McBSP0_RCR=0x00000740;  //16bit 7字     之前值为640，是7路采集，现在改为740,8路采集
 //*(unsigned volatile int *)McBSP0_RCR=0x000003A0;  //32bit 4字
  //*(unsigned volatile int *)McBSP0_RCR=0x00000740;  //16bit 8字
// *(unsigned volatile int *)McBSP0_RCR=0x000000A0;  //32bit 1字
  // 7-0     1010 0000 
  //bit 31   :0   signal_phase frame
  //bit 30-24:0   phase2'frame length
  //bit 23-21:000 8 bit transmit word length
  //bit 20-19:00  MSB 无压缩，传输从MSB(最高有效位)开始
  //bit 18   :0   传输同步信号重启传输|1：忽略
  //bit 17-16:00  1 bit data delay
  //bit 15   :0 phase normal
  //bit 14-8 :000 0011 phase1'frame length 每桢8个数据单元
  //bit 7-5  :101 16 bit transmit word length
  //bit 4    :0    禁止32-bit反相传输--->（禁止先传低位）
  //bit 3-0  ：保留
  *(unsigned volatile int *)McBSP0_XCR=0x00000040;  //发送控制寄存器
  // 7-0     0100 0000 
  //bit 31   :0   signal_phase frame
  //bit 30-24:0   phase2'frame length
  //bit 23-21:000 8 bit transmit word length
  //bit 20-19:00  MSB 无压缩，传输从MSB(最高有效位)开始
  //bit 18   :0   传输同步信号重启传输|1：忽略
  //bit 17-16:01  1 bit data delay
  //bit 15   :0 phase normal
  //bit 14-8 :000 0000 phase1'frame length 每桢一个数据单元
  //bit 7-5  :010 16 bit transmit word length
  //bit 4    :0    禁止32-bit反相传输--->（禁止先传低位）
  //bit 3-0  ：保留
   *(unsigned volatile int *)McBSP0_PCR=0x00000501;   //引脚控制寄存器
   //*(unsigned volatile int *)McBSP0_PCR=0x00000a00;   //引脚控制寄存器
  //11-0 1010 0000 1011 
  //bit 31-14   :保留
  //bit 13      :0 DX FSX CLKX 引脚设置为通信端口而非GPIO
  //bit 12      :0 DR FSR CLKR CLKS引脚设置为通信端口而非GPIO
  //BIT 11      :0 传输 帧同步信号由SPGR寄存器的FSGM位决定|0 由外部信号驱动
  //bit 10      :1 ：采样率发生器内部产生帧同步信号，FSR是输出管脚
  //bit  9      :0  传输   CLKX is an output pin drived by the internal simpal-rata generanter
  //             0         CLKX is an input pin drived by the external clock
  //bit  8      :0  接收   CLKR is an input pin drived by the external clock
  //             =1         CLKR is an output pin drived by the internal simpal-rata generanter
  //bit  7      :保留
  //bit  6      :0   当设为GPIO时， CLKS  pin 对应为逻辑低
  //bit  5      :0   当设为GPIO时， DX pin 对应为逻辑低
  //bit  4      :0   当设为GPIO时， DR pin 对应为逻辑低
  //bit  3      :0   |0 发送 传输同步脉冲高有效|1 低有效
  //bit  2      :0   接收传输同步脉冲高有效
  //bit  1      :0   在CLKX的上升沿儿发送数据采样
  //bit  0      :1   在CLKR的上升沿儿接收数据采样
   *(unsigned volatile int *)McBSP0_SPCR=0x02c00081;
   //*(unsigned volatile int *)McBSP0_SPCR=0x03c30080;
  //0000 00010 1100 0000 0000 0000 1000 0001
    
  //bit 31~26  :0000 00  reserved;
  //bit  25    :1 free run mode is enable ,在emulate halt 时，时钟不停止
  //bit  24    :1 soft mode is disable,clk stop when emulation halt |1 enable
  //bit  23    :1 /frst 帧同步发生器复位
  //bit  22    :1 /grst 采样率发生器复位
  //bit  21-20 :00 xintm 发送中断模式
  //bit  19    :0  xsynceerr 发送同步错误
  //bit  18    :0  xempty--|0 xsp is empty |1 not empty  
  //bit  17    :0  xrdy  |1--transfer is ready for dxr |0 not  
  //bit  16    :0  xrst  |串口发送器禁止
  //bit  15    :0  dlb   |0 数据链路模式 disable|1 
  //bit  14-13 :00 rjust 接收数据的符号扩展及调整模式 |01--右适应及符号扩展在DRR的高位|
  //                     |00 右适应及高位填充0 in DRR
  //bit  12-11 :00 clkstp 时钟停止模式|11-- 时钟在上升沿delay|00 clck stop mode is disable 
  //                               *****此处须注意，一开始选用的是11 。导致帧同步信号不对：
  //                               *****（高电平有效时，高电平持续整个数据发送时钟周期）
  //bit  10-8  :000  reserved;
  //bit  7     :1  DXENA |1-- DX enable is on 
  //bit  6     :0   reserved;
  //bit  5-4   :00 rintm 接收中断模式 
  //bit  3     :0  rsynceerr 接收同步错误 |0--同步错误不检测 
  //bit  2     :0  rfull |0--EBR 没
  //bit  1     :0  rrdy  |1--receiver is not ready
  //bit  0     :1  rrst   串行 receiver is enable
  //------------------mcbsp1---------------------------------
  /*
    *(unsigned volatile int *)McBSP1_SPCR=0x0;   //串口控制寄存器
    *(unsigned volatile int *)McBSP1_SRGR=0x3012001e;      //采样率控制寄存器
    *(unsigned volatile int *)McBSP1_XCR=0x00000040;  //发送控制寄存器
    *(unsigned volatile int *)McBSP1_PCR=0x00000a00;   //拇嫫�
    *(unsigned volatile int *)McBSP1_SPCR=0x03c30080;  */
}
 
void config_754a()
{
	unsigned char LSR;
	*(unsigned char *)(EMB_CE0+LJ_A4+IER_7)=0x00;//配置罗经波
	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+LJ_A4+DLL_7)=0x30;//波特率9600是0x60(19200是0x30)，晶振频率14.7456MHz
	*(unsigned char *)(EMB_CE0+LJ_A4+DLH_7)=0x00;

	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0xBF;//Xon and Xoff are accessible only when LCR is set BF
	*(unsigned char *)(EMB_CE0+LJ_A4+EFR_7)=0x10;//Enables the enhanced function
	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0x00;
	//*(unsigned char *)(EMB_CE0+LJ_A4+FCR_7)=0x06;
	*(unsigned char *)(EMB_CE0+LJ_A4+FCR_7)=0xC9;     //4月18日，开启FIFO
	//*(unsigned char *)(EMB_CE0+LJ_A4+FCR_7)=0xC3;
	//在TLR设为零时 触发级由FCR给定 RX触发级为60 TX触发级为8


	*(unsigned char *)(EMB_CE0+LJ_A4+MCR_7)=0x48;//Enable access to the TCR and TLR registers
	*(unsigned char *)(EMB_CE0+LJ_A4+TCR_7)=0x0F;//RCV  trigger level 60,RCV  trigger level to resume 0 
	//*(unsigned char *)(EMB_CE0+LJ_A4+TLR_7)=0xFF;
	*(unsigned char *)(EMB_CE0+LJ_A4+TLR_7)=0xFF;     //4月18日，设定触发级
	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0x03;//length to be transmitted is 8 bit
	*(unsigned char *)(EMB_CE0+LJ_A4+IER_7)=0x01;//只使能接收中断,Enable the RHR interrupt
 	LSR=*(unsigned  char *)(EMB_CE0+LJ_A4+IIR_7);
 	if(LSR!=0x00)
	    LSR=*(unsigned  char *)(EMB_CE0+LJ_A4+RHR_7);
}
void config_754b()
{
	unsigned char LSR;
	*(unsigned char *)(EMB_CE0+SYNC_A5+IER_7)=0x00;//配置同步器波特率（实际上是命令哦，亲）
	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+SYNC_A5+DLL_7)=0x18;//波特率38400，晶振频率14.7456MHz
	*(unsigned char *)(EMB_CE0+SYNC_A5+DLH_7)=0x00;

	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0xBF;
	*(unsigned char *)(EMB_CE0+SYNC_A5+EFR_7)=0x10;
	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0x00;
//	*(unsigned char *)(EMB_CE0+SYNC_A5+FCR_7)=0x06;     //4月18日
    *(unsigned char *)(EMB_CE0+SYNC_A5+FCR_7)=0xC9;     //4月18日，开启FIFO
	*(unsigned char *)(EMB_CE0+SYNC_A5+MCR_7)=0x48;
	*(unsigned char *)(EMB_CE0+SYNC_A5+TCR_7)=0x0F;
//	*(unsigned char *)(EMB_CE0+SYNC_A5+TLR_7)=0xFF;     //4月18日
	*(unsigned char *)(EMB_CE0+SYNC_A5+TLR_7)=0xFF;     //4月18日，设定触发级
	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0x03;
	*(unsigned char *)(EMB_CE0+SYNC_A5+IER_7)=0x01;
	LSR=*(unsigned  char *)(EMB_CE0+SYNC_A5+IIR_7);
 	if(LSR!=0x00)
	    LSR=*(unsigned  char *)(EMB_CE0+SYNC_A5+RHR_7);
 
}
void config_754c()
{
	*(unsigned char *)(EMB_CE0+DP_A6+IER_7)=0x00;//配置深度传感器波特率
	*(unsigned char *)(EMB_CE0+DP_A6+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+DP_A6+DLL_7)=0x60;//波特率9600，晶振频率14.7456MHz
	*(unsigned char *)(EMB_CE0+DP_A6+DLH_7)=0x00;
	*(unsigned char *)(EMB_CE0+DP_A6+LCR_7)=0xBF;
	*(unsigned char *)(EMB_CE0+DP_A6+EFR_7)=0x10;
	*(unsigned char *)(EMB_CE0+DP_A6+LCR_7)=0x00;
	*(unsigned char *)(EMB_CE0+DP_A6+FCR_7)=0xC9;
	*(unsigned char *)(EMB_CE0+DP_A6+MCR_7)=0x48;
	*(unsigned char *)(EMB_CE0+DP_A6+TCR_7)=0x0F;
	*(unsigned char *)(EMB_CE0+DP_A6+TLR_7)=0xFF;
	*(unsigned char *)(EMB_CE0+DP_A6+LCR_7)=0x03;
	*(unsigned char *)(EMB_CE0+DP_A6+IER_7)=0x01;
}
void config_754d()
{
	*(unsigned char *)(EMB_CE0+ORD_A7+IER_7)=0x00;//配置命令串口波特率(实际上是同步器哦，亲)
	*(unsigned char *)(EMB_CE0+ORD_A7+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+ORD_A7+DLL_7)=0x60;//波特率9600，晶振频率14.7456MHz
	*(unsigned char *)(EMB_CE0+ORD_A7+DLH_7)=0x00;
	*(unsigned char *)(EMB_CE0+ORD_A7+LCR_7)=0xBF;
	*(unsigned char *)(EMB_CE0+ORD_A7+EFR_7)=0x10;
	*(unsigned char *)(EMB_CE0+ORD_A7+LCR_7)=0x00;
	*(unsigned char *)(EMB_CE0+ORD_A7+FCR_7)=0xC9;
	*(unsigned char *)(EMB_CE0+ORD_A7+MCR_7)=0x48;
	*(unsigned char *)(EMB_CE0+ORD_A7+TCR_7)=0x0F;
	*(unsigned char *)(EMB_CE0+ORD_A7+TLR_7)=0xFF;
	*(unsigned char *)(EMB_CE0+ORD_A7+LCR_7)=0x03;
	*(unsigned char *)(EMB_CE0+ORD_A7+IER_7)=0x01;
}

void timer0_8ms_timeout(void)
{
 *(unsigned volatile int *)TIMER0_CTRL=0x00000415;  //关闭定时器
 
 *(unsigned volatile int *)TIMER0_COUNT=0x0;
 /*每计一次数，CNT寄存器加一，当值等于PRD中的值时
  在下一个CPU时钟复位到0     */
 *(unsigned volatile int *)TIMER0_PRD=0x13880;//8ms

}

void timer0_5ms_timeout(void)
{
 *(unsigned volatile int *)TIMER0_CTRL=0x00000215;  //关闭定时器
 
 *(unsigned volatile int *)TIMER0_COUNT=0x0;
 /*每计一次数，CNT寄存器加一，当值等于PRD中的值时
  在下一个CPU时钟复位到0     */
 *(unsigned volatile int *)TIMER0_PRD=0xC350;//5ms

}

void timer1_config_2s(void)
 {
 /* configure timer0 定时器0配置，使用外部10M时钟源*/
 *(unsigned volatile int *)TIMER1_CTRL=0x00000215;
 //定时器控制寄存器
 //11~0 :0010 0001 0101 
 //bit 31~16    :0 reserved;
 //bit 15       :0 在一次仿真暂停期间定时器继续计数  ?
 //bit 14~12    :0 reserved;
 //bit 11       :0 TSTAT 定时器状态位
 //bit 10       :0 不反相TINP驱动定时器
 //bit 9        :1 内部时钟源驱动；=1/8 CPU时钟
 //bit 8        :0 TSTAT为脉冲模式：在定时器达到定时器周期后一个CPU时钟时，TSTAT有效
 //bit 7        :0 HLD使能时，计数器被禁止并保持在当前状态
 //bit 6        :0 复位或启动定时器对定时器没有影响;|1:如果HDL=1，计数器被置0，在下一个时钟开始计数
 //bit 5        :0 reserved;
 //bit 4        :1 脉冲宽度位，定时器计数值等于定时器周期值的两个定时器输入周期后，TSTAT信号进入无效
 //bit 3        :0 数据输入位(等于TINP引脚的值)，TINP引脚值为逻辑低
 //bit 2        :1 数据输出位:被INVOUT反相后的TSTAT驱动TOUT|0: DATOUT驱动TOUT
 //bit 1        :0 TOUT反相位(在FUNC=1时有效)，不反相的TSTAT驱动TOUT |1: 反相的TSTAT驱动TOUT
 //bit 0        :1 TOUT引脚的功能，TOUT为一个定时器输出引脚 |0 为通用目的的输出引脚
 
 *(unsigned volatile int *)TIMER1_COUNT=0x0;
 /*每计一次数，CNT寄存器加一，当值等于PRD中的值时
  在下一个CPU时钟复位到0     */
 //*(unsigned volatile int *)TIMER0_PRD=0x05F5E100;//10s
  *(unsigned volatile int *)TIMER1_PRD=0x8F0D180;//2s	
}


void timer1_config_timeout(void)
 {
 *(unsigned volatile int *)TIMER1_CTRL=0x000004D5;
 
 *(unsigned volatile int *)TIMER1_COUNT=0x0;
 /*每计一次数，CNT寄存器加一，当值等于PRD中的值时
  在下一个CPU时钟复位到0     */
 *(unsigned volatile int *)TIMER1_PRD=0x989680;//1s

}

/*McBSP0接收事件，采用乒乓缓存的形式*/
void Config_EnableEdma13(short *pointer0,short *pointer1)//
{	
	*(volatile unsigned int *)(EVENTD_PARAMS+OPT) = 0x283D0002; 
	*(volatile unsigned int *)(EVENTD_PARAMS+SRC) = McBSP0_DRR;
	*(volatile unsigned int *)(EVENTD_PARAMS+CNT) = 0x00000F00;//0x00000F00 15*512/2单元15扇区;
	*(volatile unsigned int *)(EVENTD_PARAMS+DST) = (unsigned int)pointer0;
	*(volatile unsigned int *)(EVENTD_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+LNK) = 0x10000600;


	*(volatile unsigned int *)(EVENT13RL0_PARAMS+OPT) = 0x28302002; 
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+SRC) = McBSP0_DRR;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+CNT) = 0x00000F00;//7*512单元;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+DST) = (unsigned int)pointer1;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+LNK) = 0x10000618;


	*(volatile unsigned int *)(EVENT13RL1_PARAMS+OPT) = 0x283D0002; 
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+SRC) = McBSP0_DRR;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+CNT) = 0x00000F00;//7*512单元;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+DST) = (unsigned int)pointer0;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+LNK) = 0x10000600;
	

	*(volatile unsigned int *)CIERL |= 0x00002000; //使能edma13中断	
	*(volatile unsigned int *)CIERL |= 0x00010000; //使能edma16中断	
	*(volatile unsigned int *)EERL  |= 0x00002000; //使能edma13事件

			
}
void Config_DisableEdma13(void)
{
	*(volatile unsigned int *)EERL  &= 0xFFFFDFFF; //禁止edma13事件
	*(volatile unsigned int *)EERL  &= 0xFFFEFFFF; //禁止edma16事件
	*(volatile unsigned int *)CIERL &= 0xFFFFDFFF; //禁止edma13中断	
	*(volatile unsigned int *)CIERL &= 0xFFFEFFFF; //禁止edma16中断
	*(volatile unsigned int *)(EVENTD_PARAMS+OPT) = 0x00000000; 
	*(volatile unsigned int *)(EVENTD_PARAMS+SRC) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+CNT) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+DST) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+LNK) = 0x00000000;

	*(volatile unsigned int *)ECRL  |= 0x00002000; //清除edma13事件
	*(volatile unsigned int *)CIPRL |= 0x00002000; //清除edma13挂起中断	
	*(volatile unsigned int *)CIPRL |= 0x00010000; //禁止edma16中断
}



