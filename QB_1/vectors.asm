*****************************************
**         vector template             **
*****************************************
        ;.ref标号定义的标号，引用在本文件，定义在别的文件
        .ref _c_int00
		.ref _c_int1
		.ref _c_int2
	    .ref _c_int4
	    .ref _c_int5
		.ref _c_int6
		.ref _c_int7
	    .ref _c_int8
		.ref _c_int9
	    .ref _c_int13	
	    .ref _c_int14
		.ref _c_int15
	    .ref _boot	
		.sect   	"vectors"	;section for vectors
RESET_RST:
		mvkl	.S2	_boot,B0	;lower 16 bits --> B0
     	mvkh  	.S2 _boot,B0 	;upper 16 bits --> B0
   		;mvkl	.S2	_c_int00,B0	;lower 16 bits --> B0
  	    ;mvkh  	.S2 _c_int00,B0 	;upper 16 bits --> B0
    	B	.S2	B0 				;branch to entry address
		.loop 5     			;NOPs for remainder of FP
		NOP						;to fill 0x20 Bytes
		.endloop 
NMI_RST:	;branch to ISR 	    
 	    .loop 8
		NOP    			;fill with 8 NOPs
		.endloop 
RESV1:	;b   _c_int1
		.loop 8
		NOP 				
		.endloop
RESV2:	;b   _c_int2
		.loop 8
		NOP				
		.endloop
INT4:   b   _c_int4
	    .loop 7
		NOP  				
		.endloop
INT5:	b   _c_int5
	 	.loop 7
		NOP
		.endloop
INT6:   b   _c_int6
	 	.loop 7
		NOP
		.endloop
INT7:	b   _c_int7
		.loop 7
		NOP
		.endloop
INT8: 	b   _c_int8
         .loop 7
		NOP
		.endloop
INT9:	b   _c_int9
		.loop 7
		NOP
		.endloop
INT10:  	;branch to ISR 	
	    .loop 8
		NOP
		.endloop

INT11: 	;branch to ISR
		.loop 8	
		NOP
		.endloop

INT12: .loop 8
		NOP
		.endloop
INT13:	;b _c_int13	
		.loop 8
		NOP
		.endloop
INT14:  b _c_int14		
	    .loop 7
		NOP
		.endloop
INT15: 	b _c_int15
		.loop 7
		NOP
		.endloop
