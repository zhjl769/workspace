
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
	*(volatile unsigned int *)CCFG=0x00000000;  // L2����ȫSRAM
	/*************************************************************************/
	/*                         CONFIG EMIFA REGISTER                         */
	/*************************************************************************/
	/*void ������Ҫ��������SRAM,EMIFA��ʱ��Ϊ1/4=CPUʱ��=150MHz*/
	/*��ѡͨ=дѡͨ=9 ������=д����=1 ������=д����=1*/	 
	// EMIFA��64λ�������߽ӿڣ�	   
	*(volatile unsigned int *)EMIFA_GCTL=0x5203C;           //
	*(volatile unsigned int *)EMIFA_CE0 =0x1251C911;        //SRAM0
	*(volatile unsigned int *)EMIFA_CE1 =0x1251C911;        //SRAM1
	*(volatile unsigned int *)EMIFA_CE2 =0xA53AD40F;        //CF
	*(volatile unsigned int *)EMIFA_CE3 =0x43E48F02;
	/*************************************************************************/
	/*                         CONFIG EMIFB REGISTER                         */
	/*************************************************************************/	 
    // EMIFB��16λ�������߽ӿڣ� 
	*(volatile unsigned int *)EMIFB_GCTL=0x0001203C;   	 
	*(volatile unsigned int *)EMIFB_CE0 =0x43E48F02;   //16C754 
	*(volatile unsigned int *)EMIFB_CE1 =0x43E48F02;
	*(volatile unsigned int *)EMIFB_CE2 =0x43E48F02;  
	*(volatile unsigned int *)EMIFB_CE3 =0x43E48F02;                                         							
	
}



/*void config_gpio()// �����ͬ����ʱʹ������ 0427��
{
	*(volatile unsigned int *)GPGC =0x00000000;
	*(volatile unsigned int *)GPEN =0x0000fff9;        // ��·ͼ����GPIO_1���ã�����ʹ�ܣ��������Գ���ʱ�䣻 
	*(volatile unsigned int *)GPDIR=0x00009b09;        // ����
	*(volatile unsigned int *)GPVAL=0x00000000;        // ��ֵ����ʼֵ���ͣ�
	*(volatile unsigned int *)GPDH =0x00000000;
	*(volatile unsigned int *)GPDL =0x00000000;
	*(volatile unsigned int *)GPHM =0x00000000;
	*(volatile unsigned int *)GPLM =0x00000000;
	*(volatile unsigned int *)GPPOL=0x00000000;
}*/

void config_gpio()//����ͬ����Ҫ���޸ģ�0427��
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
  *(unsigned volatile int *)McBSP0_SPCR=0x0;   //���ڿ��ƼĴ���
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
  //0 01| 1 1 00 |0 0 0 0 |0 01 0|1 000 |1 0 00 |1 0 0 0  0x1a0288��
  //*(unsigned volatile int *)McBSP0_SRGR=0x7BB70218;      //�����ʿ��ƼĴ���(6M\2K)���ڲ�ʱ�Ӹ�McBSP
  //*(unsigned volatile int *)McBSP0_SRGR=0x51F30209;       // �����ʿ��ƼĴ���(1M\2K)CPLD��McBSP��
  *(unsigned volatile int *)McBSP0_SRGR=0x59C30201;        //�����ʿ��ƼĴ���(5M\2K)CPLD��McBSP

  //*(unsigned volatile int *)McBSP0_SRGR=0x712602FF;      //�����ʿ��ƼĴ���(581K\2K)
  //*(unsigned volatile int *)McBSP0_SRGR=0x71F40296;      //�����ʿ��ƼĴ���(1M\2K)
  // 0010|0000|0001|0000|0000|0000|0001|1110
  //bit 31 :0  
  //bit 30 :1 CLKS�½��ز����ź�CLKG��FSG
  //bit 29 :1 ����ʱ����cpuʱ�Ӿ���|0����CLKS PIN ����
  //bit 28 :1|0 transmit frame_sync signal(FSX) is generate by every DXR-to-XSR copy|1: driven by the sampal_rata generate frame_sync signal (FSG)
  //bit 27-16:3000(1101 0000 0100) 16+1 frame period value when frame_sync signal become active
  //bit 15-8 :2(0000 0010)     +1 frame wride value 
  //bit 7-0  :0x19(0001 1001)    ��Ƶ��С
  
 *(unsigned volatile int *)McBSP0_RCR=0x00000740;  //16bit 7��     ֮ǰֵΪ640����7·�ɼ������ڸ�Ϊ740,8·�ɼ�
 //*(unsigned volatile int *)McBSP0_RCR=0x000003A0;  //32bit 4��
  //*(unsigned volatile int *)McBSP0_RCR=0x00000740;  //16bit 8��
// *(unsigned volatile int *)McBSP0_RCR=0x000000A0;  //32bit 1��
  // 7-0     1010 0000 
  //bit 31   :0   signal_phase frame
  //bit 30-24:0   phase2'frame length
  //bit 23-21:000 8 bit transmit word length
  //bit 20-19:00  MSB ��ѹ���������MSB(�����Чλ)��ʼ
  //bit 18   :0   ����ͬ���ź���������|1������
  //bit 17-16:00  1 bit data delay
  //bit 15   :0 phase normal
  //bit 14-8 :000 0011 phase1'frame length ÿ��8�����ݵ�Ԫ
  //bit 7-5  :101 16 bit transmit word length
  //bit 4    :0    ��ֹ32-bit���ഫ��--->����ֹ�ȴ���λ��
  //bit 3-0  ������
  *(unsigned volatile int *)McBSP0_XCR=0x00000040;  //���Ϳ��ƼĴ���
  // 7-0     0100 0000 
  //bit 31   :0   signal_phase frame
  //bit 30-24:0   phase2'frame length
  //bit 23-21:000 8 bit transmit word length
  //bit 20-19:00  MSB ��ѹ���������MSB(�����Чλ)��ʼ
  //bit 18   :0   ����ͬ���ź���������|1������
  //bit 17-16:01  1 bit data delay
  //bit 15   :0 phase normal
  //bit 14-8 :000 0000 phase1'frame length ÿ��һ�����ݵ�Ԫ
  //bit 7-5  :010 16 bit transmit word length
  //bit 4    :0    ��ֹ32-bit���ഫ��--->����ֹ�ȴ���λ��
  //bit 3-0  ������
   *(unsigned volatile int *)McBSP0_PCR=0x00000501;   //���ſ��ƼĴ���
   //*(unsigned volatile int *)McBSP0_PCR=0x00000a00;   //���ſ��ƼĴ���
  //11-0 1010 0000 1011 
  //bit 31-14   :����
  //bit 13      :0 DX FSX CLKX ��������Ϊͨ�Ŷ˿ڶ���GPIO
  //bit 12      :0 DR FSR CLKR CLKS��������Ϊͨ�Ŷ˿ڶ���GPIO
  //BIT 11      :0 ���� ֡ͬ���ź���SPGR�Ĵ�����FSGMλ����|0 ���ⲿ�ź�����
  //bit 10      :1 �������ʷ������ڲ�����֡ͬ���źţ�FSR������ܽ�
  //bit  9      :0  ����   CLKX is an output pin drived by the internal simpal-rata generanter
  //             0         CLKX is an input pin drived by the external clock
  //bit  8      :0  ����   CLKR is an input pin drived by the external clock
  //             =1         CLKR is an output pin drived by the internal simpal-rata generanter
  //bit  7      :����
  //bit  6      :0   ����ΪGPIOʱ�� CLKS  pin ��ӦΪ�߼���
  //bit  5      :0   ����ΪGPIOʱ�� DX pin ��ӦΪ�߼���
  //bit  4      :0   ����ΪGPIOʱ�� DR pin ��ӦΪ�߼���
  //bit  3      :0   |0 ���� ����ͬ���������Ч|1 ����Ч
  //bit  2      :0   ���մ���ͬ���������Ч
  //bit  1      :0   ��CLKX�������ض��������ݲ���
  //bit  0      :1   ��CLKR�������ض��������ݲ���
   *(unsigned volatile int *)McBSP0_SPCR=0x02c00081;
   //*(unsigned volatile int *)McBSP0_SPCR=0x03c30080;
  //0000 00010 1100 0000 0000 0000 1000 0001
    
  //bit 31~26  :0000 00  reserved;
  //bit  25    :1 free run mode is enable ,��emulate halt ʱ��ʱ�Ӳ�ֹͣ
  //bit  24    :1 soft mode is disable,clk stop when emulation halt |1 enable
  //bit  23    :1 /frst ֡ͬ����������λ
  //bit  22    :1 /grst �����ʷ�������λ
  //bit  21-20 :00 xintm �����ж�ģʽ
  //bit  19    :0  xsynceerr ����ͬ������
  //bit  18    :0  xempty--|0 xsp is empty |1 not empty  
  //bit  17    :0  xrdy  |1--transfer is ready for dxr |0 not  
  //bit  16    :0  xrst  |���ڷ�������ֹ
  //bit  15    :0  dlb   |0 ������·ģʽ disable|1 
  //bit  14-13 :00 rjust �������ݵķ�����չ������ģʽ |01--����Ӧ��������չ��DRR�ĸ�λ|
  //                     |00 ����Ӧ����λ���0 in DRR
  //bit  12-11 :00 clkstp ʱ��ֹͣģʽ|11-- ʱ����������delay|00 clck stop mode is disable 
  //                               *****�˴���ע�⣬һ��ʼѡ�õ���11 ������֡ͬ���źŲ��ԣ�
  //                               *****���ߵ�ƽ��Чʱ���ߵ�ƽ�����������ݷ���ʱ�����ڣ�
  //bit  10-8  :000  reserved;
  //bit  7     :1  DXENA |1-- DX enable is on 
  //bit  6     :0   reserved;
  //bit  5-4   :00 rintm �����ж�ģʽ 
  //bit  3     :0  rsynceerr ����ͬ������ |0--ͬ�����󲻼�� 
  //bit  2     :0  rfull |0--EBR û
  //bit  1     :0  rrdy  |1--receiver is not ready
  //bit  0     :1  rrst   ���� receiver is enable
  //------------------mcbsp1---------------------------------
  /*
    *(unsigned volatile int *)McBSP1_SPCR=0x0;   //���ڿ��ƼĴ���
    *(unsigned volatile int *)McBSP1_SRGR=0x3012001e;      //�����ʿ��ƼĴ���
    *(unsigned volatile int *)McBSP1_XCR=0x00000040;  //���Ϳ��ƼĴ���
    *(unsigned volatile int *)McBSP1_PCR=0x00000a00;   //Ĵ���
    *(unsigned volatile int *)McBSP1_SPCR=0x03c30080;  */
}
 
void config_754a()
{
	unsigned char LSR;
	*(unsigned char *)(EMB_CE0+LJ_A4+IER_7)=0x00;//�����޾���
	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+LJ_A4+DLL_7)=0x30;//������9600��0x60(19200��0x30)������Ƶ��14.7456MHz
	*(unsigned char *)(EMB_CE0+LJ_A4+DLH_7)=0x00;

	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0xBF;//Xon and Xoff are accessible only when LCR is set BF
	*(unsigned char *)(EMB_CE0+LJ_A4+EFR_7)=0x10;//Enables the enhanced function
	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0x00;
	//*(unsigned char *)(EMB_CE0+LJ_A4+FCR_7)=0x06;
	*(unsigned char *)(EMB_CE0+LJ_A4+FCR_7)=0xC9;     //4��18�գ�����FIFO
	//*(unsigned char *)(EMB_CE0+LJ_A4+FCR_7)=0xC3;
	//��TLR��Ϊ��ʱ ��������FCR���� RX������Ϊ60 TX������Ϊ8


	*(unsigned char *)(EMB_CE0+LJ_A4+MCR_7)=0x48;//Enable access to the TCR and TLR registers
	*(unsigned char *)(EMB_CE0+LJ_A4+TCR_7)=0x0F;//RCV  trigger level 60,RCV  trigger level to resume 0 
	//*(unsigned char *)(EMB_CE0+LJ_A4+TLR_7)=0xFF;
	*(unsigned char *)(EMB_CE0+LJ_A4+TLR_7)=0xFF;     //4��18�գ��趨������
	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0x03;//length to be transmitted is 8 bit
	*(unsigned char *)(EMB_CE0+LJ_A4+IER_7)=0x01;//ֻʹ�ܽ����ж�,Enable the RHR interrupt
 	LSR=*(unsigned  char *)(EMB_CE0+LJ_A4+IIR_7);
 	if(LSR!=0x00)
	    LSR=*(unsigned  char *)(EMB_CE0+LJ_A4+RHR_7);
}
void config_754b()
{
	unsigned char LSR;
	*(unsigned char *)(EMB_CE0+SYNC_A5+IER_7)=0x00;//����ͬ���������ʣ�ʵ����������Ŷ���ף�
	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+SYNC_A5+DLL_7)=0x18;//������38400������Ƶ��14.7456MHz
	*(unsigned char *)(EMB_CE0+SYNC_A5+DLH_7)=0x00;

	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0xBF;
	*(unsigned char *)(EMB_CE0+SYNC_A5+EFR_7)=0x10;
	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0x00;
//	*(unsigned char *)(EMB_CE0+SYNC_A5+FCR_7)=0x06;     //4��18��
    *(unsigned char *)(EMB_CE0+SYNC_A5+FCR_7)=0xC9;     //4��18�գ�����FIFO
	*(unsigned char *)(EMB_CE0+SYNC_A5+MCR_7)=0x48;
	*(unsigned char *)(EMB_CE0+SYNC_A5+TCR_7)=0x0F;
//	*(unsigned char *)(EMB_CE0+SYNC_A5+TLR_7)=0xFF;     //4��18��
	*(unsigned char *)(EMB_CE0+SYNC_A5+TLR_7)=0xFF;     //4��18�գ��趨������
	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0x03;
	*(unsigned char *)(EMB_CE0+SYNC_A5+IER_7)=0x01;
	LSR=*(unsigned  char *)(EMB_CE0+SYNC_A5+IIR_7);
 	if(LSR!=0x00)
	    LSR=*(unsigned  char *)(EMB_CE0+SYNC_A5+RHR_7);
 
}
void config_754c()
{
	*(unsigned char *)(EMB_CE0+DP_A6+IER_7)=0x00;//������ȴ�����������
	*(unsigned char *)(EMB_CE0+DP_A6+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+DP_A6+DLL_7)=0x60;//������9600������Ƶ��14.7456MHz
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
	*(unsigned char *)(EMB_CE0+ORD_A7+IER_7)=0x00;//��������ڲ�����(ʵ������ͬ����Ŷ����)
	*(unsigned char *)(EMB_CE0+ORD_A7+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+ORD_A7+DLL_7)=0x60;//������9600������Ƶ��14.7456MHz
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
 *(unsigned volatile int *)TIMER0_CTRL=0x00000415;  //�رն�ʱ��
 
 *(unsigned volatile int *)TIMER0_COUNT=0x0;
 /*ÿ��һ������CNT�Ĵ�����һ����ֵ����PRD�е�ֵʱ
  ����һ��CPUʱ�Ӹ�λ��0     */
 *(unsigned volatile int *)TIMER0_PRD=0x13880;//8ms

}

void timer0_5ms_timeout(void)
{
 *(unsigned volatile int *)TIMER0_CTRL=0x00000215;  //�رն�ʱ��
 
 *(unsigned volatile int *)TIMER0_COUNT=0x0;
 /*ÿ��һ������CNT�Ĵ�����һ����ֵ����PRD�е�ֵʱ
  ����һ��CPUʱ�Ӹ�λ��0     */
 *(unsigned volatile int *)TIMER0_PRD=0xC350;//5ms

}

void timer1_config_2s(void)
 {
 /* configure timer0 ��ʱ��0���ã�ʹ���ⲿ10Mʱ��Դ*/
 *(unsigned volatile int *)TIMER1_CTRL=0x00000215;
 //��ʱ�����ƼĴ���
 //11~0 :0010 0001 0101 
 //bit 31~16    :0 reserved;
 //bit 15       :0 ��һ�η�����ͣ�ڼ䶨ʱ����������  ?
 //bit 14~12    :0 reserved;
 //bit 11       :0 TSTAT ��ʱ��״̬λ
 //bit 10       :0 ������TINP������ʱ��
 //bit 9        :1 �ڲ�ʱ��Դ������=1/8 CPUʱ��
 //bit 8        :0 TSTATΪ����ģʽ���ڶ�ʱ���ﵽ��ʱ�����ں�һ��CPUʱ��ʱ��TSTAT��Ч
 //bit 7        :0 HLDʹ��ʱ������������ֹ�������ڵ�ǰ״̬
 //bit 6        :0 ��λ��������ʱ���Զ�ʱ��û��Ӱ��;|1:���HDL=1������������0������һ��ʱ�ӿ�ʼ����
 //bit 5        :0 reserved;
 //bit 4        :1 ������λ����ʱ������ֵ���ڶ�ʱ������ֵ��������ʱ���������ں�TSTAT�źŽ�����Ч
 //bit 3        :0 ��������λ(����TINP���ŵ�ֵ)��TINP����ֵΪ�߼���
 //bit 2        :1 �������λ:��INVOUT������TSTAT����TOUT|0: DATOUT����TOUT
 //bit 1        :0 TOUT����λ(��FUNC=1ʱ��Ч)���������TSTAT����TOUT |1: �����TSTAT����TOUT
 //bit 0        :1 TOUT���ŵĹ��ܣ�TOUTΪһ����ʱ��������� |0 Ϊͨ��Ŀ�ĵ��������
 
 *(unsigned volatile int *)TIMER1_COUNT=0x0;
 /*ÿ��һ������CNT�Ĵ�����һ����ֵ����PRD�е�ֵʱ
  ����һ��CPUʱ�Ӹ�λ��0     */
 //*(unsigned volatile int *)TIMER0_PRD=0x05F5E100;//10s
  *(unsigned volatile int *)TIMER1_PRD=0x8F0D180;//2s	
}


void timer1_config_timeout(void)
 {
 *(unsigned volatile int *)TIMER1_CTRL=0x000004D5;
 
 *(unsigned volatile int *)TIMER1_COUNT=0x0;
 /*ÿ��һ������CNT�Ĵ�����һ����ֵ����PRD�е�ֵʱ
  ����һ��CPUʱ�Ӹ�λ��0     */
 *(unsigned volatile int *)TIMER1_PRD=0x989680;//1s

}

/*McBSP0�����¼�������ƹ�һ������ʽ*/
void Config_EnableEdma13(short *pointer0,short *pointer1)//
{	
	*(volatile unsigned int *)(EVENTD_PARAMS+OPT) = 0x283D0002; 
	*(volatile unsigned int *)(EVENTD_PARAMS+SRC) = McBSP0_DRR;
	*(volatile unsigned int *)(EVENTD_PARAMS+CNT) = 0x00000F00;//0x00000F00 15*512/2��Ԫ15����;
	*(volatile unsigned int *)(EVENTD_PARAMS+DST) = (unsigned int)pointer0;
	*(volatile unsigned int *)(EVENTD_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+LNK) = 0x10000600;


	*(volatile unsigned int *)(EVENT13RL0_PARAMS+OPT) = 0x28302002; 
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+SRC) = McBSP0_DRR;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+CNT) = 0x00000F00;//7*512��Ԫ;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+DST) = (unsigned int)pointer1;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+LNK) = 0x10000618;


	*(volatile unsigned int *)(EVENT13RL1_PARAMS+OPT) = 0x283D0002; 
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+SRC) = McBSP0_DRR;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+CNT) = 0x00000F00;//7*512��Ԫ;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+DST) = (unsigned int)pointer0;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+LNK) = 0x10000600;
	

	*(volatile unsigned int *)CIERL |= 0x00002000; //ʹ��edma13�ж�	
	*(volatile unsigned int *)CIERL |= 0x00010000; //ʹ��edma16�ж�	
	*(volatile unsigned int *)EERL  |= 0x00002000; //ʹ��edma13�¼�

			
}
void Config_DisableEdma13(void)
{
	*(volatile unsigned int *)EERL  &= 0xFFFFDFFF; //��ֹedma13�¼�
	*(volatile unsigned int *)EERL  &= 0xFFFEFFFF; //��ֹedma16�¼�
	*(volatile unsigned int *)CIERL &= 0xFFFFDFFF; //��ֹedma13�ж�	
	*(volatile unsigned int *)CIERL &= 0xFFFEFFFF; //��ֹedma16�ж�
	*(volatile unsigned int *)(EVENTD_PARAMS+OPT) = 0x00000000; 
	*(volatile unsigned int *)(EVENTD_PARAMS+SRC) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+CNT) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+DST) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+LNK) = 0x00000000;

	*(volatile unsigned int *)ECRL  |= 0x00002000; //���edma13�¼�
	*(volatile unsigned int *)CIPRL |= 0x00002000; //���edma13�����ж�	
	*(volatile unsigned int *)CIPRL |= 0x00010000; //��ֹedma16�ж�
}



