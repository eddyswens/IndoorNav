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
 * Sends data as the packet via UART (for LocusTester).
 */


#include "uart_packet_send.h"


// Вычисление чек-суммы
uint8_t fastcrc8_update(uint8_t aChecksum, const void *aBuffer, size_t aLength) {
  static const uint8_t table[] = {
    0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83,
    0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
    0x9D, 0xC3, 0x21, 0x7F, 0xFC, 0xA2, 0x40, 0x1E,
    0x5F, 0x01, 0xE3, 0xBD, 0x3E, 0x60, 0x82, 0xDC,
    0x23, 0x7D, 0x9F, 0xC1, 0x42, 0x1C, 0xFE, 0xA0,
    0xE1, 0xBF, 0x5D, 0x03, 0x80, 0xDE, 0x3C, 0x62,
    0xBE, 0xE0, 0x02, 0x5C, 0xDF, 0x81, 0x63, 0x3D,
    0x7C, 0x22, 0xC0, 0x9E, 0x1D, 0x43, 0xA1, 0xFF,
    0x46, 0x18, 0xFA, 0xA4, 0x27, 0x79, 0x9B, 0xC5,
    0x84, 0xDA, 0x38, 0x66, 0xE5, 0xBB, 0x59, 0x07,
    0xDB, 0x85, 0x67, 0x39, 0xBA, 0xE4, 0x06, 0x58,
    0x19, 0x47, 0xA5, 0xFB, 0x78, 0x26, 0xC4, 0x9A,
    0x65, 0x3B, 0xD9, 0x87, 0x04, 0x5A, 0xB8, 0xE6,
    0xA7, 0xF9, 0x1B, 0x45, 0xC6, 0x98, 0x7A, 0x24,
    0xF8, 0xA6, 0x44, 0x1A, 0x99, 0xC7, 0x25, 0x7B,
    0x3A, 0x64, 0x86, 0xD8, 0x5B, 0x05, 0xE7, 0xB9,
    0x8C, 0xD2, 0x30, 0x6E, 0xED, 0xB3, 0x51, 0x0F,
    0x4E, 0x10, 0xF2, 0xAC, 0x2F, 0x71, 0x93, 0xCD,
    0x11, 0x4F, 0xAD, 0xF3, 0x70, 0x2E, 0xCC, 0x92,
    0xD3, 0x8D, 0x6F, 0x31, 0xB2, 0xEC, 0x0E, 0x50,
    0xAF, 0xF1, 0x13, 0x4D, 0xCE, 0x90, 0x72, 0x2C,
    0x6D, 0x33, 0xD1, 0x8F, 0x0C, 0x52, 0xB0, 0xEE,
    0x32, 0x6C, 0x8E, 0xD0, 0x53, 0x0D, 0xEF, 0xB1,
    0xF0, 0xAE, 0x4C, 0x12, 0x91, 0xCF, 0x2D, 0x73,
    0xCA, 0x94, 0x76, 0x28, 0xAB, 0xF5, 0x17, 0x49,
    0x08, 0x56, 0xB4, 0xEA, 0x69, 0x37, 0xD5, 0x8B,
    0x57, 0x09, 0xEB, 0xB5, 0x36, 0x68, 0x8A, 0xD4,
    0x95, 0xCB, 0x29, 0x77, 0xF4, 0xAA, 0x48, 0x16,
    0xE9, 0xB7, 0x55, 0x0B, 0x88, 0xD6, 0x34, 0x6A,
    0x2B, 0x75, 0x97, 0xC9, 0x4A, 0x14, 0xF6, 0xA8,
    0x74, 0x2A, 0xC8, 0x96, 0x15, 0x4B, 0xA9, 0xF7,
    0xB6, 0xE8, 0x0A, 0x54, 0xD7, 0x89, 0x6B, 0x35
  };

  const uint8_t *buffer = (const uint8_t *)aBuffer;

  while (aLength--) {
    aChecksum = table[aChecksum ^ *buffer++];
  } 

  return aChecksum;
}


uint8_t calculateEventCRC(const Event* event) {
  return fastcrc8_update(0, &event->size, event->size + 3); 
}

//Функция обработки вызова пакета. На приёме переменная, определяющая тип пакета, и структура с которой мы будем брать даные
void packetHandler(uint8_t eventType, void* EventData) {
    size_t packetSize = 0;
    uint8_t* packet = createPacket(eventType, EventData, &packetSize);

    if (packet != NULL) 
    {
        //После сборки пакета отправляем его, packetSize был обновлён через указатель в createPacket
        sendTelemetryPacket(packet, packetSize);
        free(packet);
    }
}
uint8_t* createPacket(uint8_t eventType, void* EventData, size_t* packetSize) {
    uint8_t* packet = NULL;
    *packetSize = 0; 
    //Проверка на тип пакета
    switch (eventType) {
        case TELEMETRY_EVENT: {
            //Подводим переданную структуру в её истинный формат
            TagPosition* event = (TagPosition*)EventData;

            // Выделяем память под пакет + чек-сумму
           *packetSize = sizeof(TelemetryEvent) + 1;  // Сохраняем размер
           //Выделяем память пакету подстать размеру структуры
            packet = (uint8_t*)malloc(*packetSize);
            if (packet == NULL) {
                return NULL; // Обработка ошибки выделения памяти
            }

            TelemetryEvent* packet_event = (TelemetryEvent*)packet; // Приводим к типу TelemetryEvent*
            // Заполнение полей
            packet_event->e.refs = 0xFE;
            packet_event->e.size = sizeof(TelemetryEvent) - sizeof(Event);
            packet_event->e.addr = 0x00;
            packet_event->e.num = eventType;

            packet_event->orientation = event->orientation;
            packet_event->pos[0] = event->x;
            packet_event->pos[1] = event->y;
            packet_event->pos[2] = event->z;
            packet_event->vel[0] = 0;          
            packet_event->vel[1] = 0;          
            packet_event->vel[2] = 0;          
            packet_event->voltage = 0;      
            packet_event->beacons = 0;
            packet_event->status = 0;
            packet_event->posError = 28;
            
            //Вычисляем чек-сумму
            uint8_t crc = calculateEventCRC(&packet_event->e);
            packet[sizeof(TelemetryEvent)] = crc; // Записываем чек-сумму в конец пакета

            break;
        }
        default: {
            break;
        }
    }
    // Пакет готов, возвращаем его
    return packet;
}

void sendTelemetryPacket(uint8_t* packet, size_t packetsize) {
    if (packet != NULL) { // Проверка на NULL
        HAL_UART_Transmit(&huart1, packet, packetsize, HAL_MAX_DELAY);
    }
}

// // Старая функция для создания статического пакета TelemetryEvent
// TelemetryEvent createStaticTelemetryPacket(uint8_t* packet) {
//   TelemetryEvent telemetry;

//   telemetry.e.refs = 0xFE;
//   telemetry.e.size = sizeof(TelemetryEvent) - sizeof(Event);
//   telemetry.e.addr = 0x00;

//   // Заполнение структуры TelemetryEvent статическими значениями
//   telemetry.orientation = 0xEB785817; // Пример: roll=88, pitch=23, yaw=235
//   telemetry.pos[0] = 5;          // X 
//   telemetry.pos[1] = 10;          // Y
//   telemetry.pos[2] = 7;          // Z
//   telemetry.vel[0] = 0;          // X velocity
//   telemetry.vel[1] = 0;          // Y velocity
//   telemetry.vel[2] = 0;          // Z velocity
//   telemetry.voltage = 0;      // 3.2V
//   telemetry.beacons = 0;
//   telemetry.status = 0;
//   telemetry.posError = 28;

//   uint8_t crc = calculateEventCRC(&telemetry.e);
  
  
//     telemetry.e.num = 0x02;
// //telemetry.checksum= crc;

//   // Копирование структуры в массив байт
//   memcpy(packet, &telemetry, sizeof(TelemetryEvent)); 
//   return telemetry;
// }
