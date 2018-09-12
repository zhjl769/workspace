################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 --symdebug:skeletal --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/csl/include" --include_path="D:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/dsplib/include" --include_path="D:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/include" --include_path="E:/DATA/workspace/USBL_XINBIAO_100M/Release" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --define="CHIP_5509A" --define=c5509a --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


