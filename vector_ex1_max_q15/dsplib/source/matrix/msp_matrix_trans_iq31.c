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
 * Perform matrix transposition of a source matrix.
 */
msp_status msp_matrix_trans_iq31(const msp_matrix_trans_iq31_params *params, const _iq31 *src, _iq31 *dst)
{
    _iq31 rows;
    _iq31 cols;
    uint16_t counter;
    uint16_t col_offset;
    uint16_t row_counter;
    uint16_t dst_row_offset;
    uint16_t src_row_offset;
    
    /* Extract matrix parameters. */
    rows = params->rows; 
    cols = params->cols;
    
    /* Initialize counters. */
    counter = 0;
    col_offset = 0;
    row_counter = rows;
    dst_row_offset = 0;
    src_row_offset = 0;
    
    /* Iterate through source rows. */
    while(row_counter--) {
        /* Iterate through source columns. */
        while(counter < cols) {
            dst[dst_row_offset+col_offset] = src[src_row_offset+counter];
            counter++; col_offset += rows;
        }
        
        /* Update pointers. */
        counter=0;
        col_offset=0;
        dst_row_offset++; 
        src_row_offset+=cols;
    }

    return MSP_SUCCESS;
}


