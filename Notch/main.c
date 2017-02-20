#include "msp430f6638.h"
#include "function.h"
#include "type.h"
#include "table.h"
#include "init.h"

#define COMMAND_LEN 26	//����ָ���
int notchIteraStep=0;   //notch�˲�����������
int sinTabStep = 0;		//���ұ���
int wc=0;				//�˲�������wc
int ws=0;				//�˲�������ws
int err=0;				//�в�
const int*sinAdd = table;			//�˲����ź�1
const int*cosAdd = table + 0x800;	//�˲����ź�2
unsigned char uartBuffer[COMMAND_LEN-1] = {0};	//��������
unsigned char uartCnt = 0;						//�������ݼ���
unsigned char uartCommand = 0;					//�Ƿ��յ��������������
unsigned char uartFlag = 0;						//���ڱ�־
unsigned char spiBuffer[COMMAND_LEN] = {0};		//spi����
unsigned char spiBuffer1[COMMAND_LEN-2] = {0};	//
#define COMM "Communication "					//���ڷ���"ͨѶ��"ָ���ַ�
#define SUCCESS "Success\n"						//���ڷ���"�ɹ�"ָ���ַ�
#define FAULT "Fault\n"							//���ڷ���"ʧ��"ָ���ַ�
uint sum;										//û�õ�
int resFilter;									//��ǰ�˲��������
uchar stateChange;								//�Ƿ��⵽�ź�
long widthCnt;									//��⵽���źŵ���
uint noiseCnt;									//��⵽����������
uint offCnt;									//û�õ�
uint offProCnt;									//û�õ�
uint results;									//û�õ�
int envelAveOut;            					//����ƽ�����
int envelOut;									//�������
uint ADCtemp;									//ADC����ֵ
uint ADC_temp[1024]={0};						//û�õ�
int ADCin;										//ADC����ֵתΪ�з�����
int output[1024] = {0};							//û�õ�
uint out_count = 0;								//û�õ�
#define ADDRBONDAR (table + 0x2000)				//addr_bound����ַ���ޣ�����ֵ�����ұ�����
#define SINADDRORG (table)						//û�õ�
#define COSADDRORG (table + 0x800)				//û�õ�

void main()
{
	int i,j = 0;
	int checkFlag = 0;			//
	WDTCTL = WDTPW + WDTHOLD;		//�رտ��Ź�
	OMAP_POWER_OFF;				//��ʼʱOMAP�ر�
	TRANS_POWER_OFF;				//��ʼʱ�ر�
	MOTO_POWER_OFF;				//��ʼʱ�ر�
	MOTO_RELEASE_OFF;				//��ʼʱ�ر�
	initClock();					//��ʼ��ʱ��
	initPort();					//��ʼ���˿�
	initVar();					//��ʼ������
	initADC();					//��ʼ��ADC
	initUart();					//��ʼ������
	initSpi();					//��ʼ��SPI
	__bis_SR_register(GIE);		//ʹ��ȫ���ж�

	//һֱ���м�⴦��
	while(1)
	{
		if(uartCommand)
		{	//����յ���������
			uartCommand = 0;		//�����������㣬������һ�ν��д���������
			spiBuffer[0] = 0x5A;	//spi buffer[0]����
			for(j = 1;j < COMMAND_LEN;j++)
			{
				spiBuffer[j] = uartBuffer[j-1];		//���������ݸ�ֵ��spi buffer
			}
			spiSend(spiBuffer,COMMAND_LEN);			//��������ͨ��spi����
			uartSend(COMM,14);						//���ڷ��ͣ�ͨѶ��
			checkFlag = dspTo430();					//���dsp��430��������
			if(!checkFlag)
			{	//����־Ϊ0��ͨѶ�ɹ�
				uartSend(SUCCESS,8);				//���ڷ������ɹ�
			}
			else
			{	//����־��Ϊ0��ͨѶʧ��
				uartSend(FAULT,6);					//���ڷ�����ʧ��
			}
			uartSend(spiBuffer1,(COMMAND_LEN-2));	//���ڷ�������
		}
		if( stateChange == 1)
		{	//��⵽�ź�
			stateChange = 0;
		}
		else
		{	//û�м�⵽�ź�
			if( ADC12IFG & BIT1 )				//ADC�жϱ�־λ
			{
				ADCtemp = ADC12MEM1;				//ADC��������
				ADCin = (int)(ADCtemp - 0x800); 	//�޷���������з�����
				ADCin = ADCin *15;					//ADC����ֵ*15
				if(!notchFilter(ADCin))				//����north�˲����ж��Ƿ��⵽�ź�
					// output[out_count] = resFilter;
					// ADC_temp[out_count] = ADCtemp;
					// out_count++;
					// if(out_count == 1024)
					// out_count = 0;
				{	//��⵽�źŵ�״̬
					noiseCnt = 0;				//������������
					widthCnt += 1;				//����źŵ�����һ
					if(widthCnt >= 80000)		//�����⵽80000���źŵ�����ȷ����⵽�ź�
					{
						powerOnAct();			//��⵽�ź�֮����еĲ���
					}
				}
				else
				{	//δ��⵽�źŵ�
					noiseCnt += 1;				//����������һ
					if(noiseCnt >= 20)			//����20�μ�⵽������
					{
						noiseCnt = 0;			//������������
						widthCnt = 0;			//�źŵ�������
					}
				}
			}//if(uartCommand),else,if( ADC12IFG & BIT1 )
		}//if(uartCommand),else
	}//if(uartCommand)
}//main


//���ڷ�������
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

//��⵽�ź�
void powerOnAct()
{
	stateChange = 1;		//״̬�ı�Ϊ1������⵽�ź�
	ADC12CTL0 &= ~ADC12ON;	//�ж�ADC����
	OMAP_POWER_ON;			//��OMAP
	TRANS_POWER_ON;			//��ʼ����
	noiseCnt = 0;			//������������
	widthCnt = 0;			//�źż�������
}


//notch�˲�������
int notchFilter( int ADCin)
{
	//���㱾ʱ�̣���kʱ�̵�����ź�
	//resFilter=wc*coswk+ws*sinwk
	MPYS = wc;  			//wc��ֵ��Ӳ���˷���
	OP2 = *cosAdd;			//wc*coswk
	MACS = ws;				//ws��ֵ���˼���
	OP2 = *sinAdd;			//wc*coswk +ws*sinwk
	resFilter = RESHI;		//����ĸ��ֽڣ���wc*coswk +ws*sinwk�����ֵ��resFilter������ʱ�˲��������

	//�����ʱ�̣���kʱ�̵Ĳв�
	//err(t)=x(t)-resFilter
	err = (ADCin) - resFilter;

	//������һʱ�̣���k+1ʱ�̵��˲�������wc(k+1)��kʱ���˲�������wc(k)֮��
	//err*cos(w*k)*step
	MPYS = err;
	OP2 = *cosAdd;
	cosAdd = cosAdd + sinTabStep;
	//���������ұ�
	if( cosAdd >= ADDRBONDAR)
		cosAdd = cosAdd - 0x2000;
	//������һʱ�̣���k+1ʱ��У������˲�������wc(k+1)=wc(k)+u*err(k)*cos(w*k)
	MPYS = RESHI;
  	OP2 = notchIteraStep;
  	wc = wc + RESHI;

	//������һʱ�̣���k+1ʱ�̵��˲�������wk(k+1)��kʱ���˲�������wk(k)֮��
	//err*sin(w*k)*step
  	MPYS = err;
  	OP2 = *sinAdd;
  	sinAdd = sinAdd + sinTabStep;
  	//���������ұ�
  	if( sinAdd >= ADDRBONDAR)
  		sinAdd = sinAdd - 0x2000;
	//������һʱ�̣���k+1ʱ��У������˲�������wk(k+1)=wk(k)+u*err(k)*sin(w*k)
  	MPYS = RESHI;
  	OP2 = notchIteraStep;
  	ws = ws + RESHI;

  	MPYS = wc;
  	OP2 = wc;		//wc*wc
  	MACS = ws;
  	OP2 = ws;		//wc*wc+ws*ws

  	envelOut = RESHI;	//�õ���ʱ�̵İ������

  	envelOut = envelOut - envelAveOut;
  	envelOut = envelOut >> 3;	//����3λ������8
  	envelAveOut = envelAveOut + envelOut;
  	if((envelAveOut) >= 3)
  		return 0;
  	else
  		return -1;
 }

//spi��������
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

//dsp���͸�430������
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

//���ڷ�������
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
/* ����ֵ��ʼ�� */
void initVar()
{
  sinTabStep = 0x800;			//���ұ�Ĳ�����
  notchIteraStep = 1000;		//notch�˲����ĵ�������
  ws = *(sinAdd + sinTabStep);	//��ʼ���˲���ws
  wc = *(cosAdd + sinTabStep);	//��ʼ���˲���wc
  err = 0;						//�˲����Ĳв�
  resFilter = 0;				//��ǰʱ���˲��������
//  sinAdd = table;
 // cosAdd = (table + 0x1000);
  stateChange = 0;				//״̬�ı�
  widthCnt = 0;					//��⵽���źŵ���
  noiseCnt = 0;					//��⵽����������
  offCnt = 0;					//
  offProCnt = 0;				//
  envelAveOut = 0;				//�����ƽ�����
}
