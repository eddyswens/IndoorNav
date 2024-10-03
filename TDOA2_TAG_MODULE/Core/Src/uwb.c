#include "uwb.h"
#include "spi.h"
#include "gpio.h"
#include "stm32f1xx.h"

#include "lpsTdoa2Tag.h"

// #define debug(...) // printf(__VA_ARGS__)
#define CONFIG_ALGORITHM_TDOA2

uwbAlgorithm_t uwbTwrTagAlgorithm;  // DEBUG
// uwbAlgorithm_t uwbTdoa2TagAlgorithm;  // DEBUG
uwbAlgorithm_t uwbTdoa3TagAlgorithm;  // DEBUG

extern SPI_HandleTypeDef hspi1;

static lpsAlgoOptions_t algoOptions = {
  // .userRequestedMode is the wanted algorithm, available as a parameter
#if defined(CONFIG_ALGORITHM_TDOA2)
  .userRequestedMode = lpsMode_TDoA2,
#elif defined(CONFIG_ALGORITHM_TDOA3)
  .userRequestedMode = lpsMode_TDoA3,
#elif defined(CONFIG_ALGORITHM_TWR)
  .userRequestedMode = lpsMode_TWR,
#else
  .userRequestedMode = lpsMode_auto,
#endif
  // .currentRangingMode is the currently running algorithm, available as a log
  // lpsMode_auto is an impossible mode which forces initialization of the requested mode
  // at startup
  .currentRangingMode = lpsMode_auto,
  .modeAutoSearchActive = true,
  .modeAutoSearchDoInitialize = true,
};

struct {
  uwbAlgorithm_t *algorithm;
  char *name;
} algorithmsList[LPS_NUMBER_OF_ALGORITHMS + 1] = {
  [lpsMode_TWR] = {.algorithm = &uwbTwrTagAlgorithm, .name="TWR"},
  [lpsMode_TDoA2] = {.algorithm = &uwbTdoa2TagAlgorithm, .name="TDoA2"},
  [lpsMode_TDoA3] = {.algorithm = &uwbTdoa3TagAlgorithm, .name="TDoA3"},
};

static uwbAlgorithm_t *algorithm = &uwbTdoa2TagAlgorithm;  // DEBUG
// #if defined(CONFIG_ALGORITHM_TDOA2)
// static uwbAlgorithm_t *algorithm = &uwbTdoa2TagAlgorithm;
// #elif defined(CONFIG_ALGORITHM_TDOA3)
// static uwbAlgorithm_t *algorithm = &uwbTdoa3TagAlgorithm;
// #else
// static uwbAlgorithm_t *algorithm = &uwbTwrTagAlgorithm;
// #endif

static bool isInit = false;
static dwDevice_t dwm_device;
static dwDevice_t *dwm = &dwm_device;
static bool eventsToHandle = false;

static uint32_t timeout;

uint16_t alignedBuffer[64];

static void txCallback(dwDevice_t *dev)
{
    timeout = algorithm->onEvent(dev, eventPacketSent);
}

static void rxCallback(dwDevice_t *dev)
{
    timeout = algorithm->onEvent(dev, eventPacketReceived);
}

static void rxTimeoutCallback(dwDevice_t *dev)
{
    timeout = algorithm->onEvent(dev, eventReceiveTimeout);
}

static void rxFailedCallback(dwDevice_t *dev)
{
    timeout = algorithm->onEvent(dev, eventReceiveFailed);
}

bool locoDeckGetAnchorPosition(const uint8_t anchorId, point_t* position)  // NOT USED
{
  if (!isInit) {
    return false;
  }
  bool result = algorithm->getAnchorPosition(anchorId, position);
  return result;
}

uint8_t locoDeckGetAnchorIdList(uint8_t unorderedAnchorList[], const int maxListSize) // NOT USED
{
  if (!isInit) {
    return 0;
  }
  uint8_t result = algorithm->getAnchorIdList(unorderedAnchorList, maxListSize);
  return result;
}

uint8_t locoDeckGetActiveAnchorIdList(uint8_t unorderedAnchorList[], const int maxListSize) // NOT USED
{
  if (!isInit) {
    return 0;
  }
  uint8_t result = algorithm->getActiveAnchorIdList(unorderedAnchorList, maxListSize);
  return result;
}

static bool switchToMode(const lpsMode_t newMode) {
  bool result = false;

  if (lpsMode_auto != newMode && newMode <= LPS_NUMBER_OF_ALGORITHMS) {
    algoOptions.currentRangingMode = newMode;
    algorithm = algorithmsList[algoOptions.currentRangingMode].algorithm;

    algorithm->init(dwm);
    timeout = algorithm->onEvent(dwm, eventTimeout);

    result = true;
  }

  return result;
}

static void autoModeSearchTryMode(const lpsMode_t newMode, const uint32_t now) {
  // Set up next time to check
  algoOptions.nextSwitchTick = now + LPS_AUTO_MODE_SWITCH_PERIOD;
  switchToMode(newMode);
}

static lpsMode_t autoModeSearchGetNextMode() {
  lpsMode_t newMode = algoOptions.currentRangingMode + 1;
  if (newMode > LPS_NUMBER_OF_ALGORITHMS) {
    newMode = lpsMode_TWR;
  }
  return newMode;
}

static void processAutoModeSwitching() {
  uint32_t now = HAL_GetTick();

  if (algoOptions.modeAutoSearchActive) {
    if (algoOptions.modeAutoSearchDoInitialize) {
      autoModeSearchTryMode(lpsMode_TDoA2, now);
      algoOptions.modeAutoSearchDoInitialize = false;
    } else {
      if (now > algoOptions.nextSwitchTick) {
        if (algorithm->isRangingOk()) {
          // We have found an algorithm, stop searching and lock to it.
          algoOptions.modeAutoSearchActive = false;
        //   DEBUG_PRINT("Automatic mode: detected %s\n", algorithmsList[algoOptions.currentRangingMode].name);
        } else {
          lpsMode_t newMode = autoModeSearchGetNextMode();
          autoModeSearchTryMode(newMode, now);
        }
      }
    }
  }
}

static void resetAutoSearchMode() {
  algoOptions.modeAutoSearchActive = true;
  algoOptions.modeAutoSearchDoInitialize = true;
}

static void handleModeSwitch() {
  if (algoOptions.userRequestedMode == lpsMode_auto) {
    processAutoModeSwitching();
  } else {
    resetAutoSearchMode();
    if (algoOptions.userRequestedMode != algoOptions.currentRangingMode) {
      if (switchToMode(algoOptions.userRequestedMode)) {
        // DEBUG_PRINT("Switching to mode %s\n", algorithmsList[algoOptions.currentRangingMode].name);
      }
    }
  }
}

static void uwbTask()
{
  handleModeSwitch();
  HAL_Delay(1);

  if (eventsToHandle) {
      do{
          dwHandleInterrupt(dwm);
      } while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_IRQ) != 0);
      eventsToHandle = false;
  } else {
      timeout = algorithm->onEvent(dwm, eventTimeout);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_IRQ) eventsToHandle = true;
}

/************ Low level ops for libdw **********/
static void spiWrite(dwDevice_t *dev, const void *header, size_t headerLength,
                     const void *data, size_t dataLength)
{
#ifdef DEBUG_SPI
    int i;
    printf("Write to SPI: [ ");
    for (i = 0; i < headerLength; i++)
        printf("%02x ", (unsigned int)((uint8_t *)header)[i]);
    printf("] [ ");

    for (i = 0; i < dataLength; i++)
        printf("%02x ", (unsigned int)((uint8_t *)data)[i]);
    printf("]\r\n");
#endif

    HAL_GPIO_WritePin(GPIOA, CS_PIN, 0);

    memcpy(alignedBuffer, header, headerLength);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)alignedBuffer, headerLength, HAL_MAX_DELAY);
    memcpy(alignedBuffer, data, dataLength);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)alignedBuffer, dataLength, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(GPIOA, CS_PIN, 1);
}

static void spiRead(dwDevice_t *dev, const void *header, size_t headerLength,
                    void *data, size_t dataLength)
{
    // volatile int dummy;

#ifdef DEBUG_SPI
    int i;
    printf("Read from SPI: [ ");
    for (i = 0; i < headerLength; i++)
        printf("%02x ", (unsigned int)((uint8_t *)header)[i]);
    printf("] ");
#endif

    HAL_GPIO_WritePin(GPIOA, CS_PIN, 0);

    memcpy(alignedBuffer, header, headerLength);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)alignedBuffer, headerLength, HAL_MAX_DELAY);
    // printf("%lu", hspi1.Instance->SR & SPI_FLAG_RXNE);
    // while (hspi1.Instance->SR & SPI_FLAG_RXNE) {
    //   printf("R");
    //   dummy = hspi1.Instance->DR;
    // }
    // dummy;
    // printf("%lu ", hspi1.Instance->SR & SPI_FLAG_RXNE);
    HAL_SPI_Receive(&hspi1, (uint8_t *)alignedBuffer, dataLength, HAL_MAX_DELAY);
    memcpy(data, alignedBuffer, dataLength);

    HAL_GPIO_WritePin(GPIOA, CS_PIN, 1);

#ifdef DEBUG_SPI
    printf("{ ");
    for (i = 0; i < dataLength; i++)
        printf("%02x ", (unsigned int)((uint8_t *)data)[i]);
    printf("}\r\n");
#endif
}

static void spiSetSpeed(dwDevice_t *dev, dwSpiSpeed_t speed)
{
    if (speed == dwSpiSpeedLow)
    {
        MX_SPI1_Init();
    }
    else if (speed == dwSpiSpeedHigh)
    {
        MX_SPI1_Init_Fast();
    }
}


static void delayms(dwDevice_t *dev, unsigned int delay)
{
    HAL_Delay(delay);
}

static dwOps_t dwOps = {
    .spiRead = spiRead,
    .spiWrite = spiWrite,
    .spiSetSpeed = spiSetSpeed,
    .delayms = delayms,
};

/*********** DWM driver initialization ***************/

static void dwm1000Init()
{
    printf("Start initing\r\n");
    NVIC_EnableIRQ(DWM_IRQn);

    MX_SPI1_Init();
    MX_GPIO_Init();

    // Reset the DW1000 chip
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_RST, SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_RST, RESET);
    HAL_Delay(10);

    // Initialize the driver
    dwInit(dwm, &dwOps); // Init libdw

    int result = dwConfigure(dwm);
    if (result != 0)
    {
        isInit = false;
        // DEBUG_PRINT("Failed to configure DW1000!\r\n");
        return;
    }

    dwEnableAllLeds(dwm);

    dwTime_t delay = {.full = 0};
    dwSetAntenaDelay(dwm, delay);

    dwAttachSentHandler(dwm, txCallback);
    dwAttachReceivedHandler(dwm, rxCallback);
    dwAttachReceiveTimeoutHandler(dwm, rxTimeoutCallback);
    dwAttachReceiveFailedHandler(dwm, rxFailedCallback);

    dwNewConfiguration(dwm);
    dwSetDefaults(dwm);

#ifdef CONFIG_LONGER_RANGE
    dwEnableMode(dwm, MODE_SHORTDATA_MID_ACCURACY);
#else
    dwEnableMode(dwm, MODE_SHORTDATA_FAST_ACCURACY);
#endif

    dwSetChannel(dwm, CHANNEL_2);
    dwSetPreambleCode(dwm, PREAMBLE_CODE_64MHZ_9);

#ifdef CONFIG_FULL_TX_POWER
    dwUseSmartPower(dwm, false);
    dwSetTxPower(dwm, 0x1F1F1F1Ful);
#else
    dwUseSmartPower(dwm, true);
#endif

    dwSetReceiveWaitTimeout(dwm, DEFAULT_RX_TIMEOUT);

    dwCommitConfiguration(dwm);
    
    isInit = true;
}

uint16_t locoDeckGetRangingState()
{
  return algoOptions.rangingState;
}

void locoDeckSetRangingState(const uint16_t newState)
{
  algoOptions.rangingState = newState;
}

void dwStart()
{
    dwm1000Init();
}

void dwLoop()
{
   uwbTask(); 
}