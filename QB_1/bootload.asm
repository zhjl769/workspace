;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;                                                                   ;;;
;;;                       bootload.asm                                ;;;
;;;                                                                   ;;;
;;;    The program is the second bootloader to load the prg for 64x   ;;;
;;;                                                                   ;;;
;;;             2007.9.25    write by gdg     HEU                     ;;;
;;;                                                                   ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        .title    "TMS320C6416 BOOTLOADER SECTION"        ;  show the title for the asm file        
        .sect     ".boot_load"        
        .ref      _c_int00                                ;  reference the c entry point for c_int00()
        .global   _boot
        
_boot:
        ;CONFIG THE EMIFB_GBLCTL
        
        MVKL      0x01A80000, A4
      ||MVKL      0x00000080, B4
        MVKH      0x01A80000, A4
      ||MVKH      0x00000080, B4
        STW       B4        ,*A4
        
        ;CONFIG THE EMIFB_CE1CTL
        
        MVKL      0x01A80004, A4
      ||MVKL      0x43E48F02, B4
        MVKH      0x01A80004, A4
      ||MVKH      0x43E48F02, B4
        STW       B4        ,*A4
  ;---------------------------------------
  ; copy section 
  ;下面这段程序工作原理是这样的，转换出的
  ;hex文件的第一段前四个字节为c_int00的入口地址
  ;接下来的四个字节为本段的长度，紧跟的四个字节为
  ;该段代码在dsp内存的起始地址，也就是拷贝的目标起始地址
  ;随后段的代码与第一段相比只少了c_int00的入口地址，
  ;将第一段的c_int00入口地址去掉后，把所有的数据都存入到flah中
  ;搬移程序按照长度和目标起始地址信息来拷贝程序，每拷完一段后
  ;紧跟着读入下一段的信息，继续拷贝，直到全部段都拷完后
  ;程序跳转到c_int00,开始运行程序。
  ;---------------------------------------
        MVKL     0x64000400  ,A3
        MVKH     0x64000400  ,A3
COPY_SECTION_TOP:
        LDW      *A3++,B0          ;本段长度
        LDW      *A3++,A4          ;目标地址
        NOP      3
  [!B0] B        COPY_DONE        
        NOP       5
COPY_LOOP:
        LDB      *A3++,B5
        SUB      B0,1,B0
   [B0] B        COPY_LOOP
   [!B0]B        COPY_SECTION_TOP
        ZERO     A1
   [!B0]AND      3,A3,A1
        STB      B5,*A4++
   [!B0]AND      -4,A3,A5
   [ A1]ADD      4,A5,A3
;--------------------------------------
;JUMP TO ENTRY PIONT
;---------------------------------------
COPY_DONE:
         B       _c_int00
        NOP      5
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        