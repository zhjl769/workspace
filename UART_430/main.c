#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Variable.h"
#include "Other.h"
#include "Init.h"
#include "LCD1602.h"
#include "ADC12.h"
#include "DS18B20.h"



void main( ) //主函数
 {
   WDTCTL = WDTPW + WDTHOLD; //关看门狗
   IO_Init();//IO口初始化
   Init_CLK();//时钟初始化
   Init_ADC();//ADC12初始化
   UART1_Init(); //调用UART1初始化函数
//   Init_TimerA();//调用TimerA初始化函数
   LCD1602_init();//LCD1602初始化函数
   P1OUT |= BIT1 ;//+ BIT2;
   P1OUT &= ~BIT3+ ~BIT4;
   P2OUT |= BIT0;
   Delay(2000);
   P2OUT &= ~BIT0;
    _EINT(); //调用C编译器内部函数使能中断
   //_BIS_SR(LPM1_bits); //调用C编译器内部对状态寄存器某位置位的函数，
    //LPM_bits=SCG0+CPUOFF，
    // SCG0=0x0040，进入LPM1低功耗工作模式
    // CPUOFF=0x0010 关闭CPU，唤醒所有允许的中断
   //_NOP(); //调用C编译器内部空操作函数
  // 进入处理循环
    while(1)
     {
    	//Breath();
    	ADC12_Start();//获取电压值
    	get_temperature();//获取温度值

     }
 }






