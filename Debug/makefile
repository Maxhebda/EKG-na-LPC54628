################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include touchpanel/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include gui/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/serial_manager/subdir.mk
-include component/lists/subdir.mk
-include board/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: heart_monitor.axf

# Tool invocations
heart_monitor.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\DELL\Desktop\Wbudowane\wbudowane_ekg\heart_monitor\libs" -L"C:\Users\DELL\Desktop\Wbudowane\wbudowane_ekg\heart_monitor\emwin\emWin_library\MCUXpresso" -Xlinker -Map="heart_monitor.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T "Lab03_emwin_Debug.ld" -o "heart_monitor.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) heart_monitor.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "heart_monitor.axf"; # arm-none-eabi-objcopy -v -O binary "heart_monitor.axf" "heart_monitor.bin" ; # checksum -p LPC54628J512 -d "heart_monitor.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
