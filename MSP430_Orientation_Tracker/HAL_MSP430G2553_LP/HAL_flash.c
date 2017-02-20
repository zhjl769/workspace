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
 * @file HAL_flash.c
 * 
 * @details Description: Flash abstraction layer for MSP430G2553.
 * 
 * @author B. Peterson
 * @author Texas Instruments, Inc
 * @date January, 2014
 * @version 1.0
 */
 
/*!
 * @addtogroup HAL_flash_api
 * @{
 */
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>

#include "HAL_board.h"
#include "HAL_flash.h"

#define CALIBRATION_KEY 0xBE13

/*!
 * @brief Calibration password to check if data is already stored.
 *
 * Use a unique key to check if info memory has already been calibrated for this
 * device. The first time the program is loaded the calibration routine will
 * need to be run to add data.
 */
#if defined (__TI_COMPILER_VERSION__)
	#pragma NOINIT(ui16CalibrationPassword)
	#pragma LOCATION(ui16CalibrationPassword, 0x1000)
	const uint16_t ui16CalibrationPassword;
#elif defined(__IAR_SYSTEMS_ICC__)
	#pragma location=0x1000
	__no_init const uint16_t ui16CalibrationPassword;
#endif

#if defined (__TI_COMPILER_VERSION__)
	#pragma NOINIT(i16CalibrationScale)
	#pragma LOCATION(i16CalibrationScale, 0x1002)
	const int16_t i16CalibrationScale[3];
	#pragma NOINIT(i16CalibrationOffset)
	#pragma LOCATION(i16CalibrationOffset, 0x1008)
	const int16_t i16CalibrationOffset[3];
#elif defined(__IAR_SYSTEMS_ICC__)
	#pragma location=0x1002
	__no_init const int16_t i16CalibrationScale[3];
	#pragma location=0x1008
	__no_init const int16_t i16CalibrationOffset[3];
#endif

/*!
 * @brief Initialize flash controller.
 *
 * Initialize flash controller.
 *
 * @return none
 */
void flash_init(void)
{
    /* Set flash timing generator. */
    FCTL2 = FWKEY + FSSEL_1 + ((MCLK_FREQ/1000000)*3-1);
}


/*!
 * @brief State of the flash calibration data.
 *
 * State of the flash calibration data.
 *
 * @return True if the flash has been calibrated.
 */
bool flash_isCalibrated(void)
{
    return (ui16CalibrationPassword == CALIBRATION_KEY);
}

/*!
 * @brief Write calibration data to flash.
 *
 * Write calibration data to flash.
 *
 * @param i16Scale pointer to the calibration scale data to copy to flash.
 * @param i16Offset pointer to the calibration offset data to copy to flash.
 * @return none
 */
void flash_writeCalibration(int16_t *i16Scale, int16_t *i16Offset)
{
    uint16_t i;         // Loop counter
    
    /* Unlock flash and erase segment with a dummy write. */
    FCTL1 = FWKEY + ERASE;
    FCTL3 = FWKEY;
    ((int16_t *)i16CalibrationScale)[0] = 0;
    
    /* Write the calibration data to flash. */
    FCTL1 = FWKEY + WRT;
    for (i = 0; i < 3; i++) {
        ((int16_t *)i16CalibrationScale)[i] = i16Scale[i];
        ((int16_t *)i16CalibrationOffset)[i] = i16Offset[i];
    }
    
    /* Write the calibration password to the key to indicate data is stored. */
    *(int16_t *)&ui16CalibrationPassword = CALIBRATION_KEY;
    
    /* Lock the flash segment. */
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
}

/*! @} */
