################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/AT25F1024.obj: ../src/AT25F1024.C $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A -g --define=“CHIP_5509” --define=_CSL5509_LIB_ --define=c5509a --include_path="/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/c5500" --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/include" --diag_warning=225 --ptrdiff_size=32 --memory_model=large --preproc_with_compile --preproc_dependency="src/AT25F1024.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/flash_burn.obj: ../src/flash_burn.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A -g --define=“CHIP_5509” --define=_CSL5509_LIB_ --define=c5509a --include_path="/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/c5500" --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/include" --diag_warning=225 --ptrdiff_size=32 --memory_model=large --preproc_with_compile --preproc_dependency="src/flash_burn.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


