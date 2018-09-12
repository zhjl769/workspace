################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../I2C.cmd \
../IIC.cmd 

S55_SRCS += \
../vectors_i2c1.s55 

ASM_SRCS += \
../I2C.asm \
../USBL.asm \
../regs.asm \
../vector.asm 

C_SRCS += \
../main.c 

OBJS += \
./I2C.obj \
./USBL.obj \
./main.obj \
./regs.obj \
./vector.obj \
./vectors_i2c1.obj 

ASM_DEPS += \
./I2C.d \
./USBL.d \
./regs.d \
./vector.d 

S55_DEPS += \
./vectors_i2c1.d 

C_DEPS += \
./main.d 

C_DEPS__QUOTED += \
"main.d" 

OBJS__QUOTED += \
"I2C.obj" \
"USBL.obj" \
"main.obj" \
"regs.obj" \
"vector.obj" \
"vectors_i2c1.obj" 

S55_DEPS__QUOTED += \
"vectors_i2c1.d" 

ASM_DEPS__QUOTED += \
"I2C.d" \
"USBL.d" \
"regs.d" \
"vector.d" 

ASM_SRCS__QUOTED += \
"../I2C.asm" \
"../USBL.asm" \
"../regs.asm" \
"../vector.asm" 

C_SRCS__QUOTED += \
"../main.c" 

S55_SRCS__QUOTED += \
"../vectors_i2c1.s55" 


