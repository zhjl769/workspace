/*
 * DMA.h
 *
 *  Created on: 2016-6-3
 *      Author: zhjl769
 */

#ifndef DMA_H_
#define DMA_H_


//DMA���ñ���
volatile short DMAdone;//DMA���������־
volatile unsigned int DMAflg;//DMA�������ݿ��־λ

__interrupt void DMA_ISR (void);

#endif /* DMA_H_ */
