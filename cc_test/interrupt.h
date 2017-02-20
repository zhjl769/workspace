/*
 * interrupt.h
 *
 *  Created on: 2016-11-12
 *      Author: Administrator
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_


interrupt void no_isr(void)
{

}


interrupt void timer0_Isr(void)
{
	send_flag=1;

}


#endif /* INTERRUPT_H_ */
