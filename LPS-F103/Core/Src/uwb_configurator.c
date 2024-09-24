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
uwbConfig_t config;

// #define printf(...)
#define debug(...) // printf(__VA_ARGS__)

static void txcallback(dwDevice_t *dev)
{
  // dwTime_t departure;
  // dwGetTransmitTimestamp(dev, &departure);
  // departure.full += (ANTENNA_DELAY/2);

  // debug("TXCallback\r\n");

  // switch (txPacket.payload[0]) {
  //   case POLL:
  //     poll_tx = departure;
  //     break;
  //   case FINAL:
  //     final_tx = departure;
  //     break;
  // }
}

#define TYPE 0
#define SEQ 1

static void rxcallback(dwDevice_t *dev) {
  // dwTime_t arival = { .full=0 };
  // int dataLength = dwGetDataLength(dev);

  // if (dataLength == 0) return;

  // bzero(&rxPacket, MAC802154_HEADER_LENGTH);

  // debug("RXCallback(%d): ", dataLength);

  // dwGetData(dev, (uint8_t*)&rxPacket, dataLength);

  // if (memcmp(rxPacket.destAddress, config.address, 8)) {
  //   debug("Not for me! for %02x with %02x\r\n", rxPacket.destAddress[0], rxPacket.payload[0]);
  //   dwNewReceive(dev);
  //   dwSetDefaults(dev);
  //   dwStartReceive(dev);
  //   return;
  // }

  // memcpy(txPacket.destAddress, rxPacket.sourceAddress, 8);
  // memcpy(txPacket.sourceAddress, rxPacket.destAddress, 8);

  // switch(rxPacket.payload[TYPE]) {
  //   // Tag received messages
  //   case ANSWER:
  //     debug("ANSWER\r\n");

  //     if (rxPacket.payload[SEQ] != curr_seq) {
  //       debug("Wrong sequence number!\r\n");
  //       return;
  //     }

  //     txPacket.payload[0] = FINAL;
  //     txPacket.payload[SEQ] = rxPacket.payload[SEQ];

  //     dwNewTransmit(dev);
  //     dwSetData(dev, (uint8_t*)&txPacket, MAC802154_HEADER_LENGTH+2);

  //     dwWaitForResponse(dev, true);
  //     dwStartTransmit(dev);

  //     dwGetReceiveTimestamp(dev, &arival);
  //     arival.full -= (ANTENNA_DELAY/2);
  //     answer_rx = arival;
  //     break;
  //   case REPORT:
  //   {
  //     reportPayload_t *report = (reportPayload_t *)(rxPacket.payload+2);
  //     double tround1, treply1, treply2, tround2, tprop_ctn, tprop, distance;

  //     debug("REPORT\r\n");

  //     if (rxPacket.payload[SEQ] != curr_seq) {
  //       printf("Wrong sequence number!\r\n");
  //       return;
  //     }

  //     memcpy(&poll_rx, &report->pollRx, 5);
  //     memcpy(&answer_tx, &report->answerTx, 5);
  //     memcpy(&final_rx, &report->finalRx, 5);

  //     printf("%02x%08x ", (unsigned int)poll_tx.high8, (unsigned int)poll_tx.low32);
  //     printf("%02x%08x\r\n", (unsigned int)poll_rx.high8, (unsigned int)poll_rx.low32);
  //     printf("%02x%08x ", (unsigned int)answer_tx.high8, (unsigned int)answer_tx.low32);
  //     printf("%02x%08x\r\n", (unsigned int)answer_rx.high8, (unsigned int)answer_rx.low32);
  //     printf("%02x%08x ", (unsigned int)final_tx.high8, (unsigned int)final_tx.low32);
  //     printf("%02x%08x\r\n", (unsigned int)final_rx.high8, (unsigned int)final_rx.low32);

  //     tround1 = answer_rx.low32 - poll_tx.low32;
  //     treply1 = answer_tx.low32 - poll_rx.low32;
  //     tround2 = final_rx.low32 - answer_tx.low32;
  //     treply2 = final_tx.low32 - answer_rx.low32;

  //     printf("%08x %08x\r\n", (unsigned int)tround1, (unsigned int)treply2);
  //     printf("\\    /   /     \\\r\n");
  //     printf("%08x %08x\r\n", (unsigned int)treply1, (unsigned int)tround2);

  //     tprop_ctn = ((tround1*tround2) - (treply1*treply2)) / (tround1 + tround2 + treply1 + treply2);

  //     printf("TProp (ctn): %d\r\n", (unsigned int)tprop_ctn);

  //     tprop = tprop_ctn/tsfreq;
  //     distance = C * tprop;

  //     printf("distance %d: %5dmm\r\n", rxPacket.sourceAddress[0], (unsigned int)(distance*1000));

  //     dwGetReceiveTimestamp(dev, &arival);
  //     arival.full -= (ANTENNA_DELAY/2);
  //     printf("Total in-air time (ctn): 0x%08x\r\n", (unsigned int)(arival.low32-poll_tx.low32));

  //     break;
  //   }
  // }
}

void initiateRanging(dwDevice_t *dev)
{
  // printf ("Interrogating anchor %d\r\n",  config.anchors[curr_anchor]);
  // base_address[0] =  config.anchors[curr_anchor];
  // curr_anchor ++;
  // if (curr_anchor > config.anchorListSize) {
  //   curr_anchor = 0;
  // }
  // dwIdle(dev);

  // txPacket.payload[TYPE] = POLL;
  // txPacket.payload[SEQ] = ++curr_seq;

  // memcpy(txPacket.sourceAddress, config.address, 8);
  // memcpy(txPacket.destAddress, base_address, 8);

  // dwNewTransmit(dev);
  // dwSetDefaults(dev);
  // dwSetData(dev, (uint8_t*)&txPacket, MAC802154_HEADER_LENGTH+2);

  // dwWaitForResponse(dev, true);
  // dwStartTransmit(dev);
}

static void SendServicePacket(dwDevice_t *dev, uwbServiceFromSerial_t *dataToSend)
{
  uint8_t size;
  txPacket.destAddress[0] = dataToSend->destinationAddress;
  txPacket.payload[1] = dataToSend->action;
  switch(dataToSend->action) {
    case LPP_SHORT_ANCHOR_POSITION:
      size = 3*sizeof(float);
      memcpy(&txPacket.payload[2], dataToSend->position, size);
      break;
    case LPP_SHORT_REBOOT:
      size = 0;
      break;
    default:
      size = 0;
      break;

  dwNewTransmit(dev);  // Перевод UWB в режим TX
  dwSetDefaults(dev);
  dwSetData(dev, (uint8_t*)&txPacket, MAC802154_HEADER_LENGTH+2+size);
  // dwWaitForResponse(dev, true);  // Режим ожидания ответа после передачи
  dwStartTransmit(dev);  // Начать передачу
  }
}

static uint32_t ConfiguratorOnEvent(dwDevice_t *dev, uwbEvent_t event)
{
  switch(event) {
    case eventPacketReceived:
      // rxcallback(dev);
      // // 10ms between rangings
      // return 10;
      break;
    case eventPacketSent:
      // txcallback(dev);
      // return 10;
      break;
    case eventTimeout:
      // initiateRanging(dev);
      // return 10;
      break;
    case eventReceiveFailed:
      // Try again ranging in 10ms
      // return 10;
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
