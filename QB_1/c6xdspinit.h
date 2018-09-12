
/*******************************************************************************
* FILENAME
*   c6xdspinit.h
*
* DESCRIPTION
*   c641x Header File
*
*******************************************************************************/

#ifndef _C6XDSPINIT
#define _C6XDSPINIT 
#include "c6xdsp.h"


void config_chip(void);
void config_gpio(void);
void timer0_5ms_timeout(void);
void timer0_8ms_timeout(void);
void timer1_config_2s(void);
void timer1_config_timeout(void);
void McBSP_initial(void);
void config_754a(void);
void config_754b(void);
void config_754c(void);
void config_754d(void);


void Config_EnableEdma13(short *pointer0,short *pointer1);
void Config_DisableEdma13(void);//compass

#endif
