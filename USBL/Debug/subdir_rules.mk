################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
cbrev.obj: ../cbrev.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O2 -g --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files (x86)/CCSv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="cbrev.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

cfft_noscale.obj: ../cfft_noscale.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O2 -g --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files (x86)/CCSv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="cfft_noscale.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

cifft_noscale.obj: ../cifft_noscale.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O2 -g --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files (x86)/CCSv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="cifft_noscale.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

fir2.obj: ../fir2.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O2 -g --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files (x86)/CCSv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="fir2.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

h_LPF1.obj: ../h_LPF1.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O2 -g --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files (x86)/CCSv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="h_LPF1.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

h_LPF2.obj: ../h_LPF2.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O2 -g --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files (x86)/CCSv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="h_LPF2.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O2 -g --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files (x86)/CCSv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

maxvec.obj: ../maxvec.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O2 -g --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files (x86)/CCSv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="maxvec.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

twid2048.obj: ../twid2048.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O2 -g --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files (x86)/CCSv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="twid2048.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

vectors.obj: ../vectors.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O2 -g --include_path="D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/include" --include_path="D:/Program Files (x86)/CCSv5/xdais_7_21_01_07/packages/ti/xdais" --include_path="D:/PROGRA~2/CCSTUD~1.3/C5500/csl/include" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="vectors.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


