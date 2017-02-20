#include <stdio.h>
#include <stdint.h>
#include <csl.h>
#include <csl_emif.h>

/**************************************
#include "register.h"                      
#include "System_initial.h"
***************************************/

Uint16 x;
Uint32 y;
unsigned int i = 0,n= 0;
int databuffer1[1000] ={0};
int databuffer2[1000] ={0};
int *souraddr,*deminaddr;

void CLK_init ()          //时钟初始化
{
  ioport unsigned int *clkmd;
  ioport unsigned int *sysr;
  clkmd = (unsigned int * )0x1c00;
  sysr = (unsigned int *)0x07fd;
  *clkmd = 0x2712;           ///时钟频率200MHz
  *sysr = 0x0005;             //CLKOUT 为CPU CLOCLK /5*2  即 20 MHz
   }


void EMIF_init()
 {
 ioport unsigned int *EGCR =( unsigned int *)0x0800;           //EMIF全局控制器
 ioport unsigned int *EMI_RST =( unsigned int *)0x0801;        //EMIF复位控制器
 ioport unsigned int *EMI_BE =( unsigned int *)0x0802;         //EMIF总线转台错误状态控制器
 ioport unsigned int *CE01 =( unsigned int *)0x0803;           //CE0空间控制寄存器1
 ioport unsigned int *CE02 =( unsigned int *)0x0804;           //CE0空间控制寄存器2
 ioport unsigned int *CE03 =( unsigned int *)0x0805;           //CE0空间控制寄存器3
 ioport unsigned int *CE11 =( unsigned int *)0x0806;           //CE1空间控制寄存器1
 ioport unsigned int *CE12 =( unsigned int *)0x0807;           //CE1空间控制寄存器2
 ioport unsigned int *CE13 =( unsigned int *)0x0808;           //CE1空间控制寄存器3
 ioport unsigned int *CE21 =( unsigned int *)0x0809;           //CE2空间控制寄存器1
 ioport unsigned int *CE22 =( unsigned int *)0x080A;           //CE2空间控制寄存器2
 ioport unsigned int *CE23 =( unsigned int *)0x080B;           //CE2空间控制寄存器2
 ioport unsigned int *CE31 =( unsigned int *)0x080C;           //CE3空间控制寄存器1
 ioport unsigned int *CE32 =( unsigned int *)0x080D;           //CE3空间控制寄存器2
 ioport unsigned int *CE33 =( unsigned int *)0x080E;           //CE3空间控制寄存器3
 ioport unsigned int *SDC1 =( unsigned int *)0x080F;           //SDRAM控制寄存器1
 ioport unsigned int *SDPER =( unsigned int *)0x0810;          //SDRAM周期寄存器
 ioport unsigned int *SDCNT =( unsigned int *)0x0811;          //SDRAM计数器寄存器
 ioport unsigned int *INIT =( unsigned int *)0x0812;           //SDRAM初值寄存器
 ioport unsigned int *SDC2 =( unsigned int *)0x0813;           //SDRAM控制寄存器2


 *EGCR = *EGCR & 0x0ffdf;//SDRAM初始化   EGCR全局控制寄存器
 *EGCR = 0x020f;//CLKMEM=CPU时钟的一半；不响应外部总线征用请求
 *EMI_RST = 0xffff;
 *EMI_BE = 0x0000;
 *CE01 = 0x0000;
 *CE02 = 0x0000;
 *CE03 = 0x0000;
 *CE11 = 0x3000;
 *CE12 = 0x0000;
 *CE13 = 0x0000;
 *CE21 = 0x3000;
 *CE22 = 0x0000;
 *CE23 = 0x0000;
 *CE31 = 0x0000;
 *CE32 = 0x0000;
 *CE33 = 0x0000;
 *SDC1 = 0x2911;
 *SDC2 = 0x0131;
 *SDPER = 0x0410;
 *EGCR = *EGCR|0x0020;//CLKMEM引脚输出时钟使能
 *INIT = 0x0000;//初始化SDRAM
 *INIT = 0x07ff;


 }

  /****************************************************************************************
 EMIF_Config emiffig = {
  0x221, 	//EGCR  : the MEMFREQ = 00,the clock for the memory is equal to cpu frequence
  			//		  the WPE = 0 ,forbiden the writing posting when we debug the EMIF
  			//        the MEMCEN = 1,the memory clock is reflected on the CLKMEM pin
  			//        the NOHOLD = 1,HOLD requests are not recognized by the EMIF 
  0xFFFF,	//EMI_RST: any write to this register resets the EMIF state machine
  0x3FFF,	//CE0_1:  CE0 space control register 1
  			//        MTYPE = 011,Synchronous DRAM(SDRAM),16-bit data bus width
  0xFFFF,   //CE0_2:  CE0 space control register 2
  0x00FF,   //CE0_3:  CE0 space control register 3
  			//        TIMEOUT = 0xFF;
  0x7FFF,	//CE1_1:  CE0 space control register 1
  0xFFFF,	//CE1_2:  CE0 space control register 2
  0x00FF,	//CE1_3:  CE0 space control register 3
  
  0x7FFF,	//CE2_1:  CE0 space control register 1
  0xFFFF,	//CE2_2:  CE0 space control register 2
  0x00FF,	//CE2_3:  CE0 space control register 3
  
  0x7FFF,	//CE3_1:  CE0 space control register 1
  0xFFFF,	//CE3_2:  CE0 space control register 2
  0x00FF,	//CE3_3:  CE0 space control register 3
  
  0x2911,   //SDC1:   SDRAM control register 1
  			//		  TRC = 8 
  			//        SDSIZE = 0;SDWID = 0
  			//        RFEN = 1
  			//        TRCD = 2
  			//        TRP  = 2
  0x0410,	//SDPER : SDRAM period register
  			//		  7ns *4096
  0x07FF,    //SDINIT: SDRAM initialization register
  			//        any write to this register to init the all CE spaces,
  			//        do it after hardware reset or power up the C55x device
  0x0131	//SDC2:	  SDRAM control register 2
  			//        SDACC = 0;
  			//        TMRD = 01;
  			//        TRAS = 0101;
  			//        TACTV2ACTV = 0001;								
  };



  *********************************************************************************/

 void main()
{

    unsigned int a,b;
    CSL_init(); 
   //CSL库函数初始化
    CHIP_RSET(XBSR,0x0a01); 
  //EMIF为全EMIF接口
    CLK_init ();
  //时钟初始化
    EMIF_init();
  //EMIF初始化
  while (n<1000)
{
   databuffer1[n] = n;
   n++;

     }

    souraddr = (int *)0x800000;
    deminaddr =(int *)0x801000;

    while(souraddr<deminaddr)
    {
	  
     //   a= *souraddr；
		b =  databuffer1[i];
	  	*souraddr= b;
   	souraddr= souraddr+0x0001;	    
   		i++	;
    }
//	i = i++;
    souraddr =  (int *)0x40000;
    i = 0;
    while(souraddr<deminaddr)
    {
    	databuffer2[i++] = *souraddr++;
    }
    while(1);

}
