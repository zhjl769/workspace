/*********************************************************************
 * Filename:      AT25F1024.c
 *                
 * Description:   支持 AT25FX系列24位地址spi flash 烧写
              
 ********************************************************************/
                                                                     
#include <stdio.h>
#include <stdlib.h>
#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_gpio.h>
/* Instruction Codes */
#define WREN                  0x06
#define WRDI                  0x04
#define RDSR                  0x05
#define WRSR                  0x01
#define READ                  0x03
#define PROGRAM               0x02
#define SECTOR_ERASE          0x52
#define CHIP_ERASE            0x62
#define RDID                  0x15



/************************************************************************/
/* PROCEDURE: set_cs													*/
/*																		*/
/* This procedure set CE = High or CE = Low.							*/
/*																		*/
/* Input:																*/
/*		sta:  1 - H                                                     */
/*            0 - L                                                     */
/*																		*/
/* Output:																*/
/*		CE																*/
/*																		*/
/************************************************************************/
static void set_cs(unsigned char sta) 
{
    GPIO_FSET(IODATA, IO4D, sta ? 1 : 0);
}

/************************************************************************/
/* PROCEDURE: set_clk													*/
/*																		*/
/* This procedure set CLK = High or CLK = Low.							*/
/*																		*/
/* Input:																*/
/*		sta:  1 - H                                                     */
/*            0 - L                                                     */
/*																		*/
/* Output:																*/
/*		CLK																*/
/*																		*/
/************************************************************************/
static void set_clk(unsigned char sta)
{
    unsigned char i;
    
    MCBSP_FSET(PCR0, CLKXP, sta ? 1 : 0);
    for (i=0; i<5; i++);
}

/************************************************************************/
/* PROCEDURE: set_si													*/
/*																		*/
/* This procedure set SI = High or SI = Low.							*/
/*																		*/
/* Input:																*/
/*		sta:  1 - H                                                     */
/*            0 - L                                                     */
/*																		*/
/* Output:																*/
/*		SI																*/
/*																		*/
/************************************************************************/
static void set_si(unsigned char sta)
{
    MCBSP_FSET(PCR0, DXSTAT, sta ? 1 : 0);
}

/************************************************************************/
/* PROCEDURE: get_so													*/
/*																		*/
/* This procedure get SO status.										*/
/*																		*/
/* Input:																*/
/*		SO																*/
/* Output:																*/
/*																		*/
/* return:                                                              */
/*     SO status                                                        */
/*																		*/
/************************************************************************/
static unsigned char get_so(void)
{
    return(MCBSP_FGET(PCR0, DRSTAT));
}

/************************************************************************/
/* PROCEDURE: send_byte													*/
/*																		*/
/* This procedure outputs a byte shifting out 1-bit per clock rising	*/
/* edge on the the SI pin(LSB 1st).										*/
/*																		*/
/* Input:																*/
/*		dat																*/
/*																		*/
/* Output:																*/
/*		SI																*/
/************************************************************************/
static void send_byte(unsigned char dat)
{
	
	unsigned char i = 0;
	for (i = 0; i < 8; i++)
	{
		
		if ((dat & 0x80) == 0x80)	/* check if MSB is high */
			set_si(1);
		else
			set_si(0);					/* if not, set to low */
		set_clk(1);					/* toggle clock high */
		dat = (dat << 1);			/* shift 1 place for next bit */
		set_clk(0);					/* toggle clock low */
	}
}

/************************************************************************/
/* PROCEDURE: get_byte													*/
/*																		*/
/* This procedure inputs a byte shifting in 1-bit per clock falling		*/
/* edge on the SO pin(LSB 1st).											*/
/*																		*/
/* Input:																*/
/*		SO																*/
/*																		*/
/* Output:																*/
/*		None															*/
/************************************************************************/
static unsigned char get_byte(void)
{
	unsigned char i = 0, in = 0, temp = 0;
	for (i = 0; i < 8; i++)
	{
		in = (in << 1);		/* shift 1 place to the left or shift in 0 */
		temp = get_so();			/* save input */
		set_clk(1);			/* toggle clock high */
		if (temp == 1)			/* check to see if bit is high */
			in |= 0x01;		/* if high, make bit high */

		set_clk(0);			/* toggle clock low */

	}
	return in;
}

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
void flash_init(void)
{
    set_cs(1);
	set_clk(0);	/* set clock to low initial state */
}

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
unsigned char flash_read_status_register(void)
{
	unsigned char byte = 0;
    
	set_cs(0);				/* enable device */
	send_byte(0x05);		/* send RDSR command */
	byte = get_byte();		/* receive byte */
	set_cs(1);				/* disable device */
	return byte;
}

/************************************************************************/
/* PROCEDURE: flash_write_register_enable														*/
/*																		*/
/* This procedure Enables Write status register.  						*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
static void write_status_register_enable(void)
{
    set_cs(0);				/* enable device */
	send_byte(0x50);		/* enable writing to the status register */
	set_cs(1);				/* disable device */
}

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
void flash_write_status_register(unsigned char byte)
{
    write_status_register_enable();
    
	set_cs(0);				/* enable device */
	send_byte(0x01);		/* select write to status register */
	send_byte(byte);		/* data that will change the status of BPx 
							   or BPL (only bits 2,3,4,5,7 can be written) */
	set_cs(1);				/* disable the device */
}

/************************************************************************/
/* PROCEDURE: wait_busy													*/
/*																		*/
/* This procedure waits until device is no longer busy (can be used by	*/
/* Byte-Program, Sector-erase, block-erase, Chip-erase).				*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
static void wait_busy()
{
	while ((flash_read_status_register() & 0x03) == 0x03)	/* waste time until not busy */
		flash_read_status_register();
}


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
void flash_write_enable(void)
{
    write_status_register_enable();
    
	set_cs(0);				/* enable device */
	send_byte(0x06);		/* send WREN command */
	set_cs(1);				/* disable device */
}

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
void flash_write_disable(void)
{
    write_status_register_enable();
    
	set_cs(0);				/* enable device */
	send_byte(0x04);		/* send WRDI command */
	set_cs(1);				/* disable device */
}

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
unsigned long flash_jedec_id_read(void) 
{
	unsigned long temp;
	
	temp = 0;

	set_cs(0);				 /* enable device */
	send_byte(0x1D);		 /* send JEDEC ID command (9Fh) */
    temp = (temp | get_byte()) << 8;	 /* receive byte */
	temp = (temp | get_byte()) << 8;	
	temp = (temp | get_byte()); 	 /* temp value = 0xBF258D */
	set_cs(1);				/* disable device */

	return temp;
}

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
unsigned char flash_read_byte(unsigned long addr) 
{
	unsigned char byte = 0;	

	set_cs(0);				/* enable device */
	send_byte(0x03); 		/* read command */
	send_byte(((addr & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	send_byte(((addr & 0xFFFF) >> 8));
	send_byte(addr & 0xFF);
	byte = get_byte();
	set_cs(1);				/* disable device */
    
	return byte;			/* return one byte read */
}

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
void flash_read_nbyte(unsigned long addr, unsigned char *pd, unsigned long num)
{
	unsigned long i = 0;
    
	set_cs(0);					/* enable device */
	send_byte(0x03); 			/* read command */
	send_byte(((addr & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	send_byte(((addr & 0xFFFF) >> 8));
	send_byte(addr & 0xFF);
	for (i = 0; i < num; i++)		/* read until no_bytes is reached */
	{
		*pd = get_byte();	/* receive byte and store at address 80H - FFH */
        pd++;
	}
	set_cs(1);					/* disable device */

}

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
unsigned char flash_fast_read_byte(unsigned long addr) 
{
	unsigned char byte = 0;	

	set_cs(0);				/* enable device */
	send_byte(0x0B); 		/* read command */
	send_byte(((addr & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	send_byte(((addr & 0xFFFF) >> 8));
	send_byte(addr & 0xFF);
	send_byte(0xFF);		/*dummy byte*/
	byte = get_byte();
	set_cs(1);				/* disable device */
    
	return byte;			/* return one byte read */
}

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
void flash_fast_read_nbyte(unsigned long addr, unsigned short *pd, unsigned long num)
{
	unsigned long i = 0;
    unsigned char byte1, byte2;

    wait_busy();
    flash_write_enable();       /* Enable write */
    wait_busy();   

	set_cs(0);					/* enable device */
	send_byte(READ); 			/* read command */
	send_byte(((addr & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	send_byte(((addr & 0xFFFF) >> 8));
	send_byte(addr & 0xFF);
	for (i = 0; i < num; i++)		/* read until no_bytes is reached */
	{
	byte1= get_byte();	/* receive byte and store at address 80H - FFH */
	byte2= get_byte();	/* receive byte and store at address 80H - FFH */        
       	*pd = byte1<<8;
		*pd|= byte2;

        pd++;
	}
	set_cs(1);					/* disable device */
}

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
void flash_write_byte(unsigned long addr, unsigned char byte)
{
    wait_busy();
    flash_write_enable();       /* Enable write */
    
	set_cs(0);					/* enable device */
	send_byte(0x02); 			/* send Byte Program command */
	send_byte(((addr & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	send_byte(((addr & 0xFFFF) >> 8));
	send_byte(addr & 0xFF);
	send_byte(byte);			/* send byte to be programmed */
	set_cs(1);					/* disable device */
}

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
void flash_write_nword(unsigned long addr, unsigned short *pword, unsigned long num)
{
    unsigned char byte1, byte2;

//========================
    wait_busy();
    flash_write_enable();       /* Enable write */
    wait_busy();   

	set_cs(0);					/* enable device */
	send_byte(PROGRAM);			/* send AAI command */
	send_byte(((addr & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	send_byte(((addr & 0xFFFF) >> 8));
	send_byte(addr & 0xFF);
    while (num>0)
    {        
        byte1 = *pword >> 8;
        byte2= *pword & 0xff;
        send_byte(byte1);			/* send 1st byte to be programmed */	
        send_byte(byte2);			/* send 2nd byte to be programmed */
        *pword++;
        num--;
    }
    set_cs(1);
}

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
void flash_chip_erase(void)
{
    wait_busy();
    flash_write_enable();
    
	set_cs(0);					/* enable device */
	send_byte(CHIP_ERASE);			/* send Chip erase command (60h or C7h) */
	set_cs(1);					/* disable device */
    wait_busy();
}

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
void flash_sector_erase(unsigned long addr)
{
    wait_busy();
    flash_write_enable();
    
	set_cs(0);					/* enable device */
	send_byte(0x20);			/* send Sector erase command */
	send_byte(((addr & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	send_byte(((addr & 0xFFFF) >> 8));
	send_byte(addr & 0xFF);
	set_cs(1);					/* disable device */
}

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
void flash_block_erase_32k(unsigned long addr)
{
    wait_busy();
    flash_write_enable();
    
	set_cs(0);					/* enable device */
	send_byte(0x52);			/* send 32 KByte block erase command */
	send_byte(((addr & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	send_byte(((addr & 0xFFFF) >> 8));
	send_byte(addr & 0xFF);
	set_cs(1);					/* disable device */
}	

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
void flash_block_erase_64k(unsigned long addr)
{
    wait_busy();
    flash_write_enable();
    
	set_cs(0);					/* enable device */
	send_byte(0xD8);			/* send 64KByte block erase command */
	send_byte(((addr & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	send_byte(((addr & 0xFFFF) >> 8));
	send_byte(addr & 0xFF);
	set_cs(1);					/* disable device */
}

