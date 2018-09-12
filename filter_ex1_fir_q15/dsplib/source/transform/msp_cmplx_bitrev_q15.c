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
 * Perform an in-place bit reversal of the complex input array using LEASC.
 */
msp_status msp_cmplx_bitrev_q15(const msp_cmplx_bitrev_q15_params *params, _q15 *src)
{
    msp_status status;
    uint16_t length;
    uint16_t sqrtLength;
    uint16_t interruptState;
    MSP_LEASC_BITREVERSECOMPLEX_PARAMS *leascParams;

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
    
    /* Check vector size to determine which bit reverse function to use. */
    length = params->length;
    sqrtLength = 1;
    while (length > 2) {
        sqrtLength <<= 1;
        length >>= 2;
    }
        
    /* Allocate MSP_LEASC_BITREVERSECOMPLEX_PARAMS structure on LEASC stack. */
    LEASCCNF2 -= sizeof(MSP_LEASC_BITREVERSECOMPLEX_PARAMS)/sizeof(uint32_t);
    leascParams = (MSP_LEASC_BITREVERSECOMPLEX_PARAMS *)MSP_LEASC_REVERT_ADDRESS(LEASCCNF2);
    
    /* Initialize MSP_LEASC_BITREVERSECOMPLEX_PARAMS structure. */
    leascParams->sqrtVectorSize = sqrtLength;
    LEASCPMS0 = MSP_LEASC_CONVERT_ADDRESS(src);
    LEASCPMS1 = MSP_LEASC_CONVERT_ADDRESS(leascParams);
    
    /* Check if remainder is even or odd to determine which LEASC function to use. */
    if (length == 2) {
        /* Invoke the LEASCCMD__BITREVERSECOMPLEXODD command with interrupts enabled. */
        LEASCPMCB = LEASCCMD__BITREVERSECOMPLEXODD | LEASCITFLG1;
    }
    else {
        /* Invoke the LEASCCMD__BITREVERSECOMPLEXEVEN command with interrupts enabled. */
        LEASCPMCB = LEASCCMD__BITREVERSECOMPLEXEVEN | LEASCITFLG1;
    }

    /* Clear DSPLib flags, restore interrupts and enter LPM0. */
    msp_leasc_flags = 0;
    __bis_SR_register(GIE+LPM0_bits);

    /* Free MSP_LEASC_BITREVERSECOMPLEX_PARAMS from LEASC stack. */
    LEASCCNF2 += sizeof(MSP_LEASC_BITREVERSECOMPLEX_PARAMS)/sizeof(uint32_t);
    
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
}

#else //MSP_USE_LEASC

/*
 * Perform an in-place bit reversal of the complex input array using a lookup
 * table.
 */
msp_status msp_cmplx_bitrev_q15(const msp_cmplx_bitrev_q15_params *params, _q15 *src)
{
    uint16_t i;                     // loop counter
    uint16_t index;                 // left justified index
    uint16_t indexInc;              // index increment
    uint16_t length;                // src length
    uint16_t indexBitRev;           // index bit reversal
    uint32_t temp;                  // Temporary storage
    uint32_t *srcPtr;               // Treat complex data pairs as 32-bit data
    
    /* Initialize source pointer and length. */
    srcPtr = (uint32_t *)src;
    length = params->length;
    index = 0;
    indexInc = 2;
    
    /* Calculate index increment for left justified index. */
    while (length < 0x8000) {
        indexInc <<= 1;
        length <<= 1;
    }
        
#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Validate that the length is a power of two. */
    if (length != 0x8000) {
        return MSP_SIZE_ERROR;
    }
#endif //DSPLIB_DISABLE_DIAGNOSTICS
    
    /* In-place bit-reversal using a larger table length than source data length. */
    length = params->length;
    for (i = 0; i < length; i++, index += indexInc) {
        /* Calculate bit reversed index. */
        indexBitRev = ((uint16_t)msp_cmplx_bitrev_table_ui8[index & 0xff] << 8)
            + ((uint16_t)msp_cmplx_bitrev_table_ui8[(index >> 8) & 0xff]);
        
        if (i < indexBitRev) {
            /* Swap inputs. */
            temp = srcPtr[i];
            srcPtr[i] = srcPtr[indexBitRev];
            srcPtr[indexBitRev] = temp;
        }
    }
    
    return MSP_SUCCESS;
}

#endif //MSP_USE_LEASC
