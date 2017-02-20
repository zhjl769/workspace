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
 * Perform matrix multiplication of two source matrices.
 */
msp_status msp_matrix_mpy_q15(const msp_matrix_mpy_q15_params *params, const _q15 *srcA, const _q15 *srcB, _q15 *dst)
{
    uint16_t srcARows;
    uint16_t srcACols;
    uint16_t srcBRows;
    uint16_t srcBCols;

    /* Initialize the row and column sizes. */
    srcARows = params->srcARows;
    srcACols = params->srcACols;
    srcBRows = params->srcBRows;
    srcBCols = params->srcBCols;

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that column of A equals rows of B */
    if (srcACols != srcBRows) {
        return MSP_SIZE_ERROR;
    }
#endif //DSPLIB_DISABLE_DIAGNOSTICS

#if defined(MSP_USE_LEASC)
    msp_status status;
    uint16_t interruptState;
    MSP_LEASC_MPYMATRIXROW_PARAMS *leascParams;

    /* Save interrupt state and disable interrupts. */
    interruptState = __get_interrupt_state();
    __disable_interrupt();

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that the data arrays are aligned and in a valid memory segment. */
    if (!(MSP_LEASC_VALID_ADDRESS(srcA, 4) &
          MSP_LEASC_VALID_ADDRESS(srcB, 4) &
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

    /* Allocate MSP_LEASC_MPYMATRIXROW_PARAMS structure on LEASC stack. */
    LEASCCNF2 -= sizeof(MSP_LEASC_MPYMATRIXROW_PARAMS)/sizeof(uint32_t);
    leascParams = (MSP_LEASC_MPYMATRIXROW_PARAMS *)MSP_LEASC_REVERT_ADDRESS(LEASCCNF2);

    /* Set status flag. */
    status = MSP_SUCCESS;

    /* Iterate through each row of srcA */
    while (srcARows--) {
        /* Set MSP_LEASC_MPYMATRIXROW_PARAMS structure. */
        leascParams->rowSize = srcBRows;
        leascParams->colSize = srcBCols;
        leascParams->colVector = MSP_LEASC_CONVERT_ADDRESS(srcB);
        leascParams->output = MSP_LEASC_CONVERT_ADDRESS(dst);

        /* Load source arguments to LEA. */
        LEASCPMS0 = MSP_LEASC_CONVERT_ADDRESS(srcA);
        LEASCPMS1 = MSP_LEASC_CONVERT_ADDRESS(leascParams);

        /* Invoke the LEASCCMD__MPYMATRIXROW command with interrupts enabled. */
        LEASCPMCB = LEASCCMD__MPYMATRIXROW | LEASCITFLG1;

        /* Clear DSPLib flags, restore interrupts and enter LPM0. */
        msp_leasc_flags = 0;
        __bis_SR_register(GIE+LPM0_bits);

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
        /* Check LEASC interrupt flags for any errors. */
        if (msp_leasc_flags & LEASCCOVLIFG) {
            status = MSP_LEASC_COMMAND_OVERFLOW;
            break;
        }
        else if (msp_leasc_flags & LEASCOORIFG) {
            status = MSP_LEASC_OUT_OF_RANGE;
            break;
        }
        else if (msp_leasc_flags & LEASCSDIIFG) {
            status = MSP_LEASC_SCALAR_INCONSISTENCY;
            break;
        }
#endif //DSPLIB_DISABLE_DIAGNOSTICS

        /* Increment srcA and dst pointers. */
        srcA += srcACols;
        dst += srcBCols;
    }

    /* Free MSP_LEASC_ADDMATRIX_PARAMS from LEASC stack. */
    LEASCCNF2 += sizeof(MSP_LEASC_ADDMATRIX_PARAMS)/sizeof(uint32_t);

    /* Restore interrupt state. */
    __set_interrupt_state(interruptState);

    return status;
#else //MSP_USE_LEASC
    uint16_t cntr;
    uint16_t dst_row;
    uint16_t dst_col;
    uint16_t row_offset;
    uint16_t col_offset;
    uint16_t dst_row_offset;

    /* In initialize loop counters. */
    cntr = 0;
    dst_row = 0;
    dst_col = 0;
    row_offset = 0;
    col_offset = 0;
    dst_row_offset = 0;

#if defined(__MSP430_HAS_MPY32__)
    /* If MPY32 is available save control context, set to fractional mode, set saturation mode. */
    uint16_t ui16MPYState = MPY32CTL0;
    MPY32CTL0 = MPYFRAC | MPYDLYWRTEN + MPYSAT;

    /* Loop through all srcA rows. */
    while(srcARows--) {
        /* Loop through all srcB columns. */
        while (dst_col < srcBCols) {
            /* Reset result accumulator. */
            MPY32CTL0 &= ~MPYC;
            RESLO = 0; RESHI = 0;
            
            /* Loop through all elements in srcA column and srcB row. */
            while(cntr < srcACols) {
                MACS = srcA[row_offset + cntr];
                OP2 = srcB[col_offset + dst_col];
                col_offset += srcBCols;
                cntr++;
            }
            
            /* Store the result */
            dst[dst_row_offset + dst_col] = RESHI;

            /* Update pointers. */
            dst_col++;
            cntr = 0;
            col_offset = 0;
        }

        /* Update pointers. */
        dst_row++;
        dst_col = 0;
        row_offset += srcACols;
        dst_row_offset += srcBCols;
    }

    /* Restore MPY32 control context, previous saturation state. */
    MPY32CTL0 = ui16MPYState;

#else //__MSP430_HAS_MPY32__
    _iq31 result;

    /* Loop through all srcA rows. */
    while(srcARows--) {
        /* Loop through all srcB columns. */
        while (dst_col < srcBCols) {
            /* Initialize accumulator. */
            result = 0;
            
            /* Loop through all elements in srcA column and srcB row. */
            while(cntr < srcACols) {
                result += (_iq31)srcA[row_offset + cntr] * (_iq31)srcB[col_offset + dst_col];
                col_offset += srcBCols;
                cntr++;
            }

            /* Saturate and store the result */
            dst[dst_row_offset + dst_col] = (_q15)__saturate(result >> 15, INT16_MIN, INT16_MAX);

            /* Update pointers. */
            dst_col++;
            cntr = 0;
            col_offset = 0;
        }

        /* Update pointers. */
        dst_row++;
        dst_col = 0;
        row_offset += srcACols;
        dst_row_offset += srcBCols;
    }
#endif //__MSP430_HAS_MPY32__

    return MSP_SUCCESS;
#endif //MSP_USE_LEASC
}
