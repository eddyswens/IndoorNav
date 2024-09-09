#include "uwb.h"
#include "spi.h"
#include "stm32f1xx.h"

extern SPI_HandleTypeDef hspi1;

static bool isInit = false;
static dwDevice_t dwm_device;
static dwDevice_t *dwm = &dwm_device;

uint16_t alignedBuffer[64];

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

/*********** Deck driver initialization ***************/

static void dwm1000Init()
{
    NVIC_EnableIRQ(DWM_IRQn);

    MX_SPI1_Init();
    // Init pins
    pinMode(CS_PIN, OUTPUT);
    pinMode(GPIO_PIN_RESET, OUTPUT);
    pinMode(GPIO_PIN_IRQ, INPUT);

    // Reset the DW1000 chip
    digitalWrite(GPIO_PIN_RESET, 0);
    HAL_Delay(10);
    digitalWrite(GPIO_PIN_RESET, 1);
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

    // memoryRegisterHandler(&memDef);

    // algoSemaphore = xSemaphoreCreateMutex();

    // xTaskCreate(uwbTask, LPS_DECK_TASK_NAME, LPS_DECK_STACKSIZE, NULL,
    //             LPS_DECK_TASK_PRI, &uwbTaskHandle);

    isInit = true;
}