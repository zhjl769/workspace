
/*******************************************************************************
* FILENAME
*   c674xdspinit.h
*
* DESCRIPTION
*   c674x Header File
*
* 	如果用到什么外设，直接在这个头文件中声明一下
*	必须添加"evmomapl138_bsl.lib"
*******************************************************************************/

#ifndef _C674XDSPINIT
#define _C674XDSPINIT 

#include <stdio.h>

#include "types.h"

#include "evmomapl138.h"				//此头文件必须添加

#include "evmomapl138_timer.h"			//若有需要定时器，此文件添加

#include "evmomapl138_i2c.h"			//若有需要i2c，此文件添加

#include "evmomapl138_ddr.h"			//若有需要ddr2，此文件添加

#include "evmomapl138_mcbsp.h"			//若有需要mcbsp，此文件添加

#include "evmomapl138_edma3cc.h"		//若有需要edma3cc，此文件添加	

#include "evmomapl138_uart.h"			//若有需要uart，此文件添加

#include "evmomapl138_gpio.h"			//若有需要uart，此文件添加

#include "evmomapl138_interrupts.h"		//中断使用声明头文件，必须添加

#endif



