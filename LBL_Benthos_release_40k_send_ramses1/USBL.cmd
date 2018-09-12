

-m USBL_test.map 
MEMORY
{
    PAGE 0:

        MMR     : origin = 0000000h, length = 00000c0h 
        SPRAM   : origin = 00000c0h, length = 0000140h  
		DARAM0  : origin = 0001000h, length = 0001000h
        DARAM1  : origin = 0002000h, length = 0002000h   /*Rev1*/
		DARAM2  : origin = 0004000h, length = 0002000h   /*Rev2*/
		DARAM3  : origin = 0006000h, length = 0001000h   /*twiddle*/
        DARAM4  : origin = 0007000h, length = 0001000h  /*Rev*/
		DARAM5  : origin = 0008000h, length = 0002000h  /*Rev_FFT*/
		DARAM6  : origin = 000a000h, length = 0002000h  /*X1*/
		DARAM7  : origin = 000c000h, length = 0002000h  /*Rev_xcorr*/
		DARAM8  : origin = 000e000h, length = 0001000h  /*Rev_xcorrout*/
		DARAM9  : origin = 000f000h, length = 0002000h  /*Rev_envelope*/

        SARAM1  : origin = 0011000h, length = 0001000h  /*Rev_bp1*/
        SARAM2  : origin = 0012000h, length = 0001000h   /*Rev_bp2*/
		SARAM3  : origin = 0013000h, length = 0001000h   /*Rev_bp1_abs*/
		SARAM4  : origin = 0014000h, length = 0001000h   /*Rev_bp2_abs*/
		SARAM5  : origin = 0015000h, length = 0001000h   /*Rev_bp2_abs*/
		SARAM6  : origin = 0016000h, length = 0001000h   
	    SARAM7  : origin = 0017000h, length = 0009000h   
		SARAM8  : origin = 0020000h, length = 0008000h   
		SARAM9  : origin = 0028000h, length = 0008000h   
		SARAM10 : origin = 0030000h, length = 0010000h   



        CE0     : origin = 0040000h, length = 03c0000h 
        CE1     : origin = 0400000h, length = 0400000h
        CE2     : origin = 0800000h, length = 0400000h
        CE3     : origin = 0c00000h, length = 03f8000h

        PDROM   : origin = 0ff8000h, length = 07f00h
        VECS    : origin = 0000e00h, length = 00200h  /* reset vector */
}	

 
SECTIONS
{
        .vectors     : {} > VECS   PAGE 0         /* interrupt vector table */
        .cinit       : {} > DARAM0 PAGE 0
        .text        : {} > SARAM8 PAGE 0
        .twiddle     : {} > DARAM3 PAGE 0

        .stack       : {} > DARAM0 PAGE 0
        .sysstack    : {} > DARAM0 PAGE 0
        .sysmem      : {} > DARAM0 PAGE 0
        .data        : {} > SARAM8 PAGE 0
        .bss         : {} > SARAM8 PAGE 0
        .const       : {} > SARAM8 PAGE 0
		
        receive1     : {} > DARAM1 PAGE 0  
        receive2     : {} > DARAM2 PAGE 0      
        NFFT	     : {} > DARAM4 PAGE 0	      
        FFT          : {} > DARAM5 PAGE 0
		xcorr        : {} > DARAM7 PAGE 0
		xcorrout     : {} > DARAM8 PAGE 0
		envelope     : {} > DARAM9 PAGE 0
		bp1          : {} > SARAM1 PAGE 0
		bp2          : {} > SARAM2 PAGE 0
		bp1abs       : {} > SARAM3 PAGE 0
		bp2abs       : {} > SARAM4 PAGE 0
		bp1envelope  : {} > SARAM5 PAGE 0
		bp2envelope  : {} > SARAM6 PAGE 0
		.ref         : {} > DARAM6 PAGE 0
		revsave      : {} > SARAM7 PAGE 0
		filter       : {} > SARAM9 PAGE 0
        send         : {} > SARAM10 PAGE 0

}