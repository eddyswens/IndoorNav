#include <stdio.h>

#include "stm32f1xx_hal.h"

#include "lpp.h"

#include "cfg.h"
#include "uwb.h"

#include "bootmode.h"

#define debug(...) // printf(__VA_ARGS__)

void lppHandleShortPacket(char *data, size_t length)
{
  if (length < 1) return;

  int type  = data[0];

  debug("Handling LPP short packet of type %02x, length %d\r\n", type, length);
  debug("Raw data: ");
  for (int i=0; i<length; i++) {
    debug("%02x ", data[i]);
  }
  debug("\r\n");

  switch(type) {
    case LPP_SHORT_ANCHOR_POSITION:
    {
      struct lppShortAnchorPosition_s* newpos = (struct lppShortAnchorPosition_s*)&data[1];

      if (length != 3*sizeof(float) + 1) {
        debug("LPP: Wrong set-anchor position length\r\n");
        break;
      }

      cfgWriteFP32list(cfgAnchorPos, newpos->position, 3);
      uwbConfig_t *uwbConfig = uwbGetConfig();
      uwbConfig->position[0] = newpos->position[0];
      uwbConfig->position[1] = newpos->position[1];
      uwbConfig->position[2] = newpos->position[2];
      uwbConfig->positionEnabled = true;

      debug("Setting new anchor position to %f, %f, %f\r\n", newpos->position[0],
                                                             newpos->position[1],
                                                             newpos->position[2]);
      break;
    }
    case LPP_SHORT_REBOOT:
    {
      struct lppShortReboot_s* rebootInfo = (struct lppShortReboot_s*)&data[1];

      // Set boot flags
      if (rebootInfo->bootMode == LPP_SHORT_REBOOT_TO_BOOTLOADER) {
        bootmodeSetBootloaderModeFlag();
      } else if (rebootInfo->bootMode == LPP_SHORT_REBOOT_TO_FIRMWARE) {
        bootmodeClearBootloaderModeFlag();
      }

      // Then resets!
      NVIC_SystemReset();

      break;
    }
    case LPP_SHORT_MODE:
    {
      struct lppShortMode_s* modeInfo = (struct lppShortMode_s*)&data[1];

      // Set new mode
      if (modeInfo->mode == LPP_SHORT_MODE_TWR) {
        cfgWriteU8(cfgMode, MODE_ANCHOR);
      } else if (modeInfo->mode == LPP_SHORT_MODE_TDOA2) {
        cfgWriteU8(cfgMode, MODE_TDOA_ANCHOR2);
      } else if (modeInfo->mode == LPP_SHORT_MODE_TDOA3) {
        cfgWriteU8(cfgMode, MODE_TDOA_ANCHOR3);
      }

      // Then resets!
      NVIC_SystemReset();

      break;
    }
    case LPP_SHORT_UWB:
    {
      struct lppShortUWB_s *txPower = (struct lppShortUWB_s*)&data[1];

      // Set new power settings
      debug("SmartPower: %d, Force txPower: %d", txPower->enableSmartPower, txPower->forceTxPower);
      debug("Seting power to 0x%04X\r\n", (unsigned int)txPower->txPower);

      cfgWriteU8(cfgSmartPower, txPower->enableSmartPower);
      cfgWriteU8(cfgForceTxPower, txPower->forceTxPower);
      cfgWriteU32(cfgTxPower, txPower->txPower);

      // Then resets!
      NVIC_SystemReset();

      break;
    }
    case LPP_SHORT_UWB_MODE:
    {
      struct lppShortUWBMode_s *mode = (struct lppShortUWBMode_s*)&data[1];

      // Set new power settings
      debug("Low bitrate: %d, Long preamble: %d", mode->enableLowBitrate, mode->enableLongPreamble);

      cfgWriteU8(cfgLowBitrate, mode->enableLowBitrate);
      cfgWriteU8(cfgLongPreamble, mode->enableLongPreamble);

      // Then resets!
      NVIC_SystemReset();

      break;
    }
  }
}
