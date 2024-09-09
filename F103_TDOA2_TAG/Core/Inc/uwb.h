#ifndef __UWB_H
#define __UWB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "libdw1000.h"

#define DEFAULT_RX_TIMEOUT 10000

#define LOW 0x0
#define HIGH 0x1

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3

#define CS_PIN GPIO_PIN_4  // PA4
#define GPIO_PIN_IRQ GPIO_PIN_9  // PA9
#define GPIO_PIN_RESET GPIO_PIN_7  // PC7

#define DWM_IRQn EXTI9_5_IRQn

#ifdef __cplusplus
}
#endif

#endif /* __UWB_H */