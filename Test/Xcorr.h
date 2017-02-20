/*
 * Xcorr.h
 *
 *  Created on: 2016-6-7
 *      Author: zhjl769
 */
/*
Structure: FIR
Type: Lowpass
Method: Rectangular
QType: _Q15
Name: Fir
Order: 32
Wc: 0.08

*/
#ifndef XCORR_H_
#define XCORR_H_

#include "DSPLib.h"


void Build_rev(float f, unsigned long fsc, _q15 *temp, int Number);
void xcorr(short number, const int *inbuf, short num, const int *ref, _q15 *result);
void msp_mean_q15(short num, short div, short block, _q15 *inb, _q15 *outb);
void msp_Fang_q15(short num, _q15 *inb, _q15 *outb);
void Savefft_q15(short num, short div, _q15 *inb, _q15 *outb);
void DATAabs_q15(unsigned short num, _q15 *inbuf, _q15 *outbuf);
void FFTsave_in_q15(unsigned short num, _q15 *inbuf, _q15 *outbuf);
void FFTsave_out_q15(unsigned short num, _q15 *inbuf, _q15 *outbuf);
void Neicha_q15(uint16_t *MaxIndex_in, float *Maxout, _q15 *inbuf);




#endif /* XCORR_H_ */
