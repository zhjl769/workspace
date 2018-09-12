/*********************************************************************
 * Filename:      sst25vf040b.h
 *                
 * Description:   
 *                
 * Copyright (C) 2009,  Vi.Wang
 * Author:        Vi.Wang <redfox_ww@126.com>
 * Created at:    Sat Apr 25 12:43:30 2009
 *                
 * Modified by:   Vi.Wang <redfox_ww@126.com>
 * Modified at:   Sat Apr 25 13:11:29 2009
 *                
 * Status:        Experimental, do not distribute.
 * Update count:  0
 *                
 ********************************************************************/

#ifndef SST25VF040B_H
#define SST25VF040B_H

/************************************************************************/
/* PROCEDURE: flash_init												*/
/*																		*/
/* This procedure initializes the SCK to low and the CE to high.        */
/* Must be called prior to up mode 0.									*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Output:																*/
/*		CE																*/
/*		SCK																*/
/************************************************************************/
extern void flash_init(void);

/************************************************************************/
/* PROCEDURE: flash_read_status_register										*/
/*																		*/
/* This procedure read the status register and returns the byte.		*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		byte															*/
/************************************************************************/
extern unsigned char flash_read_status_register(void);

/************************************************************************/
/* PROCEDURE: flash_write_status_register														*/
/*																		*/
/* This procedure writes a byte to the status register.					*/
/*																		*/
/* Input:																*/
/*		byte															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
extern void flash_write_status_register(unsigned char byte);

/************************************************************************/
/* PROCEDURE: flash_write_enable										*/
/*																		*/
/* This procedure enables the Write Enable Latch.  It can also be used 	*/
/* to Enables Write status register.									*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
extern void flash_write_enable(void);

/************************************************************************/
/* PROCEDURE: flash_write_disable										*/
/*																		*/
/* This procedure disables the Write Enable Latch.						*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
extern void flash_write_disable(void);

/************************************************************************/
/* PROCEDURE: flash_jedec_id_read										*/
/*																		*/
/* This procedure reads the manufacturer's ID (BFh), memory type (25h)  */
/* and device ID (8Dh).  It will use 9Fh as the JEDEC ID command.    	*/
/* Please see the product datasheet for details.						*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		IDs_read:ID1(Manufacture's ID = BFh, Memory Type (25h),			*/
/*		 and Device ID (8Dh)											*/
/*																		*/
/************************************************************************/
extern unsigned long flash_jedec_id_read(void);

/************************************************************************/
/* PROCEDURE:	flash_read_byte													*/
/*																		*/		
/* This procedure reads one address of the device.  It will return the 	*/
/* byte read in variable byte.											*/
/*																		*/
/*																		*/
/*																		*/
/* Input:																*/
/*		addr:	Destination Address 000000H - 07FFFFH					*/
/*      																*/
/*																		*/
/* Returns:																*/
/*		byte															*/
/*																		*/
/************************************************************************/
extern unsigned char flash_read_byte(unsigned long addr);

/************************************************************************/
/* PROCEDURE:	flash_read_nbyte										*/
/*																		*/		
/* This procedure reads multiple addresses of the device and stores		*/
/* data into 128 byte buffer. Maximum byte that can be read is 128 bytes*/
/*																		*/
/* Input:																*/
/*		addr:		Destination Address 000000H - 07FFFFH				*/
/*      pd:         Point to data                                       */
/*      num     	Number of bytes to read	(max = 128)					*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
extern void flash_read_nbyte(unsigned long addr, unsigned char *pd, unsigned long num);

/************************************************************************/
/* PROCEDURE:	flash_fast_read_byte									*/
/*																		*/		
/* This procedure reads one address of the device.  It will return the 	*/
/* byte read in variable byte.											*/
/*																		*/
/*																		*/
/*																		*/
/* Input:																*/
/*		addr:	Destination Address 000000H - 07FFFFH					*/
/*      																*/
/*																		*/
/* Returns:																*/
/*		byte															*/
/*																		*/
/************************************************************************/
extern unsigned char flash_fast_read_byte(unsigned long addr);

/************************************************************************/
/* PROCEDURE:	flash_fast_read_nbyte									*/
/*																		*/		
/* This procedure reads multiple addresses of the device and stores		*/
/* data into 128 byte buffer. Maximum byte that can be read is 128 bytes*/
/*																		*/
/* Input:																*/
/*		addr:		Destination Address 000000H - 07FFFFH               */
/*      *pd:        Piont to data                                       */
/*      num:    	Number of bytes to read	(max = 128)					*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
extern void flash_fast_read_nbyte(unsigned long addr,
                                  unsigned short *pd, unsigned long num);

/************************************************************************/
/* PROCEDURE:	flash_write_byte										*/
/*																		*/
/* This procedure programs one address of the device.					*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*				block protected.										*/
/*																		*/
/*																		*/
/*																		*/
/* Input:																*/
/*		addr:		Destination Address 000000H - 07FFFFH				*/
/*		byte:		byte to be programmed								*/
/*      																*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
extern void flash_write_byte(unsigned long addr, unsigned char byte);

/************************************************************************/
/* PROCEDURE:	flash_write_nword										*/
/*																		*/
/* This procedure programs consecutive addresses of 2 bytes of data into*/
/* the device:  1st data byte will be programmed into the initial		*/
/* address [A23-A1] and with A0 = 0.  The 2nd data byte will be be		*/
/* programmed into initial address [A23-A1] and with A0  = 1.  This		*/
/* is used to to start the AAI process.  It should be followed by 		*/
/* Auto_Add_IncB.														*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*				block protected.										*/
/*																		*/
/*																		*/
/* Note: Only RDSR command can be executed once in AAI mode with SO  	*/
/* 	 disable to output RY/BY# status.  Use WRDI to exit AAI mode 		*/
/*	 unless AAI is programming the last address or last address of		*/
/* 	 unprotected block, which automatically exits AAI mode.				*/
/*																		*/
/* Input:																*/
/*		addr:		Destination Address 000000H - 07FFFFH				*/
/*		pword:		Point of word to be programmed   					*/
/*      num:		Num of pword            							*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
extern void flash_write_nword(unsigned long addr,
                              unsigned short *pword, unsigned long num);


/************************************************************************/
/* PROCEDURE: flash_chip_erase											*/
/*																		*/
/* This procedure erases the entire Chip.								*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
extern void flash_chip_erase(void);

/************************************************************************/
/* PROCEDURE: flash_sector_erase										*/
/*																		*/
/* This procedure Sector erases the Chip.								*/
/*																		*/
/* Input:																*/
/*		addr:		Destination Address 000000H - 07FFFFH				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
extern void flash_sector_erase(unsigned long addr);

/************************************************************************/
/* PROCEDURE: flash_block_erase_32k										*/
/*																		*/
/* This procedure block erases 32 KByte of the Chip.					*/
/*																		*/
/* Input:																*/
/*		addr:		Destination Address 000000H - 07FFFFH				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
extern void flash_block_erase_32k(unsigned long addr);

/************************************************************************/
/* PROCEDURE: flash_block_erase_64K										*/
/*																		*/
/* This procedure block erases 64 KByte of the Chip.					*/
/*																		*/
/* Input:																*/
/*		addr:		Destination Address 000000H - 07FFFFH				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
extern void flash_block_erase_64k(unsigned long addr);

#endif
