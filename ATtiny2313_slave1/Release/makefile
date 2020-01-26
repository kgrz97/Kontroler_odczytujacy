################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
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
LSS += \
ATtiny2313_slave.lss \

FLASH_IMAGE += \
ATtiny2313_slave.hex \

EEPROM_IMAGE += \
ATtiny2313_slave.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: ATtiny2313_slave.elf secondary-outputs

# Tool invocations
ATtiny2313_slave.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,ATtiny2313_slave.map -mmcu=attiny2313 -o "ATtiny2313_slave.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ATtiny2313_slave.lss: ATtiny2313_slave.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S ATtiny2313_slave.elf  >"ATtiny2313_slave.lss"
	@echo 'Finished building: $@'
	@echo ' '

ATtiny2313_slave.hex: ATtiny2313_slave.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex ATtiny2313_slave.elf  "ATtiny2313_slave.hex"
	@echo 'Finished building: $@'
	@echo ' '

ATtiny2313_slave.eep: ATtiny2313_slave.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex ATtiny2313_slave.elf  "ATtiny2313_slave.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: ATtiny2313_slave.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=attiny2313 ATtiny2313_slave.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) ATtiny2313_slave.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
