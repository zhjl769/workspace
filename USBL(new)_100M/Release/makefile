################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"../USBL.cmd" \
"./vectors.obj" \
"./twid4096.obj" \
"./maxvec.obj" \
"./h_LPF2.obj" \
"./h_LPF1.obj" \
"./fir2.obj" \
"./cifft_scale.obj" \
"./cifft_noscale.obj" \
"./cfft_scale.obj" \
"./cfft_noscale.obj" \
"./cbrev.obj" \
"./main.obj" \
-l"D:\Program Files\ccsv5\ccsv5\tools\compiler\c5500_4.4.1\lib\rts55x.lib" \
-l"csl5509x.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include SourceFile/subdir_vars.mk
-include subdir_rules.mk
-include SourceFile/subdir_rules.mk
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
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
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

# All Target
all: USBL.out

# Tool invocations
USBL.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C5500 Linker'
	"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 --symdebug:skeletal --define="CHIP_5509" --define=c5509a --display_error_number --ptrdiff_size=32 -z -m"USBL.map" --stack_size=0x200 --heap_size=0x400 -i"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/lib" -i"D:/Program Files (x86)/CCSv5/ccsv5/tools/compiler/c5500_4.4.1/include" -i"E:/DATA/workspace/USBL(new)_100M" -i"E:/DATA/workspace/USBL(new)_100M/SourceFile" -i"D:/PROGRA~2/CCSTUD~1.3/C5500/csl/lib" -i"D:/Program Files (x86)/CCSv5/bios_5_42_01_09/packages/ti/rtdx/lib/c5500" -i"D:/Program Files (x86)/CCSv5/bios_5_42_01_09/packages/ti/bios/lib" --reread_libs --display_error_number --warn_sections --xml_link_info="USBL(new)_100M_linkInfo.xml" --rom_model --sys_stacksize=0x200 --strict_compatibility=off -o "USBL.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C5500_EXECUTABLE_OUTPUTS__QUOTED)$(GEN_HDRS__QUOTED)$(GEN_MISC_FILES__QUOTED)$(GEN_FILES__QUOTED)$(GEN_CMDS__QUOTED) "USBL.out"
	-$(RM) "main.pp" 
	-$(RM) "main.obj" "cbrev.obj" "cfft_noscale.obj" "cfft_scale.obj" "cifft_noscale.obj" "cifft_scale.obj" "fir2.obj" "h_LPF1.obj" "h_LPF2.obj" "maxvec.obj" "twid4096.obj" "vectors.obj" 
	-$(RM) "SourceFile\cbrev.pp" "SourceFile\cfft_noscale.pp" "SourceFile\cfft_scale.pp" "SourceFile\cifft_noscale.pp" "SourceFile\cifft_scale.pp" "SourceFile\fir2.pp" "SourceFile\h_LPF1.pp" "SourceFile\h_LPF2.pp" "SourceFile\maxvec.pp" "SourceFile\twid4096.pp" "SourceFile\vectors.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

