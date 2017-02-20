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
#include "msp430.h"

#include <stdint.h>
#include <stdbool.h>
#include "../../include/DSPLib.h"

/*
 * Filter source data with biquad direct form 2.
 */
msp_status msp_biquad_df2_q15(msp_biquad_df2_q15_params *params, const _q15 *src, _q15 *dst) {
    uint16_t length;

    /* Initialize local length variable. */
    length = params->length;

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that length parameter is a multiple of two. */
    if (length & 1) {
        return MSP_SIZE_ERROR;
    }
#endif //DSPLIB_DISABLE_DIAGNOSTICS

#if defined(MSP_USE_LEASC)
    msp_status status;
    uint16_t interruptState;
    MSP_LEASC_IIR_DF2_PARAMS *leascParams;

    /* Save interrupt state and disable interrupts. */
    interruptState = __get_interrupt_state();
    __disable_interrupt();

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that the data arrays are aligned and in a valid memory segment. */
    if (!(MSP_LEASC_VALID_ADDRESS(src, 4) & MSP_LEASC_VALID_ADDRESS(dst, 4))) {
        __set_interrupt_state(interruptState);
        return MSP_LEASC_INVALID_ADDRESS;
    }

    /* Return MSP_LEASC_BUSY error if LEA is busy. */
    if (LEASCCNF1 & LEASCBUSY) {
        __set_interrupt_state(interruptState);
        return MSP_LEASC_BUSY;
    }
#endif //DSPLIB_DISABLE_DIAGNOSTICS

    /* Initialize LEASC if it is not enabled. */
    if (!(LEASCPMCTL & LEASCCMDEN)) {
        msp_leasc_init();
    }

    /* Allocate MSP_LEASC_IIR_DF2_PARAMS structure on LEASC stack. */
    LEASCCNF2 -= sizeof(MSP_LEASC_IIR_DF2_PARAMS) / sizeof(uint32_t);
    leascParams = (MSP_LEASC_IIR_DF2_PARAMS *) MSP_LEASC_REVERT_ADDRESS(
            LEASCCNF2);

    /* Set MSP_LEASC_IIR_DF2_PARAMS structure. */
    leascParams->vectorSize = params->length;
    leascParams->output = MSP_LEASC_CONVERT_ADDRESS(dst);
    leascParams->Q1 = params->Q1;
    leascParams->Q2 = params->Q2;
    leascParams->bias = params->bias;
    leascParams->a0 = params->coeffs[0];
    leascParams->a1 = -params->coeffs[1];
    leascParams->a2 = -params->coeffs[2];
    leascParams->a3 = -params->coeffs[3];
    leascParams->b0 = params->coeffs[4];
    leascParams->b1 = params->coeffs[5];
    leascParams->b2 = params->coeffs[6];
    leascParams->reserved1 = 0;
    leascParams->sumMin = params->sumMin;
    leascParams->reserved2 = 0;
    leascParams->sumMax = params->sumMax;

    /* Load source arguments to LEA. */
    LEASCPMS0 = MSP_LEASC_CONVERT_ADDRESS(src);
    LEASCPMS1 = MSP_LEASC_CONVERT_ADDRESS(leascParams);

    /* Invoke the LEASCCMD__IIR_DF2 command with interrupts enabled. */
    LEASCPMCB = LEASCCMD__BIQUADWITHBIASANDMINMAX | LEASCITFLG1;

    /* Clear DSPLib flags, restore interrupts and enter LPM0. */
    msp_leasc_flags = 0;
    __bis_SR_register(GIE + LPM0_bits);

    /* Free MSP_LEASC_IIR_PARAMS and coefficient vector from LEASC stack. */
    LEASCCNF2 += sizeof(MSP_LEASC_IIR_DF2_PARAMS) / sizeof(uint32_t);

    /* Save, min, max, theta values, set status flag. */
    params->Q1 = leascParams->Q1;
    params->Q2 = leascParams->Q2;
    params->sumMin = leascParams->sumMax;
    params->sumMax = leascParams->sumMin;
    status = MSP_SUCCESS;


#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check LEASC interrupt flags for any errors. */
    if (msp_leasc_flags & LEASCCOVLIFG) {
        status = MSP_LEASC_COMMAND_OVERFLOW;
    } else if (msp_leasc_flags & LEASCOORIFG) {
        status = MSP_LEASC_OUT_OF_RANGE;
    } else if (msp_leasc_flags & LEASCSDIIFG) {
        status = MSP_LEASC_SCALAR_INCONSISTENCY;
    }
#endif

    /* Restore interrupt state and return status. */
    __set_interrupt_state(interruptState);

    return status;
#else //MSP_USE_LEASC

    _q15 w0;
    _q15 w1;
    _q15 w2;
    _q15 sumMin;
    _q15 sumMax;
    const _q15 *coeffs;
    int32_t iq31Acc;
    
    /* Initialize states, minimum and maximum. */
    w1 = params->Q1;
    w2 = params->Q2;
    sumMin = INT16_MAX;
    sumMax = INT16_MIN;
    coeffs = params->coeffs;

#if defined(__MSP430_HAS_MPY32__)
    /* If MPY32 is available save control context and set to fractional mode. */
    uint16_t ui16MPYState = MPY32CTL0;
    MPY32CTL0 = MPYFRAC | MPYDLYWRTEN;
#endif

    /* Calculate filtered output using direct form 2. */
    while(length--) {
        /* Calculate intermediate state. */
        iq31Acc = params->bias;
        iq31Acc += __q15mpy(*src++, coeffs[0]);
        iq31Acc += __q15mpy(w1, -coeffs[1]);
        iq31Acc += __q15mpy(w1, -coeffs[2]);
        iq31Acc += __q15mpy(w2, -coeffs[3]);

        /* Saturate and store intermediate state. */
        w0 = (_q15)__saturate(iq31Acc, INT16_MIN, INT16_MAX);
        
        /* Calculate output. */
        iq31Acc  = __q15mpy(w0, coeffs[4]);
        iq31Acc += __q15mpy(w1, coeffs[5]);
        iq31Acc += __q15mpy(w2, coeffs[6]);

        /* Saturate and store intermediate result. */
        *dst = (_q15)__saturate(iq31Acc, INT16_MIN, INT16_MAX);

        /* Update minimum and maximum. */
        if (*dst < sumMin) {
            sumMin = *dst;
        }
        else if (*dst > sumMax) {
            sumMax = *dst;
        }
        
        /* Update pointers and states. */
        dst++;
        w2 = w1;
        w1 = w0;
    }

#if defined(__MSP430_HAS_MPY32__)
    /* Restore MPY32 control context. */
    MPY32CTL0 = ui16MPYState;
#endif

    /* Store the states and minimum and maximum results. */
    params->Q1 = w1;
    params->Q2 = w2;
    params->sumMax = sumMax;
    params->sumMin = sumMin;

    return MSP_SUCCESS;
#endif //MSP_USE_LEASC
}
