################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../touchpanel/fsl_ft5406.c 

OBJS += \
./touchpanel/fsl_ft5406.o 

C_DEPS += \
./touchpanel/fsl_ft5406.d 


# Each subdirectory must supply rules for building sources it contributes
touchpanel/%.o: ../touchpanel/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__USE_CMSIS -DCPU_LPC54628J512ET180=1 -DSDK_DEBUGCONSOLE=1 -DPRINTF_FLOAT_ENABLE=1 -DCPU_LPC54628J512ET180 -DCPU_LPC54628J512ET180_cm4 -D__MCUXPRESSO -DDEBUG -D__REDLIB__ -I../board -I"C:\Users\DELL\Desktop\Wbudowane\wbudowane_ekg\heart_monitor\gui" -I../source -I../ -I../drivers -I../device -I../CMSIS -I../touchpanel -I../emwin/emWin_header -I../emwin/emWin_Config -I../utilities -I../component/serial_manager -I../component/uart -I../component/lists -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


