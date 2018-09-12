################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../test_omapl138.cmd 

LIB_SRCS += \
D:/lib/dsplib64plus.lib \
E:/DATA/bsl_zq_change_0718_noprintf/lib/evmomapl138_bsl.lib \
D:/Program\ Files\ (x86)/CCSv5/ccsv5/tools/compiler/c6000_7.4.4/lib/rts67plus.lib 

ASM_SRCS += \
../intvecs.asm 

C_SRCS += \
../evmomapl138.c \
../evmomapl138_gpio.c \
../evmomapl138_timer.c \
../function.c \
../main_sys.c \
../sys_interrupts.c \
../user_evmomapl138.c 

OBJS += \
./evmomapl138.obj \
./evmomapl138_gpio.obj \
./evmomapl138_timer.obj \
./function.obj \
./intvecs.obj \
./main_sys.obj \
./sys_interrupts.obj \
./user_evmomapl138.obj 

ASM_DEPS += \
./intvecs.pp 

C_DEPS += \
./evmomapl138.pp \
./evmomapl138_gpio.pp \
./evmomapl138_timer.pp \
./function.pp \
./main_sys.pp \
./sys_interrupts.pp \
./user_evmomapl138.pp 

C_DEPS__QUOTED += \
"evmomapl138.pp" \
"evmomapl138_gpio.pp" \
"evmomapl138_timer.pp" \
"function.pp" \
"main_sys.pp" \
"sys_interrupts.pp" \
"user_evmomapl138.pp" 

OBJS__QUOTED += \
"evmomapl138.obj" \
"evmomapl138_gpio.obj" \
"evmomapl138_timer.obj" \
"function.obj" \
"intvecs.obj" \
"main_sys.obj" \
"sys_interrupts.obj" \
"user_evmomapl138.obj" 

ASM_DEPS__QUOTED += \
"intvecs.pp" 

C_SRCS__QUOTED += \
"../evmomapl138.c" \
"../evmomapl138_gpio.c" \
"../evmomapl138_timer.c" \
"../function.c" \
"../main_sys.c" \
"../sys_interrupts.c" \
"../user_evmomapl138.c" 

ASM_SRCS__QUOTED += \
"../intvecs.asm" 


