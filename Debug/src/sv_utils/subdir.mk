################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sv_utils/sound_visualizer.c 

OBJS += \
./src/sv_utils/sound_visualizer.o 

C_DEPS += \
./src/sv_utils/sound_visualizer.d 


# Each subdirectory must supply rules for building sources it contributes
src/sv_utils/%.o: ../src/sv_utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -march=armv7-m -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -Dgcc -I/opt/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.4.1/include/ -I/usr/arm-none-eabi/include -std=c99 -ldriver -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


