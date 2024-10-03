#include <stm32f1xx_hal.h>
#include "ssd1306.h"
#include "led.h"

static bool isBlinking[N_LEDS];
static uint32_t disableTime[N_LEDS];

void ledInit(void) {
  /* Do nothing */
}

static inline void setLed(led_e led, bool value)
{
    switch (led) {
    case ledRx:
        ssd1306_draw_string(0, 49, 128, 30, &Font, L"Rx", value?White:Black);
        break;
    case ledTx:
        ssd1306_draw_string(20, 49, 128, 30, &Font, L"Tx", value?White:Black);
        break;
    case ledSync:
        ssd1306_draw_string(39, 49, 128, 30, &Font, L"Sync", value?White:Black);
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
