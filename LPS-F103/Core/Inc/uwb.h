#ifndef __UWB_H__
#define __UWB_H__

#include <stdbool.h>
#include <stdint.h>
#include "cfg.h"
#include "mac.h"
#include <libdw1000.h>
#include <FreeRTOS.h>

#define MAX_ANCHORS 6

/*-------------TDOA THINGS-------------*/
// Packet formats
#define PACKET_TYPE_TDOA2 0x22

// Timeout for receiving a service packet after we TX ours  800
#define RECEIVE_SERVICE_TIMEOUT 800

#define TS_TX_SIZE 4

#define NSLOTS 6

// Still using modulo 2 calculation for slots
// TODO: If A0 is the TDMA master it could transmit slots parameters and frame
//       start so that we would not be limited to modulo 2 anymore
#define TDMA_SLOT_BITS 26 // 26: 2ms timeslot
#define TDMA_NSLOT_BITS 3

#define TDMA_FRAME_BITS (TDMA_SLOT_BITS + TDMA_NSLOT_BITS)
#define TDMA_SLOT_LEN (1ull<<(TDMA_SLOT_BITS+1))
#define TDMA_FRAME_LEN (1ull<<(TDMA_FRAME_BITS+1))

#define TDMA_LAST_FRAME(NOW) ( NOW & ~(TDMA_FRAME_LEN-1) )

// Time length of the preamble
#define PREAMBLE_LENGTH_S ( 128 * 1017.63e-9 )
#define PREAMBLE_LENGTH (uint64_t)( PREAMBLE_LENGTH_S * 499.2e6 * 128 )

// Guard length to account for clock drift and time of flight
#define TDMA_GUARD_LENGTH_S ( 1e-6 )
#define TDMA_GUARD_LENGTH (uint64_t)( TDMA_GUARD_LENGTH_S * 499.2e6 * 128 )

// Timeout for receiving a packet in a timeslot
#define RECEIVE_TIMEOUT 300

/*-------------TDOA THINGS END-------------*/

typedef struct uwbConfig_s {
  uint8_t mode;
  uint8_t address[8];
  uint8_t anchorListSize;
  uint8_t anchors[MAX_ANCHORS];
  float position[3];

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
