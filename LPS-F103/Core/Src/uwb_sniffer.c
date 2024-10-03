#include "uwb.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "cfg.h"
#include "led.h"

#include "libdw1000.h"

#include "dwOps.h"
#include "mac.h"


static uint32_t SnifferOnEvent(dwDevice_t *dev, uwbEvent_t event)
{
  static dwTime_t arrival;
  static packet_t rxPacket;

  if (event == eventPacketReceived) {
    ledOn(ledSync);
    int dataLength = dwGetDataLength(dev);
    dwGetRawReceiveTimestamp(dev, &arrival);
    dwGetData(dev, (uint8_t*)&rxPacket, dataLength);

    dwNewReceive(dev);
    dwSetDefaults(dev);
    dwStartReceive(dev);

    printf("From %02x to %02x @%02x%08x: ", rxPacket.sourceAddress[0],
                                          rxPacket.destAddress[0],
                                          (unsigned int) arrival.high8,
                                          (unsigned int) arrival.low32);
    for (int i=0; i<(dataLength - MAC802154_HEADER_LENGTH); i++) {
      printf("%02x", rxPacket.payload[i]);
    }
    printf("\r\n");

  } else {
    ledOff(ledSync);
    dwNewReceive(dev);
    dwSetDefaults(dev);
    dwStartReceive(dev);
  }

  return MAX_TIMEOUT;
}

static void SnifferInit(uwbConfig_t * newconfig, dwDevice_t *dev)
{

}

uwbAlgorithm_t uwbSnifferAlgorithm = {
    .init = SnifferInit,
    .onEvent = SnifferOnEvent,
};
