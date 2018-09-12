
/*******************************************************************************
* FILENAME
*   isr.h
*
* DESCRIPTION
*   c641x Header File
*
*******************************************************************************/
#ifndef _INTSUBPROG 
#define _INTSUBPROG 
#include "c6x.h"
#include "c6xdsp.h"
#include "c6xinterrupts.h"
#include "c6xdspinit.h"
#include "databuffer.h"


interrupt void c_int4(void);//syn_int,TL16C754c_int
interrupt void c_int5(void);
interrupt void c_int6(void);
interrupt void c_int7(void);
interrupt void c_int8(void);
interrupt void c_int9(void);
interrupt void c_int14(void);
interrupt void c_int15(void);
#endif

unsigned char Compass_Int_flag = 0;


