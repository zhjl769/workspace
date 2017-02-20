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

#if defined(MSP_USE_LEASC)
    
/*
 * Perform element wise complex conjugate of a single complex source vector using LEA.
 */
msp_status msp_cmplx_conj_iq31(const msp_cmplx_conj_iq31_params *params, const _iq31 *src, _iq31 *dst)
{
    uint16_t length;
    msp_status status;
    uint16_t interruptState;
    MSP_LEASC_MPYLONGMATRIX_PARAMS *leascParams;
    
    /* Initialize the vector length. */
    length = params->length;

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that length parameter is a multiple of two. */
    if (length & 1) {
        return MSP_SIZE_ERROR;
    }
#endif //DSPLIB_DISABLE_DIAGNOSTICS

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that the data arrays are aligned and in a valid memory segment. */
    if (!(MSP_LEASC_VALID_ADDRESS(src, 4) && MSP_LEASC_VALID_ADDRESS(dst, 4))) {
        return MSP_LEASC_INVALID_ADDRESS;
    }
#endif //DSPLIB_DISABLE_DIAGNOSTICS

    /* Save interrupt state and disable interrupts. */
    interruptState = __get_interrupt_state();
    __disable_interrupt();

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
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
        
    /* Allocate MSP_LEASC_MPYMATRIX_PARAMS structure on LEASC stack. */
    LEASCCNF2 -= sizeof(MSP_LEASC_MPYLONGMATRIX_PARAMS)/sizeof(uint32_t);
    leascParams = (MSP_LEASC_MPYLONGMATRIX_PARAMS *)MSP_LEASC_REVERT_ADDRESS(LEASCCNF2);

    /* Set MSP_LEASC_MPYLONGMATRIX_PARAMS structure. */
    leascParams->input2 = MSP_LEASC_CONST_ZERO;
    leascParams->output = MSP_LEASC_CONVERT_ADDRESS(&CMPLX_REAL(dst));
    leascParams->blkSize = length;
    leascParams->input1Offset = 2;
    leascParams->input2Offset = 0;
    leascParams->outputOffset = 2;

    /* Load source arguments to LEA. */
    LEASCPMS0 = MSP_LEASC_CONVERT_ADDRESS(&CMPLX_REAL(src));
    LEASCPMS1 = MSP_LEASC_CONVERT_ADDRESS(leascParams);

    /* Invoke the LEASCCMD__ADDLONGMATRIX command with interrupts enabled. */
    LEASCPMCB = LEASCCMD__ADDLONGMATRIX | LEASCITFLG1;

    /* Clear DSPLib flags, restore interrupts and enter LPM0. */
    msp_leasc_flags = 0;
    __bis_SR_register(GIE+LPM0_bits);

    /* Reset MSP_LEASC_MPYLONGMATRIX_PARAMS source 2 and dst pointers. */
    leascParams->input2 = MSP_LEASC_IQ31_CONST_NEG_ONE;
    leascParams->output = MSP_LEASC_CONVERT_ADDRESS(&CMPLX_IMAG(dst));

    /* Load source arguments to LEA. */
    LEASCPMS0 = MSP_LEASC_CONVERT_ADDRESS(&CMPLX_IMAG(src));
    LEASCPMS1 = MSP_LEASC_CONVERT_ADDRESS(leascParams);

    /* Invoke the LEASCCMD__MPYLONGMATRIX command with interrupts enabled. */
    LEASCPMCB = LEASCCMD__MPYLONGMATRIX | LEASCITFLG1;

    /* Clear DSPLib flags, restore interrupts and enter LPM0. */
    msp_leasc_flags = 0;
    __bis_SR_register(GIE+LPM0_bits);

    /* Free MSP_LEASC_MPYMATRIX_PARAMS from LEASC stack. */
    LEASCCNF2 += sizeof(MSP_LEASC_MPYLONGMATRIX_PARAMS)/sizeof(uint32_t);
    
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
#endif //DSPLIB_DISABLE_DIAGNOSTICS

    /* Restore interrupt state and return status. */
    __set_interrupt_state(interruptState);
    
    return status;
}

#else //MSP_USE_LEASC

/*
 * Perform element wise complex conjugate of a single complex source vector.
 */
msp_status msp_cmplx_conj_iq31(const msp_cmplx_conj_iq31_params *params, const _iq31 *src, _iq31 *dst)
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
    
    /* Loop through all vector elements. */
    while (length--) {
        /* Take the complex conjugate of src and store to dst. */
        *dst++ = *src++;    // real
        *dst++ = -*src++;   // imaginary
    }

    return MSP_SUCCESS;
}

#endif //MSP_USE_LEASC
