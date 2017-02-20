/* ======================================================================= */
/* lnk.cmd -- Linker command file                                          */
/*                                                                         */
/* Rev 0.0.2                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

-c
-heap  0x1000
-stack 0x1000
-m test_omapl138.map

/* MODULE MEM */
MEMORY {
   entry_point : origin = 0x11800000,  len = 0x100
   L2SRAM      : origin = 0x11800100,  len = 0x3FF00
   Shared_RAM  : origin = 0x80000000,  len = 0x20000
   DDR2_RAM    : origin = 0xC6000000,  len = 0x80000
   DDR2_part1  : origin = 0xC6080000,  len = 0x400 
   DDR2_part2  : origin = 0xC6080400,  len = 0x400   
   DDR2_part3  : origin = 0xC6080800,  len = 0x258000
}

SECTIONS
{	

	buffer_pingpang	   :> L2SRAM
	buffer_action	   :> L2SRAM
	buffer_env		   :> L2SRAM
	buffer_coef_fft	   :> L2SRAM
	buffer_env_ddr2    :> L2SRAM
	buffer_send_sig    :> Shared_RAM


    .text:_c_int00     :> entry_point
    .vecs	           :> L2SRAM
	.cinit 			   :> L2SRAM
    .text              :> L2SRAM
    .stack             :> L2SRAM
    .bss               :> L2SRAM
    .const             :> L2SRAM
    .far               :> L2SRAM
    .switch            :> L2SRAM

    ddr2_dat   	       :> DDR2_part1
	ddr2_cmd		   :> DDR2_part2	
	ddr2_buffer		   :> DDR2_part3

}

