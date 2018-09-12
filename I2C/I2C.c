/*
 * Copyright (C) 2003 Texas Instruments Incorporated
 * All Rights Reserved
 */

#include <csl_i2c.h>
#include <csl_i2chal.h>
#include <stdio.h>

#define       IODOR          *(volatile ioport unsigned int *)0x03400
#define       IODATA         *(volatile ioport unsigned int *)0x03401

//---------Global data definition---------
int x,y,z;
Uint16 databyte1[10]={0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0};
Uint16 slaveaddressreceive[2] = {0,0};
Uint16 datareceive1[100]={0};


/* Create and initialize an I2C initialization structure */
I2C_Setup myInit = {
        0,              /* 7 bit address mode */
        0x0000,         /* own address - don't care if master */
        144,            /* clkout value (Mhz)  */
        400,            /* a number between 10 and 400*/
        0,              /* number of bits/byte to be received or transmitted (8)*/
        0,              /* DLB mode on*/
        1               /* FREE mode of operation on*/
};

void delay(long x)
{
	long i;
	for(i = 0;i < x;i ++)
	{
	}
}


//---------Function prototypes---------
void taskFunc(void);
void CSL_init(void);
void delay(long x);

//---------main routine---------
void main(void)
{
    /* Initialize CSL library - This is REQUIRED !!! */
    CSL_init();


    /* Call I2C example task/func */
    taskFunc();
}

void taskFunc(void) {
 
    int err = 1;
    /* Initialize I2C, using parameters in init structure */

    I2C_FSET(I2CSTR,BB,0x01);

    I2C_setup(&myInit);
    
    I2C_RSET(I2CCLKL,145);//value = (f/12/fi2c-10)/2
    I2C_RSET(I2CCLKH,145);

    /* Write a data byte to I2C */

    x=I2C_write(databyte1,10,1,0x0050,1,30000);
    delay(200000);

    /* Read data byte from I2C */
   // z=I2C_read(datareceive1,1,1,0x20,1,30000,0);
  // y=I2C_write(slaveaddressreceive,2,1,0x50,2,30000);
   delay(200000);

   z=I2C_read(datareceive1,100,1,0x50,3,30000,0);
   delay(200000);
    
    /* Make sure the received byte is same as one sent */
    if (databyte1[0]==datareceive1[0]) {
       err = 0;
    }
    printf ("%s\n",err?"TEST FAILED" : "TEST PASSED");

    while(1);

}
