#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "bootmode.h"


#define MAGIC_BOOT_MODE_MARKER 0xBCbc0d07

static void writeBuRegister(uint32_t data);

void bootmodeSetBootloaderModeFlag() {
  writeBuRegister(MAGIC_BOOT_MODE_MARKER);
}

void bootmodeClearBootloaderModeFlag() {
  writeBuRegister(0);
}


static void writeBuRegister(uint32_t data) {
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  // Enable backup register access (STM32F10x Specific)
  PWR->CR |= PWR_CR_DBP;

  // Write data to the backup register
  BKP->DR1 = data; 

  // Disable backup register access (optional, but good practice)
  PWR->CR &= ~PWR_CR_DBP;
}