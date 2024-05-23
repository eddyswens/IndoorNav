################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/dw1000_lib/DW1000.cpp \
../Core/Src/dw1000_lib/DW1000Device.cpp \
../Core/Src/dw1000_lib/DW1000Mac.cpp \
../Core/Src/dw1000_lib/DW1000Ranging.cpp \
../Core/Src/dw1000_lib/DW1000Time.cpp 

OBJS += \
./Core/Src/dw1000_lib/DW1000.o \
./Core/Src/dw1000_lib/DW1000Device.o \
./Core/Src/dw1000_lib/DW1000Mac.o \
./Core/Src/dw1000_lib/DW1000Ranging.o \
./Core/Src/dw1000_lib/DW1000Time.o 

CPP_DEPS += \
./Core/Src/dw1000_lib/DW1000.d \
./Core/Src/dw1000_lib/DW1000Device.d \
./Core/Src/dw1000_lib/DW1000Mac.d \
./Core/Src/dw1000_lib/DW1000Ranging.d \
./Core/Src/dw1000_lib/DW1000Time.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/dw1000_lib/%.o Core/Src/dw1000_lib/%.su Core/Src/dw1000_lib/%.cyclo: ../Core/Src/dw1000_lib/%.cpp Core/Src/dw1000_lib/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F302x8 -c -I../Core/Inc -I../Core/Src/dw1000_lib -I../Core/Src/decadriver -I../Core/Src/compiler -I../Core/Src/platform -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-dw1000_lib

clean-Core-2f-Src-2f-dw1000_lib:
	-$(RM) ./Core/Src/dw1000_lib/DW1000.cyclo ./Core/Src/dw1000_lib/DW1000.d ./Core/Src/dw1000_lib/DW1000.o ./Core/Src/dw1000_lib/DW1000.su ./Core/Src/dw1000_lib/DW1000Device.cyclo ./Core/Src/dw1000_lib/DW1000Device.d ./Core/Src/dw1000_lib/DW1000Device.o ./Core/Src/dw1000_lib/DW1000Device.su ./Core/Src/dw1000_lib/DW1000Mac.cyclo ./Core/Src/dw1000_lib/DW1000Mac.d ./Core/Src/dw1000_lib/DW1000Mac.o ./Core/Src/dw1000_lib/DW1000Mac.su ./Core/Src/dw1000_lib/DW1000Ranging.cyclo ./Core/Src/dw1000_lib/DW1000Ranging.d ./Core/Src/dw1000_lib/DW1000Ranging.o ./Core/Src/dw1000_lib/DW1000Ranging.su ./Core/Src/dw1000_lib/DW1000Time.cyclo ./Core/Src/dw1000_lib/DW1000Time.d ./Core/Src/dw1000_lib/DW1000Time.o ./Core/Src/dw1000_lib/DW1000Time.su

.PHONY: clean-Core-2f-Src-2f-dw1000_lib

