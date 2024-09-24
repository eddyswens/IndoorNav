#ifndef __UWB_H__
#define __UWB_H__

#include <stdbool.h>
#include <stdint.h>
#include "cfg.h"
#include "mac.h"
#include <libdw1000.h>
#include <FreeRTOS.h>

#define MAX_ANCHORS 6

typedef struct uwbConfig_s {
  uint8_t mode;
  uint8_t address[8];
  uint8_t anchorListSize;
  uint8_t anchors[MAX_ANCHORS];
  float position[3];
  bool positionEnabled;

  bool smartPower;
  bool forceTxPower;
  uint32_t txPower;

  bool lowBitrate;
  bool longPreamble;
} uwbConfig_t;

typedef struct uwbServiceFromSerial_s {
  uint8_t action;
  uint8_t destinationAddress;

  float position[3];

  uint8_t mode;

  bool enableSmartPower;
  bool forceTxPower;
  uint32_t txPower;

  bool enableLowBitrate;
  bool enableLongPreamble;
} uwbServiceFromSerial_t;

#define MODE_SNIFFER 0
#define MODE_TDOA_ANCHOR2 1
#define MODE_TDOA_ANCHOR3 2
#define MODE_CONFIGURATOR 3

typedef enum uwbEvent_e {
  eventTimeout,
  eventPacketReceived,
  eventPacketSent,
  eventReceiveTimeout,
  eventReceiveFailed,
} uwbEvent_t;

// Callback for one uwb algorithm
typedef struct uwbAlgorithm_s {
  void (*init)(uwbConfig_t * config, dwDevice_t *dev);
  uint32_t (*onEvent)(dwDevice_t *dev, uwbEvent_t event);
  void (*sendService)(dwDevice_t *dev, uwbServiceFromSerial_t *dataToSend);
} uwbAlgorithm_t;


#define MAX_TIMEOUT portMAX_DELAY

void uwbInit();
bool uwbTest();
void uwbStart();
char * uwbStrError();
struct uwbConfig_s * uwbGetConfig();
int uwbAlgorithmCount();
char * uwbAlgorithmName(unsigned int id);
void sendServiceData(uwbServiceFromSerial_t *newPacket);

#endif //__UWB_H__
