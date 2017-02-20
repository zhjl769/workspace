/*
 * function.h
 *
 *  Created on: 2015Äê12ÔÂ10ÈÕ
 *      Author: Zane
 */

#ifndef _function
#define _function
#define OMAP_POWER_ON (P9OUT |= BIT6)
#define OMAP_POWER_OFF (P9OUT &= ~BIT6)
#define TRANS_POWER_ON (P9OUT &= ~BIT7)
#define TRANS_POWER_OFF (P9OUT |= BIT7)
#define MOTO_POWER_ON (P9OUT |= BIT2)
#define MOTO_POWER_OFF (P9OUT &= ~BIT2)
#define MOTO_RELEASE_ON (P9OUT |= BIT3)
#define MOTO_RELEASE_OFF (P9OUT &= ~BIT3)
void initVar();
void initPort();
void initADC();
void initClock();
int notchFilter( int );
void powerOnAct();
void initUart();
void initSpi();
void spiSend(unsigned char*data,int len);
void uartSend(unsigned char* data,int len);
int dspTo430(void);
#endif

