/***************************************/
// BMP085 IIC测试程序
// 使用单片机MSP430F1611
// 晶振：8M
// 显示：LCD12864
// 编译环境 CCS5.5
// 时间：2016年2月25日
//****************************************/
#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
	return 0;
}

