#ifndef __CFG_H__
#define __CFG_H__

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  cfgAddress = 0,
  cfgMode,
  cfgAnchorlist,
  cfgAnchorPos,
  cfgSmartPower,
  cfgForceTxPower,
  cfgTxPower,
  cfgLowBitrate,
  cfgLongPreamble,
} ConfigField;

void cfgInit();

bool cfgReset(void);

bool cfgFieldSize(ConfigField field, uint8_t * size);

bool cfgReadU8(ConfigField field, uint8_t * value);

bool cfgWriteU8(ConfigField field, uint8_t data);

bool cfgReadU32(ConfigField field, uint32_t * value);

bool cfgWriteU32(ConfigField field, uint32_t value);

bool cfgReadU8list(ConfigField field, uint8_t list[], uint8_t lenth);

bool cfgWriteU8list(ConfigField field, uint8_t list[], uint8_t lenth);

bool cfgReadFP32listLength(ConfigField field, uint8_t * size);

bool cfgReadFP32list(ConfigField field, float list[], uint8_t length);

bool cfgWriteFP32list(ConfigField field, float list[], uint8_t length);

#endif
