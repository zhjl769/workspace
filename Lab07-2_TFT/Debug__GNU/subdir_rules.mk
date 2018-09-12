################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/msp430-gcc-7.3.1.24_win32/bin/msp430-elf-gcc.exe" -c -I"E:/Program Files (x86)/CCSv6/ccsv6/tools/compiler/msp430-gcc-7.3.1.24_win32/msp430-elf/include" -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


