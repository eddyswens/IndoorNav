#ifndef __LPP_H__
#define __LPP_H__

#include <stddef.h>
#include <stdint.h>

void lppHandleShortPacket(char *data, size_t length);

#define SHORT_LPP 0xF0

#define LPP_SHORT_ANCHOR_POSITION 0x01
#define LPP_SHORT_REBOOT 0x02
#define LPP_SHORT_MODE 0x03
#define LPP_SHORT_UWB 0x04
#define LPP_SHORT_UWB_MODE 0x05

struct lppShortAnchorPosition_s {
  float position[3];
} __attribute__((packed));

#define LPP_SHORT_REBOOT_TO_BOOTLOADER 0x00
#define LPP_SHORT_REBOOT_TO_FIRMWARE 0x01

struct lppShortReboot_s {
  uint8_t bootMode;
} __attribute__((packed));

#define LPP_SHORT_MODE_TWR 0x01
#define LPP_SHORT_MODE_TDOA2 0x02
#define LPP_SHORT_MODE_TDOA3 0x03

struct lppShortMode_s {
  uint8_t mode;
} __attribute__((packed));

struct lppShortUWB_s {
  uint8_t enableSmartPower :1;
  uint8_t forceTxPower :1;
  uint32_t txPower;
} __attribute__((packed));

struct lppShortUWBMode_s {
  uint8_t enableLowBitrate :1;
  uint8_t enableLongPreamble :1;
} __attribute__((packed));

#endif //__LPP_H__
