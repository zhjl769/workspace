/*
 * System.h
 *
 *  Created on: 2016-4-18
 *      Author: zhjl769
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

/*DSP的时钟配置*/

PLL_Config PLLconfig = {
		0,    //PLL锁定，与进入IDLE之前的状态保持一致， Initialize After Idle
		1,    // 时钟发生器切换到旁路模式，并重新开始PLL的锁相过程。Initialize On Break
		6,      //输入时钟乘以该值并除以PLLDIV就是时钟频率。 PLL Multiply value//48MHz
		0,   //0，不分频，1,2分频，2三分频，3四分频。 PLL Divide value
};

/*IIC配置*/
I2C_Config I2Cconfig = {
		0x0000, /* I2COAR */
		0xFFFF, /* I2CIER */
		0xFFFF, /* I2CSTR */
		10, /* I2CCLKL */
		10, /* I2CCLKH */
		0x00FF, /* I2CCNT */
		0x0050, /* I2CSAR */
		0x0020, /* I2CMDR */
		0xFFFF, /* I2CISRC *///3MHz
		0x0018, /* I2CPSC */
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








#endif /* SYSTEM_H_
