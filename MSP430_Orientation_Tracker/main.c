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
 * @file main.c
 * 
 * @details Description: main routine of QmathLib Orientation Tracker demo
 * 
 * @author B. Peterson
 * @author Texas Instruments, Inc
 * @date January, 2014
 * @version 1.0
 */
 
/*!
 * @addtogroup main_api
 * @{
 */
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>

#include "QmathLib.h"

#include "HAL_accelerometer.h"
#include "HAL_board.h"
#include "HAL_buzzer.h"
#include "HAL_flash.h"
#include "HAL_LCD.h"
#include "HAL_RGB_LED.h"

#include "calculation.h"
#include "calibration.h"
#include "display.h"

/*!
 * @brief Raw measurement from the 3D-accelerometer.
 */
struct Vector3D i16RawData;

/*!
 * @brief Vector corresponding to gravity in Q12 format.
 */
struct Vector3D q12GravityVector;

/*!
 * @brief Magnitude of the gravity vector in Q12 format.
 */
_q12 q12GravityMagnitude;

/*!
 * @brief Pitch angle in Q12 format.
 */
_q12 q12PitchAngle;

/*!
 * @brief Roll angle in Q12 format.
 */
_q12 q12RollAngle;

/*!
 * @brief Main routine of the application.
 *
 * The main application routine will initialize all of the board and hardware
 * layers for operation. The calibration routine is called and will program
 * calibration data if it is not present.
 *
 * The application will loop forever, reading data from the accelerometer and
 * calculating the magnitude of gravity and current orientation of the board
 * with pitch and roll angles. This data is output to the LCD every other
 * sample.
 *
 * @returns This function never returns.
 */
int main(void) {
    bool bUpdateLCD = true;     // Update LCD ever other sample
    
    /* Initialize board and hardware layers. */
    board_init();
    accelerometer_init();
    buzzer_init();
    flash_init();
    LCD_init();
    RGB_LED_init();
    
    /* Calibrate the accelerometer. Return when done or if skipped. */
    calibrate_accelerometer();
    
    /* Loop forever. */
    while (true) {
        /* Go to sleep and wait for wake up. */
        board_gotoSleep();
        
#ifdef ENABLE_BENCHMARK
        board_benchmarkStart();
#endif
        
        /*Sample the raw accelerometer data. */
        i16RawData.x = accelerometer_read(accelerometer_x);
        i16RawData.y = accelerometer_read(accelerometer_y);
        i16RawData.z = accelerometer_read(accelerometer_z);
        
        /* Compute the gravity vector from the raw accelerometer data. */
        calculate_gravityVector(&i16RawData, &q12GravityVector);
        
        /*
         * Calculate the magnitude of the gravity vector and detect if the
         * device in currently in free fall.
         */
        calculate_magnitude(&q12GravityVector, &q12GravityMagnitude);
        calculate_dropDetection(&q12GravityMagnitude);
        
        /* Calculate the pitch and roll angles using the gravity vector. */
        calculate_angles(&q12GravityVector, &q12PitchAngle, &q12RollAngle);
        
#ifdef ENABLE_BENCHMARK
        board_benchmarkStop();
#endif

        /* Write the data to the LCD every other sample. */
        if (bUpdateLCD ^= 1) {
            display_update(&q12PitchAngle, &q12RollAngle, &q12GravityMagnitude);
        }
    }
}

/*! @} */
