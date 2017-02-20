
I2C.obj
-e .text
-o I2C.out
-m I2C.map

MEMORY
{
	PROG:	origin=0x200,	len=0x2c00
	DATA1:	origin=0x2e00,	len=0x200
	DATA2:	origin=0x3000,	len=0xfff
	VECT:	origin=0x4000,	len=0x6ff
	STACK:	origin=0x4700,	len=0x6ff
	DARAM:  origin=0x10000,	len=0x20000

   

}
SECTIONS
{
	.text:	    {}   >  PROG
	.data:	    {}   >  DATA1
	 buffer:    {}   >  DATA2
	 ints:      {}   >  VECT
	 stack:     {}   >  STACK
	 boot:     {}   >  DARAM
	
	
}


