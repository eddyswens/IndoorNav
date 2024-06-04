################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/decadriver/deca_device.c \
../Core/Src/decadriver/deca_params_init.c 

C_DEPS += \
./Core/Src/decadriver/deca_device.d \
./Core/Src/decadriver/deca_params_init.d 

OBJS += \
./Core/Src/decadriver/deca_device.o \
./Core/Src/decadriver/deca_params_init.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/decadriver/%.o Core/Src/decadriver/%.su Core/Src/decadriver/%.cyclo: ../Core/Src/decadriver/%.c Core/Src/decadriver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Core/Src/dw1000_lib -I../Core/Src/platform -I../Core/Src/decadriver -I../Core/Src/compiler -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-decadriver

clean-Core-2f-Src-2f-decadriver:
	-$(RM) ./Core/Src/decadriver/deca_device.cyclo ./Core/Src/decadriver/deca_device.d ./Core/Src/decadriver/deca_device.o ./Core/Src/decadriver/deca_device.su ./Core/Src/decadriver/deca_params_init.cyclo ./Core/Src/decadriver/deca_params_init.d ./Core/Src/decadriver/deca_params_init.o ./Core/Src/decadriver/deca_params_init.su

.PHONY: clean-Core-2f-Src-2f-decadriver

