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
 * Perform element wise iq31 multiplication and accumulate (dot product) of two source vectors.
 */
msp_status msp_mac_iq31(const msp_mac_iq31_params *params, const _iq31 *srcA, const _iq31 *srcB, _iq31 *result)
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
    
#ifdef MSP_USE_LEASC
    msp_status status;
    uint16_t interruptState;
    MSP_LEASC_MACLONG_PARAMS *leascParams;

    /* Save interrupt state and disable interrupts. */
    interruptState = __get_interrupt_state();
    __disable_interrupt();

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that the data arrays are aligned and in a valid memory segment. */
    if (!(MSP_LEASC_VALID_ADDRESS(srcA, 4) &
          MSP_LEASC_VALID_ADDRESS(srcB, 4) &
          MSP_LEASC_VALID_ADDRESS(result, 4))) {
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
        
    /* Allocate bytes for the MSP_LEASC_MACLONG_PARAMS structure on LEASC stack. */
    LEASCCNF2 -= sizeof(MSP_LEASC_MACLONG_PARAMS)/sizeof(uint32_t);
    leascParams = (MSP_LEASC_MACLONG_PARAMS *)MSP_LEASC_REVERT_ADDRESS(LEASCCNF2);

    /* Set MSP_LEASC_MACLONG_PARAMS structure. */
    leascParams->input2 = MSP_LEASC_CONVERT_ADDRESS(srcB);
    leascParams->output = MSP_LEASC_CONVERT_ADDRESS(result);
    leascParams->blkSize = length;
    leascParams->input1Offset = 1;
    leascParams->input2Offset = 1;


    /* Load source arguments to LEA. */
    LEASCPMS0 = MSP_LEASC_CONVERT_ADDRESS(srcA);
    LEASCPMS1 = MSP_LEASC_CONVERT_ADDRESS(leascParams);

    /* Invoke the LEASCCMD__MACLONGMATRIX command with interrupts enabled. */
    LEASCPMCB = LEASCCMD__MACLONGMATRIX | LEASCITFLG1;

    /* Clear DSPLib flags, restore interrupts and enter LPM0. */
    msp_leasc_flags = 0;
    __bis_SR_register(GIE+LPM0_bits);

    /* Free bytes for the MSP_LEASC_MACLONG_PARAMS from LEASC stack. */
    LEASCCNF2 -= sizeof(MSP_LEASC_MACLONG_PARAMS)/sizeof(uint32_t);
    
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
    
    /* Initialize the result. */
    *result = 0;
    
#ifdef __MSP430_HAS_MPY32__
    /* If MPY32 is available save control context and set to fractional mode. */
    uint16_t ui16MPYState = MPY32CTL0;
    MPY32CTL0 = MPYFRAC | MPYDLYWRTEN | MPYSAT;
#endif //__MSP430_HAS_MPY32__
    
    /* Loop through all vector elements. */
    while (length--) {
        /* Multiply srcA and srcB and accumulate to the result. */
        *result += __q31mpy(*srcA++,*srcB++);
    }
    
#ifdef __MSP430_HAS_MPY32__
    /* Restore MPY32 control context. */
    MPY32CTL0 = ui16MPYState;
#endif //__MSP430_HAS_MPY32__

    return MSP_SUCCESS;
#endif //MSP_USE_LEASC
}
