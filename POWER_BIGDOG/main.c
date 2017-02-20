#include  <msp430x14x.h>



void Softdelay();
long Num_of_saves = 2000;
long A0results = 0;       //��������ѹ
long A1results = 0;
long A2results = 0;
long A3results = 0;
long A4results = 0;
long A0saves = 0;       //���Ƚ�
long A1saves = 0;
long A2saves = 0;
long A3saves = 0;
long A4saves = 0;

char ii;
char A0[9] = {"A0:"} ;       //���͵��ַ���
char A1[9] = {"A1:"} ;
char A2[9] = {"A2:"} ;
char A3[9] = {"A3:"} ;
char A4[9] = {"A4:"} ;

float max_voltage = 48;   //����ĵ�ѹֵ
float max_saves = 1489;   //����ѹֵ��Ӧ��AD����ֵ

float A0_value = 0;    //���ڷ�������ת�����ͣ���Ҫ������
float A1_value = 0;
float A2_value = 0;
float A3_value = 0;
float A4_value = 0;
float measuer0,measuer1,measuer2,measuer3,measuer4;     //������ĵ�ѹֵ��������
int a0,a1,a2,a3,a4, b0,b1,b2,b3,b4, c0,c1,c2,c3,c4;     //��ʾ�������͸�TXBUF0
int timeoutCounter;

void Softdelay(long Factor)
  {
	long a,b;
     for (a = 0;a < Factor;a++)
     {
    	 for (b = 0;b < Factor;b++);
     }
  }

void main(void)
{
////��������

  P4DIR |= 0x01F;                           // P1.0~P1.4 output
  P4OUT  = 0x000;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P6SEL |= 0x01F;                           // P6.0~P6.4 ADC option select


  P4OUT  = 0x001;                           // P1.0~P1.4 output
  Softdelay(200);
  ADC12CTL0 = SHT0_2 + ADC12ON + MSC;       // Set sampling time, turn on ADC12
  ADC12CTL1 = SHP + CONSEQ_3+ADC12DIV_4;               // Use sampling timer
//  ADC12CTL1 = SHP + CONSEQ_3;               // Use sampling timer
  ADC12IE = 0x10;                           // Enable interrupt

  ADC12MCTL0 = INCH_0;                      // ref+=AVcc, channel = A0
  ADC12MCTL1 = INCH_1;                      // ref+=AVcc, channel = A1
  ADC12MCTL2 = INCH_2;                      // ref+=AVcc, channel = A2
  ADC12MCTL3 = INCH_3;                      // ref+=AVcc, channel = A3
  ADC12MCTL4 = INCH_4 + EOS;                // ref+=AVcc, channel = A4,�����жϱ�־
  ADC12IE = 0x0010;                         //ʹ���жϱ�־

  ////����ͨ�ų���
  P3SEL = 0xc0;                             // P3.6,7 = USART0 TXD/RXD
  ME1 |= UTXE1 + URXE1;                     // Enabled USART0 TXD/RXD
  UCTL1 |= CHAR;                            // 8-bit character, SWRST=1
  UTCTL1 |= SSEL0;                          // UCLK = ACLK
  UBR01 = 0x03;                             // 9600 from 1Mhz
  UBR11 = 0x00;                             //
  UMCTL1 = 0x4A;                            // Modulation
  UCTL1 &= ~SWRST;                          // Initialize USART state machine
  ADC12CTL0 |= ENC;                         // Conversion enabled
  while(1)
  {
	  ADC12CTL0 |= ADC12SC;                   // Sampling open

    _BIS_SR(CPUOFF + GIE);                  // LPM0, ADC12_ISR will force exit
  }
}

// ADC12 interrupt service routine
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR (void)
{


 A0saves = 0;   //��ƽ��ֵ���ȸ���
 A1saves = 0;
 A2saves = 0;
 A3saves = 0;
 A4saves = 0;

  int n = 0;
  for (n = 0;n < Num_of_saves;n++)
     {
	A0saves += ADC12MEM4;
	A1saves += ADC12MEM3;
	A2saves += ADC12MEM2;
	A3saves += ADC12MEM1;
	A4saves += ADC12MEM0;
      }

	A0saves /= Num_of_saves;//��ƽ��ֵ�����ٴ���Ӱ��
	A1saves /= Num_of_saves;
	A2saves /= Num_of_saves;
	A3saves /= Num_of_saves;
	A4saves /= Num_of_saves;

	if (A0saves > 0x450 && (A0results < 0x430 || A0results > 0x460)) //��48V--0x5D1��24V--0x2E9,36V--45C������ѹ��������ʱ����A0resultsδ��ֵʱ��ʾ������ѹ����ѹ���͵����޸���ʱ����ֵ������ֵ��ͨ��1������������ϣ��Ӵ˲��ٱȽ�A0��ѹ�����ڴ�ͨ��1�����
	        {
		     P4OUT = 0x01;
		     if (A0saves < 0x45C)
		    	 A0results = A0saves;
	        }

	    else if (A1saves > 0x450 && (A1results < 0x430 || A1results > 0x460))//����һ��ͨ����ѹ���͵�36���󣬱Ƚϵڶ���ͨ����ѹ���Դ����ơ�
	            {
		         P4OUT = 0x02;
		         if (A1saves < 0x45C)
		        	 A1results = A1saves;
	            }

	         else if (A2saves > 0x450 && (A2results < 0x430 || A2results > 0x460))
		        {
			     P4OUT = 0x04;
			     if (A2saves < 0x45C)
			    	 A2results = A2saves;
		        }

	              else if (A3saves > 0x450 && (A3results < 0x430 || A3results > 0x460))
		                  {
			               P4OUT = 0x08;
			               if (A3saves < 0x45C)
			            	  A3results = A3saves;
		                   }

	                    else if (A4saves > 0x450 && (A4results < 0x430 || A4results > 0x460))
		                        {
		                    	 P4OUT = 0x10;
			                     if (A4saves < 0x45C)
				                    A4results = A4saves;

		                          }

	                           else P4OUT &= ~0xFF;//û���ˡ�



    _BIC_SR_IRQ(CPUOFF);                    // Clear CPUOFF bit from 0(SR)
}




