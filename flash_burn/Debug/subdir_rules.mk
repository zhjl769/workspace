################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
flash_burn.obj: ../flash_burn.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large --symdebug:skeletal --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files/ccsv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="C:/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --verbose_diagnostics --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="flash_burn.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

sst25vf040b.obj: ../sst25vf040b.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large --symdebug:skeletal --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files/ccsv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="C:/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --verbose_diagnostics --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="sst25vf040b.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


