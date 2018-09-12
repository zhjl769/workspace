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
 * Find the unsigned minimum value of a single source vector.
 */
msp_status msp_min_uq15(const msp_min_uq15_params *params, const _uq15 *src, _uq15 *min, uint16_t *index)
{
    uint16_t length;
    
    /* Initialize the loop counter with the vector length. */
    length = params->length;

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that length parameter is a multiple of two. */
    if (length & 1) {
        return MSP_SIZE_ERROR;
    }
#endif //DSPLIB_DISABLE_DIAGNOSTICS

#if defined(MSP_USE_LEASC)
    int16_t *output;
    msp_status status;
    uint16_t interruptState;
    MSP_LEASC_MINUNSIGNED_PARAMS *leascParams;

    /* Save interrupt state and disable interrupts. */
    interruptState = __get_interrupt_state();
    __disable_interrupt();

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that the data arrays are aligned and in a valid memory segment. */
    if (!(MSP_LEASC_VALID_ADDRESS(src, 4))) {
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
        
    /* Allocate MSP_LEASC_MINUNSIGNED_PARAMS structure on LEASC stack. */
    LEASCCNF2 -= sizeof(MSP_LEASC_MINUNSIGNED_PARAMS)/sizeof(uint32_t);
    leascParams = (MSP_LEASC_MINUNSIGNED_PARAMS *)MSP_LEASC_REVERT_ADDRESS(LEASCCNF2);
        
    /* Allocate output vector of length two on LEASC stack. */
    LEASCCNF2 -= 2*sizeof(int16_t)/sizeof(uint32_t);
    output = (int16_t *)MSP_LEASC_REVERT_ADDRESS(LEASCCNF2);

    /* Set MSP_LEASC_MINUNSIGNED_PARAMS structure. */
    leascParams->blkSize = length;
    leascParams->output = MSP_LEASC_CONVERT_ADDRESS(output);

    /* Load source arguments to LEA. */
    LEASCPMS0 = MSP_LEASC_CONVERT_ADDRESS(src);
    LEASCPMS1 = MSP_LEASC_CONVERT_ADDRESS(leascParams);

    /* Invoke the LEASCCMD__MINUNSIGNED command with interrupts enabled. */
    LEASCPMCB = LEASCCMD__MINUNSIGNED | LEASCITFLG1;

    /* Clear DSPLib flags, restore interrupts and enter LPM0. */
    msp_leasc_flags = 0;
    __bis_SR_register(GIE+LPM0_bits);
    
    /* Write results. */
    *min = output[0];
    *index = output[1] - 1;

    /* Free MSP_LEASC_MINUNSIGNED_PARAMS and vector from LEASC stack. */
    LEASCCNF2 += sizeof(MSP_LEASC_MINUNSIGNED_PARAMS)/sizeof(uint32_t) + 2*sizeof(int16_t)/sizeof(uint32_t);
    
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
    _uq15 temp;
    _uq15 minimum;
    
    /* Initialize the minimum value and index. */
    minimum = UINT16_MAX;
    i = 0;
    
    /* Loop through all vector elements. */
    while (length--) {
        /* Store vector element to local variable. */
        temp = *src++;
        
        /* Compare vector element with current minimum value. */
        if (temp <= minimum) {
            /* Update minimum value and index. */
            minimum = temp;
            i = length;
        }
    }
    
    /* Save local minimum and index to output arguments. */
    *min = minimum;
    *index = params->length - (i + 1);

    return MSP_SUCCESS;
#endif //MSP_USE_LEASC
}
