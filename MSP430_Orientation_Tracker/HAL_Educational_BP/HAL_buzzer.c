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
 * @file HAL_buzzer.c
 * 
 * @details Description: Buzzer abstraction layer for Educational BoosterPack.
 * 
 * @author B. Peterson
 * @author Texas Instruments, Inc
 * @date January, 2014
 * @version 1.0
 */
 
/*!
 * @addtogroup HAL_buzzer_api
 * @{
 */
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>

#include "HAL_board.h"
#include "HAL_buzzer.h"

/*!
 * @brief Setup buzzer for TA0 PWM output.
 *
 * Setup buzzer for TA0 PWM output.
 *
 * @return none
 */
void buzzer_init(void)
{
    P2DIR |= BIT6;
    P2SEL |= BIT6;
    P2SEL &= ~BIT7;
    P2SEL2 &= ~(BIT6 | BIT7);
    
    buzzer_off();
    
    return;
}

/*!
 * @brief Turn on the buzzer using TA0 and CCR1 for PWM.
 *
 * Turn on the buzzer using TA0 and CCR1 for PWM.
 *
 * @return none
 */
void buzzer_on(void)
{
    TA0CCTL1 = OUTMOD_7;
    TA0CCR0 = MCLK_FREQ/(BUZZER_FREQ);
    TA0CCR1 = MCLK_FREQ/(BUZZER_FREQ)/2;
    TA0CTL = TASSEL_2 | ID_0 | MC_1;
}

/*!
 * @brief Turn off the buzzer.
 *
 * Turn off the buzzer.
 *
 * @return none
 */
void buzzer_off(void)
{
    TA0CTL = 0;
}

/*! @} */
