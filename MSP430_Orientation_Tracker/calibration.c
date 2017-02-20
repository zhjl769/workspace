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
 * @file calibration.c
 * 
 * @details Description: Calibrate the accelerometer for operation.
 * 
 * @author B. Peterson
 * @author Texas Instruments, Inc
 * @date January, 2014
 * @version 1.0
 */
 
/*!
 * @addtogroup calibration_api
 * @{
 */
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>

#include "QmathLib.h"

#include "HAL_accelerometer.h"
#include "HAL_board.h"
#include "HAL_flash.h"
#include "HAL_LCD.h"
#include "HAL_RGB_LED.h"

#include "calculation.h"
#include "calibration.h"
#include "display.h"

/*!
 * @brief Characters to display orientation during calibration process.
 *
 * These characters represent the direction to align the sensor during the
 * calibration process. The direction pointed should be opposite the direction
 * or gravity.
 *
 * The calibration characters in order: +X, -X, +Y, -Y, +Z, -Z
 */
char calibrateDirection[6] = {'^', 'v', '<', '>', 'o', 'x'};

/*!
 * @brief Accelerometer channels to use for calibration readings.
 */
const accelerometer_channel calibrateAxis[3] = {
    accelerometer_x,
    accelerometer_y,
    accelerometer_z
};

/*!
 * @brief Scale measurements to store calibration data before writing to flash.
 */
int16_t i16Scale[3];

/*!
 * @brief Offset measurements to store calibration data before writing to flash.
 */
int16_t i16Offset[3];

/*!
 * @brief Calibrate the accelerometer with measurements along all six axis.
 *
 * Calibrate the accelerometer with measurements along all six axis.
 *
 * @return none
 */
void calibrate_accelerometer(void)
{
    uint16_t i, j;          // Loop counters
    int16_t i16Temp;        // Temporary variable
    
    /* Check if we already have calibration data stored. */
    if (flash_isCalibrated()) {
        /* Check if button is pressed at start to store new data. */
        if (!board_buttonPressed()) {
            return;
        }
    }
    
    /* Wait until button is released. */
    while(board_buttonPressed());
    
    /* Clear the LCD and display the direction. */
    LCD_clear();
    LCD_setPosition(0, 0);
    LCD_writeString("CALIBRATE: ");
    
    /* Measure accelerometer along each positive and negative axis. */
    for (i = 0; i < 6; i++) {
        /* Odd iteration: negate scale so result is positive - negative axis. */
        if (i & 1) {
            i16Scale[i>>1] = -i16Scale[i>>1];
        }
        /* Even iteration: initialize scale and offset. */
        else {
            i16Scale[i>>1] = 0;
            i16Offset[i>>1] = 0;
        }
        
        /* Reset position and display the direction. */
        LCD_setPosition(0, 12);
        LCD_writeData(&calibrateDirection[i], 1);
        
        /*
         * Set LED to green and wait for button press, then delay 0.5 second for
         * sensor to stablize.
         */
        RGB_LED_set(RGB_LED_green);
        while(!board_buttonPressed());
        while(board_buttonPressed());
        RGB_LED_set(RGB_LED_red);
        __delay_ms(500);
        
        /* Sample the ADC 8 times at ~8Hz. */
        for (j = 8; j; j--) {
            i16Temp = accelerometer_read(calibrateAxis[i>>1]);
            i16Scale[i>>1] -= i16Temp;
            i16Offset[i>>1] += i16Temp;
            __delay_ms(125);
        }
    }
    
    /* Turn off RGB LED. */
    RGB_LED_set(RGB_LED_none);
    
    /* Save calibration data to flash. */
    flash_writeCalibration(i16Scale, i16Offset);
    
    /* Clear LCD and reset position. */
    LCD_clear();
    LCD_setPosition(0, 0);
}

/*! @} */
