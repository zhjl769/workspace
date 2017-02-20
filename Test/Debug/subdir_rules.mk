################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ADC12.obj: ../ADC12.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/bin/cl430" -vmspx --abi=eabi --code_model=large -O3 --opt_for_speed=0 --include_path="D:/Program Files (x86)/CCSv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/MSPWare_3_30_00_18/iqmathlib/include" --include_path="E:/DATA/workspace/Test/dsplib/include" --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/include" --advice:power="all" -g --define=__MSP430F6638__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="ADC12.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

DMA.obj: ../DMA.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/bin/cl430" -vmspx --abi=eabi --code_model=large -O3 --opt_for_speed=0 --include_path="D:/Program Files (x86)/CCSv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/MSPWare_3_30_00_18/iqmathlib/include" --include_path="E:/DATA/workspace/Test/dsplib/include" --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/include" --advice:power="all" -g --define=__MSP430F6638__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="DMA.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Inlt.obj: ../Inlt.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/bin/cl430" -vmspx --abi=eabi --code_model=large -O3 --opt_for_speed=0 --include_path="D:/Program Files (x86)/CCSv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/MSPWare_3_30_00_18/iqmathlib/include" --include_path="E:/DATA/workspace/Test/dsplib/include" --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/include" --advice:power="all" -g --define=__MSP430F6638__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Inlt.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Interrupt.obj: ../Interrupt.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/bin/cl430" -vmspx --abi=eabi --code_model=large -O3 --opt_for_speed=0 --include_path="D:/Program Files (x86)/CCSv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/MSPWare_3_30_00_18/iqmathlib/include" --include_path="E:/DATA/workspace/Test/dsplib/include" --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/include" --advice:power="all" -g --define=__MSP430F6638__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Interrupt.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Other.obj: ../Other.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/bin/cl430" -vmspx --abi=eabi --code_model=large -O3 --opt_for_speed=0 --include_path="D:/Program Files (x86)/CCSv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/MSPWare_3_30_00_18/iqmathlib/include" --include_path="E:/DATA/workspace/Test/dsplib/include" --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/include" --advice:power="all" -g --define=__MSP430F6638__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Other.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Xcorr.obj: ../Xcorr.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/bin/cl430" -vmspx --abi=eabi --code_model=large -O3 --opt_for_speed=0 --include_path="D:/Program Files (x86)/CCSv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/MSPWare_3_30_00_18/iqmathlib/include" --include_path="E:/DATA/workspace/Test/dsplib/include" --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/include" --advice:power="all" -g --define=__MSP430F6638__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Xcorr.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/bin/cl430" -vmspx --abi=eabi --code_model=large -O3 --opt_for_speed=0 --include_path="D:/Program Files (x86)/CCSv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/MSPWare_3_30_00_18/iqmathlib/include" --include_path="E:/DATA/workspace/Test/dsplib/include" --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/include" --advice:power="all" -g --define=__MSP430F6638__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


