################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
QB_main.obj: ../QB_main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/include" --include_path="D:/Program Files (x86)/CCSv6/xdais_7_24_00_04/packages/ti/xdais" --include_path="/include" --define="_DEBUG" --define="CHIP_6416" --define="_TMS320C6400" --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="QB_main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sync.obj: ../Sync.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/include" --include_path="D:/Program Files (x86)/CCSv6/xdais_7_24_00_04/packages/ti/xdais" --include_path="/include" --define="_DEBUG" --define="CHIP_6416" --define="_TMS320C6400" --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sync.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

bootload.obj: ../bootload.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/include" --include_path="D:/Program Files (x86)/CCSv6/xdais_7_24_00_04/packages/ti/xdais" --include_path="/include" --define="_DEBUG" --define="CHIP_6416" --define="_TMS320C6400" --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="bootload.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

c6xdspinit.obj: ../c6xdspinit.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/include" --include_path="D:/Program Files (x86)/CCSv6/xdais_7_24_00_04/packages/ti/xdais" --include_path="/include" --define="_DEBUG" --define="CHIP_6416" --define="_TMS320C6400" --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="c6xdspinit.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

c6xinterrupts.obj: ../c6xinterrupts.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/include" --include_path="D:/Program Files (x86)/CCSv6/xdais_7_24_00_04/packages/ti/xdais" --include_path="/include" --define="_DEBUG" --define="CHIP_6416" --define="_TMS320C6400" --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="c6xinterrupts.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

cf.obj: ../cf.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/include" --include_path="D:/Program Files (x86)/CCSv6/xdais_7_24_00_04/packages/ti/xdais" --include_path="/include" --define="_DEBUG" --define="CHIP_6416" --define="_TMS320C6400" --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="cf.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

databuffer.obj: ../databuffer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/include" --include_path="D:/Program Files (x86)/CCSv6/xdais_7_24_00_04/packages/ti/xdais" --include_path="/include" --define="_DEBUG" --define="CHIP_6416" --define="_TMS320C6400" --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="databuffer.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

isr.obj: ../isr.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/include" --include_path="D:/Program Files (x86)/CCSv6/xdais_7_24_00_04/packages/ti/xdais" --include_path="/include" --define="_DEBUG" --define="CHIP_6416" --define="_TMS320C6400" --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="isr.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

serial_order.obj: ../serial_order.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/include" --include_path="D:/Program Files (x86)/CCSv6/xdais_7_24_00_04/packages/ti/xdais" --include_path="/include" --define="_DEBUG" --define="CHIP_6416" --define="_TMS320C6400" --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="serial_order.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

vectors.obj: ../vectors.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/include" --include_path="D:/Program Files (x86)/CCSv6/xdais_7_24_00_04/packages/ti/xdais" --include_path="/include" --define="_DEBUG" --define="CHIP_6416" --define="_TMS320C6400" --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="vectors.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


