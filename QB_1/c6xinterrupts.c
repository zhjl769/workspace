
/*********************************************************
 *  Interrupt Initialization Functions
 *
 *  (CSR and IER are CPU registers defined in c6x.h)
 *
 *********************************************************/
#include "c6xinterrupts.h"

/* Enable Interrupts Globally (set GIE bit in CSR = 1)   */
void enableGlobalINT(void)
{
  CSR |= 0x1;        //maskable interrupt globally enabled
}

/* Enable NMI (non-maskable interrupt); must be enabled 
 * or no other interrupts can be recognized by 'C6000 CPU */
void enableNMI(void)
{
  IER = _set(IER, 1, 1);
}

/* Enable a specific interrupt; 
 *  (INTnumber = {4,5,6,...,15})                      */
void enableSpecificINT(int INTnumber)
{
  IER = _set(IER, INTnumber, INTnumber); 
}

void disableSpecificINT(int INTnumber)
{
  IER = _clr(IER, INTnumber, INTnumber); 
}

/*********************************************************
 * C6000 devices have hardware configurable interrupts.
 * To use the McBSP interrupts you must configure them since
 *    they are selected by default. 
 * You must set the appropriate interrupt select bits in the
 *    IML and IMH memory-mapped int select registers.
 * IML and IMH addresses are defined in c6211dsk.h.
 * INTnumber对应CPU内部终端号，INTsource由用户自己定义
 *********************************************************/
void config_Interrupt_Selector(int INTnumber, int INTsource)
{
   /* INTnumber = {4,5,6,...,15}
      INTsource = see #define list above
  */

  union 
  {
  struct
   { unsigned int INTSEL4  : 5;
     unsigned int INTSEL5  : 5;
  	 unsigned int INTSEL6  : 5;
	 unsigned int rsvbit15 : 1;
	 unsigned int INTSEL7  : 5;
     unsigned int INTSEL8  : 5;
	 unsigned int INTSEL9  : 5;
	 unsigned int rsvbit31 : 1;
   }exp; 
   unsigned int reg;
   }IMLvalue;					/* = {0,0,0,0,0,0,0,0}; */
  
  union
  {
   unsigned int reg;
   struct
   { unsigned int INTSEL10  : 5;  //这里定义了一个寄存器各个位的意义，数字表示位长
     unsigned int INTSEL11  : 5;
	 unsigned int INTSEL12  : 5;
	 unsigned int rsvbit15  : 1;
	 unsigned int INTSEL13  : 5;
     unsigned int INTSEL14  : 5;
	 unsigned int INTSEL15  : 5;
	 unsigned int rsvbit31  : 1;
   }exp; 
   }IMHvalue;
     
  IMLvalue.reg = *(unsigned volatile int *)MUXL;
  IMHvalue.reg = *(unsigned volatile int *)MUXH;

  switch (INTnumber)
  {
    case 4 :
      IMLvalue.exp.INTSEL4 = INTsource;
      break;
    
    case 5 :
      IMLvalue.exp.INTSEL5 = INTsource;
      break;

    case 6 :
      IMLvalue.exp.INTSEL6 = INTsource;
      break;

    case 7 :
      IMLvalue.exp.INTSEL7 = INTsource;
      break;

    case 8 :
      IMLvalue.exp.INTSEL8 = INTsource;
      break;

    case 9 :
      IMLvalue.exp.INTSEL9 = INTsource;
      break;

    case 10 :
      IMHvalue.exp.INTSEL10 = INTsource;
      break;

    case 11 :
      IMHvalue.exp.INTSEL11 = INTsource;
      break;

    case 12 :
      IMHvalue.exp.INTSEL12 = INTsource;
      break;

    case 13 :
      IMHvalue.exp.INTSEL13 = INTsource;
      break;

    case 14 :
      IMHvalue.exp.INTSEL14 = INTsource;
      break;

    case 15 :
      IMHvalue.exp.INTSEL15 = INTsource;
      break;

    default : break;
  }

  *(unsigned volatile int *)MUXL = IMLvalue.reg;
  *(unsigned volatile int *)MUXH = IMHvalue.reg;
                                           
  return;
}


