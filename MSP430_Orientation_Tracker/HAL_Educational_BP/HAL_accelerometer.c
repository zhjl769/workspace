/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
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
/*
 * @file HAL_accelerometer.c
 * 
 * @details Description: Accelerometer abstraction layer for Educational BoosterPack.
 * 
 * @author B. Peterson
 * @author Texas Instruments, Inc
 * @date January, 2014
 * @version 1.0
 */
 
/*!
 * @addtogroup HAL_accelerometer_api
 * @{
 */
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>

#include "HAL_board.h"
#include "HAL_accelerometer.h"

/*!
 * @brief Initialize the accelerometer for operation.
 *
 * Initialize the accelerometer for operation. The boosterpack shares pins with
 * the button so the ADC10 will be initialized even time the accelerometer is
 * read and then turned off.
 *
 * @return none
 */
void accelerometer_init(void)
{
    /* Do initialization in read function, nothing here. */
    return;
}

/*!
 * @brief Read a single channel from the accelerometer.
 *
 * Read a channel from the accelerometer corresponding to an axis passed in as
 * an argument.
 *
 * @param channel channel of the accelerometer to read.
 * @return raw ADC reading of the accelerometer channel requested.
 */
int16_t accelerometer_read(accelerometer_channel channel)
{
    uint16_t ui16Result;
    
    /* Initialize ADC10 here since the pin is shared with SW2. */
    ADC10AE0 = BIT4 | BIT3 | BIT0;
    ADC10CTL0 = ADC10ON;
    
    /* Read the ADC channel requested. */
    ADC10CTL1 = channel;
    ADC10CTL0 |= ENC | ADC10SC;
    while (ADC10CTL1 & BUSY);
    ui16Result = ADC10MEM;
    
    /* Turn off ADC10 since the the pin is shared with SW2. */
    ADC10AE0 = 0;
    ADC10CTL0 = 0;
    
    return ui16Result;
}

/*! @} */
