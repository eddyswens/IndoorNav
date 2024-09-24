#include <stm32f1xx_hal.h>

#include "uwb.h"

#include "libdw1000.h"
#include "dwOps.h"

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

// Implemented UWB algoritm. The dummy one is at the end of this file.
static uwbAlgorithm_t dummyAlgorithm;
extern uwbAlgorithm_t uwbSnifferAlgorithm;
extern uwbAlgorithm_t uwbTdoa2Algorithm;
extern uwbAlgorithm_t uwbTdoa3Algorithm;
extern uwbAlgorithm_t uwbConfiguratorAlgorithm;
static uwbAlgorithm_t *algorithm = &dummyAlgorithm;

struct {
  uwbAlgorithm_t *algorithm;
  char *name;
} availableAlgorithms[] = {
  {.algorithm = &uwbSnifferAlgorithm,   .name = "Sniffer"},
  {.algorithm = &uwbTdoa2Algorithm,     .name = "TDoA Anchor v2"},
  {.algorithm = &uwbTdoa3Algorithm,     .name = "TDoA Anchor v3"},
  {.algorithm = &uwbConfiguratorAlgorithm,     .name = "USB Configurator"},
  {NULL, NULL},
};

// Low level radio handling context (to be separated)
static bool isInit = false;
static int uwbErrorCode = 0;
static SemaphoreHandle_t irqSemaphore;
static dwDevice_t dwm_device;
static dwDevice_t *dwm = &dwm_device;

// System configuration
static struct uwbConfig_s config = {
  address: {0,0,0,0,0,0,0xec,0xdc},
};

static uint32_t timeout;

static void txcallback(dwDevice_t *dev)
{
  timeout = algorithm->onEvent(dev, eventPacketSent);
}

static void rxcallback(dwDevice_t *dev)
{
  timeout = algorithm->onEvent(dev, eventPacketReceived);
}

static void rxTimeoutCallback(dwDevice_t * dev) {
  timeout = algorithm->onEvent(dev, eventReceiveTimeout);
}

static void rxfailedcallback(dwDevice_t *dev) {
  timeout = algorithm->onEvent(dev, eventReceiveFailed);
}

void sendServiceData(uwbServiceFromSerial_t *newPacket)
{
  algorithm->sendService(dwm, newPacket);
}

void uwbInit()
{
  // Initializing the low level radio handling
  static StaticSemaphore_t irqSemaphoreBuffer;
  irqSemaphore = xSemaphoreCreateBinaryStatic(&irqSemaphoreBuffer);

  dwInit(dwm, &dwOps);       // Init libdw
  dwOpsInit(dwm);
  uwbErrorCode = dwConfigure(dwm); // Configure the dw1000 chip
  if (uwbErrorCode == 0) {
    dwEnableAllLeds(dwm);
  } else {
    return;
  }
  dwTime_t delay = {.full = 0};
  dwSetAntenaDelay(dwm, delay);

  // Reading and setting node configuration
  cfgReadU8(cfgAddress, &config.address[0]);
  cfgReadU8(cfgMode, &config.mode);
  cfgFieldSize(cfgAnchorlist, &config.anchorListSize);
  if (config.anchorListSize <= MAX_ANCHORS) {
    cfgReadU8list(cfgAnchorlist, config.anchors, config.anchorListSize);
  }

  if (config.mode < uwbAlgorithmCount()) {
    algorithm = availableAlgorithms[config.mode].algorithm;
  } else {
    algorithm = &dummyAlgorithm;
  }

  config.positionEnabled = cfgReadFP32list(cfgAnchorPos, config.position, 3);

  dwAttachSentHandler(dwm, txcallback);
  dwAttachReceivedHandler(dwm, rxcallback);
  dwAttachReceiveTimeoutHandler(dwm, rxTimeoutCallback);
  dwAttachReceiveFailedHandler(dwm, rxfailedcallback);

  dwNewConfiguration(dwm);
  dwSetDefaults(dwm);

  uint8_t useLowBitrate = 0;
  cfgReadU8(cfgLowBitrate, &useLowBitrate);
  #ifdef LPS_LONGER_RANGE
  useLowBitrate = 1;
  #endif
  config.lowBitrate = (useLowBitrate == 1);

  uint8_t useLongPreamble = 0;
  cfgReadU8(cfgLongPreamble, &useLongPreamble);
  config.longPreamble = (useLongPreamble == 1);

  const uint8_t* mode = MODE_SHORTDATA_FAST_ACCURACY;
  if (useLowBitrate && !useLongPreamble) {
    mode = MODE_SHORTDATA_MID_ACCURACY;
  } else if (!useLowBitrate && useLongPreamble) {
    mode = MODE_LONGDATA_FAST_ACCURACY;
  } else if (useLowBitrate && useLongPreamble) {
    mode = MODE_LONGDATA_MID_ACCURACY;
  }
  
  dwEnableMode(dwm, mode);

  dwSetChannel(dwm, CHANNEL_2);

  // Enable smart power by default
  uint8_t enableSmartPower = 1;
  cfgReadU8(cfgSmartPower, &enableSmartPower);
  config.smartPower = enableSmartPower != 0;
  if (enableSmartPower) {
    dwUseSmartPower(dwm, true);
  }

  // Do not force power by default
  uint8_t forceTxPower = 0;
  cfgReadU8(cfgForceTxPower, &forceTxPower);
  config.forceTxPower = forceTxPower != 0;
  if (forceTxPower) {
    uint32_t txPower = 0x1F1F1F1Ful;
    cfgReadU32(cfgTxPower, &txPower);
    config.txPower = txPower;
    dwSetTxPower(dwm, txPower);
  }

  dwSetPreambleCode(dwm, PREAMBLE_CODE_64MHZ_9);

  dwCommitConfiguration(dwm);

  isInit = true;
}

bool uwbTest()
{
  return isInit;
}

int uwbAlgorithmCount()
{
  int count = 0;

  while (availableAlgorithms[count].algorithm != NULL) {
    count ++;
  }
  return count;
}

char * uwbAlgorithmName(unsigned int id)
{
  if (id < uwbAlgorithmCount()) {
    return availableAlgorithms[id].name;
  } else {
    return "UKNOWN";
  }
}

static int checkIrq()
{
  return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
}

static void uwbTask(void* parameters)
{
  configASSERT(isInit);

  algorithm->init(&config, dwm);

  while(1) {
    BaseType_t res = xSemaphoreTake(irqSemaphore, timeout / portTICK_PERIOD_MS);
    if (res)
    {
      do{
          dwHandleInterrupt(dwm);
      } while(checkIrq() != 0);
    }
    else
    {
      timeout = algorithm->onEvent(dwm, eventTimeout);
    }
  }
}

void uwbStart()
{
  static StaticTask_t uwbStaticTask;
  static StackType_t uwbStaticStack[2*configMINIMAL_STACK_SIZE];

  if (isInit) {
    xTaskCreateStatic(uwbTask, "uwb", 2*configMINIMAL_STACK_SIZE, NULL,
                      configMAX_PRIORITIES - 1, uwbStaticStack, &uwbStaticTask);
  }
}

char * uwbStrError()
{
  return dwStrError(uwbErrorCode);
}

struct uwbConfig_s * uwbGetConfig()
{
  return &config;
}

uint8_t uwbGetCurrentMode()
{
  return config.mode;
}

/**** DWM1000 interrupt handling *****/
#define DWM_IRQn EXTI9_5_IRQn
#define DWM_IRQ_PIN GPIO_PIN_9

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  BaseType_t higherPriorityTaskWoken = pdFALSE;

  switch (GPIO_Pin) {
    case DWM_IRQ_PIN:
      xSemaphoreGiveFromISR(irqSemaphore, &higherPriorityTaskWoken);

      HAL_NVIC_ClearPendingIRQ(DWM_IRQn);
      break;
    default:
      break;
  }
  portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

/* Dummy algorithm (used if UKNOWN algorithm is selected ...)*/
static void dummyInit(uwbConfig_t * config, dwDevice_t *dev)
{
  ;
}

static uint32_t dummyOnEvent(dwDevice_t *dev, uwbEvent_t event)
{
  return MAX_TIMEOUT;
}

static uwbAlgorithm_t dummyAlgorithm = {
  .init = dummyInit,
  .onEvent = dummyOnEvent,
};
