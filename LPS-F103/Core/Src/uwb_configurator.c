#include "uwb.h"

#include <string.h>
#include <stdio.h>

#include "cfg.h"
#include "led.h"

#include "libdw1000.h"
#include "lpp.h"
#include "mac.h"

static uint8_t base_address[] = {0,0,0,0,0,0,0xec,0xdc};

static packet_t txPacket;
static volatile uint8_t curr_seq = 0;
static int curr_anchor = 0;
static uwbConfig_t config;
static bool serviceToSent = false;
static uint8_t globalSize = 0;
static dwTime_t tdmaFrameStart;

static dwTime_t startTxTicks;
static dwTime_t finishTxTicks;

typedef struct rangePacketListener_s {
  uint8_t type;
  uint8_t pid[NSLOTS];  // Packet id of the timestamps
  uint8_t timestamps[NSLOTS][TS_TX_SIZE];  // Relevant time for anchors
  uint16_t distances[NSLOTS];
} __attribute__((packed)) rangePacketListener_t;

// #define printf(...)
#define debug(...) // printf(__VA_ARGS__)

static void txcallback(dwDevice_t *dev)
{

}

static bool rxcallback(dwDevice_t *dev) {
  packet_t rxPacket;
  dwTime_t rxTime = { .full = 0 };

  dwGetRawReceiveTimestamp(dev, &rxTime);
  dwCorrectTimestamp(dev, &rxTime);

  int dataLength = dwGetDataLength(dev);  // Получаем длину данных
  dwGetData(dev, (uint8_t*)&rxPacket, dataLength);

  if (dataLength == 0 || rxPacket.payload[0] != PACKET_TYPE_TDOA2) {
    return;
  }
  
  rangePacketListener_t * rangePacket = (rangePacketListener_t *)rxPacket.payload;

  uint8_t sourceAddr = rxPacket.sourceAddress[0];
  if (sourceAddr == 0x0) {
    // Resync local frame start to packet from anchor 0
    dwTime_t pkTxTime = { .full = 0 };
    memcpy(&pkTxTime, rangePacket->timestamps[sourceAddr], TS_TX_SIZE);
    tdmaFrameStart.full = rxTime.full - (pkTxTime.full - TDMA_LAST_FRAME(pkTxTime.full));
    tdmaFrameStart.full += TDMA_FRAME_LEN;
  }

  if(rxPacket.sourceAddress[0] == 0x0 && serviceToSent)
  {
    transmitData(dev, globalSize);
    serviceToSent = false;
    return true;
  }
  else
  {
    return false;
  }
}

/* Adjust time for schedule transfer by DW1000 radio. Set 9 LSB to 0 */
static uint32_t adjustTxRxTime(dwTime_t *time)
{
  uint32_t added = (1<<9) - (time->low32 & ((1<<9)-1));

  time->low32 = (time->low32 & ~((1<<9)-1)) + (1<<9);

  return added;
}

/* Calculate the transmit time for a given timeslot in the next frame */
static dwTime_t transmitTimeForSlot(int slot)
{
  dwTime_t transmitTime = { .full = 0 };

  transmitTime.full = tdmaFrameStart.full + slot*TDMA_SLOT_LEN + TDMA_SLOT_LEN/8;

  // Add guard and preamble time
  transmitTime.full += TDMA_GUARD_LENGTH;
  transmitTime.full += PREAMBLE_LENGTH;

  // DW1000 can only schedule time with 9 LSB at 0, adjust for it
  adjustTxRxTime(&transmitTime);

  return transmitTime;
}

static void setRadioInReceiveMode(dwDevice_t *dev) {
  dwNewReceive(dev);
  dwSetDefaults(dev);
  dwStartReceive(dev);
}

void transmitData(dwDevice_t *dev, uint8_t length)
{ 
  dwTime_t txTime = transmitTimeForSlot(0);
  
  dwNewTransmit(dev);  // Перевод UWB в режим TX
  dwSetDefaults(dev);
  dwSetData(dev, (uint8_t*)&txPacket, MAC802154_HEADER_LENGTH+2+length);  // length of data + 1 type + 1 action
  dwSetTxRxTime(dev, txTime);

  dwWaitForResponse(dev, true);
  dwStartTransmit(dev);  // Начать передачу
}

static void SendServicePacket(dwDevice_t *dev, uwbServiceFromSerial_t *dataToSend)
{
  uint8_t size;
  switch(dataToSend->action) {
    case LPP_SHORT_ANCHOR_POSITION:
      size = 3 * sizeof(float);
      txPacket.destAddress[0] = dataToSend->destinationAddress;  // DESTINATION
      txPacket.payload[1] = LPP_SHORT_ANCHOR_POSITION;  // ACTION (payload[1])
      memcpy(&txPacket.payload[2], dataToSend->position, size);  // DATA (payload[2])
      globalSize = size;
      serviceToSent = true;
      // transmitData(dev, size);
      break;
    case LPP_SHORT_REBOOT:
      size = 0;
      txPacket.destAddress[0] = 0xFF;  // DESTINATION
      txPacket.payload[1] = LPP_SHORT_REBOOT;  // ACTION - 1
      globalSize = size;
      serviceToSent = true;
      // transmitData(dev, size);
      break;
    default:
      break;
  }
}

static uint32_t ConfiguratorOnEvent(dwDevice_t *dev, uwbEvent_t event)
{
  switch(event) {
    case eventPacketReceived:
      if(!rxcallback(dev))
      {
        setRadioInReceiveMode(dev);
      }
      break;
    case eventPacketSent:
      txcallback(dev);
      break;
    case eventReceiveFailed:
    case eventTimeout:
    case eventReceiveTimeout:  // Таймаут, Неудача получения пакета, Таймаут получения
      setRadioInReceiveMode(dev);  // Переходим снова в режим RX
      break;
    default:
      configASSERT(false);
  }

  return MAX_TIMEOUT;
}

static void ConfiguratorInit(uwbConfig_t * newconfig, dwDevice_t *dev)
{
  // Set the LED for anchor mode
  //ledOn(ledMode);
  config = *newconfig;

  // Initialize the packet in the TX buffer
  MAC80215_PACKET_INIT(txPacket, MAC802154_TYPE_DATA);
  txPacket.pan = 0xdcec;
  txPacket.payload[0] = SHORT_LPP;
  memcpy(txPacket.sourceAddress, base_address, 8);  // базовый
  txPacket.sourceAddress[0] = config.address[0];  
  memcpy(txPacket.destAddress, base_address, 8);
  // txPacket.destAddress[0] = 0xff;

  // onEvent is going to be called with eventTimeout which will start ranging
}

uwbAlgorithm_t uwbConfiguratorAlgorithm = {
    .init = ConfiguratorInit,
    .onEvent = ConfiguratorOnEvent,
    .sendService = SendServicePacket,
};
