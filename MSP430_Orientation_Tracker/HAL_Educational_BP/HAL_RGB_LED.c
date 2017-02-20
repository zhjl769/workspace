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
 * @file HAL_RGB_LED.c
 * 
 * @details Description: RGB LED abstraction layer for Educational BoosterPack.
 * 
 * @author B. Peterson
 * @author Texas Instruments, Inc
 * @date January, 2014
 * @version 1.0
 */
 
/*!
 * @addtogroup HAL_RGB_LED_api
 * @{
 */
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>

#include "HAL_board.h"
#include "HAL_RGB_LED.h"

/*!
 * @brief Initialize the pins for RGB LED operation.
 *
 * Initialize the pins for RGB LED operation.
 *
 * @return none
 */
void RGB_LED_init(void)
{
    P2DIR |= RGB_LED_all;
    P2SEL &= ~RGB_LED_all;
    P2SEL2 &= ~RGB_LED_all;
    
    RGB_LED_off(RGB_LED_all);
    
    return;
}

/*!
 * @brief Turn off an LED color channel.
 *
 * Turn off an LED color channel.
 *
 * @param channel RGB channel to turn off.
 * @return none
 */
void RGB_LED_off(RGB_LED_channel channel)
{
    P2OUT &= ~channel;
}

/*!
 * @brief Turn on an LED color channel.
 *
 * Turn on an LED color channel.
 *
 * @param channel RGB channel to turn on.
 * @return none
 */
void RGB_LED_on(RGB_LED_channel channel)
{
    P2OUT |= channel;
}

/*!
 * @brief Set the LED state.
 *
 * Set the LED state.
 *
 * @param channel RGB channel to set the state to.
 * @return none
 */
void RGB_LED_set(RGB_LED_channel channel)
{
    P2OUT &= ~ RGB_LED_all;
    P2OUT |= channel;
}

/*! @} */
