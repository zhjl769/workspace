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
 * Filter source data with biquad direct form 1.
 */
msp_status msp_biquad_df1_q15(const msp_biquad_df1_q15_params *params, const _q15 *src, _q15 *dst)
{
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
    int16_t *coeffVector;
    msp_status status;
    uint16_t interruptState;
    MSP_LEASC_IIR_PARAMS *leascParams;

    /* Save interrupt state and disable interrupts. */
    interruptState = __get_interrupt_state();
    __disable_interrupt();

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that the data arrays are aligned and in a valid memory segment. */
    if (!(MSP_LEASC_VALID_ADDRESS(src, 4) &
          MSP_LEASC_VALID_ADDRESS(dst, 4))) {
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
        
    /* Allocate MSP_LEASC_IIR_PARAMS structure on LEASC stack. */
    LEASCCNF2 -= sizeof(MSP_LEASC_IIR_PARAMS)/sizeof(uint32_t);
    leascParams = (MSP_LEASC_IIR_PARAMS *)MSP_LEASC_REVERT_ADDRESS(LEASCCNF2);
        
    /* Allocate coefficient vector of length six on LEASC stack. */
    LEASCCNF2 -= 6*sizeof(int16_t)/sizeof(uint32_t);
    coeffVector = (int16_t *)MSP_LEASC_REVERT_ADDRESS(LEASCCNF2);
    coeffVector[0] = (params->coeffs)[0];
    coeffVector[1] = (params->coeffs)[1] >> 1;
    coeffVector[2] = 0;
    coeffVector[3] = (params->coeffs)[2];
    coeffVector[4] = (params->coeffs)[3];
    coeffVector[5] = (params->coeffs)[4];
    
    /* Set MSP_LEASC_IIR_PARAMS structure. */
    leascParams->vectorSize = length;
    leascParams->output = MSP_LEASC_CONVERT_ADDRESS(dst);
    leascParams->state = MSP_LEASC_CONVERT_ADDRESS(params->stateBuffer);
    leascParams->coeffs = MSP_LEASC_CONVERT_ADDRESS(coeffVector);
    leascParams->direction = 1;

    /* Load source arguments to LEA. */
    LEASCPMS0 = MSP_LEASC_CONVERT_ADDRESS(src);
    LEASCPMS1 = MSP_LEASC_CONVERT_ADDRESS(leascParams);

    /* Invoke the LEASCCMD__IIR command with interrupts enabled. */
    LEASCPMCB = LEASCCMD__IIR | LEASCITFLG1;

    /* Clear DSPLib flags, restore interrupts and enter LPM0. */
    msp_leasc_flags = 0;
    __bis_SR_register(GIE+LPM0_bits);

    /* Free MSP_LEASC_IIR_PARAMS and coefficient vector from LEASC stack. */
    LEASCCNF2 += sizeof(MSP_LEASC_IIR_PARAMS)/sizeof(uint32_t) + 6*sizeof(int16_t)/sizeof(uint32_t);
        
    /* Set status flag. */
    status = MSP_SUCCESS;

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check LEASC interrupt flags for any errors. */
    if (msp_leasc_flags & LEASCCOVLIFG) {
        status = MSP_LEASC_COMMAND_OVERFLOW;
    }
    else if (msp_leasc_flags & LEASCOORIFG) {
        status = MSP_LEASC_OUT_OF_RANGE;
    }
    else if (msp_leasc_flags & LEASCSDIIFG) {
        status = MSP_LEASC_SCALAR_INCONSISTENCY;
    }
#endif

    /* Restore interrupt state and return status. */
    __set_interrupt_state(interruptState);
    
    return status;
#else //MSP_USE_LEASC
    uint16_t i;
    const _q15 *coeffs;
    
    /* Initialize coefficient pointer. */
    coeffs = params->coeffs;
    
#if defined(__MSP430_HAS_MPY32__)
    /* If MPY32 is available save control context and set to fractional mode. */
    uint16_t ui16MPYState = MPY32CTL0;
    MPY32CTL0 = MPYFRAC | MPYDLYWRTEN + MPYSAT;
    
    /* Calculate first result using state buffer */
    MPYS = params->stateBuffer[0];  OP2  = coeffs[0];
    MACS = params->stateBuffer[1];  OP2  = coeffs[1];
    MACS = src[0];                  OP2  = coeffs[2];
    MACS = -params->stateBuffer[2]; OP2  = coeffs[3];
    MACS = -params->stateBuffer[3]; OP2 =  coeffs[4];  
    dst[0] = RESHI;
    
    /* Calculate second result using state buffer */
    MPYS = params->stateBuffer[1];  OP2  = coeffs[0];
    MACS = src[0];                  OP2  = coeffs[1];
    MACS = src[1];                  OP2  = coeffs[2];
    MACS = -params->stateBuffer[3]; OP2 =  coeffs[3];  
    MACS = -dst[0];                 OP2  = coeffs[4];
    dst[1] = RESHI;

    /* Calculate remaining results. */
    for (i = 2; i < params->length; i++) {
        MPYS = src[i-2];    OP2  = coeffs[0];
        MACS = src[i-1];    OP2  = coeffs[1];
        MACS = src[i];      OP2  = coeffs[2];
        MACS = -dst[i-2];   OP2 =  coeffs[3];  
        MACS = -dst[i-1];   OP2  = coeffs[4];
        dst[i] = RESHI;
    }
        
    /* Restore MPY32 control context. */
    MPY32CTL0 = ui16MPYState;
#else 
    _iq31 result;
    
    /* Calculate first result using state buffer */
    result =  (_iq31)params->stateBuffer[0] * (_iq31)coeffs[0];
    result += (_iq31)params->stateBuffer[1] * (_iq31)coeffs[1];
    result += (_iq31)src[0] * (_iq31)coeffs[2];
    result -= (_iq31)params->stateBuffer[2] * (_iq31)coeffs[3];
    result -= (_iq31)params->stateBuffer[3] * (_iq31)coeffs[4];
    dst[0] = (_q15)__saturate(result >> 15, INT16_MIN, INT16_MAX);

    /* Calculate second result using state buffer */
    result =  (_iq31)params->stateBuffer[1] * (_iq31)coeffs[0];
    result += (_iq31)src[0] * (_iq31)coeffs[1];
    result += (_iq31)src[1] * (_iq31)coeffs[2];
    result -= (_iq31)dst[0] * (_iq31)coeffs[4];
    result -= (_iq31)params->stateBuffer[3] * (_iq31)coeffs[3];
    dst[1] = (_q15)__saturate(result >> 15, INT16_MIN, INT16_MAX);

    /* Calculate remaining results. */
    for (i = 2; i < params->length; i++) {
        result =  (_iq31)src[i-2] * (_iq31)coeffs[0];
        result += (_iq31)src[i-1] * (_iq31)coeffs[1];
        result += (_iq31)src[i] * (_iq31)coeffs[2];
        result -= (_iq31)dst[i-2] * (_iq31)coeffs[3];
        result -= (_iq31)dst[i-1] * (_iq31)coeffs[4];
        dst[i] = (_q15)__saturate(result >> 15, INT16_MIN, INT16_MAX);
    }
#endif
   
    /* Store the last two source and result values in case they're needed for a repeat call */
    params->stateBuffer[0] = src[params->length-2];
    params->stateBuffer[1] = src[params->length-1];
    params->stateBuffer[2] = dst[params->length-2];
    params->stateBuffer[3] = dst[params->length-1];
    
    return MSP_SUCCESS;
#endif //MSP_USE_LEASC
}
