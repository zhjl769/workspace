################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"../USBL.cmd" \
"./main.obj" \
"./cbrev.obj" \
"./cfft_noscale.obj" \
"./cfft_scale.obj" \
"./cifft_noscale.obj" \
"./cifft_scale.obj" \
"./fir2.obj" \
"./h_LPF1.obj" \
"./h_LPF2.obj" \
"./maxvec.obj" \
"./twid4096.obj" \
"./vectors.obj" \
$(GEN_CMDS__FLAG) \
-l"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/lib/rts55x.lib" \
-l"D:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/csl/lib/csl5509ax.lib" \
-llibc.a \

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
USBL.out \

EXE_OUTPUTS__QUOTED += \
"USBL.out" \

BIN_OUTPUTS += \
USBL.hex \

BIN_OUTPUTS__QUOTED += \
"USBL.hex" \


# All Target
all: USBL.out

# Tool invocations
USBL.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C5500 Linker'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/cl55" -v5509A --memory_model=large -O3 --symdebug:skeletal --define="CHIP_5509A" --define=c5509a --display_error_number --ptrdiff_size=32 -z -m"USBL.map" --stack_size=0x200 --heap_size=0x400 -i"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/lib" -i"D:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/csl/include" -i"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/include" -i"E:/DATA/workspace/USBL_XINBIAO_100M/SourceFile" -i"D:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/dsplib/include" -i"D:/Program Files (x86)/CCSv6/ccsv6/ccs_base/c5500/include" -i"D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/rtdx/lib/c5500" -i"D:/Program Files (x86)/CCSv6/bios_5_42_02_10/packages/ti/bios/lib" --reread_libs --display_error_number --warn_sections --xml_link_info="USBL_XINBIAO_100M_linkInfo.xml" --rom_model --sys_stacksize=0x200 --strict_compatibility=off -o "USBL.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

USBL.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C5500 Hex Utility'
	"D:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/c5500_4.4.1/bin/hex55"  -o "USBL.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_HDRS__QUOTED)$(GEN_MISC_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_CMDS__QUOTED)
	-$(RM) "main.d" 
	-$(RM) "main.obj" "cbrev.obj" "cfft_noscale.obj" "cfft_scale.obj" "cifft_noscale.obj" "cifft_scale.obj" "fir2.obj" "h_LPF1.obj" "h_LPF2.obj" "maxvec.obj" "twid4096.obj" "vectors.obj" 
	-$(RM) "SourceFile\cbrev.d" "SourceFile\cfft_noscale.d" "SourceFile\cfft_scale.d" "SourceFile\cifft_noscale.d" "SourceFile\cifft_scale.d" "SourceFile\fir2.d" "SourceFile\h_LPF1.d" "SourceFile\h_LPF2.d" "SourceFile\maxvec.d" "SourceFile\twid4096.d" "SourceFile\vectors.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

