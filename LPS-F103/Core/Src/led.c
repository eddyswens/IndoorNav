/*
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * LPS node firmware.
 *
 * Copyright 2016, Bitcraze AB
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
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_gpio.h>
#include "ssd1306.h"
#include "led.h"

typedef struct {
  uint32_t pin;
  GPIO_TypeDef * port;
} led_t;


static const led_t leds_revc[] = {
    [ledRanging] = {.pin = GPIO_PIN_0, .port = GPIOC}, //красный, при запуске
    [ledSync] = {.pin = GPIO_PIN_1, .port = GPIOC}, //зелёный? не вызывается
    [ledMode] = {.pin = GPIO_PIN_2, .port = GPIOC} //синий, большую часть времени горит
};

static bool isBlinking[N_LEDS];
static uint32_t disableTime[N_LEDS];

void ledInit(void) {
  /* Do nothing */
}

static inline void setLed(led_e led, bool value)
{
  HAL_GPIO_WritePin(leds_revc[led].port, leds_revc[led].pin, value?GPIO_PIN_SET:GPIO_PIN_RESET);
    switch (led) {
    case ledRanging:
        ssd1306_draw_string(0, 49, 128, 30, &Font, L"Ranging", value?White:Black);
        break;
    case ledMode:
       // ssd1306_draw_string(0, 49, 128, 30, &Font, L"Mode", value?White:Black);
        break;
    case ledSync:
        ssd1306_draw_string(0, 49, 128, 30, &Font, L"Sync", value?White:Black);
        break;
  }
}

void ledOn(led_e led) {
  isBlinking[led] = false;
  setLed(led, true);
  
}

void ledOff(led_e led) {
  isBlinking[led] = false;
  setLed(led, false);
}

void ledBlink(led_e led, bool oneshot)
{
  isBlinking[led] = true;
  if (oneshot) {
    disableTime[led] = HAL_GetTick() + 50;
    setLed(led, true);
  }
}

void ledTick()
{
  static uint32_t last_tick;
  static bool blinkStatus;

  for (int led=0; led<N_LEDS; led++) {
    if (isBlinking[led] && disableTime[led] && disableTime[led]<HAL_GetTick()) {
      setLed(led, false);
      disableTime[led] = 0;
      isBlinking[led] = false;
    }
  }

  if (HAL_GetTick()>(last_tick+250)) {
    blinkStatus = !blinkStatus;
    last_tick = HAL_GetTick();
    for (int led=0; led<N_LEDS; led++) {
      if (isBlinking[led] && !disableTime[led]) {
        setLed(led, blinkStatus);
      }
    }
  }
}
