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
 * @file calculation.c
 * 
 * @details Description: Functions to calculate the 3D gravity vector from raw
 *                       accelerometer data, magnitude of a 3D vector, pitch
 *                       and roll angles from a 3D gravity vector and drop
 *                       detection.
 * 
 * @author B. Peterson
 * @author Texas Instruments, Inc
 * @date January, 2014
 * @version 1.0
 */
 
/*!
 * @addtogroup calculation_api
 * @{
 */
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>

#include "QmathLib.h"
     
#include "HAL_board.h"
#include "HAL_buzzer.h"
#include "HAL_LCD.h"

#include "calculation.h"
#include "calibration.h"
#include "display.h"

/*!
 * @brief Calculate the gravity vector from raw accelerometer data.
 *
 * Calculate the 3D gravity vector in q12 format using the stored calibration 
 * offset and scale values from the raw accelerometer data. The calibration
 * data is scaled by 16 so the input must be scaled up to calculate the result.
 * This is accomplished by solving the following equations for each axis.
 *
 * @latexonly
 * \begin{equation}
 * result_x=(input_x-offset_x)/scale_x
 * \end{equation}
 * \begin{equation}
 * result_y=(input_y-offset_y)/scale_y
 * \end{equation}
 * \begin{equation}
 * result_z=(input_z-offset_z)/scale_z
 * \end{equation}
 * @endlatexonly
 *
 * @param i16VInput Pointer to raw accelerometer readings.
 * @param q12VOutput Pointer to gravity vector in Q12 format.
 * @return none
 */
void calculate_gravityVector(struct Vector3D *i16VInput, struct Vector3D *q12VOutput)
{
    /* Calculate the gravity vector using the calibrated offset and scale. */
    q12VOutput->x = _Q12div(_Qmpy16(i16VInput->x) - i16CalibrationOffset[0], i16CalibrationScale[0]);
    q12VOutput->y = _Q12div(_Qmpy16(i16VInput->y) - i16CalibrationOffset[1], i16CalibrationScale[1]);
    q12VOutput->z = _Q12div(_Qmpy16(i16VInput->z) - i16CalibrationOffset[2], i16CalibrationScale[2]);
    
    return;
}

/*!
 * @brief Calculate the magnitude of a 3D input vector in q12 format.
 *
 * This function calculates the magnitude of a 3D vector using the
 * following equation:
 *
 * @latexonly
 * \begin{equation}
 * magnitude=\sqrt[]{x^2+y^2+z^2}
 * \end{equation}
 * @endlatexonly
 *
 * The QmathLib does not include a three input magnitude function so the
 * function is implemented in the following way with two calls to the QmathLib
 * magnitude function.
 *
 * @latexonly
 * \begin{equation}
 * magnitude=\sqrt[]{(\sqrt[]{x^2+y^2})^2+z^2}
 * \end{equation}
 * @endlatexonly
 *
 * @param q12VInput Pointer to gravity vector in Q12 format.
 * @param q12magnitude Pointer to write the magnitude result.
 * @return none
 */
void calculate_magnitude(struct Vector3D *q12VInput, _q12 *q12magnitude)
{
    /* Calculate the magnitude of the gravity vector. */
    *q12magnitude = _Q12mag(_Q12mag(q12VInput->x, q12VInput->y), q12VInput->z);
    
    return;
}

/*!
 * @brief Calculate pitch and roll angles from a 3D input vector in q12 format.
 *
 * Calculate the pitch and roll angles in q12 format using the 3D input vector
 * and the arc tangent function provided by the QmathLib. The roll angle is
 * calculated first and is limited to a range of -PI to +PI. The pitch angle is
 * calculated second and has a range of -PI/2 to +PI/2. The two calculations
 * are shown below.
 *
 * @latexonly
 * \begin{equation}
 * roll=atan(y/z)
 * \end{equation}
 * \begin{equation}
 * pitch=atan(x/\sqrt[]{y^2+z^2})
 * \end{equation}
 * @endlatexonly
 
 * @param q12VInput Pointer to gravity vector in Q12 format.
 * @param q12pitch Pointer to write the Q12 format pitch result.
 * @param q12roll Pointer to write the Q12 format roll result.
 * @return none
 */
void calculate_angles(struct Vector3D *q12VInput, _q12 *q12pitch, _q12 *q12roll)
{
    /* Calculate roll and pitch angles. */
    *q12roll = _Q12atan2(q12VInput->y, q12VInput->z);
    *q12pitch = _Q12atan2(q12VInput->x, _Q12mag(q12VInput->y, q12VInput->z));
    
    return;
}

/*!
 * @brief Turn on the buzzer if the magnitude is below the drop threshold.
 *
 * Compare the q12 magnitude input against the drop threshold. If the magnitude
 * is below the threshold turn on the buzzer.
 *
 * @param q12magnitude Pointer to magnitude in Q12 format.
 * @return none
 */
void calculate_dropDetection(_q12 *q12magnitude)
{
    static bool bBuzzerOn = false;      // Buzzer state
    
    /* If the magnitude is less than the threshold activate the buzzer. */
    if (*q12magnitude < DROP_THRESHOLD) {
        if (!bBuzzerOn) {
            bBuzzerOn = true;
            buzzer_on();
        }
    }
    else {
        bBuzzerOn = false;
        buzzer_off();
    }
}

/*! @} */
