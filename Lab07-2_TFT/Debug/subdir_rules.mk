################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
dr_tft.obj: ../dr_tft.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/ti-cgt-msp430_16.9.8.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/msp430/include" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/ti-cgt-msp430_16.9.8.LTS/include" --define=__MSP430F6638__ -g --printf_support=minimal --diag_warning=225 --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dr_tft.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dr_tft2.obj: ../dr_tft2.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/ti-cgt-msp430_16.9.8.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/msp430/include" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/ti-cgt-msp430_16.9.8.LTS/include" --define=__MSP430F6638__ -g --printf_support=minimal --diag_warning=225 --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="dr_tft2.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/ti-cgt-msp430_16.9.8.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/msp430/include" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/ti-cgt-msp430_16.9.8.LTS/include" --define=__MSP430F6638__ -g --printf_support=minimal --diag_warning=225 --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


