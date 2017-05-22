



-m USBL_test.map 
-heap  0x10000
MEMORY

{





        MMR     : origin = 0000000h, length = 00000c0h 

        SPRAM   : origin = 00000c0h, length = 0000140h  
		DARAM0  : origin = 0001000h, length = 0001000h
        DARAM1  : origin = 0002000h, length = 0002000h   /*Rev1*/
		DARAM2  : origin = 0004000h, length = 0001000h   /*Rev2*/
		DARAM22 : origin = 0005000h, length = 0001000h
		DARAM3  : origin = 0006000h, length = 0001000h   /*twiddle*/
        DARAM4  : origin = 0007000h, length = 0001000h
  /*Rev*/
		DARAM5  : origin = 0008000h, length = 0002000h
  /*Rev_FFT*/
		DARAM6  : origin = 000a000h, length = 0002000h
  /*X1*/
		DARAM7  : origin = 000c000h, length = 0002000h
  /*Rev_xcorr*/
		DARAM8  : origin = 000e000h, length = 0001000h
  /*Rev_xcorrout*/
		DARAM9  : origin = 000f000h, length = 0002000h
  /*Rev_envelope*/


        SARAM1  : origin = 0011000h, length = 0001000h
  /*Rev_bp1*/
        SARAM2  : origin = 0012000h, length = 0001000h   /*Rev_bp2*/
		SARAM3  : origin = 0013000h, length = 0001000h   /*Rev_bp1_abs*/
		SARAM4  : origin = 0014000h, length = 0001000h   /*Rev_bp2_abs*/
		SARAM5  : origin = 0015000h, length = 0001000h   /*Rev_bp2_abs*/
		SARAM6  : origin = 0016000h, length = 0001000h
	    SARAM7  : origin = 0017000h, length = 0009000h
		SARAM8  : origin = 0020000h, length = 0020000h






        CE0     : origin = 0040000h, length = 03c0000h 

        CE1     : origin = 0400000h, length = 0400000h

        CE2     : origin = 0800000h, length = 0100000h//1MB

        CE_heap : origin = 0900000h, length = 0050000h//512KB

        CE_no1  : origin = 0950000h, length = 0010000h//64KB没用到

        CE_no2  : origin = 0960000h, length = 0001000h//4KB无用，避开页首

        CE_sig1 : origin = 0961000h, length = 001f000h//124KB

        CE_no3  : origin = 0980000h, length = 0001000h//4KB无用，避开页首

        CE_sig2 : origin = 0981000h, length = 001f000h//124KB

        CE_no4  : origin = 09a0000h, length = 0001000h//4KB无用，避开页首

        CE_sig3 : origin = 09a1000h, length = 001f000h//124KB

        CE3     : origin = 0a00000h, length = 0600000h//6MB



        //PDROM   : origin = 0ff8000h, length = 07f00h

        VECS    : origin = 0000e00h, length = 00200h  /* reset vector */

}	



 

SECTIONS

{

        .vectors     : {} > VECS        /* interrupt vector table */
         vectors     : {} > VECS
        .cinit       : {} > SARAM8

        .text        : {} > SARAM8

        .twiddle     : {} > SARAM8

        .csldata     : {} > DARAM0

        .stack       : {} > DARAM0

        .sysstack    : {} > DARAM0

        .sysmem      : {} > CE_heap

        .data        : {} > SARAM8

        .bss         : {} > SARAM8

        .const       : {} > SARAM8


		
        receive1     : {} > DARAM1
  
        receive2     : {} > DARAM2
      
        NFFT	     : {} > DARAM4
	      
        FFT          : {} > DARAM5
		.ref         : {} > DARAM6
		xcorr        : {} > DARAM7
		xcorrout     : {} > DARAM8
		envelope     : {} > DARAM9
		bp1          : {} > SARAM1
		bp2          : {} > SARAM2
		bp1abs       : {} > SARAM3
		bp2abs       : {} > SARAM4
		bp1envelope  : {} > SARAM5
		bp2envelope  : {} > SARAM6
		revsave      : {} > SARAM7
		filter       : {} > CE_sig1
        send         : {} > CE_sig1

        temp		 : {} > DARAM3//FFT
        Match_Data   : {} > CE3
		Up_Spread    : {} > CE3
		Sig_Tx_temp  : {} > DARAM22
		Sig_LFM      : {} > CE_sig2
		Sig_CW       : {} > CE_sig3

}