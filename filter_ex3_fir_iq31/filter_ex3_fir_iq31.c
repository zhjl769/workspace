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
// filter_ex3_fir_iq31 - Real 32-bit FIR filter.
//
// This example demonstrates how to use the msp_fir_iq31 API to filter 32-bit 
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
#define COEFF_LENTH     42
#define VECTOR_LENGTH   160

/* Input signals. */
#define FREQUENCY1      200
#define AMPLITUDE1      0.4
#define FREQUENCY2      2100
#define AMPLITUDE2      0.4

/* Filter input */
DSPLIB_DATA(input,4)
_iq31 input[VECTOR_LENGTH+COEFF_LENTH];

/* Filter result */
DSPLIB_DATA(result,4)
_iq31 result[VECTOR_LENGTH];

/* 41st-order low pass filter coefficients (Fs=8192, Fc=1200) */
DSPLIB_DATA(coeffs,4)
_iq31 coeffs[COEFF_LENTH] = {
    _IQ31(-0.000892699697462104), _IQ31(-0.003576159571443372), 
    _IQ31(-0.007834369388892412), _IQ31(-0.010894705039662209), 
    _IQ31(-0.008516684083431033), _IQ31( 0.000966111867386252), 
    _IQ31( 0.012801319007017732), _IQ31( 0.017313381522088472), 
    _IQ31( 0.008106385932956473), _IQ31(-0.009355040308503327), 
    _IQ31(-0.018788438273212200), _IQ31(-0.006885739782730809), 
    _IQ31( 0.020642094039339212), _IQ31( 0.038087186469008563), 
    _IQ31( 0.020712821913368942), _IQ31(-0.027153448416223466), 
    _IQ31(-0.064146987812634837), _IQ31(-0.038416599825024711), 
    _IQ31( 0.067756682520723463), _IQ31( 0.212900907629066160), 
    _IQ31( 0.317105105240617080), _IQ31( 0.317105105240617080), 
    _IQ31( 0.212900907629066160), _IQ31( 0.067756682520723463), 
    _IQ31(-0.038416599825024711), _IQ31(-0.064146987812634837), 
    _IQ31(-0.027153448416223466), _IQ31( 0.020712821913368942), 
    _IQ31( 0.038087186469008563), _IQ31( 0.020642094039339212), 
    _IQ31(-0.006885739782730809), _IQ31(-0.018788438273212200), 
    _IQ31(-0.009355040308503327), _IQ31( 0.008106385932956473), 
    _IQ31( 0.017313381522088472), _IQ31( 0.012801319007017732), 
    _IQ31( 0.000966111867386252), _IQ31(-0.008516684083431033), 
    _IQ31(-0.010894705039662209), _IQ31(-0.007834369388892412), 
    _IQ31(-0.003576159571443372), _IQ31(-0.000892699697462104)
};

/* DSPLib FIR parameter structure */
msp_fir_iq31_params params;

/* Benchmark cycle counts */
volatile uint32_t cycleCount;

void main(void)
{
    float pi;
    uint16_t i;
    msp_status status;
    
    /* Disable WDT. */
    WDTCTL = WDTPW + WDTHOLD;

    /* Generate Q15 input sin wave. */
    pi = acosf(-1);
    for (i = 0; i < (VECTOR_LENGTH+COEFF_LENTH); i++) {
        input[i] =  _IQ31(AMPLITUDE1*sinf(2*pi*FREQUENCY1*(float)i/(float)FS));
        input[i] += _IQ31(AMPLITUDE2*sinf(2*pi*FREQUENCY2*(float)i/(float)FS));
    }
    
    /* Initialize the parameter structure. */
    params.length = VECTOR_LENGTH;
    params.tapLength = COEFF_LENTH;
    params.coeffs = coeffs;
    
    /* Invoke the msp_fir_iq31 function. */
    msp_benchmarkStart(TIMER_A0_BASE, 64);
    status = msp_fir_iq31(&params, input, result);
    cycleCount = msp_benchmarkStop(TIMER_A0_BASE);
    
    /* Check status flag. */
    if (status != MSP_SUCCESS) {
        /* ERROR! */
        while(1);
    }
    
    /* End of program. */
    while (1);
 }
        
