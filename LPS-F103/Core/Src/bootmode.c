/*
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * LPS node bootloader.
 *
 * Copyright 2017, Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

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