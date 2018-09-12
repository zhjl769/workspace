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
  ;������γ�����ԭ���������ģ�ת������
  ;hex�ļ��ĵ�һ��ǰ�ĸ��ֽ�Ϊc_int00����ڵ�ַ
  ;���������ĸ��ֽ�Ϊ���εĳ��ȣ��������ĸ��ֽ�Ϊ
  ;�öδ�����dsp�ڴ����ʼ��ַ��Ҳ���ǿ�����Ŀ����ʼ��ַ
  ;���εĴ������һ�����ֻ����c_int00����ڵ�ַ��
  ;����һ�ε�c_int00��ڵ�ַȥ���󣬰����е����ݶ����뵽flah��
  ;���Ƴ����ճ��Ⱥ�Ŀ����ʼ��ַ��Ϣ����������ÿ����һ�κ�
  ;�����Ŷ�����һ�ε���Ϣ������������ֱ��ȫ���ζ������
  ;������ת��c_int00,��ʼ���г���
  ;---------------------------------------
        MVKL     0x64000400  ,A3
        MVKH     0x64000400  ,A3
COPY_SECTION_TOP:
        LDW      *A3++,B0          ;���γ���
        LDW      *A3++,A4          ;Ŀ���ַ
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
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        