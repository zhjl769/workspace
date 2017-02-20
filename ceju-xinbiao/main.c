
#include <msp430.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define PI 3.141592654


char PWM_table[1250];
int control;
void SetVcoreUp (unsigned int level);
void Port_Mapping(void);

void produceref(void);
void uart_send(void);
void uart_send1(void);
void get_voltage();
void send_sig();
unsigned int PWM_index;
int flag_1s;
int flag_1s_temp;
int counter_timer;
char PC_DATA[2];
int index;
int chuankou;
int voltage;


int _system_pre_init(void)  //定义大数组的时候必须存在此函数
{
	/* Insert your low-level initializations here */
	WDTCTL = WDTPW + WDTHOLD; // Stop Watchdog timer
	/*==================================*/
	/* Choose if segment initialization */
	/* should be done or not. */
	/* Return: 0 to omit initialization */
	/* 1 to run initialization */
	/*==================================*/
	return (1);
}

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
  SetVcoreUp (0x01);
  SetVcoreUp (0x02);  
  SetVcoreUp (0x03); 
  Port_Mapping();
  memset(PC_DATA,0,2*sizeof(char));
  index = 0;
  chuankou = 0;
  counter_timer = 0;
  flag_1s = 0;
  flag_1s_temp = 0;
  
  P1DIR |= BIT1 + BIT2 + BIT3;
  P2DIR |= BIT4 + BIT5;
  P2SEL |= BIT4 + BIT5;
  P2DIR &=~ BIT2;
  P8DIR |= BIT3;
  
  P3DIR &=~ BIT0;
  P3SEL |= BIT0; 
  
  P1DIR = 0xff;
  //AD引脚初始化
  P6SEL |= 0x01;                            // Enable A/D channel A0
  REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to 
                                            // ADC12_A ref control registers
  ADC12CTL0 = ADC12ON+ADC12SHT02+ADC12REFON+ADC12REF2_5V;
                                            // Turn on ADC12, Sampling time
                                            // On Reference Generator and set to
                                            // 2.5V
  ADC12CTL1 = ADC12SHP;                     // Use sampling timer
  ADC12MCTL0 = ADC12SREF_1;                 // Vr+=Vref+ and Vr-=AVss
  for ( i=0; i<0x30; i++);                  // Delay for reference start-up

  ADC12CTL0 |= ADC12ENC;                    // Enable conversions

      //同步端口设置
  P2IES &= ~BIT1;                           // P1.4 Lo/Hi edge
  P2IFG &= ~BIT1;                           // P1.4 IFG cleared
//  P2IE |= BIT0;
  
  P1DIR &=~ BIT0;                            // ACLK set out to pins
  P1SEL |= BIT0;                            
                        

  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
  UCSCTL2 = FLLD_1 + 365;                   // Set DCO Multiplier for 12MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (374 + 1) * 32768 = 12MHz
                                            // Set FLL Div = fDCOCLK/2
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 6xx
  // UG for optimization.
  // 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
  __delay_cycles(375000);
	
  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
 produceref();
  
//串口模块配置
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
  UCA0BR0 = 0x38;                           // 12MHz/38400=312.5 (see User's Guide)
  UCA0BR1 = 0x01;                           //
  UCA0MCTL = UCBRS_4+UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

//定时器配置
                            // CCR0 interrupt enabled
  TA0CCR0 = 39;
  TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, upmode, clear TAR
  PWM_index = 0;
  
  TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TA1CCR0 = 49999;
  TA1CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, contmode, clear TAR
  

  flag_1s = 0;
  flag_1s_temp = 0;
  P1OUT = 0x00;
  P8OUT &=~ BIT3;

  __bis_SR_register(GIE);       // Enter LPM3, interrupts enabled
  
  send_sig();
//  uart_send();

  while(1)
  {
    if(chuankou == 1)
      {
        chuankou = 0;
        P2IE |= BIT1;
        flag_1s_temp = 0;
        flag_1s = 0;
        counter_timer = 0;
        while(flag_1s == 0);
        flag_1s = 0;

        chuankou = 0;
        uart_send1();

      }
    while(1)
    {
    //  control = 0x04&P2IN;
      control = 0x04;
     
      if(flag_1s == 1)
      {
        P8OUT |= BIT3;
        if(control == 0x04)
        {
          send_sig();
          
          
        }
        flag_1s = 0;
        for(i=0;i<5000;i++)
       P8OUT &=~ BIT3;
      }
 //     uart_send();
      if(chuankou == 1)
       {
            TA1CCTL0 &=~ CCIE; 
            flag_1s_temp = 0;
            flag_1s = 0;
            break;
            
       }

    }
  }


}


void produceref(void)
{
  int lfm_head;
  float t_lfm;
  int templfm;
  for(lfm_head = 0; lfm_head < 1250; lfm_head++)
   {
      t_lfm = (float)lfm_head/300000.0;//采样频率400kHz
      templfm = (int)(20000*(cos(2*PI*50000*t_lfm + PI*15000*t_lfm*t_lfm/0.005)));//起始频率50kHz，截止频率70kHz,5ms
      if(templfm < 0)
      {
        PWM_table[lfm_head]=0x0a;//GPIO1=1;GPIO2=0;GPIO3=1;
       }
       else
       {
        PWM_table[lfm_head]=0x06;//GPIO1=1;GPIO2=1;GPIO3=0;
       }


   }
}

void uart_send(void)
{
    get_voltage();
    if(voltage < 2253)
    {
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '0';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = 'O';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '0';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '%';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '\n';                  // TX -> RXed character
    }
    else if(voltage < 2273)
    {
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '0';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '2';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '5';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '%';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '\n';                  // TX -> RXed character
    }
    else if(voltage < 2314)
    {
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '0';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '5';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '0';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '%';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '\n';                  // TX -> RXed character
    }
    else if(voltage < 2396)
    {
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '0';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '7';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '5';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '%';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '\n';                  // TX -> RXed character
    }
    else
    {
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '1';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '0';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '0';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '%';                  // TX -> RXed character
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = '\n';                  // TX -> RXed character
    }
    
}

void uart_send1(void)
{
    
    while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA0TXBUF = 0x0cc;                  // TX -> RXed character
    while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA0TXBUF = 0x0dd;                  // TX -> RXed character
}

void send_sig()
{


      TA0CCTL0 = CCIE;
      while(1)
      {
        if(PWM_index == 1250)
        {
          PWM_index = 0;
          P1OUT = 0x00;
          break;
        }
      }

}
void SetVcoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;              
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}

void Port_Mapping(void)
{
  // Disable Interrupts before altering Port Mapping registers
  __disable_interrupt();     
  // Enable Write-access to modify port mapping registers
  PMAPPWD = 0x02D52;                        
  
  #ifdef PORT_MAP_RECFG                    
  // Allow reconfiguration during runtime
  PMAPCTL = PMAPRECFG;                     
  #endif  
  

  P2MAP4 = PM_UCA0TXD;
  P2MAP5 = PM_UCA0RXD;

  // Disable Write-Access to modify port mapping registers
  PMAPPWD = 0;                              
  #ifdef PORT_MAP_EINT
  __enable_interrupt();                     // Re-enable all interrupts
  #endif  
}


void get_voltage()
{
    ADC12CTL0 |= ADC12SC;                   // Start conversion
    while (!(ADC12IFG & BIT0));
    voltage = ADC12MEM0;

}
// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    PC_DATA[index] = UCA0RXBUF;
    index++;
    if(index == 2)
    {
      index = 0;
    }
    if((PC_DATA[0] == 0xee) && (PC_DATA[1] == 0xff))
    {
      chuankou = 1;
      memset(PC_DATA,0,2*sizeof(char));
    }
    if((PC_DATA[0] == 0xaa) && (PC_DATA[1] == 0xaa))
    {
      uart_send();
      memset(PC_DATA,0,2*sizeof(char));
    }
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;  
  }
}


// Timer1 A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
  P1OUT = PWM_table[PWM_index];                            // Toggle P1.0
  PWM_index++;
  if(PWM_index == 1250)
  {
    TA0CCTL0 &=~ CCIE;
  }
}


// Timer0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
  flag_1s_temp++;
  if(flag_1s_temp == 100)
  {
    flag_1s_temp = 0;
    flag_1s = 1;
  }

}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  counter_timer++;
  P2IFG &= ~BIT1;                         // Clear P1.4 IFG
  if(counter_timer == 3)
  {
    counter_timer = 0;
    TA1CTL |= TACLR;         // SMCLK, contmode, clear TAR
    TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
    
    P2IE &=~ BIT1;
  }

}
