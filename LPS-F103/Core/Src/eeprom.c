#include <stm32f1xx_hal.h>
#include <stdio.h>

#include <stdbool.h>
#include <string.h>

#include "eeprom.h"

#include "FreeRTOS.h"
#include "task.h"

static I2C_HandleTypeDef * hi2c;
static int devAddr = 0xA0;

void eepromInit(I2C_HandleTypeDef * i2c)
{
  hi2c = i2c;
}

bool eepromTest()
{
  uint8_t test;

  return eepromRead(0, &test, 1);
}

bool eepromRead(int address, void* data, size_t length)
{
  int status;

  status = HAL_I2C_Mem_Read(hi2c, devAddr, address, I2C_MEMADD_SIZE_16BIT, data, length, 100);

  if (status == HAL_OK)
    return true;

  return false;
}

bool eepromWrite(int address, void* data, size_t length)
{
  int status = HAL_OK;
  size_t start_address = address;
  size_t start_i = 0;

  for (int i=0; i<length; i++) {
    if ((address+i+1)%32 == 0) {
      status = HAL_I2C_Mem_Write(hi2c, devAddr, start_address, I2C_MEMADD_SIZE_16BIT, ((uint8_t*)data)+start_i, (i+1) - start_i, 100);
      start_address = address + i + 1;
      start_i = i+1;
      vTaskDelay(10);
    }
  }
  if (start_i != length) {
    status = HAL_I2C_Mem_Write(hi2c, devAddr, start_address, I2C_MEMADD_SIZE_16BIT, ((uint8_t*)data)+start_i, length - start_i, 100);
    vTaskDelay(10);
  }

  if (status == HAL_OK)
    return true;

  return false;
}
