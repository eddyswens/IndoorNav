cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_HAL_DRIVER 
	STM32F103xB
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../Core/Inc
    ../../Core/Inc/f1
    ../../Drivers/STM32F1xx_HAL_Driver/Inc
    ../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
    ../../Drivers/CMSIS/Device/ST/STM32F1xx/Include
    ../../Drivers/CMSIS/Include
    ../../STM32_USB_Device_Library/Class/CDC/Inc
    ../../STM32_USB_Device_Library/Core/Inc
    ../../libdw1000/inc
    ../../freertos/inc
)

target_sources(stm32cubemx INTERFACE
    ../../Core/Src/main.c

    ../../Core/Src/f1/gpio.c
   #  ../../Core/Src/f1/i2c.c
     ../../Core/Src/f1/spi.c
    ../../Core/Src/f1/system.c
     ../../Core/Src/f1/usart.c
    ../../Core/Src/f1/usbd_conf.c
    ../../Core/Src/bootmode.c
    ../../Core/Src/button.c
    ../../Core/Src/cfg.c
    ../../Core/Src/dwOps.c
    ../../Core/Src/eeprom.c
    ../../Core/Src/led.c
    ../../Core/Src/lpp.c
    ../../Core/Src/lps25h.c
    ../../Core/Src/production_test.c
    ../../Core/Src/test_support.c
    ../../Core/Src/usb_device.c
    ../../Core/Src/usbcomm.c
    ../../Core/Src/usbd_cdc_if.c
    ../../Core/Src/usbd_desc.c
    ../../Core/Src/uwb_sniffer.c
    ../../Core/Src/uwb_tdoa_anchor2.c
    ../../Core/Src/uwb_tdoa_anchor3.c
    ../../Core/Src/uwb_twr_anchor.c
    ../../Core/Src/uwb_twr_tag.c
    ../../Core/Src/uwb.c
    ../../freertos/src/event_groups.c
    ../../freertos/src/list.c
    ../../freertos/src/port.c
    ../../freertos/src/queue.c
    ../../freertos/src/tasks.c
    ../../freertos/src/timers.c
    ../../libdw1000/src/libdw1000.c
    ../../libdw1000/src/libdw1000Spi.c
    ../../STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    ../../STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    ../../STM32_USB_Device_Library/Core/Src/usbd_core.c
    ../../STM32_USB_Device_Library/Core/Src/usbd_conf_template.c

    ../../Core/Src/stm32f1xx_it.c
    ../../Core/Src/stm32f1xx_hal_msp.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
    ../../Core/Src/system_stm32f1xx.c
    ../../Core/Src/sysmem.c
    ../../Core/Src/syscalls.c
    ../../startup_stm32f103xb.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


