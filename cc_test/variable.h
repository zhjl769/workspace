/*
 * variable.h
 *
 *  Created on: 2016-11-10
 *      Author: Administrator
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#pragma DATA_SECTION(Send_sig,"Send")
short Send_sig[4096]={0};
#pragma DATA_SECTION(Send_sig_SARAM,"Send_SARAM")
short Send_sig_SARAM[4096];
short send_flag=0;
/********************************************************************************************************/
/*************************************************定义句柄*************************************************/
/********************************************************************************************************/
DMA_Handle hDmaSend0;
TIMER_Handle mhTimer0,mhTimer1;

/********************************************************************************************************/
/*************************************************DMA配置*************************************************/
/********************************************************************************************************/
volatile Uint16 RevsrcAddrHi,RevsrcAddrLo;
volatile Uint16 Revdst1AddrHi,Revdst1AddrLo;
volatile Uint16 Revdst2AddrHi,Revdst2AddrLo;//DMA接收地址
volatile Uint16 sendsrc0AddrHi, sendsrc0AddrLo;
volatile Uint16 senddst0AddrHi, senddst0AddrLo;
volatile Uint16 sendsrc1AddrHi, sendsrc1AddrLo;
volatile Uint16 senddst1AddrHi, senddst1AddrLo;//发射DMA地址
















#endif /* VARIABLE_H_ */
