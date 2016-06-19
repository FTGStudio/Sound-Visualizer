################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc.c \
../src/can.c \
../src/cmdline.c \
../src/comp.c \
../src/cpu.c \
../src/cpu_usage.c \
../src/crc.c \
../src/eeprom.c \
../src/epi.c \
../src/ethernet.c \
../src/fan.c \
../src/flash.c \
../src/flash_pb.c \
../src/fpu.c \
../src/gpio.c \
../src/hibernate.c \
../src/i2c.c \
../src/i2s.c \
../src/interrupt.c \
../src/isqrt.c \
../src/lpc.c \
../src/main.c \
../src/mpu.c \
../src/peci.c \
../src/pwm.c \
../src/qei.c \
../src/ringbuf.c \
../src/rit128x96x4.c \
../src/scheduler.c \
../src/sine.c \
../src/softi2c.c \
../src/softssi.c \
../src/softuart.c \
../src/ssi.c \
../src/startup_gcc.c \
../src/sysctl.c \
../src/sysexc.c \
../src/systick.c \
../src/timer.c \
../src/uart.c \
../src/uartstdio.c \
../src/udma.c \
../src/usb.c \
../src/ustdlib.c \
../src/watchdog.c 

OBJS += \
./src/adc.o \
./src/can.o \
./src/cmdline.o \
./src/comp.o \
./src/cpu.o \
./src/cpu_usage.o \
./src/crc.o \
./src/eeprom.o \
./src/epi.o \
./src/ethernet.o \
./src/fan.o \
./src/flash.o \
./src/flash_pb.o \
./src/fpu.o \
./src/gpio.o \
./src/hibernate.o \
./src/i2c.o \
./src/i2s.o \
./src/interrupt.o \
./src/isqrt.o \
./src/lpc.o \
./src/main.o \
./src/mpu.o \
./src/peci.o \
./src/pwm.o \
./src/qei.o \
./src/ringbuf.o \
./src/rit128x96x4.o \
./src/scheduler.o \
./src/sine.o \
./src/softi2c.o \
./src/softssi.o \
./src/softuart.o \
./src/ssi.o \
./src/startup_gcc.o \
./src/sysctl.o \
./src/sysexc.o \
./src/systick.o \
./src/timer.o \
./src/uart.o \
./src/uartstdio.o \
./src/udma.o \
./src/usb.o \
./src/ustdlib.o \
./src/watchdog.o 

C_DEPS += \
./src/adc.d \
./src/can.d \
./src/cmdline.d \
./src/comp.d \
./src/cpu.d \
./src/cpu_usage.d \
./src/crc.d \
./src/eeprom.d \
./src/epi.d \
./src/ethernet.d \
./src/fan.d \
./src/flash.d \
./src/flash_pb.d \
./src/fpu.d \
./src/gpio.d \
./src/hibernate.d \
./src/i2c.d \
./src/i2s.d \
./src/interrupt.d \
./src/isqrt.d \
./src/lpc.d \
./src/main.d \
./src/mpu.d \
./src/peci.d \
./src/pwm.d \
./src/qei.d \
./src/ringbuf.d \
./src/rit128x96x4.d \
./src/scheduler.d \
./src/sine.d \
./src/softi2c.d \
./src/softssi.d \
./src/softuart.d \
./src/ssi.d \
./src/startup_gcc.d \
./src/sysctl.d \
./src/sysexc.d \
./src/systick.d \
./src/timer.d \
./src/uart.d \
./src/uartstdio.d \
./src/udma.d \
./src/usb.d \
./src/ustdlib.d \
./src/watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -march=armv7-m -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -Dgcc -std=c99 -ldriver -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


