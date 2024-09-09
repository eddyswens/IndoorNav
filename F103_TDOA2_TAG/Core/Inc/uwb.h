#ifndef __UWB_H
#define __UWB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "libdw1000.h"

#define DEFAULT_RX_TIMEOUT 10000
#define LPS_NUMBER_OF_ALGORITHMS 3
#define LPS_AUTO_MODE_SWITCH_PERIOD 1000  // should be 1 to 1 ms to ticks

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

/* x,y,z vector */
struct vec3_s {
  uint32_t timestamp; // Timestamp when the data was computed

  float x;
  float y;
  float z;
};
typedef struct vec3_s point_t;

typedef enum uwbEvent_e {
  eventTimeout,
  eventPacketReceived,
  eventPacketSent,
  eventReceiveTimeout,
  eventReceiveFailed,
} uwbEvent_t;

typedef enum {
  lpsMode_auto = 0,
  lpsMode_TWR = 1,
  lpsMode_TDoA2 = 2,
  lpsMode_TDoA3 = 3,
} lpsMode_t;

typedef struct {
  // The status of anchors. A bit field (bit 0 - anchor 0, bit 1 - anchor 1 and so on)
  // where a set bit indicates that an anchor reentry has been detected
  volatile uint16_t rangingState;

  // Requested and current ranging mode
  lpsMode_t userRequestedMode;
  lpsMode_t currentRangingMode;

  // State of the ranging mode auto detection
  bool modeAutoSearchDoInitialize;
  bool modeAutoSearchActive;
  uint32_t nextSwitchTick;
} lpsAlgoOptions_t;

// Callbacks for uwb algorithms
typedef struct uwbAlgorithm_s {
  void (*init)(dwDevice_t *dev);
  uint32_t (*onEvent)(dwDevice_t *dev, uwbEvent_t event);
  bool (*isRangingOk)();
  bool (*getAnchorPosition)(const uint8_t anchorId, point_t* position);
  uint8_t (*getAnchorIdList)(uint8_t unorderedAnchorList[], const int maxListSize);
  uint8_t (*getActiveAnchorIdList)(uint8_t unorderedAnchorList[], const int maxListSize);
} uwbAlgorithm_t;

#ifdef __cplusplus
}
#endif

#endif /* __UWB_H */