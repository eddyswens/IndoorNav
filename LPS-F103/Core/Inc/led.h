#ifndef LED_H_
#define LED_H_

#ifdef STM32F103xB
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_gpio.h>
#else
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#endif

#include <stdbool.h>

#define N_LEDS 11

typedef enum {
  ledA0=0,
  ledA1,
  ledA2,
  ledA3,
  ledA4,
  ledA5,
  ledA6,
  ledA7,
  ledRx,
  ledTx,
  ledSync
} led_e;



void ledInit(void);

void ledOn(led_e led);

void ledOff(led_e led);

void ledBlink(led_e led, bool oneshot);

void ledTick();

#endif /* LED_H_ */
