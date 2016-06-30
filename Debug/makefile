################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/sv_utils/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Sound-Visualizer.hex \

SECONDARY_SIZE += \
Sound-Visualizer.siz \


# All Target
all: Sound-Visualizer.elf secondary-outputs

# Tool invocations
Sound-Visualizer.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -march=armv7-m -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T /home/michael/workspace/Sound-Visualizer/src/soundVisualizer.ld -Xlinker --gc-sections -L/opt/CodeSourcery/Sourcery_G++_Lite/arm-none-eabi/lib/thumb2 -L/home/michael/StellarisWare/driverlib/gcc-cm3 -Wl,-Map,"Sound-Visualizer.map" -Wl,--gc-sections -Wl,--entry=ResetISR -o "Sound-Visualizer.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Sound-Visualizer.hex: Sound-Visualizer.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "Sound-Visualizer.elf"  "Sound-Visualizer.hex"
	@echo 'Finished building: $@'
	@echo ' '

Sound-Visualizer.siz: Sound-Visualizer.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "Sound-Visualizer.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) Sound-Visualizer.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
