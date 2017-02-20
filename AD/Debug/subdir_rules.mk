################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
AD.obj: ../AD.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -g --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files/ccsv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="C:/CCSTUD~1.3/C5500/csl/include" --define="_DEBUG" --define=c5509a --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="AD.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


