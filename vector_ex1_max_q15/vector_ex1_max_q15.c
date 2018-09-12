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
// vector_ex1_max_q15 - Find the maximum value and index of a real vector.
//
// This example demonstrates how to use the msp_max_q15 API to find the maximum
// value and index of a real vector.
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
#define SAMPLES     200
#define FS          4096
#define FREQ1       250

/* Input vector A */
DSPLIB_DATA(vectorA,4)
_q15 vectorA[SAMPLES];

/* Result maximum value */
_q15 q15MaxVector;

/* Index of result */
uint16_t uint16MaxIndex;

/* Allocate DSPLib parameter structure. */
msp_max_q15_params params;

/* Benchmark cycle counts */
volatile uint32_t cycleCount;

void main(void)
{
    float pi;
    float time;
    uint16_t i;
    msp_status status;
    
    /* Disable WDT. */
    WDTCTL = WDTPW + WDTHOLD;
    
    /* Initialize local variables. */
    pi = acosf(-1);                      

    /*
     * Generate a floating point sine wave and convert to Q15.
     *     time = 1/(SAMPLES*2) * [0:1:SAMPLES-1];
     *     vectorA = sin(2*pi*FREQ1*time);
     */
    for (i = 0; i < SAMPLES; i++) {
        time = (float)i/(float)FS;
        vectorA[i] = _Q15(sinf(2*pi*FREQ1*time));
    }
    
    /* Initialize the parameter structure. */
    params.length = SAMPLES;
    
    /* Invoke the msp_max_q15 API. */
    msp_benchmarkStart(TIMER_A0_BASE, 1);
    status = msp_max_q15(&params, vectorA, &q15MaxVector, &uint16MaxIndex);
    cycleCount = msp_benchmarkStop(TIMER_A0_BASE);
    
    /* Check status flag. */
    if (status != MSP_SUCCESS) {
        /* ERROR! */
        while(1);
    }
    
    /* End of program. */
    while (1);
}
