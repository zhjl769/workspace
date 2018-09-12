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
 * Perform the last stage of a real FFT of in place using LEA.
 */
msp_status msp_split_q15(const msp_split_q15_params *params, int16_t *src)
{
    uint16_t i;
    uint16_t log2Size;
    uint16_t length;
    uint16_t interruptState;
    msp_status status;
    MSP_LEASC_FFT_PARAMS *leascParams;
    
    /* Get vector length parameter. */
    length = params->length;
    
    /* Calculate log2Size parameter. */
    i = length;
    log2Size = 0;
    while (i > 1) {
        log2Size++;
        i >>= 1;
    }

    /* Save interrupt state and disable interrupts. */
    interruptState = __get_interrupt_state();
    __disable_interrupt();

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that the data arrays are aligned and in a valid memory segment. */
    if (!(MSP_LEASC_VALID_ADDRESS(src, length))) {
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
        
    /* Allocate MSP_LEASC_FFT_PARAMS structure on LEASC stack. */
    LEASCCNF2 -= sizeof(MSP_LEASC_FFT_PARAMS)/sizeof(uint32_t);
    leascParams = (MSP_LEASC_FFT_PARAMS *)MSP_LEASC_REVERT_ADDRESS(LEASCCNF2);

    /* Set MSP_LEASC_FFT_PARAMS structure. */
    leascParams->vectorSize = length;
    leascParams->log2Size = log2Size;
    
    /* Load source arguments to LEA. */
    LEASCPMS0 = MSP_LEASC_CONVERT_ADDRESS(src);
    LEASCPMS1 = MSP_LEASC_CONVERT_ADDRESS(leascParams);

    /* Invoke the LEASCCMD__FFT command with interrupts enabled. */
    LEASCPMCB = LEASCCMD__FFT | LEASCITFLG1;

    /* Clear DSPLib flags, restore interrupts and enter LPM0. */
    msp_leasc_flags = 0;
    __bis_SR_register(GIE+LPM0_bits);

    /* Free MSP_LEASC_FFT_PARAMS from LEASC stack. */
    LEASCCNF2 += sizeof(MSP_LEASC_FFT_PARAMS)/sizeof(uint32_t);
    
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
 * This function performs the last stage of a real FFT of size M in place.
 * Before this step is performed, a size M/2 complex FFT must be performed
 * on the input data with bit reversal. This algorithm has been optimized
 * to use a single coefficient lookup table and a reduced number of multiply
 * operations. The complex result G(k) where k = 0,1,2...N-1 and N = M/2 is
 * calculated as follows:
 *
 *     G(k) = 0.5*(X(k) + X*(N-k)) - 0.5*j*(e^-j2*pi*k/2N)*(X(k) - X*(N-k))
 *
 * This can be optimized by calculating G(N-k) in parallel using the following
 * simplification:
 *
 *     G(N-k) = 0.5*(X(N-k) + X*(N-(N-k))) - 0.5*j*(e^-j2*pi*(N-k)/2N)*(X(N-k) - X*(N-(N-k)))
 *     G(N-k) = 0.5*(X(N-k) + X*(k)) - 0.5*j*(e^-j2*pi*(N-k)/2N)*(X(N-k) - X*(k))
 *     G(N-k) = 0.5*(X(N-k) + X*(k)) - 0.5*j*-(e^-j2*pi*(N-k)/2N)*(X*(k) - X(N-k))
 *     G(N-k) = 0.5*(X(k) + X*(N-k))* - 0.5*j*((e^-j2*pi*k/2N)*(X(k) - X*(N-k)))*
 *
 * The common components of G(k) and G(N-k) can be calculated once as local
 * variables A and B:
 *
 *     A(k) = 0.5*(X(k) + X*(N-k))
 *     B(k) = 0.5*(e^-j2*pi*k/2N)*(X(k) - X*(N-k))
 *
 * Finally, performing substitutions to the G(k) and G(N-k) equations yields
 * the following optimized equations:
 * 
 *     G(k) = A(k) - j*B(k)
 *     G(N-k) = A*(k) - j*B*(k)
 * 
 * Reference: http://www.ti.com/lit/an/spra291/spra291.pdf
 */
msp_status msp_split_q15(const msp_split_q15_params *params, int16_t *src)
{
    uint16_t length;            // src length
    uint16_t tableLength;       // Coefficient table length
    uint16_t coeffOffset;       // Coefficient table increment
    int16_t aR;                 // Temp variable
    int16_t aI;                 // Temp variable
    int16_t bR;                 // Temp variable
    int16_t bI;                 // Temp variable
    int16_t cR;                 // Temp variable
    int16_t cI;                 // Temp variable
    int16_t *srcPtrK;           // Source pointer to X(k)
    int16_t *srcPtrNK;          // Source pointer to X(N-k)
    const int16_t *coeffPtr;    // Coefficient pointer
    
#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that the provided table is the correct length. */
    if (*(uint16_t *)params->twiddleTable < params->length) {
        return MSP_TABLE_SIZE_ERROR;
    }
#endif //DSPLIB_DISABLE_DIAGNOSTICS
    
    /* 
     * Calculate the first result bin (DC component).
     *
     *     X(N) = X(0)
     *     G(0) = 0.5*(X(0) + X*(0)) - j*0.5*(e^-j*0)*(X(0) - X*(0))
     *     G(0) = Xr(0) + Xi(0)
     */
    CMPLX_REAL(src) = CMPLX_REAL(src) + CMPLX_IMAG(src);
    CMPLX_IMAG(src) = 0;
    
    /* Initialize Src(k) and Src(N/2-k) pointers when k=1. */
    srcPtrK = src + CMPLX_INCREMENT;
    srcPtrNK = src + params->length - CMPLX_INCREMENT;
    
    /* Calculate coefficient table offset. */
    length = params->length;
    coeffOffset = 2;
    tableLength = *(uint16_t *)params->twiddleTable;
    while (length < tableLength) {
        coeffOffset *= 2;
        length *= 2;
    }
    
    /* Initialize coefficient pointer to index k=1. */
    coeffPtr = &params->twiddleTable[DSPLIB_TABLE_OFFSET] + coeffOffset;
    
    /*
     * Initialize length of split operations to perform. G(k) and G(N/2-k) are
     * calculated in the same loop iteration so only half of the N/2 iterations
     * are required, N/4. The last iteration where k = N/2-k will be calculated
     * separately.
     */
    length = (params->length >> 2) - 1;
    
    /* If MPY32 is available save control context and set to fractional mode. */
#if defined(__MSP430_HAS_MPY32__)
    uint16_t ui16MPYState = MPY32CTL0;
    MPY32CTL0 = MPYFRAC | MPYDLYWRTEN;
#endif
    
    /* Loop through and perform all of the split operations. */
    while(length--) {
        /* Calculate X(k) - X*(N-k) to local temporary variables. */
        bR = CMPLX_REAL(srcPtrK) - CMPLX_REAL(srcPtrNK);
        bI = CMPLX_IMAG(srcPtrK) + CMPLX_IMAG(srcPtrNK);
        
        /* B(k) = 0.5*(e^-j2*pi*k/2N)*(X(k) - X(N-k)) */
        cR = CMPLX_REAL(coeffPtr) >> 1;
        cI = CMPLX_IMAG(coeffPtr) >> 1;
        __q15cmpy(&bR, &bI, &cR, &cI);
        
        /*
         * Ar(k) = 0.5*(Xr(k) + Xr(N-k))
         * Ai(k) = 0.5*(Xi(k) - Xi(N-k))
         */
        aR = (CMPLX_REAL(srcPtrK) + CMPLX_REAL(srcPtrNK)) >> 1;
        aI = (CMPLX_IMAG(srcPtrK) - CMPLX_IMAG(srcPtrNK)) >> 1;
        
        /*
         * Gr(k) = Ar(k) + Bi(k)
         * Gi(k) = Ai(k) - Br(k)
         * Gr(N-k) = Ar(k) - Bi(k)
         * Gi(N-k) = -(Ai(k) + Br(k))
         */
        CMPLX_REAL(srcPtrK) = aR + bI;
        CMPLX_IMAG(srcPtrK) = aI - bR;
        CMPLX_REAL(srcPtrNK) = aR - bI;
        CMPLX_IMAG(srcPtrNK) = -(aI + bR);
        
        /* Update pointers. */
        srcPtrK += CMPLX_INCREMENT;
        srcPtrNK -= CMPLX_INCREMENT;
        coeffPtr += coeffOffset;
    }
    
    /* Restore MPY32 control context. */
#if defined(__MSP430_HAS_MPY32__)
    MPY32CTL0 = ui16MPYState;
#endif
    
    /* 
     * Calculate the last result bin where k = N/2-k.
     *
     *     X(k) = X(N-k)
     *     G(k) = 0.5*(X(k) + X*(k)) - j*0.5*(e^-j*pi/2)*(X(k) - X*(k))
     *     G(k) = 0.5(2*Xr(k)) - j*0.5*(-j)*(2*j*Xi(k))
     *     G(k) = Xr(k) - j*Xi(k)
     */
    CMPLX_REAL(srcPtrK) = CMPLX_REAL(srcPtrK);
    CMPLX_IMAG(srcPtrK) = -CMPLX_IMAG(srcPtrK);
    
    return MSP_SUCCESS;
}

#endif //MSP_USE_LEASC
