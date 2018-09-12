################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
dr_dc_motor.obj: ../dr_dc_motor.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/bin/cl430" -vmsp --abi=eabi --include_path="D:/Program Files (x86)/CCSv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/include" --advice:power=all -g --define=__MSP430F1611__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="dr_dc_motor.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dr_lcdseg.obj: ../dr_lcdseg.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/bin/cl430" -vmsp --abi=eabi --include_path="D:/Program Files (x86)/CCSv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/include" --advice:power=all -g --define=__MSP430F1611__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="dr_lcdseg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/bin/cl430" -vmsp --abi=eabi --include_path="D:/Program Files (x86)/CCSv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/msp430_4.2.7/include" --advice:power=all -g --define=__MSP430F1611__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


