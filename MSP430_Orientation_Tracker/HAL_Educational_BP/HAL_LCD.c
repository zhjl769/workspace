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
 * @file HAL_LCD.c
 * 
 * @details Description: LCD abstraction layer for Educational BoosterPack.
 * 
 * @author B. Peterson
 * @author Texas Instruments, Inc
 * @date January, 2014
 * @version 1.0
 */
 
/*!
 * @addtogroup HAL_LCD_api
 * @{
 */
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>

#include "HAL_board.h"
#include "HAL_LCD.h"

#define LCD_DATA()              (P2OUT |= BIT3)
#define LCD_INSTRUCTION()       (P2OUT &= ~BIT3)

/*!
 * @brief Send an instruction to the LCD display.
 *
 * Send an instruction to the LCD display.
 *
 * @param ui8Data Instruction to send to the LCD.
 * @return none
 */
static void LCD_sendInstruction(uint8_t ui8Data)
{
    /* Set LCD RS signal to instruction. */
    LCD_INSTRUCTION();
    
    /* Send the instruction to the LCD. */
    while (!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = ui8Data;
    
    return;
}

/*!
 * @brief Write a character to the LCD display.
 *
 * Write a character to the LCD display.
 *
 * @param ui8Data Data to send to the LCD.
 * @return none
 */
static void LCD_sendData(uint8_t ui8Data)
{
    /* Set LCD RS signal to data. */
    LCD_DATA();
    
    /* Send the character to the LCD. */
    while (!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = ui8Data;
    
    return;
}

/*!
 * @brief Initialize the LCD screen.
 *
 * Initialize the pins and peripherals for LCD operation. Perform basic setup
 * of the LCD required for operation.
 *
 * @return none
 */
void LCD_init(void)
{
    /* Configure UCB0 for SPI with SMCLK source. */
    P2DIR |= BIT3;
    P2SEL &= ~BIT3;
    P2SEL2 &= ~BIT3;
    P1SEL |= BIT5 | BIT6 | BIT7;
    P1SEL2 |= BIT5 | BIT6 | BIT7;
    UCB0CTL0 = UCCKPL | UCMST | UCMSB | UCSYNC;
    UCB0CTL1 |= UCSSEL_2;
    UCB0BR0 = (MCLK_FREQ/SPI_FREQ) & 0xff;
    UCB0BR1 = (MCLK_FREQ/SPI_FREQ) >> 8;
    UCB0CTL1 &= ~UCSWRST;
    
    /* Initialize the LCD to basic state. This display is very timing critical. */
    __delay_ms(20);
    LCD_sendInstruction(0x30);   //Function set
    __delay_ms(2);
    LCD_sendInstruction(0x30);   //Function set
    LCD_sendInstruction(0x30);   //Function set
    LCD_sendInstruction(0x39);   //Select IS=1
    LCD_sendInstruction(0x14);   //Freq set
    LCD_sendInstruction(0x6D);   //Set follower control
    LCD_sendInstruction(0x55);   //Set contrast
    LCD_sendInstruction(0x7C);   //Set contrast
    LCD_sendInstruction(0x0C);   //Display on, cursor off
    LCD_sendInstruction(0x06);   //Set direction and shift
    LCD_sendInstruction(0x01);   //Clear display
    __delay_ms(10);
}

/*!
 * @brief Clear the display of characters.
 *
 * Clear the display of characters.
 *
 * @return none
 */
void LCD_clear(void)
{
    /* Send clear instruction and delay 2ms to allow the LCD to clear. */
    LCD_sendInstruction(0x01);
    __delay_ms(2);
    
    return;
}

/*!
 * @brief Set LCD to a row and column position.
 *
 * Set LCD to a row and column position.
 *
 * @param ui8Row Row to set the LCD position, 0-1
 * @param ui8Column Column to set the LCD position, 0-39
 * @return none
 */
void LCD_setPosition(uint8_t ui8Row, uint8_t ui8Column)
{
    uint8_t ui8Instruction = 0x80;
    
    ui8Instruction |= (ui8Row << 6) & 0x40;
    ui8Instruction |= ui8Column;
    
    LCD_sendInstruction(ui8Instruction);
    
    /* Delay 20us to allow the LCD to update. */
    __delay_us(20);
    
    return;
}

/*!
 * @brief Send a string of characters to the LCD display.
 *
 * Send a string of characters to the LCD display.
 *
 * @param pcData Pointer to the string to write to the LCD.
 * @return none
 */
void LCD_writeString(char *pcData)
{
    /* Send the data to the LCD. */
    while (*pcData) {
        LCD_sendData(*pcData++);
    }
    
    return;
}

/*!
 * @brief Send a buffer of character data to the LCD display.
 *
 * Send a buffer of character data to the LCD display.
 *
 * @param pcData Pointer to the data buffer to write.
 * @param ui16Count Size of data buffer to write.
 * @return none
 */
void LCD_writeData(char *pcData, uint16_t ui16Count)
{
    /* Send the data to the LCD. */
    while (ui16Count--) {
        LCD_sendData(*pcData++);
    }
    
    return;
}

/*! @} */
