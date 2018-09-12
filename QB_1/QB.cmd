
/*C6xdsk.cmd Generic Linker command file*/


-m QB.map  /* ����all_fun.map�ļ�*/
-stack 0x2000    /* ����stack ��СΪ0x400*/
-heap 0x1000


MEMORY
{
	VECS	:	org =         0h, len =      0x400
	PMEM	:   org = 0x00000400, len = 0x00011000 /*internal memory*/ 
	DMEM	:	org = 0x00011400, len = 0x000EB000 /*internal memory*/ 
	
}

SECTIONS
{ 
	vectors :>	VECS
	.boot_load :>VECS
	.text	:>	PMEM
	.switch :>	DMEM
	.cinit	:>	DMEM
	.const	:>	DMEM
	.data	:>	DMEM
	.cio 	:> 	DMEM
	.far 	:>	DMEM
	.stack	:>	DMEM
	.bss 	:>	DMEM
	.sysmem :>	DMEM 



}

