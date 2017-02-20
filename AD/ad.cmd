
-e main
-o ad.out
-m ad.map
MEMORY
{
	PROG:	   origin=0x200,	     len=0x2400
	DATA:	   origin=0x2600,	     len=0x200
	DARAM1:    origin=0x2800,        len=0x200
	DARAM2:    origin=0x4000,        len=0xc000
/*	DARAM3:    origin=0x20000,       len=0x16800 */
	STACK:	   origin=0x2a00,	     len=0x0200  
	VECT:	   origin=0x02c00,	     len=0x100
	INITIAL:   origin=0x2d00,       len=0x1000
}
SECTIONS
{
	.text:    {} >      PROG
	.data:    {} >      DATA
 	variable: {} >      DARAM1
 	data1:	  {} >      DARAM2
 	data2:	  {} >      DARAM2
 	
 /*	"data1_sect"{} >      DARAM2*/
	ints:     {} >      VECT
	stack:    {} >      STACK
	.cinit:   {} >      INITIAL
	.const:   {} >      INITIAL
	.cio:     {} >      INITIAL
}



