/*********************************************************
 * Define the number of interrupts,which are used as
 * format parameter
 *********************************************************/
 
#ifndef _C6XDSPINTERRUPT
#define _C6XDSPINTERRUPT 

#include <c6x.h>
#include "c6xdsp.h"

#define DSPINT     0x0	/* 00000b  DSPINT Host port host to DSP interrupt */
#define TINT0      0x1	/* 00001b  TINT0 Timer 0 interrupt */
#define TINT1      0x2	/* 00010b  TINT1 Timer 1 interrupt */
#define SD_INT     0x3	/* 00011b  SD_INT EMIF SDRAM timer interrupt */
#define EXT_INT4   0x4	/* 00100b  EXT_INT4 External interrupt 4 */
#define EXT_INT5   0x5	/* 00101b  EXT_INT5 External interrupt 5 */
#define EXT_INT6   0x6	/* 00110b  EXT_INT6 External interrupt 6 */
#define EXT_INT7   0x7	/* 00111b  EXT_INT7 External interrupt 7 */
#define EDMA_INT   0x8	/* 01000b  EDMA_INT EDMA channel (0 through 63) interrupt */
#define EMU_DTDMA  0x9	/* 01001b   */
#define EMU_RTDXRX 0xA	/* 01010b   */
#define EMU_RTDXTX 0xB	/* 01011b   */
#define XINT0      0xC	/* 01100b  XINT0 McBSP 0 transmit interrupt */
#define RINT0      0xD	/* 01101b  RINT0 McBSP 0 receive interrupt */
#define XINT1      0xE	/* 01110b  XINT1 McBSP 1 transmit interrupt */
#define RINT1      0xF	/* 01111b  RINT1 McBSP 1 receive interrupt */

/*********************************************************
 *  Interrupt Initialization Functions
 *
 *  (CSR and IER are CPU registers defined in c6x.h)
 *
 *********************************************************/
/* Enable Interrupts Globally (set GIE bit in CSR = 1)   */
void enableGlobalINT(void);


/* Enable NMI (non-maskable interrupt); must be enabled 
 * or no other interrupts can be recognized by 'C6000 CPU */
void enableNMI(void);

/* Enable a specific interrupt; 
 *  (INTnumber = {4,5,6,...,15})                      */
void enableSpecificINT(int INTnumber);


void disableSpecificINT(int INTnumber);


/*********************************************************
 * C6000 devices have hardware configurable interrupts.
 * To use the McBSP interrupts you must configure them since
 *    they are selected by default. 
 * You must set the appropriate interrupt select bits in the
 *    IML and IMH memory-mapped int select registers.
 * IML and IMH addresses are defined in c6211dsk.h.
 *********************************************************/
void config_Interrupt_Selector(int INTnumber, int INTsource);

#endif
