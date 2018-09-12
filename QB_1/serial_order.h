
/*******************************************************************************
* FILENAME
*   serial_order.h
*
* DESCRIPTION
*   c641x Header File
*
*******************************************************************************/
#ifndef SERIAL_ORDER_H 
#define SERIAL_ORDER_H 
#include "c6x.h"
#include "c6xdsp.h"

void R232_To_LJ(unsigned char *pCommand,int bytesSize);
void R232_To_sync(unsigned char *pSync,int bytesSize);
void R232_To_order(unsigned char *pCommand,int bytesSize);
void CompassData_to_buffer(void);
void Command_to_buffer(void);
void RS485_to_KELLER(unsigned char *pRS485,int bytesSize);

#endif
