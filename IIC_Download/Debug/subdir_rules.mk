################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
AT24CXX.obj: ../AT24CXX.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -g --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/c5500/dsplib/55x_src" --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/c5500/csl/include" --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/include" --define=c5509a --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="AT24CXX.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -g --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/c5500/dsplib/55x_src" --include_path="D:/Program Files/ccsv5/ccsv5/ccs_base/c5500/csl/include" --include_path="D:/Program Files/ccsv5/ccsv5/tools/compiler/c5500_4.4.1/include" --define=c5509a --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


