################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"../QB.cmd" \
"./QB_main.obj" \
"./Sync.obj" \
"./bootload.obj" \
"./c6xdspinit.obj" \
"./c6xinterrupts.obj" \
"./cf.obj" \
"./databuffer.obj" \
"./isr.obj" \
"./serial_order.obj" \
"./vectors.obj" \
"C:/Users/zhjl769/C6000/csl/lib/csl6416.lib" \
"C:/Users/zhjl769/c6400/dsplib/lib/dsp64x.lib" \
"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/lib/rts6400.lib" \
$(GEN_CMDS__FLAG) \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
QB_1.out \

EXE_OUTPUTS__QUOTED += \
"QB_1.out" \


# All Target
all: QB_1.out

# Tool invocations
QB_1.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/bin/cl6x" -mv6400 --abi=coffabi -g --define="_DEBUG" --define="CHIP_6416" --define="_TMS320C6400" --diag_wrap=off --display_error_number -z -m"QB_1.map" -i"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/lib" -i"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/include" -i"E:/DATA/workspace/QB_1" -i"/lib" --reread_libs --display_error_number --warn_sections --diag_wrap=off --xml_link_info="QB_1_linkInfo.xml" --rom_model -o "QB_1.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "QB_main.d" "Sync.d" "c6xdspinit.d" "c6xinterrupts.d" "cf.d" "databuffer.d" "isr.d" "serial_order.d" 
	-$(RM) "QB_main.obj" "Sync.obj" "bootload.obj" "c6xdspinit.obj" "c6xinterrupts.obj" "cf.obj" "databuffer.obj" "isr.obj" "serial_order.obj" "vectors.obj" 
	-$(RM) "bootload.d" "vectors.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

