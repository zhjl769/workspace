/*
 * init.h
 *
 *  Created on: 2015��12��10��
 *      Author: Zane
 */

#define SMCLK_FREQ 20000000
#define MCLK_FREQ 20000000

#ifndef INIT_H_
#define INIT_H_


void initPort();
void initADC();
void initClock();

void initUart();
void initSpi();



#endif /* INIT_H_ */
