################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Inlt.obj: ../Inlt.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmsp --abi=eabi -Ooff --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power=all -g --define=__MSP430F1611__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="Inlt.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

LCD1602.obj: ../LCD1602.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmsp --abi=eabi -Ooff --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power=all -g --define=__MSP430F1611__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="LCD1602.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Other.obj: ../Other.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmsp --abi=eabi -Ooff --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power=all -g --define=__MSP430F1611__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="Other.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmsp --abi=eabi -Ooff --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power=all -g --define=__MSP430F1611__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


