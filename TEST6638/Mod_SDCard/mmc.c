// ***********************************************************
// File: mmc.c 
// Description: Library to access a MultiMediaCard 
//              functions: init, read, write ...
//  C. Speck / S. Schauer
//  Texas Instruments, Inc
//  June 2005
//
// Version 1.1
//   corrected comments about connection the MMC to the MSP430
//   increased timeout in mmcGetXXResponse
//
// ***********************************************************
// MMC Lib
// ***********************************************************

/* ***********************************************************
 * THIS PROGRAM IS PROVIDED "AS IS". TI MAKES NO WARRANTIES OR
 * REPRESENTATIONS, EITHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
 * COMPLETENESS OF RESPONSES, RESULTS AND LACK OF NEGLIGENCE.
 * TI DISCLAIMS ANY WARRANTY OF TITLE, QUIET ENJOYMENT, QUIET
 * POSSESSION, AND NON-INFRINGEMENT OF ANY THIRD PARTY
 * INTELLECTUAL PROPERTY RIGHTS WITH REGARD TO THE PROGRAM OR
 * YOUR USE OF THE PROGRAM.
 *
 * IN NO EVENT SHALL TI BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * CONSEQUENTIAL OR INDIRECT DAMAGES, HOWEVER CAUSED, ON ANY
 * THEORY OF LIABILITY AND WHETHER OR NOT TI HAS BEEN ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGES, ARISING IN ANY WAY OUT
 * OF THIS AGREEMENT, THE PROGRAM, OR YOUR USE OF THE PROGRAM.
 * EXCLUDED DAMAGES INCLUDE, BUT ARE NOT LIMITED TO, COST OF
 * REMOVAL OR REINSTALLATION, COMPUTER TIME, LABOR COSTS, LOSS
 * OF GOODWILL, LOSS OF PROFITS, LOSS OF SAVINGS, OR LOSS OF
 * USE OR INTERRUPTION OF BUSINESS. IN NO EVENT WILL TI'S
 * AGGREGATE LIABILITY UNDER THIS AGREEMENT OR ARISING OUT OF
 * YOUR USE OF THE PROGRAM EXCEED FIVE HUNDRED DOLLARS
 * (U.S.$500).
 *
 * Unless otherwise stated, the Program written and copyrighted
 * by Texas Instruments is distributed as "freeware".  You may,
 * only under TI's copyright in the Program, use and modify the
 * Program without any charge or restriction.  You may
 * distribute to third parties, provided that you transfer a
 * copy of this license to the third party and the third party
 * agrees to these terms by its first use of the Program. You
 * must reproduce the copyright notice and any other legend of
 * ownership on each copy or partial copy, of the Program.
 *
 * You acknowledge and agree that the Program contains
 * copyrighted material, trade secrets and other TI proprietary
 * information and is protected by copyright laws,
 * international copyright treaties, and trade secret laws, as
 * well as other intellectual property laws.  To protect TI's
 * rights in the Program, you agree not to decompile, reverse
 * engineer, disassemble or otherwise translate any object code
 * versions of the Program to a human-readable form.  You agree
 * that in no event will you alter, remove or destroy any
 * copyright notice included in the Program.  TI reserves all
 * rights not specifically granted under this license. Except
 * as specifically provided herein, nothing in this agreement
 * shall be construed as conferring by implication, estoppel,
 * or otherwise, upon you, any license or other right under any
 * TI patents, copyrights or trade secrets.
 *
 * You may not use the Program in non-TI devices.
 * ********************************************************* */

// ********************************************************
//
//   
//
//            MSP430F149                  MMC Card 
//         -----------------          -----------------
//     /|\|              XIN|-   /|\ |                 |
//      | |                 |     |  |                 |
//      --|RST          XOUT|-    |--|Pin4/Vcc         |
//        |                 |        |                 |
//        |                 |        |                 |
//        |          (P5.5) |<-------|Pin6/CD          |
//        |            P5.4 |------->|Pin1/CS          |
//        |                 |        |                 |
//        |      P5.2/SOMI1 |<-------|Pin2/DIN         |
//        |      P5.1/SIMO1 |------->|Pin7/DOUT        |
//        |      P5.3/UCLK1 |------->|Pin5/CLK         |
//        |                 |        |                 |
//        |                 |     |--|Pin3/GND         |
//                                |
//                                =
//
//  Pin configuration at MSP430F149:
//  --------------------------------
//  MSP430F149      MSP Pin        MMC             MMC Pin
//  -------------------------------------------------------------
//  P5.4              48           ChipSelect       1
//  P5.2 / SOMI       46           DataIn           2
//                                 GND              3 (0 V)
//                                 VDD              4 (3.3 V)
//  P5.3 / UCLK1      47           Clock            5
//  P5.5              44           CardDetect       6
//  P5.1 / SIMO       45           DataOut          7
//  -------------------------------------------------------------
//
//
//
//
#ifndef _MMCLIB_C
#define _MMCLIB_C
//
//---------------------------------------------------------------
#include "mmc.h"
#include "msp430f6638.h"

//#define withDMA

// Function Prototypes
char mmcGetResponse(void);
char mmcGetXXResponse(const char resp);
char mmcCheckBusy(void);
void initSPI(void);
uint8 spiSendByte(const uint8 data);
char mmc_GoIdle();

// Varialbes
char mmc_buffer[512] =
{ 0 };               // Buffer for mmc i/o for data and registers

//---------------------------------------------------------------------

// setup usart1 in spi mode
void initSPI(void)
{
	UCB0CTL1 |= UCSWRST;                           // 8-bit SPI Master **SWRST**
	UCB0CTL0 = UCCKPH | UCMODE_0 | UCMST | UCSYNC | UCMSB; // SMCLK, 3-pin mode, clock idle low, data valid on rising edge, UCLK delayed
	UCB0CTL1 = UCSSEL_2 | UCSWRST;         // 8-bit SPI Master **SWRST**
	UCB0BR0 = 0x02;             // 0x02: UCLK/2 (4 MHz), works also with 3 and 4
	UCB0BR1 = 0x00;                             // -"-
	//UCMCTL = 0x00;                            // no modulation
	UCB0CTL1 &= ~UCSWRST;                          // clear SWRST
	//ME2 |= USPIE1;                            // Enable USART1 SPI mode
	while (!(UCB0IFG & UCTXIFG))
		;                // USART1 TX buffer ready (empty)?
}

// Initialize MMC card
char initMMC(void)
{

	//raise SS and MOSI for 80 clock cycles
	//SendByte(0xff) 10 times with SS high
	//RAISE SS
	int i;

	// Port 5 Function           Dir       On/Off
	//         5.5-mmcCD         Inp       0 - card inserted
	//         5.1-Dout          Out       0 - off    1 - On -> init in SPI_Init
	//         5.2-Din           Inp       0 - off    1 - On -> init in SPI_Init
	//         5.3-Clk           Out       -                 -> init in SPI_Init
	//         5.4-mmcCS         Out       0 - Active 1 - none Active

	//P5SEL |= 0x0E;
	//P5SEL &= ~0x11;
	//P5OUT |= 0x10;
	//P5DIR |= 0x1A;
	P2SEL |= BIT1 + BIT2 + BIT3;
	P2DIR |= BIT1 + BIT3;
	//P3REN |= BIT1;
	//P3OUT |= BIT1;
	P5SEL &= ~BIT6;
	P5OUT |= BIT6;
	P5DIR |= BIT6;
	initSPI();
	//initialization sequence on PowerUp
	CS_HIGH();
	for (i = 0; i <= 9; i++)
		spiSendByte(0xff);

	return (mmc_GoIdle());
}

char mmc_GoIdle()
{
	char response = 0x01;
	CS_LOW();

	//Send Command 0 to put MMC in SPI mode
	mmcSendCmd(MMC_GO_IDLE_STATE, 0, 0x95);
	//Now wait for READY RESPONSE
	if (mmcGetResponse() != 0x01)
		return MMC_INIT_ERROR;

	while (response == 0x01)
	{
		CS_HIGH();
		spiSendByte(0xff);
		CS_LOW();
		mmcSendCmd(MMC_SEND_OP_COND, 0x00, 0xff);
		response = mmcGetResponse();
	}
	CS_HIGH();
	spiSendByte(0xff);
	return (MMC_SUCCESS);
}

// mmc Get Responce
char mmcGetResponse(void)
{
	//Response comes 1-8bytes after command
	//the first bit will be a 0
	//followed by an error code
	//data will be 0xff until response
	int i = 0;

	char response;

	while (i <= 64)
	{
		response = spiSendByte(0xff);
		if (response == 0x00)
			break;
		if (response == 0x01)
			break;
		i++;
	}
	return response;
}

char mmcGetXXResponse(const char resp)
{
	//Response comes 1-8bytes after command
	//the first bit will be a 0
	//followed by an error code
	//data will be 0xff until response
	int i = 0;

	char response;

	while (i <= 1000)
	{
		response = spiSendByte(0xff);
		if (response == resp)
			break;
		i++;
	}
	return response;
}
char mmcCheckBusy(void)
{
	//Response comes 1-8bytes after command
	//the first bit will be a 0
	//followed by an error code
	//data will be 0xff until response
	int i = 0;

	char response;
	char rvalue;
	while (i <= 64)
	{
		response = spiSendByte(0xff);
		response &= 0x1f;
		switch (response)
		{
		case 0x05:
			rvalue = MMC_SUCCESS;
			break;
		case 0x0b:
			return (MMC_CRC_ERROR);
		case 0x0d:
			return (MMC_WRITE_ERROR);
		default:
			rvalue = MMC_OTHER_ERROR;
			break;
		}
		if (rvalue == MMC_SUCCESS)
			break;
		i++;
	}
	i = 0;
	do
	{
		response = spiSendByte(0xff);
		i++;
	} while (response == 0);
	return response;
}

// The card will respond with a standard response token followed by a data
// block suffixed with a 16 bit CRC.

char mmcReadBlock(const uint32 address, const uint32 count, uint8 *pBuffer)
{
	uint32 i = 0;
	char rvalue = MMC_RESPONSE_ERROR;

	// Set the block length to read
	if (mmcSetBlockLength(count) == MMC_SUCCESS)   // block length could be set
	{
		// SS = LOW (on)
		CS_LOW ();
		// send read command MMC_READ_SINGLE_BLOCK=CMD17
		mmcSendCmd(MMC_READ_SINGLE_BLOCK, address, 0xFF);
		// Send 8 Clock pulses of delay, check if the MMC acknowledged the read block command
		// it will do this by sending an affirmative response
		// in the R1 format (0x00 is no errors)
		if (mmcGetResponse() == 0x00)
		{
			// now look for the data token to signify the start of
			// the data
			if (mmcGetXXResponse(
					MMC_START_DATA_BLOCK_TOKEN) == MMC_START_DATA_BLOCK_TOKEN)
			{
#ifndef withDMA
				// clock the actual data transfer and receive the bytes; spi_read automatically finds the Data Block
				for (i = 0; i < count; i++)
					pBuffer[i] = spiSendByte(0xff); // is executed with card inserted
#else
							U1IFG &= ~(URXIFG1 + URXIFG1); /* clear flags */
							/* Get the block */
							/* DMA trigger is UART1 receive for both DMA0 and DMA1 */
							DMACTL0 &= ~(DMA0TSEL_15 | DMA1TSEL_15);
							DMACTL0 |= (DMA0TSEL_9 | DMA1TSEL_9);
							/* Source DMA address: receive register.  */
							DMA0SA = U1RXBUF_;
							/* Destination DMA address: the user data buffer. */
							DMA0DA = (unsigned short)pBuffer;
							/* The size of the block to be transferred */
							DMA0SZ = count;
							/* Configure the DMA transfer*/
							DMA0CTL =
							DMAIE | /* Enable interrupt */
							DMADT_0 | /* Single transfer mode */
							DMASBDB | /* Byte mode */
							DMAEN | /* Enable DMA */
							DMADSTINCR1 | DMADSTINCR0; /* Increment the destination address */

							/* We depend on the DMA priorities here.  Both triggers occur at
							 the same time, since the source is identical.  DMA0 is handled
							 first, and retrieves the byte.  DMA1 is triggered next, and
							 sends the next byte. */
							/* Source DMA address: constant 0xFF (don't increment)*/
							DMA1SA = U1TXBUF_;
							/* Destination DMA address: the transmit buffer. */
							DMA1DA = U1TXBUF_;
							/* Increment the destination address */
							/* The size of the block to be transferred */
							DMA1SZ = count-1;
							/* Configure the DMA transfer*/
							DMA1CTL =
							DMADT_0 | /* Single transfer mode */
							DMASBDB | /* Byte mode */
							DMAEN; /* Enable DMA */

							/* Kick off the transfer by sending the first byte */
							U1TXBUF = 0xFF;
//      while (DMA0CTL & DMAEN) _NOP(); //LPM0;  // wait till done
//      while (DMA0CTL & DMAEN) _EINT(); LPM0;  // wait till done
							_EINT(); LPM0;// wait till done
#endif
				// get CRC bytes (not really needed by us, but required by MMC)
				spiSendByte(0xff);
				spiSendByte(0xff);
				rvalue = MMC_SUCCESS;
			}
			else
			{
				// the data token was never received
				rvalue = MMC_DATA_TOKEN_ERROR;      // 3
			}
		}
		else
		{
			// the MMC never acknowledge the read command
			rvalue = MMC_RESPONSE_ERROR;          // 2
		}
	}
	else
	{
		rvalue = MMC_BLOCK_SET_ERROR;           // 1
	}
	CS_HIGH ();
	spiSendByte(0xff);
	return rvalue;
}           // mmc_read_block

//---------------------------------------------------------------------
//char mmcWriteBlock (const uint32 address)
char mmcWriteBlock(const uint32 address, const uint32 count, uint8 *pBuffer)
{
	uint32 i = 0;
	char rvalue = MMC_RESPONSE_ERROR;         // MMC_SUCCESS;
	//  char c = 0x00;

	// Set the block length to read
	if (mmcSetBlockLength(count) == MMC_SUCCESS)   // block length could be set
	{
		// SS = LOW (on)
		CS_LOW ();
		// send write command
		mmcSendCmd(MMC_WRITE_BLOCK, address, 0xFF);

		// check if the MMC acknowledged the write block command
		// it will do this by sending an affirmative response
		// in the R1 format (0x00 is no errors)
		if (mmcGetXXResponse(MMC_R1_RESPONSE) == MMC_R1_RESPONSE)
		{
			spiSendByte(0xff);
			// send the data token to signify the start of the data
			spiSendByte(0xfe);
			// clock the actual data transfer and transmitt the bytes
#ifndef withDMA
			for (i = 0; i < count; i++)
				spiSendByte(pBuffer[i]);
#else
			/* Get the block */
			/* DMA trigger is UART send */
			DMACTL0 &= ~(DMA0TSEL_15);
			DMACTL0 |= (DMA0TSEL_9);
			/* Source DMA address: the data buffer.  */
			DMA0SA = (unsigned short)pBuffer;
			/* Destination DMA address: the UART send register. */
			DMA0DA = U1TXBUF_;
			/* The size of the block to be transferred */
			DMA0SZ = count;
			/* Configure the DMA transfer*/
			DMA0CTL =
			DMAREQ | /* start transfer */
			DMADT_0 | /* Single transfer mode */
			DMASBDB | /* Byte mode */
			DMAEN | /* Enable DMA */
			DMASRCINCR1 | DMASRCINCR0; /* Increment the source address */
#endif
			// put CRC bytes (not really needed by us, but required by MMC)
			spiSendByte(0xff);
			spiSendByte(0xff);
			// read the data response xxx0<status>1 : status 010: Data accected, status 101: Data
			//   rejected due to a crc error, status 110: Data rejected due to a Write error.
			mmcCheckBusy();
			rvalue = MMC_SUCCESS;
		}
		else
		{
			// the MMC never acknowledge the write command
			rvalue = MMC_RESPONSE_ERROR;   // 2
		}
	}
	else
	{
		rvalue = MMC_BLOCK_SET_ERROR;   // 1
	}
	// give the MMC the required clocks to finish up what ever it needs to do
	//  for (i = 0; i < 9; ++i)
	//    spiSendByte(0xff);

	CS_HIGH ();
	// Send 8 Clock pulses of delay.
	spiSendByte(0xff);
	return rvalue;
} // mmc_write_block

//---------------------------------------------------------------------
void mmcSendCmd(const char cmd, uint32 data, const char crc)
{
	char frame[6];
	char temp;
	int i;
	frame[0] = (cmd | 0x40);
	for (i = 3; i >= 0; i--)
	{
		temp = (char) (data >> (8 * i));
		frame[4 - i] = (temp);
	}
	frame[5] = (crc);
	for (i = 0; i < 6; i++)
		spiSendByte(frame[i]);
}

//--------------- set blocklength 2^n ------------------------------------------------------
char mmcSetBlockLength(const uint32 blocklength)
{
	//  char rValue = MMC_TIMEOUT_ERROR;
	//  char i = 0;
	// SS = LOW (on)
	CS_LOW ();
	// Set the block length to read
	//MMC_SET_BLOCKLEN =CMD16
	mmcSendCmd(MMC_SET_BLOCKLEN, blocklength, 0xFF);

	// get response from MMC - make sure that its 0x00 (R1 ok response format)
	if (mmcGetResponse() != 0x00)
	{
		initMMC();
		mmcSendCmd(MMC_SET_BLOCKLEN, blocklength, 0xFF);
		mmcGetResponse();
	}

	CS_HIGH ();

	// Send 8 Clock pulses of delay.
	spiSendByte(0xff);

	return MMC_SUCCESS;
} // Set block_length

uint8 spiSendByte(const uint8 data)
{
	while ((UCB0IFG & UCTXIFG) == 0)
		;   // wait while not ready / for RX
	UCB0TXBUF = data;                  // write
	while ((UCB0IFG & UCRXIFG) == 0)
		;   // wait for RX buffer (full)
	return (UCB0RXBUF);
}

// Reading the contents of the CSD and CID registers in SPI mode is a simple
// read-block transaction.
char mmcReadRegister(const char cmd_register, const uint8 length,
		uint8 *pBuffer)
{
	char uc = 0;
	char rvalue = MMC_TIMEOUT_ERROR;

	if (mmcSetBlockLength(length) == MMC_SUCCESS)
	{
		CS_LOW ();
		// CRC not used: 0xff as last byte
		mmcSendCmd(cmd_register, 0x000000, 0xff);

		// wait for response
		// in the R1 format (0x00 is no errors)
		if (mmcGetResponse() == 0x00)
		{
			if (mmcGetXXResponse(0xfe) == 0xfe)
				for (uc = 0; uc < length; uc++)
					pBuffer[uc] = spiSendByte(0xff); //mmc_buffer[uc] = spiSendByte(0xff);
				// get CRC bytes (not really needed by us, but required by MMC)
			spiSendByte(0xff);
			spiSendByte(0xff);
			rvalue = MMC_SUCCESS;
		}
		else
			rvalue = MMC_RESPONSE_ERROR;
		// CS = HIGH (off)
		CS_HIGH ();

		// Send 8 Clock pulses of delay.
		spiSendByte(0xff);
	}
	CS_HIGH ();
	return rvalue;
} // mmc_read_register

#include "math.h"
uint32 MMC_ReadCardSize(void)
{
	// Read contents of Card Specific Data (CSD)

	uint32 MMC_CardSize;
	unsigned short i,      // index
			j,      // index
			b,      // temporary variable
			response,   // MMC response to command
			mmc_C_SIZE;

	uint8 mmc_READ_BL_LEN,  // Read block length
			mmc_C_SIZE_MULT;

	CS_LOW ();

	spiSendByte(MMC_READ_CSD);   // CMD 9
	for (i = 4; i > 0; i--)      // Send four dummy bytes
		spiSendByte(0);
	spiSendByte(0xFF);   // Send CRC byte

	response = mmcGetResponse();

	// data transmission always starts with 0xFE
	b = spiSendByte(0xFF);

	if (!response)
	{
		while (b != 0xFE)
			b = spiSendByte(0xFF);
		// bits 127:87
		for (j = 5; j > 0; j--)       // Host must keep the clock running for at
			b = spiSendByte(0xff);

		// 4 bits of READ_BL_LEN
		// bits 84:80
		b = spiSendByte(0xff);  // lower 4 bits of CCC and
		mmc_READ_BL_LEN = b & 0x0F;

		b = spiSendByte(0xff);

		// bits 73:62  C_Size
		// xxCC CCCC CCCC CC
		mmc_C_SIZE = (b & 0x03) << 10;
		b = spiSendByte(0xff);
		mmc_C_SIZE += b << 2;
		b = spiSendByte(0xff);
		mmc_C_SIZE += b >> 6;

		// bits 55:53
		b = spiSendByte(0xff);

		// bits 49:47
		mmc_C_SIZE_MULT = (b & 0x03) << 1;
		b = spiSendByte(0xff);
		mmc_C_SIZE_MULT += b >> 7;

		// bits 41:37
		b = spiSendByte(0xff);

		b = spiSendByte(0xff);

		b = spiSendByte(0xff);

		b = spiSendByte(0xff);

		b = spiSendByte(0xff);

	}

	for (j = 4; j > 0; j--)          // Host must keep the clock running for at
		b = spiSendByte(0xff);  // least Ncr (max = 4 bytes) cycles after
								// the card response is received
	b = spiSendByte(0xff);
	CS_LOW ();

	MMC_CardSize = (mmc_C_SIZE + 1);
	// power function with base 2 is better with a loop
	// i = (pow(2,mmc_C_SIZE_MULT+2)+0.5);
	for (i = 2, j = mmc_C_SIZE_MULT + 2; j > 1; j--)
		i <<= 1;
	MMC_CardSize *= i;
	// power function with base 2 is better with a loop
	//i = (pow(2,mmc_READ_BL_LEN)+0.5);
	for (i = 2, j = mmc_READ_BL_LEN; j > 1; j--)
		i <<= 1;
	MMC_CardSize *= i;

	return (MMC_CardSize);

}

//char mmc_ping(void)
//{
//  if (!(P5IN & 0x20))
//    return (MMC_SUCCESS);
//  else
//    return (MMC_INIT_ERROR);
//}

#ifdef withDMA
#ifdef __IAR_SYSTEMS_ICC__
#if __VER__ < 200
interrupt[DACDMA_VECTOR] void DMA_isr(void)
#else
#pragma vector = DACDMA_VECTOR
__interrupt void DMA_isr(void)
#endif
#endif

#ifdef __CROSSWORKS__
void DMA_isr(void) __interrupt[DACDMA_VECTOR]
#endif

#ifdef __TI_COMPILER_VERSION__
__interrupt void DMA_isr(void);
DMA_ISR(DMA_isr)
__interrupt void DMA_isr(void)
#endif
{
	DMA0CTL &= ~(DMAIFG);
	LPM3_EXIT;
}
#endif

//---------------------------------------------------------------------
#endif /* _MMCLIB_C */
