/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
// filter_ex1_fir_q15 - Real 16-bit FIR filter.
//
// This example demonstrates how to use the msp_fir_q15 API to filter 16-bit 
// input data. The input signal is the sum of two signals, one that will pass
// though the filter and one that will be filtered out. The input and result
// can be compared to see the effect of the filter.
//
// Jeremy Friesenhahn, Brent Peterson
// Texas Instruments Inc.
// April 2016
//******************************************************************************
#include "msp430.h"

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "DSPLib.h"

/* Filter parameters */
#define FS              8192
#define COEFF_LENTH     20
#define VECTOR_LENGTH   256

/* Input signals. */
#define FREQUENCY1      200
#define AMPLITUDE1      0.4
#define FREQUENCY2      2100
#define AMPLITUDE2      0.4

/* Filter input */
DSPLIB_DATA(input,4)
_q15 input[VECTOR_LENGTH+COEFF_LENTH];

/* Filter result */
DSPLIB_DATA(result,4)
_q15 result[VECTOR_LENGTH];

/* 15th-order low pass filter coefficients (Fs=8192, Fc=1200) */
DSPLIB_DATA(coeffs,4)
_q15 coeffs[COEFF_LENTH] = {
    _Q15( 0.018674185598117728), _Q15( 0.033546537455869274),
    _Q15( 0.022336553424256996), _Q15(-0.013267589251417564),
    _Q15(-0.050147679212162101), _Q15(-0.055277378073562297),
    _Q15(-0.006765789378712789), _Q15( 0.088926174952780296),
    _Q15( 0.195863038248838360), _Q15( 0.266111946235991990),
    _Q15( 0.266111946235991990), _Q15( 0.195863038248838360),
    _Q15( 0.088926174952780296), _Q15(-0.006765789378712789),
    _Q15(-0.055277378073562297), _Q15(-0.050147679212162101),
    _Q15(-0.013267589251417564), _Q15( 0.022336553424256996),
    _Q15( 0.033546537455869274), _Q15( 0.018674185598117728)
};

/* DSPLib FIR parameter structure */
msp_fir_q15_params params;

/* Benchmark cycle counts */
volatile uint32_t cycleCount;



int _system_pre_init(void)  //定义大数组的时候必须存在此函数
{
	/* Insert your low-level initializations here */
	WDTCTL = WDTPW + WDTHOLD; // Stop Watchdog timer
	/*==================================*/
	/* Choose if segment initialization */
	/* should be done or not. */
	/* Return: 0 to omit initialization */
	/* 1 to run initialization */
	/*==================================*/
	return (1);
}

void main(void)
{
    float pi;
    uint16_t i;
    P1OUT |= BIT0;//P1.0使能端,拉低发射使能端
    P1DIR = 0xFF; //设置P1口方向寄存器P1口作为输出
    P1SEL &= ~BIT0;//P1.0选择I/O功能
    msp_status status;
    
    /* Disable WDT. */
    WDTCTL = WDTPW + WDTHOLD;

    /* Generate Q15 input sin wave. */
    pi = acosf(-1);
    for (i = 0; i < (VECTOR_LENGTH+COEFF_LENTH); i++) {
        input[i] =  _Q15(AMPLITUDE1*sinf(2*pi*FREQUENCY1*(float)i/(float)FS));
        input[i] += _Q15(AMPLITUDE2*sinf(2*pi*FREQUENCY2*(float)i/(float)FS));
    }
    
    /* Initialize the parameter structure. */
    params.length = VECTOR_LENGTH;
    params.tapLength = COEFF_LENTH;
    params.coeffs = coeffs;
    
    /* Invoke the msp_fir_q15 function. */
    msp_benchmarkStart(TIMER_A0_BASE, 16);
    P1OUT &= ~BIT0;//发射上电
    status = msp_fir_q15(&params, input, result);
    P1OUT |= BIT0;//发射下电
    cycleCount = msp_benchmarkStop(TIMER_A0_BASE);
    
    /* Check status flag. */
    if (status != MSP_SUCCESS) {
        /* ERROR! */
        while(1);
    }
    
    /* End of program. */
    while (1);
 }
        
