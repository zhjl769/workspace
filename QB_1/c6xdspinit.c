
/*******************************************************************************
* FILENAME
*   c6xdspinit.c
*
* DESCRIPTION
*   c641x Header File
*
*******************************************************************************/
#include "c6xdspinit.h"

void config_chip()		
{ 
	*(volatile unsigned int *)CCFG=0x00000000;  // L2ÓÃ×÷È«SRAM
	/*************************************************************************/
	/*                         CONFIG EMIFA REGISTER                         */
	/*************************************************************************/
	/*void º¯ÊıÖ÷ÒªÓÃÀ´ÅäÖÃSRAM,EMIFAµÄÊ±ÖÓÎª1/4=CPUÊ±ÖÓ=150MHz*/
	/*¶ÁÑ¡Í¨=Ğ´Ñ¡Í¨=9 ¶Á²Ù×÷=Ğ´²Ù×÷=1 ¶Á±£³Ö=Ğ´±£³Ö=1*/	 
	// EMIFAÊÇ64Î»Êı¾İ×ÜÏß½Ó¿Ú£»	   
	*(volatile unsigned int *)EMIFA_GCTL=0x5203C;           //
	*(volatile unsigned int *)EMIFA_CE0 =0x1251C911;        //SRAM0
	*(volatile unsigned int *)EMIFA_CE1 =0x1251C911;        //SRAM1
	*(volatile unsigned int *)EMIFA_CE2 =0xA53AD40F;        //CF
	*(volatile unsigned int *)EMIFA_CE3 =0x43E48F02;
	/*************************************************************************/
	/*                         CONFIG EMIFB REGISTER                         */
	/*************************************************************************/	 
    // EMIFBÊÇ16Î»Êı¾İ×ÜÏß½Ó¿Ú£» 
	*(volatile unsigned int *)EMIFB_GCTL=0x0001203C;   	 
	*(volatile unsigned int *)EMIFB_CE0 =0x43E48F02;   //16C754 
	*(volatile unsigned int *)EMIFB_CE1 =0x43E48F02;
	*(volatile unsigned int *)EMIFB_CE2 =0x43E48F02;  
	*(volatile unsigned int *)EMIFB_CE3 =0x43E48F02;                                         							
	
}



/*void config_gpio()// ²»Ìí¼ÓÍ¬²½Æ÷Ê±Ê¹ÓÃÅäÖÃ 0427£»
{
	*(volatile unsigned int *)GPGC =0x00000000;
	*(volatile unsigned int *)GPEN =0x0000fff9;        // µçÂ·Í¼ÉÏÊÇGPIO_1¿ÉÓÃ£¬ËùÒÔÊ¹ÄÜ£¬ÓÃ×÷²âÊÔ³ÌĞòÊ±¼ä£» 
	*(volatile unsigned int *)GPDIR=0x00009b09;        // ·½Ïò£»
	*(volatile unsigned int *)GPVAL=0x00000000;        // ¸³Öµ£¬³õÊ¼ÖµÀ­µÍ£»
	*(volatile unsigned int *)GPDH =0x00000000;
	*(volatile unsigned int *)GPDL =0x00000000;
	*(volatile unsigned int *)GPHM =0x00000000;
	*(volatile unsigned int *)GPLM =0x00000000;
	*(volatile unsigned int *)GPPOL=0x00000000;
}*/

void config_gpio()//¸ù¾İÍ¬²½Æ÷ÒªÇóĞŞ¸Ä£»0427£»
{
	*(volatile unsigned int *)GPGC =0x00000010;
	*(volatile unsigned int *)GPEN =0x0000FFFF; 
	*(volatile unsigned int *)GPDIR=0x00009B08;
//	*(volatile unsigned int *)GPVAL=0x00000000;
//	*(volatile unsigned int *)GPDH =0x00000004;
//	*(volatile unsigned int *)GPDL =0x00000004;
	*(volatile unsigned int *)GPHM =0x00000004;
	*(volatile unsigned int *)GPLM =0x00000000;
//	*(volatile unsigned int *)GPPOL=0x00000000;
}


void McBSP_initial(void)
{ /* configure DSP2 McBSP1 */
  *(unsigned volatile int *)McBSP0_SPCR=0x0;   //´®¿Ú¿ØÖÆ¼Ä´æÆ÷
   //bit 31~26	:0  reserved;
  //bit 25,24	:01 when emulator is halted,the mcbsp is halted;
  //bit 23		:1	frame synchron is generated;
  //bit 22		:1	sample generator is working;
  //bit 21,20	:00	xint is drived by copy;
  //bit 19		:0	synchronization error isn't detected;
  //bit 18		:0	read only;
  //bit 17		:0	read only;
  //bit 16		:0	transmit port is disable; !!!!!!!
  //bit 15		:0	digitial loop mode is disable;
  //bit 14,13	:01	right-justed and sign-extended in msb;
  //bit 12,11	:01	clock stop mode is disable; !!!!!!!
  //bit 10~8	:000reserved;
  //bit 7		:1	dx is enabled;
  //bit 6		:0	reserved;
  //bit 5,4		:00	don't care;
  //bit 3		:1	don't care;
  //bit 2		:0	read only;
  //bit 1		:0	read only;
  //bit	0		:0	receiver reset port is disable;  !!!!!!!!
  //0 01| 1 1 00 |0 0 0 0 |0 01 0|1 000 |1 0 00 |1 0 0 0  0x1a0288¡¢
  //*(unsigned volatile int *)McBSP0_SRGR=0x7BB70218;      //²ÉÑùÂÊ¿ØÖÆ¼Ä´æÆ÷(6M\2K)ÊÇÄÚ²¿Ê±ÖÓ¸øMcBSP
  //*(unsigned volatile int *)McBSP0_SRGR=0x51F30209;       // ²ÉÑùÂÊ¿ØÖÆ¼Ä´æÆ÷(1M\2K)CPLD¸øMcBSP£»
  *(unsigned volatile int *)McBSP0_SRGR=0x59C30201;        //²ÉÑùÂÊ¿ØÖÆ¼Ä´æÆ÷(5M\2K)CPLD¸øMcBSP

  //*(unsigned volatile int *)McBSP0_SRGR=0x712602FF;      //²ÉÑùÂÊ¿ØÖÆ¼Ä´æÆ÷(581K\2K)
  //*(unsigned volatile int *)McBSP0_SRGR=0x71F40296;      //²ÉÑùÂÊ¿ØÖÆ¼Ä´æÆ÷(1M\2K)
  // 0010|0000|0001|0000|0000|0000|0001|1110
  //bit 31 :0  
  //bit 30 :1 CLKSÏÂ½µÑØ²úÉúĞÅºÅCLKG¡¢FSG
  //bit 29 :1 ²ÉÑùÊ±ÖÓÓÉcpuÊ±ÖÓ¾ö¶¨|0£ºÓÉCLKS PIN ¾ö¶¨
  //bit 28 :1|0 transmit frame_sync signal(FSX) is generate by every DXR-to-XSR copy|1: driven by the sampal_rata generate frame_sync signal (FSG)
  //bit 27-16:3000(1101 0000 0100) 16+1 frame period value when frame_sync signal become active
  //bit 15-8 :2(0000 0010)     +1 frame wride value 
  //bit 7-0  :0x19(0001 1001)    ·ÖÆµ´óĞ¡
  
 *(unsigned volatile int *)McBSP0_RCR=0x00000740;  //16bit 7×Ö     Ö®Ç°ÖµÎª640£¬ÊÇ7Â·²É¼¯£¬ÏÖÔÚ¸ÄÎª740,8Â·²É¼¯
 //*(unsigned volatile int *)McBSP0_RCR=0x000003A0;  //32bit 4×Ö
  //*(unsigned volatile int *)McBSP0_RCR=0x00000740;  //16bit 8×Ö
// *(unsigned volatile int *)McBSP0_RCR=0x000000A0;  //32bit 1×Ö
  // 7-0     1010 0000 
  //bit 31   :0   signal_phase frame
  //bit 30-24:0   phase2'frame length
  //bit 23-21:000 8 bit transmit word length
  //bit 20-19:00  MSB ÎŞÑ¹Ëõ£¬´«Êä´ÓMSB(×î¸ßÓĞĞ§Î»)¿ªÊ¼
  //bit 18   :0   ´«ÊäÍ¬²½ĞÅºÅÖØÆô´«Êä|1£ººöÂÔ
  //bit 17-16:00  1 bit data delay
  //bit 15   :0 phase normal
  //bit 14-8 :000 0011 phase1'frame length Ã¿èå8¸öÊı¾İµ¥Ôª
  //bit 7-5  :101 16 bit transmit word length
  //bit 4    :0    ½ûÖ¹32-bit·´Ïà´«Êä--->£¨½ûÖ¹ÏÈ´«µÍÎ»£©
  //bit 3-0  £º±£Áô
  *(unsigned volatile int *)McBSP0_XCR=0x00000040;  //·¢ËÍ¿ØÖÆ¼Ä´æÆ÷
  // 7-0     0100 0000 
  //bit 31   :0   signal_phase frame
  //bit 30-24:0   phase2'frame length
  //bit 23-21:000 8 bit transmit word length
  //bit 20-19:00  MSB ÎŞÑ¹Ëõ£¬´«Êä´ÓMSB(×î¸ßÓĞĞ§Î»)¿ªÊ¼
  //bit 18   :0   ´«ÊäÍ¬²½ĞÅºÅÖØÆô´«Êä|1£ººöÂÔ
  //bit 17-16:01  1 bit data delay
  //bit 15   :0 phase normal
  //bit 14-8 :000 0000 phase1'frame length Ã¿èåÒ»¸öÊı¾İµ¥Ôª
  //bit 7-5  :010 16 bit transmit word length
  //bit 4    :0    ½ûÖ¹32-bit·´Ïà´«Êä--->£¨½ûÖ¹ÏÈ´«µÍÎ»£©
  //bit 3-0  £º±£Áô
   *(unsigned volatile int *)McBSP0_PCR=0x00000501;   //Òı½Å¿ØÖÆ¼Ä´æÆ÷
   //*(unsigned volatile int *)McBSP0_PCR=0x00000a00;   //Òı½Å¿ØÖÆ¼Ä´æÆ÷
  //11-0 1010 0000 1011 
  //bit 31-14   :±£Áô
  //bit 13      :0 DX FSX CLKX Òı½ÅÉèÖÃÎªÍ¨ĞÅ¶Ë¿Ú¶ø·ÇGPIO
  //bit 12      :0 DR FSR CLKR CLKSÒı½ÅÉèÖÃÎªÍ¨ĞÅ¶Ë¿Ú¶ø·ÇGPIO
  //BIT 11      :0 ´«Êä Ö¡Í¬²½ĞÅºÅÓÉSPGR¼Ä´æÆ÷µÄFSGMÎ»¾ö¶¨|0 ÓÉÍâ²¿ĞÅºÅÇı¶¯
  //bit 10      :1 £º²ÉÑùÂÊ·¢ÉúÆ÷ÄÚ²¿²úÉúÖ¡Í¬²½ĞÅºÅ£¬FSRÊÇÊä³ö¹Ü½Å
  //bit  9      :0  ´«Êä   CLKX is an output pin drived by the internal simpal-rata generanter
  //             0         CLKX is an input pin drived by the external clock
  //bit  8      :0  ½ÓÊÕ   CLKR is an input pin drived by the external clock
  //             =1         CLKR is an output pin drived by the internal simpal-rata generanter
  //bit  7      :±£Áô
  //bit  6      :0   µ±ÉèÎªGPIOÊ±£¬ CLKS  pin ¶ÔÓ¦ÎªÂß¼­µÍ
  //bit  5      :0   µ±ÉèÎªGPIOÊ±£¬ DX pin ¶ÔÓ¦ÎªÂß¼­µÍ
  //bit  4      :0   µ±ÉèÎªGPIOÊ±£¬ DR pin ¶ÔÓ¦ÎªÂß¼­µÍ
  //bit  3      :0   |0 ·¢ËÍ ´«ÊäÍ¬²½Âö³å¸ßÓĞĞ§|1 µÍÓĞĞ§
  //bit  2      :0   ½ÓÊÕ´«ÊäÍ¬²½Âö³å¸ßÓĞĞ§
  //bit  1      :0   ÔÚCLKXµÄÉÏÉıÑØ¶ù·¢ËÍÊı¾İ²ÉÑù
  //bit  0      :1   ÔÚCLKRµÄÉÏÉıÑØ¶ù½ÓÊÕÊı¾İ²ÉÑù
   *(unsigned volatile int *)McBSP0_SPCR=0x02c00081;
   //*(unsigned volatile int *)McBSP0_SPCR=0x03c30080;
  //0000 00010 1100 0000 0000 0000 1000 0001
    
  //bit 31~26  :0000 00  reserved;
  //bit  25    :1 free run mode is enable ,ÔÚemulate halt Ê±£¬Ê±ÖÓ²»Í£Ö¹
  //bit  24    :1 soft mode is disable,clk stop when emulation halt |1 enable
  //bit  23    :1 /frst Ö¡Í¬²½·¢ÉúÆ÷¸´Î»
  //bit  22    :1 /grst ²ÉÑùÂÊ·¢ÉúÆ÷¸´Î»
  //bit  21-20 :00 xintm ·¢ËÍÖĞ¶ÏÄ£Ê½
  //bit  19    :0  xsynceerr ·¢ËÍÍ¬²½´íÎó
  //bit  18    :0  xempty--|0 xsp is empty |1 not empty  
  //bit  17    :0  xrdy  |1--transfer is ready for dxr |0 not  
  //bit  16    :0  xrst  |´®¿Ú·¢ËÍÆ÷½ûÖ¹
  //bit  15    :0  dlb   |0 Êı¾İÁ´Â·Ä£Ê½ disable|1 
  //bit  14-13 :00 rjust ½ÓÊÕÊı¾İµÄ·ûºÅÀ©Õ¹¼°µ÷ÕûÄ£Ê½ |01--ÓÒÊÊÓ¦¼°·ûºÅÀ©Õ¹ÔÚDRRµÄ¸ßÎ»|
  //                     |00 ÓÒÊÊÓ¦¼°¸ßÎ»Ìî³ä0 in DRR
  //bit  12-11 :00 clkstp Ê±ÖÓÍ£Ö¹Ä£Ê½|11-- Ê±ÖÓÔÚÉÏÉıÑØdelay|00 clck stop mode is disable 
  //                               *****´Ë´¦Ğë×¢Òâ£¬Ò»¿ªÊ¼Ñ¡ÓÃµÄÊÇ11 ¡£µ¼ÖÂÖ¡Í¬²½ĞÅºÅ²»¶Ô£º
  //                               *****£¨¸ßµçÆ½ÓĞĞ§Ê±£¬¸ßµçÆ½³ÖĞøÕû¸öÊı¾İ·¢ËÍÊ±ÖÓÖÜÆÚ£©
  //bit  10-8  :000  reserved;
  //bit  7     :1  DXENA |1-- DX enable is on 
  //bit  6     :0   reserved;
  //bit  5-4   :00 rintm ½ÓÊÕÖĞ¶ÏÄ£Ê½ 
  //bit  3     :0  rsynceerr ½ÓÊÕÍ¬²½´íÎó |0--Í¬²½´íÎó²»¼ì²â 
  //bit  2     :0  rfull |0--EBR Ã»
  //bit  1     :0  rrdy  |1--receiver is not ready
  //bit  0     :1  rrst   ´®ĞĞ receiver is enable
  //------------------mcbsp1---------------------------------
  /*
    *(unsigned volatile int *)McBSP1_SPCR=0x0;   //´®¿Ú¿ØÖÆ¼Ä´æÆ÷
    *(unsigned volatile int *)McBSP1_SRGR=0x3012001e;      //²ÉÑùÂÊ¿ØÖÆ¼Ä´æÆ÷
    *(unsigned volatile int *)McBSP1_XCR=0x00000040;  //·¢ËÍ¿ØÖÆ¼Ä´æÆ÷
    *(unsigned volatile int *)McBSP1_PCR=0x00000a00;   //Ä´æÆ÷
    *(unsigned volatile int *)McBSP1_SPCR=0x03c30080;  */
}
 
void config_754a()
{
	unsigned char LSR;
	*(unsigned char *)(EMB_CE0+LJ_A4+IER_7)=0x00;//ÅäÖÃÂŞ¾­²¨
	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+LJ_A4+DLL_7)=0x30;//²¨ÌØÂÊ9600ÊÇ0x60(19200ÊÇ0x30)£¬¾§ÕñÆµÂÊ14.7456MHz
	*(unsigned char *)(EMB_CE0+LJ_A4+DLH_7)=0x00;

	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0xBF;//Xon and Xoff are accessible only when LCR is set BF
	*(unsigned char *)(EMB_CE0+LJ_A4+EFR_7)=0x10;//Enables the enhanced function
	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0x00;
	//*(unsigned char *)(EMB_CE0+LJ_A4+FCR_7)=0x06;
	*(unsigned char *)(EMB_CE0+LJ_A4+FCR_7)=0xC9;     //4ÔÂ18ÈÕ£¬¿ªÆôFIFO
	//*(unsigned char *)(EMB_CE0+LJ_A4+FCR_7)=0xC3;
	//ÔÚTLRÉèÎªÁãÊ± ´¥·¢¼¶ÓÉFCR¸ø¶¨ RX´¥·¢¼¶Îª60 TX´¥·¢¼¶Îª8


	*(unsigned char *)(EMB_CE0+LJ_A4+MCR_7)=0x48;//Enable access to the TCR and TLR registers
	*(unsigned char *)(EMB_CE0+LJ_A4+TCR_7)=0x0F;//RCV  trigger level 60,RCV  trigger level to resume 0 
	//*(unsigned char *)(EMB_CE0+LJ_A4+TLR_7)=0xFF;
	*(unsigned char *)(EMB_CE0+LJ_A4+TLR_7)=0xFF;     //4ÔÂ18ÈÕ£¬Éè¶¨´¥·¢¼¶
	*(unsigned char *)(EMB_CE0+LJ_A4+LCR_7)=0x03;//length to be transmitted is 8 bit
	*(unsigned char *)(EMB_CE0+LJ_A4+IER_7)=0x01;//Ö»Ê¹ÄÜ½ÓÊÕÖĞ¶Ï,Enable the RHR interrupt
 	LSR=*(unsigned  char *)(EMB_CE0+LJ_A4+IIR_7);
 	if(LSR!=0x00)
	    LSR=*(unsigned  char *)(EMB_CE0+LJ_A4+RHR_7);
}
void config_754b()
{
	unsigned char LSR;
	*(unsigned char *)(EMB_CE0+SYNC_A5+IER_7)=0x00;//ÅäÖÃÍ¬²½Æ÷²¨ÌØÂÊ£¨Êµ¼ÊÉÏÊÇÃüÁîÅ¶£¬Ç×£©
	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+SYNC_A5+DLL_7)=0x18;//²¨ÌØÂÊ38400£¬¾§ÕñÆµÂÊ14.7456MHz
	*(unsigned char *)(EMB_CE0+SYNC_A5+DLH_7)=0x00;

	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0xBF;
	*(unsigned char *)(EMB_CE0+SYNC_A5+EFR_7)=0x10;
	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0x00;
//	*(unsigned char *)(EMB_CE0+SYNC_A5+FCR_7)=0x06;     //4ÔÂ18ÈÕ
    *(unsigned char *)(EMB_CE0+SYNC_A5+FCR_7)=0xC9;     //4ÔÂ18ÈÕ£¬¿ªÆôFIFO
	*(unsigned char *)(EMB_CE0+SYNC_A5+MCR_7)=0x48;
	*(unsigned char *)(EMB_CE0+SYNC_A5+TCR_7)=0x0F;
//	*(unsigned char *)(EMB_CE0+SYNC_A5+TLR_7)=0xFF;     //4ÔÂ18ÈÕ
	*(unsigned char *)(EMB_CE0+SYNC_A5+TLR_7)=0xFF;     //4ÔÂ18ÈÕ£¬Éè¶¨´¥·¢¼¶
	*(unsigned char *)(EMB_CE0+SYNC_A5+LCR_7)=0x03;
	*(unsigned char *)(EMB_CE0+SYNC_A5+IER_7)=0x01;
	LSR=*(unsigned  char *)(EMB_CE0+SYNC_A5+IIR_7);
 	if(LSR!=0x00)
	    LSR=*(unsigned  char *)(EMB_CE0+SYNC_A5+RHR_7);
 
}
void config_754c()
{
	*(unsigned char *)(EMB_CE0+DP_A6+IER_7)=0x00;//ÅäÖÃÉî¶È´«¸ĞÆ÷²¨ÌØÂÊ
	*(unsigned char *)(EMB_CE0+DP_A6+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+DP_A6+DLL_7)=0x60;//²¨ÌØÂÊ9600£¬¾§ÕñÆµÂÊ14.7456MHz
	*(unsigned char *)(EMB_CE0+DP_A6+DLH_7)=0x00;
	*(unsigned char *)(EMB_CE0+DP_A6+LCR_7)=0xBF;
	*(unsigned char *)(EMB_CE0+DP_A6+EFR_7)=0x10;
	*(unsigned char *)(EMB_CE0+DP_A6+LCR_7)=0x00;
	*(unsigned char *)(EMB_CE0+DP_A6+FCR_7)=0xC9;
	*(unsigned char *)(EMB_CE0+DP_A6+MCR_7)=0x48;
	*(unsigned char *)(EMB_CE0+DP_A6+TCR_7)=0x0F;
	*(unsigned char *)(EMB_CE0+DP_A6+TLR_7)=0xFF;
	*(unsigned char *)(EMB_CE0+DP_A6+LCR_7)=0x03;
	*(unsigned char *)(EMB_CE0+DP_A6+IER_7)=0x01;
}
void config_754d()
{
	*(unsigned char *)(EMB_CE0+ORD_A7+IER_7)=0x00;//ÅäÖÃÃüÁî´®¿Ú²¨ÌØÂÊ(Êµ¼ÊÉÏÊÇÍ¬²½Æ÷Å¶£¬Ç×)
	*(unsigned char *)(EMB_CE0+ORD_A7+LCR_7)=0x80;
	*(unsigned char *)(EMB_CE0+ORD_A7+DLL_7)=0x60;//²¨ÌØÂÊ9600£¬¾§ÕñÆµÂÊ14.7456MHz
	*(unsigned char *)(EMB_CE0+ORD_A7+DLH_7)=0x00;
	*(unsigned char *)(EMB_CE0+ORD_A7+LCR_7)=0xBF;
	*(unsigned char *)(EMB_CE0+ORD_A7+EFR_7)=0x10;
	*(unsigned char *)(EMB_CE0+ORD_A7+LCR_7)=0x00;
	*(unsigned char *)(EMB_CE0+ORD_A7+FCR_7)=0xC9;
	*(unsigned char *)(EMB_CE0+ORD_A7+MCR_7)=0x48;
	*(unsigned char *)(EMB_CE0+ORD_A7+TCR_7)=0x0F;
	*(unsigned char *)(EMB_CE0+ORD_A7+TLR_7)=0xFF;
	*(unsigned char *)(EMB_CE0+ORD_A7+LCR_7)=0x03;
	*(unsigned char *)(EMB_CE0+ORD_A7+IER_7)=0x01;
}

void timer0_8ms_timeout(void)
{
 *(unsigned volatile int *)TIMER0_CTRL=0x00000415;  //¹Ø±Õ¶¨Ê±Æ÷
 
 *(unsigned volatile int *)TIMER0_COUNT=0x0;
 /*Ã¿¼ÆÒ»´ÎÊı£¬CNT¼Ä´æÆ÷¼ÓÒ»£¬µ±ÖµµÈÓÚPRDÖĞµÄÖµÊ±
  ÔÚÏÂÒ»¸öCPUÊ±ÖÓ¸´Î»µ½0     */
 *(unsigned volatile int *)TIMER0_PRD=0x13880;//8ms

}

void timer0_5ms_timeout(void)
{
 *(unsigned volatile int *)TIMER0_CTRL=0x00000215;  //¹Ø±Õ¶¨Ê±Æ÷
 
 *(unsigned volatile int *)TIMER0_COUNT=0x0;
 /*Ã¿¼ÆÒ»´ÎÊı£¬CNT¼Ä´æÆ÷¼ÓÒ»£¬µ±ÖµµÈÓÚPRDÖĞµÄÖµÊ±
  ÔÚÏÂÒ»¸öCPUÊ±ÖÓ¸´Î»µ½0     */
 *(unsigned volatile int *)TIMER0_PRD=0xC350;//5ms

}

void timer1_config_2s(void)
 {
 /* configure timer0 ¶¨Ê±Æ÷0ÅäÖÃ£¬Ê¹ÓÃÍâ²¿10MÊ±ÖÓÔ´*/
 *(unsigned volatile int *)TIMER1_CTRL=0x00000215;
 //¶¨Ê±Æ÷¿ØÖÆ¼Ä´æÆ÷
 //11~0 :0010 0001 0101 
 //bit 31~16    :0 reserved;
 //bit 15       :0 ÔÚÒ»´Î·ÂÕæÔİÍ£ÆÚ¼ä¶¨Ê±Æ÷¼ÌĞø¼ÆÊı  ?
 //bit 14~12    :0 reserved;
 //bit 11       :0 TSTAT ¶¨Ê±Æ÷×´Ì¬Î»
 //bit 10       :0 ²»·´ÏàTINPÇı¶¯¶¨Ê±Æ÷
 //bit 9        :1 ÄÚ²¿Ê±ÖÓÔ´Çı¶¯£»=1/8 CPUÊ±ÖÓ
 //bit 8        :0 TSTATÎªÂö³åÄ£Ê½£ºÔÚ¶¨Ê±Æ÷´ïµ½¶¨Ê±Æ÷ÖÜÆÚºóÒ»¸öCPUÊ±ÖÓÊ±£¬TSTATÓĞĞ§
 //bit 7        :0 HLDÊ¹ÄÜÊ±£¬¼ÆÊıÆ÷±»½ûÖ¹²¢±£³ÖÔÚµ±Ç°×´Ì¬
 //bit 6        :0 ¸´Î»»òÆô¶¯¶¨Ê±Æ÷¶Ô¶¨Ê±Æ÷Ã»ÓĞÓ°Ïì;|1:Èç¹ûHDL=1£¬¼ÆÊıÆ÷±»ÖÃ0£¬ÔÚÏÂÒ»¸öÊ±ÖÓ¿ªÊ¼¼ÆÊı
 //bit 5        :0 reserved;
 //bit 4        :1 Âö³å¿í¶ÈÎ»£¬¶¨Ê±Æ÷¼ÆÊıÖµµÈÓÚ¶¨Ê±Æ÷ÖÜÆÚÖµµÄÁ½¸ö¶¨Ê±Æ÷ÊäÈëÖÜÆÚºó£¬TSTATĞÅºÅ½øÈëÎŞĞ§
 //bit 3        :0 Êı¾İÊäÈëÎ»(µÈÓÚTINPÒı½ÅµÄÖµ)£¬TINPÒı½ÅÖµÎªÂß¼­µÍ
 //bit 2        :1 Êı¾İÊä³öÎ»:±»INVOUT·´ÏàºóµÄTSTATÇı¶¯TOUT|0: DATOUTÇı¶¯TOUT
 //bit 1        :0 TOUT·´ÏàÎ»(ÔÚFUNC=1Ê±ÓĞĞ§)£¬²»·´ÏàµÄTSTATÇı¶¯TOUT |1: ·´ÏàµÄTSTATÇı¶¯TOUT
 //bit 0        :1 TOUTÒı½ÅµÄ¹¦ÄÜ£¬TOUTÎªÒ»¸ö¶¨Ê±Æ÷Êä³öÒı½Å |0 ÎªÍ¨ÓÃÄ¿µÄµÄÊä³öÒı½Å
 
 *(unsigned volatile int *)TIMER1_COUNT=0x0;
 /*Ã¿¼ÆÒ»´ÎÊı£¬CNT¼Ä´æÆ÷¼ÓÒ»£¬µ±ÖµµÈÓÚPRDÖĞµÄÖµÊ±
  ÔÚÏÂÒ»¸öCPUÊ±ÖÓ¸´Î»µ½0     */
 //*(unsigned volatile int *)TIMER0_PRD=0x05F5E100;//10s
  *(unsigned volatile int *)TIMER1_PRD=0x8F0D180;//2s	
}


void timer1_config_timeout(void)
 {
 *(unsigned volatile int *)TIMER1_CTRL=0x000004D5;
 
 *(unsigned volatile int *)TIMER1_COUNT=0x0;
 /*Ã¿¼ÆÒ»´ÎÊı£¬CNT¼Ä´æÆ÷¼ÓÒ»£¬µ±ÖµµÈÓÚPRDÖĞµÄÖµÊ±
  ÔÚÏÂÒ»¸öCPUÊ±ÖÓ¸´Î»µ½0     */
 *(unsigned volatile int *)TIMER1_PRD=0x989680;//1s

}

/*McBSP0½ÓÊÕÊÂ¼ş£¬²ÉÓÃÆ¹ÅÒ»º´æµÄĞÎÊ½*/
void Config_EnableEdma13(short *pointer0,short *pointer1)//
{	
	*(volatile unsigned int *)(EVENTD_PARAMS+OPT) = 0x283D0002; 
	*(volatile unsigned int *)(EVENTD_PARAMS+SRC) = McBSP0_DRR;
	*(volatile unsigned int *)(EVENTD_PARAMS+CNT) = 0x00000F00;//0x00000F00 15*512/2µ¥Ôª15ÉÈÇø;
	*(volatile unsigned int *)(EVENTD_PARAMS+DST) = (unsigned int)pointer0;
	*(volatile unsigned int *)(EVENTD_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+LNK) = 0x10000600;


	*(volatile unsigned int *)(EVENT13RL0_PARAMS+OPT) = 0x28302002; 
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+SRC) = McBSP0_DRR;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+CNT) = 0x00000F00;//7*512µ¥Ôª;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+DST) = (unsigned int)pointer1;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENT13RL0_PARAMS+LNK) = 0x10000618;


	*(volatile unsigned int *)(EVENT13RL1_PARAMS+OPT) = 0x283D0002; 
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+SRC) = McBSP0_DRR;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+CNT) = 0x00000F00;//7*512µ¥Ôª;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+DST) = (unsigned int)pointer0;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENT13RL1_PARAMS+LNK) = 0x10000600;
	

	*(volatile unsigned int *)CIERL |= 0x00002000; //Ê¹ÄÜedma13ÖĞ¶Ï	
	*(volatile unsigned int *)CIERL |= 0x00010000; //Ê¹ÄÜedma16ÖĞ¶Ï	
	*(volatile unsigned int *)EERL  |= 0x00002000; //Ê¹ÄÜedma13ÊÂ¼ş

			
}
void Config_DisableEdma13(void)
{
	*(volatile unsigned int *)EERL  &= 0xFFFFDFFF; //½ûÖ¹edma13ÊÂ¼ş
	*(volatile unsigned int *)EERL  &= 0xFFFEFFFF; //½ûÖ¹edma16ÊÂ¼ş
	*(volatile unsigned int *)CIERL &= 0xFFFFDFFF; //½ûÖ¹edma13ÖĞ¶Ï	
	*(volatile unsigned int *)CIERL &= 0xFFFEFFFF; //½ûÖ¹edma16ÖĞ¶Ï
	*(volatile unsigned int *)(EVENTD_PARAMS+OPT) = 0x00000000; 
	*(volatile unsigned int *)(EVENTD_PARAMS+SRC) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+CNT) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+DST) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+IDX) = 0x00000000;
	*(volatile unsigned int *)(EVENTD_PARAMS+LNK) = 0x00000000;

	*(volatile unsigned int *)ECRL  |= 0x00002000; //Çå³ıedma13ÊÂ¼ş
	*(volatile unsigned int *)CIPRL |= 0x00002000; //Çå³ıedma13¹ÒÆğÖĞ¶Ï	
	*(volatile unsigned int *)CIPRL |= 0x00010000; //½ûÖ¹edma16ÖĞ¶Ï
}



