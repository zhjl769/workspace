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
msp_status msp_matrix_mpy_iq31(const msp_matrix_mpy_iq31_params *params, const _iq31 *srcA, const _iq31 *srcB, _iq31 *dst)
{
    uint16_t cntr;
    uint16_t dst_row;
    uint16_t dst_col;
    uint16_t srcARows;
    uint16_t srcACols;
    uint16_t srcBRows;
    uint16_t srcBCols;
    uint16_t row_offset;
    uint16_t col_offset;
    uint16_t dst_row_offset;

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

    /* In initialize loop counters. */
    cntr = 0;
    dst_row = 0;
    dst_col = 0;
    row_offset = 0;
    col_offset = 0;
    dst_row_offset = 0;

#if defined(__MSP430_HAS_MPY32__)
    int32_t result;

    /* If MPY32 is available save control context, set to fractional mode, set saturation mode. */
    uint16_t ui16MPYState = MPY32CTL0;
    MPY32CTL0 = MPYFRAC | MPYDLYWRTEN + MPYSAT;

    /* Loop through all srcA rows. */
    while(srcARows--) {
        /* Loop through all srcB columns. */
        while (dst_col < srcBCols) {
            /* Reset result registers */
            MPY32CTL0 &= ~MPYC;
            RES3 = 0; RES2 = 0; RES1 = 0; RES0 = 0;

            /* Multiply and accumulate inputs. */
            while(cntr < srcACols) {
                MACS32L = srcA[row_offset + cntr] & 0xffff;
                MACS32H = srcA[row_offset + cntr] >> 16;
                OP2L = srcB[col_offset + dst_col] & 0xffff;
                OP2H = srcB[col_offset + dst_col] >> 16;
                col_offset += srcBCols;
                cntr++;
            }

            /* Store accumulated result. */
            result  = RES2;
            result |= ((uint32_t)RES3 << 16);
            dst[dst_row_offset + dst_col] = result;

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
    int64_t result;

    /* Loop through all srcA rows. */
    while(srcARows--) {
        /* Loop through all srcB columns. */
        while (dst_col < srcBCols) {
            /* Initialize accumulator. */
            result = 0;

            /* Loop through all elements in srcA column and srcB row. */
            while(cntr < srcACols) {
                result += (int64_t)srcA[row_offset+cntr] * (int64_t)srcB[col_offset+dst_col];
                col_offset += srcBCols;
                cntr++;
            }

            /* Scale and saturate accumulator and store to destination. */
            dst[dst_row_offset+dst_col] = (_iq31)__saturate((result >> 31), INT32_MIN, INT32_MAX);

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
}
