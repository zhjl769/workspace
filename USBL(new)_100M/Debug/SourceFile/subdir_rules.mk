################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
cbrev.obj: ../SourceFile/cbrev.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="E:/DATA/workspace/USBL(new)_100M/SourceFile" --include_path="/packages/ti/xdais" --include_path="/include" --include_path="E:/DATA/workspace/USBL(new)_100M/Debug" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/include/c5500" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="SourceFile/cbrev.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

cfft_noscale.obj: ../SourceFile/cfft_noscale.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="E:/DATA/workspace/USBL(new)_100M/SourceFile" --include_path="/packages/ti/xdais" --include_path="/include" --include_path="E:/DATA/workspace/USBL(new)_100M/Debug" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/include/c5500" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="SourceFile/cfft_noscale.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

cfft_scale.obj: ../SourceFile/cfft_scale.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="E:/DATA/workspace/USBL(new)_100M/SourceFile" --include_path="/packages/ti/xdais" --include_path="/include" --include_path="E:/DATA/workspace/USBL(new)_100M/Debug" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/include/c5500" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="SourceFile/cfft_scale.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

cifft_noscale.obj: ../SourceFile/cifft_noscale.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="E:/DATA/workspace/USBL(new)_100M/SourceFile" --include_path="/packages/ti/xdais" --include_path="/include" --include_path="E:/DATA/workspace/USBL(new)_100M/Debug" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/include/c5500" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="SourceFile/cifft_noscale.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

cifft_scale.obj: ../SourceFile/cifft_scale.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="E:/DATA/workspace/USBL(new)_100M/SourceFile" --include_path="/packages/ti/xdais" --include_path="/include" --include_path="E:/DATA/workspace/USBL(new)_100M/Debug" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/include/c5500" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="SourceFile/cifft_scale.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

fir2.obj: ../SourceFile/fir2.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="E:/DATA/workspace/USBL(new)_100M/SourceFile" --include_path="/packages/ti/xdais" --include_path="/include" --include_path="E:/DATA/workspace/USBL(new)_100M/Debug" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/include/c5500" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="SourceFile/fir2.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

h_LPF1.obj: ../SourceFile/h_LPF1.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="E:/DATA/workspace/USBL(new)_100M/SourceFile" --include_path="/packages/ti/xdais" --include_path="/include" --include_path="E:/DATA/workspace/USBL(new)_100M/Debug" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/include/c5500" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="SourceFile/h_LPF1.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

h_LPF2.obj: ../SourceFile/h_LPF2.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="E:/DATA/workspace/USBL(new)_100M/SourceFile" --include_path="/packages/ti/xdais" --include_path="/include" --include_path="E:/DATA/workspace/USBL(new)_100M/Debug" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/include/c5500" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="SourceFile/h_LPF2.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

maxvec.obj: ../SourceFile/maxvec.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="E:/DATA/workspace/USBL(new)_100M/SourceFile" --include_path="/packages/ti/xdais" --include_path="/include" --include_path="E:/DATA/workspace/USBL(new)_100M/Debug" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/include/c5500" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="SourceFile/maxvec.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

twid4096.obj: ../SourceFile/twid4096.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="E:/DATA/workspace/USBL(new)_100M/SourceFile" --include_path="/packages/ti/xdais" --include_path="/include" --include_path="E:/DATA/workspace/USBL(new)_100M/Debug" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/include/c5500" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="SourceFile/twid4096.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

vectors.obj: ../SourceFile/vectors.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" --include_path="E:/DATA/workspace/USBL(new)_100M/SourceFile" --include_path="/packages/ti/xdais" --include_path="/include" --include_path="E:/DATA/workspace/USBL(new)_100M/Debug" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/include" --include_path="D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/include/c5500" --define="CHIP_5509" --define=c5509a --quiet --display_error_number --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="SourceFile/vectors.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


