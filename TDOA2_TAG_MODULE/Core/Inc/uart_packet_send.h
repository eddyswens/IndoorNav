#ifndef __UART_PACKET_SEND_H__
#define __UART_PACKET_SEND_H__

#include "tdoaStorage.h"
#include "stdint.h"
#include "tdoaEngine.h"
#include "usart.h"

// Pragma, для того, чтобы отключить выравнивание типов данных
#pragma pack(1)

typedef struct {
  uint8_t refs;
  uint8_t size; // размер полезных данных структуры
  uint8_t addr;
  uint8_t num;
} Event;

typedef struct { // 27+4 bytes
  Event e;
  uint32_t orientation; // packed Euler angles: bits 0..10 - roll; 11..20 - pitch; 21..31 - yaw
  int32_t pos[3];
  int16_t vel[3];
  uint16_t voltage;
  uint8_t beacons;
  uint8_t status;
  uint8_t posError;

} TelemetryEvent;

typedef struct {
	Event e;
	uint16_t amp[4];
} BeaconAmplitude;

#pragma pack()

//структура-заглушка, из которой берутся данные для пакета, потом они будут браться из нормальной структуры позиции
typedef struct 
{
 uint32_t x;
 uint32_t y;
 uint32_t z;

 uint32_t orientation;

}TagPosition;

// Структура типов пакетов
enum
{
  TELEMETRY_EVENT = 0x02,
  BEACON_AMPLITUDE_EVENT = 0x33,
  OTHER_EVENT = 0x03,
};

void packetHandler(uint8_t eventType, void* EventData);
uint8_t* createPacket(uint8_t eventType, void* EventData, size_t* packetSize);
void sendTelemetryPacket(uint8_t* packet, size_t packetsize);
uint8_t calculateEventCRC(const Event* event);
uint8_t fastcrc8_update(uint8_t aChecksum, const void *aBuffer, size_t aLength);

#endif // __UART_PACKET_SEND_H__