################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
intvecs.obj: ../intvecs.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="D:/workspace_v5_5/lbl_omapl138/src" --include_path="D:/workspace_v5_5/lbl_omapl138/lib" --include_path="D:/workspace_v5_5/lbl_omapl138/inc" --define=omapl138 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="intvecs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="D:/workspace_v5_5/lbl_omapl138/src" --include_path="D:/workspace_v5_5/lbl_omapl138/lib" --include_path="D:/workspace_v5_5/lbl_omapl138/inc" --define=omapl138 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


