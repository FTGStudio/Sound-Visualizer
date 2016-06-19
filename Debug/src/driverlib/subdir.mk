################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/driverlib/adc.c \
../src/driverlib/can.c \
../src/driverlib/comp.c \
../src/driverlib/cpu.c \
../src/driverlib/eeprom.c \
../src/driverlib/epi.c \
../src/driverlib/ethernet.c \
../src/driverlib/fan.c \
../src/driverlib/flash.c \
../src/driverlib/fpu.c \
../src/driverlib/gpio.c \
../src/driverlib/hibernate.c \
../src/driverlib/i2c.c \
../src/driverlib/i2s.c \
../src/driverlib/interrupt.c \
../src/driverlib/lpc.c \
../src/driverlib/mpu.c \
../src/driverlib/peci.c \
../src/driverlib/pwm.c \
../src/driverlib/qei.c \
../src/driverlib/ssi.c \
../src/driverlib/sysctl.c \
../src/driverlib/sysexc.c \
../src/driverlib/systick.c \
../src/driverlib/timer.c \
../src/driverlib/uart.c \
../src/driverlib/udma.c \
../src/driverlib/usb.c \
../src/driverlib/watchdog.c 

OBJS += \
./src/driverlib/adc.o \
./src/driverlib/can.o \
./src/driverlib/comp.o \
./src/driverlib/cpu.o \
./src/driverlib/eeprom.o \
./src/driverlib/epi.o \
./src/driverlib/ethernet.o \
./src/driverlib/fan.o \
./src/driverlib/flash.o \
./src/driverlib/fpu.o \
./src/driverlib/gpio.o \
./src/driverlib/hibernate.o \
./src/driverlib/i2c.o \
./src/driverlib/i2s.o \
./src/driverlib/interrupt.o \
./src/driverlib/lpc.o \
./src/driverlib/mpu.o \
./src/driverlib/peci.o \
./src/driverlib/pwm.o \
./src/driverlib/qei.o \
./src/driverlib/ssi.o \
./src/driverlib/sysctl.o \
./src/driverlib/sysexc.o \
./src/driverlib/systick.o \
./src/driverlib/timer.o \
./src/driverlib/uart.o \
./src/driverlib/udma.o \
./src/driverlib/usb.o \
./src/driverlib/watchdog.o 

C_DEPS += \
./src/driverlib/adc.d \
./src/driverlib/can.d \
./src/driverlib/comp.d \
./src/driverlib/cpu.d \
./src/driverlib/eeprom.d \
./src/driverlib/epi.d \
./src/driverlib/ethernet.d \
./src/driverlib/fan.d \
./src/driverlib/flash.d \
./src/driverlib/fpu.d \
./src/driverlib/gpio.d \
./src/driverlib/hibernate.d \
./src/driverlib/i2c.d \
./src/driverlib/i2s.d \
./src/driverlib/interrupt.d \
./src/driverlib/lpc.d \
./src/driverlib/mpu.d \
./src/driverlib/peci.d \
./src/driverlib/pwm.d \
./src/driverlib/qei.d \
./src/driverlib/ssi.d \
./src/driverlib/sysctl.d \
./src/driverlib/sysexc.d \
./src/driverlib/systick.d \
./src/driverlib/timer.d \
./src/driverlib/uart.d \
./src/driverlib/udma.d \
./src/driverlib/usb.d \
./src/driverlib/watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
src/driverlib/%.o: ../src/driverlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


