/*
 * uart.h
 *
 *  Created on: 2016Äê3ÔÂ21ÈÕ
 *      Author: zane
 */

#ifndef UART_H_
#define UART_H_

void SEND_BYTE_232(unsigned char send);
void SEND_STR_232(unsigned char *pos);
void SEND_BYTE_485(unsigned char send);
void SEND_STR_485(unsigned char *pos);


#endif /* UART_H_ */
