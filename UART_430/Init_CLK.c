/*
 * Init_CLK.c
 *
 *  Created on: 2015-10-17
 *      Author: zhjl769
 */

#include <msp430f1611.h>

void Init_CLK(void)
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


