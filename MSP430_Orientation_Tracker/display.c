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
 * @file display.c
 * 
 * @details Description: Functions for writing the data to the display.
 * 
 * @author B. Peterson
 * @author Texas Instruments, Inc
 * @date January, 2014
 * @version 1.0
 */
 
/*!
 * @addtogroup display_api
 * @{
 */
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>

#include "QmathLib.h"

#include "HAL_board.h"
#include "HAL_LCD.h"

#include "calculation.h"
#include "calibration.h"
#include "display.h"

/*!
 * @brief Write the measurements to the display.
 *
 * Update the LCD screen with the pitch, roll and magnitude calculations in
 * q12 format. The angles are converted to q6 format and degrees and then
 * to ASCII characters with three integer digits and sign. The magnitude is 
 * converted to ASCII characters with a single integer and single fractional
 * digit.
 *
 * @param q12Pitch Pointer to pitch angle measurement.
 * @param q12Roll Pointer to roll angle measurement.
 * @param q12GravityMagnitude Pointer to gravity magnitude measurement.
 * @return none
 */
void display_update(_q12 *q12Pitch, _q12 *q12Roll, _q12 *q12GravityMagnitude)
{
    _q6 q6tmp;
    char cBuffer[8];
    
    /* Clear LCD and write top row of labels to the LCD. */
    LCD_clear();
    LCD_setPosition(0, 0);
    LCD_writeString("Pitch Roll  Grav");
    
    /* Set display to second line. */
    LCD_setPosition(1, 0);
    
    /* Print pitch angle. */
    q6tmp = _Q12mpy(*q12Pitch, _Q6(RAD_TO_DEG)) + _Q6(0.5);
    _Q6toa(cBuffer, "%3.0f", q6tmp);
    if (q6tmp < 0) {
        LCD_writeData(cBuffer, 4);
    }
    else {
        LCD_writeData("+", 1);
        LCD_writeData(cBuffer, 3);
    }
    
    /* Print roll angle. */
    LCD_writeData("  ", 2);
    q6tmp = _Q12mpy(*q12Roll, _Q6(RAD_TO_DEG)) + _Q6(0.5);
    _Q6toa(cBuffer, "%3.0f", q6tmp);
    if (q6tmp < 0) {
        LCD_writeData(cBuffer, 4);
    }
    else {
        LCD_writeData("+", 1);
        LCD_writeData(cBuffer, 3);
    }
    
    /* Print magnitude. */
    LCD_writeData("  ", 2);
    _Q12toa(cBuffer, "%1.1f", *q12GravityMagnitude + _Q12(0.05));
    LCD_writeData(cBuffer, 3);
    LCD_writeData("G", 1);
}

/*! @} */
