################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
I2C.obj: ../I2C.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -g --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/csl/include" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/dsplib/55x_src" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --define=CHIP_5509A --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="I2C.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

USBL.obj: ../USBL.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -g --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/csl/include" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/dsplib/55x_src" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --define=CHIP_5509A --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="USBL.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -g --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/csl/include" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/dsplib/55x_src" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --define=CHIP_5509A --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

regs.obj: ../regs.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -g --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/csl/include" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/dsplib/55x_src" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --define=CHIP_5509A --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="regs.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

vector.obj: ../vector.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -g --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/csl/include" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/dsplib/55x_src" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --define=CHIP_5509A --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="vector.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

vectors_i2c1.obj: ../vectors_i2c1.s55 $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -g --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/csl/include" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/dsplib/55x_src" --include_path="E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --define=CHIP_5509A --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="vectors_i2c1.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


