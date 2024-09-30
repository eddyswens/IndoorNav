/*
 *  _____________  ____________   _  __
   / ___/ __/ __ \/ __/ ___/ _ | / |/ /
  / (_ / _// /_/ /\ \/ /__/ __ |/    /   
  \___/___/\____/___/\___/_/ |_/_/|_/  
 * 
 * Geoscan UWB firmware
 *
 * Copyright (C) 2024 Geoscan LLC
 * https://www.geoscan.ru
 * 
 * uwb_configuration.c - mode for remote control anchors
 */

#include "uwb.h"

#include <string.h>
#include <stdio.h>

#include "cfg.h"
#include "led.h"

#include "libdw1000.h"
#include "lpp.h"
#include "mac.h"


#include "task.h"

// #define printf(...)
#define debug(...) // printf(__VA_ARGS__)

static uint8_t base_address[] = {0,0,0,0,0,0,0xec,0xdc};

static packet_t txPacket;
static uwbConfig_t config;
static bool serviceToSent = false;
static uint8_t payloadSize = 0;
static dwTime_t tdmaFrameStart;
static uint16_t timeForTrying;  // Timeout Receiving packet from target anchor
static uint64_t startTime;
static bool extIds[MAX_ANCHORS];
static bool synced = false;

typedef struct rangePkg_s {
  uint8_t type;
  uint8_t pid[NSLOTS];  // Packet id of the timestamps
  uint8_t timestamps[NSLOTS][TS_TX_SIZE];  // Relevant time for anchors
  uint16_t distances[NSLOTS];
} __attribute__((packed)) rangePkg_t;

static void setRadioInReceiveMode(dwDevice_t *dev) {
  dwNewReceive(dev);
  dwSetDefaults(dev);
  dwStartReceive(dev);
}
/* Adjust time for schedule transfer by DW1000 radio. Set 9 LSB to 0 */
static uint32_t adjustTxRxTime(dwTime_t *time)
{
  uint32_t added = (1<<9) - (time->low32 & ((1<<9)-1));
  time->low32 = (time->low32 & ~((1<<9)-1)) + (1<<9);
  return added;
}
/* Calculate the transmit time for a given timeslot in the next frame */
static dwTime_t transmittingTimeForSlotInNextFrame(int slot)
{
  uint8_t mul;
  if (slot == 0) mul = MAX_ANCHORS;
  else mul = slot;
  dwTime_t transmitTime = { .full = 0 };
  transmitTime.full = tdmaFrameStart.full + mul*TDMA_FRAME_LEN + slot*TDMA_SLOT_LEN + TDMA_SLOT_LEN/8 ;
  // Add guard and preamble time
  transmitTime.full += TDMA_GUARD_LENGTH;
  transmitTime.full += PREAMBLE_LENGTH;
  // DW1000 can only schedule time with 9 LSB at 0, adjust for it
  adjustTxRxTime(&transmitTime);
  return transmitTime;
}

void transmitServicePacket(dwDevice_t *dev, uint8_t length, uint8_t destId)
{ 
  dwNewTransmit(dev);  // Перевод UWB в режим TX
  dwSetDefaults(dev);
  dwSetData(dev, (uint8_t*)&txPacket, MAC802154_HEADER_LENGTH+2+length);  // length of data + 1 type + 1 action
  dwTime_t txTime = transmittingTimeForSlotInNextFrame(destId);
  dwSetTxRxTime(dev, txTime);

  debug("Sending pkg for addr %u\r\n", destId);

  dwWaitForResponse(dev, true);
  dwStartTransmit(dev);  // Начать передачу
}

static void clearExtIds()
{
  for (uint8_t idx=0;idx<MAX_ANCHORS;idx++) {
    extIds[idx] = false;
  }
}

static bool isNeedSendServiceToId(uint8_t externalId)
{
  if (extIds[externalId]) return false;

  uint8_t targetId = txPacket.destAddress[0];
  if (targetId == 0xFF || targetId == externalId) {
    extIds[externalId] = true;
    return true;
  }
  return false;
}

static void updateSendServiceFlag()
{
  uint16_t timeSinceStart = HAL_GetTick() - startTime;
  if (timeSinceStart > timeForTrying) {
    serviceToSent = false;
    clearExtIds();
  }
}

static void rxcallback(dwDevice_t *dev) {
  packet_t rxPacket;
  dwTime_t rxTime = { .full = 0 };

  int dataLength = dwGetDataLength(dev);  // Получаем длину данных
  dwGetRawReceiveTimestamp(dev, &rxTime);  // Время получения
  dwGetData(dev, (uint8_t*)&rxPacket, dataLength);  // Получаем данные

  setRadioInReceiveMode(dev);

  dwCorrectTimestamp(dev, &rxTime);
  if (dataLength == 0 || rxPacket.payload[0] != PACKET_TYPE_TDOA2) return;

  uint8_t sourceId = rxPacket.sourceAddress[0];
  if (sourceId == 0x0) {
    // Resync local frame start to packet from anchor 0
    rangePkg_t * rangePacket = (rangePkg_t *)rxPacket.payload;
    dwTime_t pkTxTime = { .full = 0 };
    memcpy(&pkTxTime, rangePacket->timestamps[sourceId], TS_TX_SIZE);
    tdmaFrameStart.full = rxTime.full - (pkTxTime.full - TDMA_LAST_FRAME(pkTxTime.full));
    synced = true;
  }

  if (serviceToSent && synced) {
    if (isNeedSendServiceToId(sourceId)) {
      synced = false;
      transmitServicePacket(dev, payloadSize, sourceId);
    }
    updateSendServiceFlag();
  }
}

static void SendServicePacket(dwDevice_t *dev, uwbServiceFromSerial_t *dataToSend)
{
  // We prepare service packet and set flag "serviceToSent"
  uint8_t size;
  clearExtIds();
  switch(dataToSend->action) {
    case LPP_SHORT_ANCHOR_POSITION:
      size = 3 * sizeof(float);
      txPacket.destAddress[0] = dataToSend->destinationAddress;  // DESTINATION
      txPacket.payload[1] = dataToSend->action;  // ACTION (payload[1])
      memcpy(&txPacket.payload[2], dataToSend->position, size);  // DATA (payload[2])

      timeForTrying = 3000;
      startTime = HAL_GetTick();
      payloadSize = size;
      serviceToSent = true;
      break;

    case LPP_SHORT_REBOOT:
      size = 0;
      txPacket.destAddress[0] = dataToSend->destinationAddress;  // DESTINATION
      txPacket.payload[1] = dataToSend->action;  // ACTION - 1

      timeForTrying = 3000;
      startTime = HAL_GetTick();
      payloadSize = size;
      serviceToSent = true;
      break;

    default:
      break;
  }
}

static uint32_t ConfiguratorOnEvent(dwDevice_t *dev, uwbEvent_t event)
{
  switch(event) {
    case eventPacketReceived:
      rxcallback(dev);
      break;
    case eventPacketSent:
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