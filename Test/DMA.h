/*
 * DMA.h
 *
 *  Created on: 2016-6-3
 *      Author: zhjl769
 */

#ifndef DMA_H_
#define DMA_H_


//DMA配置变量
volatile short DMAdone;//DMA传输结束标志
volatile unsigned int DMAflg;//DMA接收数据块标志位

__interrupt void DMA_ISR (void);

#endif /* DMA_H_ */
