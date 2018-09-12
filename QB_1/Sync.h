
/*******************************************************************************
* FILENAME
*   Sync.h
*
* DESCRIPTION
*   c641x Header File
*
*******************************************************************************/

#ifndef Sync_H 
#define Sync_H
#include "c6x.h"
#include "c6xdsp.h"

void self_check(void);
void order_return(void);
void compass_error(void);
void Sync_start(void);
void order_clear(void);
void Sync_order_clear(void);
void Sync_successed(void);
void check_Sync(void);
#endif



