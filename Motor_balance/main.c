#include <msp430.h>
#include <stdint.h>
#include "dr_lcdseg.h"
#include "clock_setting.h"
#include "dr_dc_motor.h"

uint16_t adc0;

void initClock()
{
	    unsigned int i;
		//将寄存器的内容清零
		//XT2震荡器开启
		//LFTX1工作在低频模式
		//ACLK的分频因子为1
	    BCSCTL1 &= ~XT2OFF;
	   // BCSCTL1 = 0X00;
	    do
	    {
		IFG1 &= ~OFIFG;                       // 清除OSCFault标志
		for (i = 0x20; i > 0; i--);
	    }
	    while ((IFG1 & OFIFG) == OFIFG);      // 如果OSCFault =1

	    BCSCTL2 = 0X00;			//将寄存器的内容清零
	    BCSCTL2 += SELM1;		//MCLK的时钟源为TX2CLK，分频因子为1
	    BCSCTL2 |= SELS;		//SMCLK的时钟源为TX2CLK，分频因子为1
}

void initADC()
{
  //内存变量初始化
  adc0 = 0;
  //IO设定
  P7SEL |= BIT7; //保证没有输出干扰模拟信号输入
  //TB0设定，用于ADC触发脉冲
  TB0CTL = TBSSEL__SMCLK + MC__UP; // SMCLK, up-mode
  TB0CCR0 = SMCLK_FREQ / 6400 - 1; // 4MHz -> 6.4kHz
  TB0CCTL0 = OUTMOD_4; //实际只有3.2kHz
  //ADC设定
  ADC12CTL1 = ADC12SHS_2 + ADC12SHP + ADC12CONSEQ_1; //TB0.0，脉冲采样，多通道单次
  ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_15 + ADC12EOS; //3.3V电源参考，A15，通道末尾
  ADC12CTL0 = ADC12MSC + ADC12ON + ADC12SHT0_8 + ADC12ENC; //多重采样，ADC12ON，MEM0~MEM7使用256周期采样，ENC
  ADC12IE = BIT0; //开启中断(通道0转换完成后中断)
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
  adc0 = ADC12MEM0; //保存转换结果
  ADC12CTL0 &= ~ADC12ENC; //重置ADC，准备下一次采样
  ADC12CTL0 |= ADC12ENC;
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  _DINT();
  initClock();
  initADC();
  initLcdSeg();
  initDCMotor();
  _EINT();

  while(1)
  {
    int32_t val;
    val = adc0; //获取ADC转换结果
    val = val * PWM_COUNT * 2 / 4095 - PWM_COUNT; //将ADC转换结果从0~4095转换到-399~399
    LCDSEG_DisplayNumber(getRPM(), 0); //更新转速显示
    PWM_SetOutput(val); //修改PWM输出电压
    __delay_cycles(MCLK_FREQ / 10); //延时100ms
  }
}
