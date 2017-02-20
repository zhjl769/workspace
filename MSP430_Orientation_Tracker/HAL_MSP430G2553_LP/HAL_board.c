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
 * @file HAL_board.c
 * 
 * @details Description: Initialization and board specific configuration for
 * MSP430G2553.
 * 
 * @author B. Peterson
 * @author Texas Instruments, Inc
 * @date January, 2014
 * @version 1.0
 */
 
/*!
 * @addtogroup HAL_board_api
 * @{
 */
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>

#include "HAL_board.h"

/*!
 * @brief Initialize the Launchpad for operation.
 *
 * Initialize the Launchpad for operation.
 *
 * @return none
 */
void board_init(void)
{
    /* Halt the watchdog timer. */
    WDTCTL = WDTPW | WDTHOLD;   

    /* Setup 1MHz DCO and MCLK/SMCLK divider to 1 */
    DCOCTL = 0;
#if (MCLK_FREQ == 1000000)
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
#elif (MCLK_FREQ == 8000000)
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;
#elif (MCLK_FREQ == 12000000)
    BCSCTL1 = CALBC1_12MHZ;
    DCOCTL = CALDCO_12MHZ;
#elif (MCLK_FREQ == 16000000)
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
#else
    #error "MCLK FREQUENCY NOT SUPPORTED BY DCO!!!"
#endif
    
    /*
     * Setup the WDT for LMP wakeup using VLO. WDT is sourced from ACLK which
     * is configured to use the VLO with a divider of 1. The WDT divider is
     * set to 512 for a total of 512. This gives us 12000/512 = ~24 samples
     * per second. Adjust the ACLK and WDT dividers to adjust sample frequency.
     */
    BCSCTL1 |= DIVA_0;
    BCSCTL3 |= LFXT1S_2;
    WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTSSEL | WDTIS1;
    IE1 |= WDTIE;    
    
    /* Setup Switch GPIO to input */
    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    
    /* Setup benchmark pin. */
#ifdef ENABLE_BENCHMARK
    P1SEL &= ~ BIT1;
    P1SEL2 &= ~ BIT1;
    P1DIR |= BIT1;
#endif
    
    return;
}

/*!
 * @brief Enter LPM3 and return when the WDT wakes the device up.
 *
 * Enter LPM3 and return when the WDT wakes the device up.
 *
 * @return none
 */
void board_gotoSleep(void)
{
    /* Enter LPM3 w/ interrupts and wait for WDT wakeup. */
    __bis_SR_register(LPM3_bits + GIE);
    
    return;
}

/*!
 * @brief WDT interrupt to wake up the CPU at fixed sample intervals.
 *
 * WDT interrupt to wake up the CPU at fixed sample intervals.
 *
 * @return none
 */
#pragma vector=WDT_VECTOR
__interrupt void board_watchdogISR (void)
{
    /* Wake up the CPU */
    __bic_SR_register_on_exit(LPM3_bits);
}

/*! @} */
