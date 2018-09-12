################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../QB.cmd 

LIB_SRCS += \
C:/Users/zhjl769/C6000/csl/lib/csl6416.lib \
C:/Users/zhjl769/c6400/dsplib/lib/dsp64x.lib \
D:/Program\ Files\ (x86)/CCSv6/ccsv6/tools/compiler/c6000_7.4.23/lib/rts6400.lib 

ASM_SRCS += \
../bootload.asm \
../vectors.asm 

C_SRCS += \
../QB_main.c \
../Sync.c \
../c6xdspinit.c \
../c6xinterrupts.c \
../cf.c \
../databuffer.c \
../isr.c \
../serial_order.c 

OBJS += \
./QB_main.obj \
./Sync.obj \
./bootload.obj \
./c6xdspinit.obj \
./c6xinterrupts.obj \
./cf.obj \
./databuffer.obj \
./isr.obj \
./serial_order.obj \
./vectors.obj 

ASM_DEPS += \
./bootload.d \
./vectors.d 

C_DEPS += \
./QB_main.d \
./Sync.d \
./c6xdspinit.d \
./c6xinterrupts.d \
./cf.d \
./databuffer.d \
./isr.d \
./serial_order.d 

C_DEPS__QUOTED += \
"QB_main.d" \
"Sync.d" \
"c6xdspinit.d" \
"c6xinterrupts.d" \
"cf.d" \
"databuffer.d" \
"isr.d" \
"serial_order.d" 

OBJS__QUOTED += \
"QB_main.obj" \
"Sync.obj" \
"bootload.obj" \
"c6xdspinit.obj" \
"c6xinterrupts.obj" \
"cf.obj" \
"databuffer.obj" \
"isr.obj" \
"serial_order.obj" \
"vectors.obj" 

ASM_DEPS__QUOTED += \
"bootload.d" \
"vectors.d" 

C_SRCS__QUOTED += \
"../QB_main.c" \
"../Sync.c" \
"../c6xdspinit.c" \
"../c6xinterrupts.c" \
"../cf.c" \
"../databuffer.c" \
"../isr.c" \
"../serial_order.c" 

ASM_SRCS__QUOTED += \
"../bootload.asm" \
"../vectors.asm" 


