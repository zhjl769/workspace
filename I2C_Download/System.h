/*
 * System.h
 *
 *  Created on: 2016-10-18
 *      Author: zhjl769
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

/*DSP的时钟配置*/

PLL_Config PLLconfig = {
		0,    //PLL锁定，与进入IDLE之前的状态保持一致， Initialize After Idle
		1,    // 时钟发生器切换到旁路模式，并重新开始PLL的锁相过程。Initialize On Break
		6,   //输入时钟乘以该值并除以PLLDIV就是时钟频率。 PLL Multiply value//48MHz
		0,   //0，不分频，1,2分频，2三分频，3四分频。 PLL Divide value
};

/*IIC配置*/
I2C_Config I2Cconfig = {
		0x0000, /* I2COAR */
		0xFFFF, /* I2CIER */
		0xFFFF, /* I2CSTR */
		6, /* I2CCLKL */
		4, /* I2CCLKH */
		0x00FF, /* I2CCNT */
		0x0050, /* I2CSAR */
		0x0020, /* I2CMDR */
		0xFFFF, /* I2CISRC */
		0x000b, /* I2CPSC  48/(I2CPSC + 1) = 4MHz*/
};

I2C_Setup I2Cinit = {
		1, /* 7 or 10 bit address mode */
		0x0000, /* own address − don’t care if master */
		48, /* clkout value (Mhz) */
		100, /* a number between 10 and 400 */
		0, /* number of bits/byte to be received or */
		/* transmitted (8 bits) */
		0, /* DLB mode */
		1 /* FREE mode of operation */
};

void delay(long x)
{
	long i;
	for(i = 0;i < x;i ++)
	{
	}
}

int check( Uint8 *sword, Uint8 *dword, int len)
{
	while (len > 0)
	{
		if (*sword != *dword)
		{
			return -1;
		}
		sword++;
		dword++;
		len--;
	}
	return 1;
}

#endif /* SYSTEM_H_*/
