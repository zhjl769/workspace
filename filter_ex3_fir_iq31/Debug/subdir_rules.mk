################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
filter_ex3_fir_iq31.obj: ../filter_ex3_fir_iq31.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx --abi=eabi --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/msp430_4.2.1/include" --include_path="E:/DATA/workspace/filter_ex3_fir_iq31/dsplib/include" --advice:power=all -g --define=__MSP430FR5969__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="filter_ex3_fir_iq31.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

