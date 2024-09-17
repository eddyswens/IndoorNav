#include <stdbool.h>

#include "gpio.h"
#include "button.h" 

static ButtonEvent state;

static bool buttonRead(void)
{
  return (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);
}

void buttonInit(ButtonEvent initialEvent)
{
  state = initialEvent;
}

void buttonProcess()
{
  static unsigned int lastTick;
  static unsigned int pressedTick;
  static bool pressed;

  if (lastTick != HAL_GetTick())
  {
    lastTick = HAL_GetTick();

    if (pressed == false && buttonRead() == BUTTON_PRESSED)
    {
      pressed = true;
      pressedTick = HAL_GetTick();
    }
    else if (pressed == true && buttonRead() == BUTTON_RELEASED)
    {
      pressed = false;
      if ((HAL_GetTick() - pressedTick) < BUTTON_LONGPRESS_TICK)
      {
        state = buttonShortPress;
      }
      else
      {
        state = buttonLongPress;
      }
    }
  }
}

ButtonEvent buttonGetState()
{
  ButtonEvent currentState = state;

  state = buttonIdle;

  return currentState;
}


