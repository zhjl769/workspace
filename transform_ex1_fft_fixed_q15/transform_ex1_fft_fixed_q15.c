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
// transform_ex1_fft_fixed_q15 - Real FFT with fixed scaling.
//
// This example demonstrates how to use the msp_fft_auto_q15 API to transform a
// real input data array to the frequency domain using the fast fourier
// transform (FFT) with fixed scaling.
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

/* Global defines */
#define SAMPLES     256
#define FS          4096
#define FREQ1       250
#define FREQ2       1000
#define AMPL1       0.25
#define AMPL2       0.10

/* Input vector A */
DSPLIB_DATA(vectorA,MSP_ALIGN_FFT_Q15(SAMPLES))
_q15 vectorA[SAMPLES];

/* Input vector B */
DSPLIB_DATA(vectorB,MSP_ALIGN_FFT_Q15(SAMPLES))
_q15 vectorB[SAMPLES];

/* Allocate DSPLib parameter structures. */
msp_add_q15_params addParams;
msp_fft_q15_params fftParams;

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
    float time;
    uint16_t i;
    msp_status status;
    
    /* Disable WDT. */
    WDTCTL = WDTPW + WDTHOLD;
    
    P1OUT |= BIT0;//P1.0使能端,拉低发射使能端
    P1DIR = 0xFF; //设置P1口方向寄存器P1口作为输出
    P1SEL &= ~BIT0;//P1.0选择I/O功能

    /* Initialize local variables. */
    pi = acosf(-1);

    /*
     * Generate a floating point sine wave and convert to Q15.
     *     time = 1/(SAMPLES*2) * [0:1:SAMPLES-1];
     *     vectorA = AMPL1*sin(2*pi*FREQ1*time);
     *     vectorB = AMPL2*sin(2*pi*FREQ2*time);
     */
    for (i = 0; i < SAMPLES; i++) {
        time = (float)i/(float)FS;
        vectorA[i] = _Q15(AMPL1*sinf(2*pi*FREQ1*time));
        vectorB[i] = _Q15(AMPL2*sinf(2*pi*FREQ2*time));
    }
    
    /* Call vector add function to sum vectorA and vectorB. */
    addParams.length = SAMPLES;
    status = msp_add_q15(&addParams, vectorA, vectorB, vectorB);
    
    /* Check status flag. */
    if (status != MSP_SUCCESS) {
        /* ERROR! */
        __no_operation();
    }
    
    /* Initialize the fft parameter structure. */
    fftParams.length = SAMPLES;
    fftParams.bitReverse = 1;
    fftParams.twiddleTable = msp_cmplx_twiddle_table_256_q15;
    
    /* Call fixed scaling real fft function. */
    msp_benchmarkStart(TIMER_A0_BASE, 16);
    P1OUT &= ~BIT0;//发射上电
    status = msp_fft_fixed_q15(&fftParams, vectorB);
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
