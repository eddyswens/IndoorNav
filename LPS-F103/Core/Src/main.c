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
 */

#include <stm32f1xx_hal.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "FreeRTOS.h" 
#include "task.h"

#include "system.h"
#include "spi.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "led.h"
#include "button.h"

#include "cfg.h"
#include "eeprom.h"
#include "ssd1306.h"
#include "fonts.h"

#include "usb_device.h"
#include "usbcomm.h"

#include "test_support.h"

#include "uwb.h"
#include "lpp.h"

#include <semphr.h>


#define POWER_LEVELS 10
#define USE_FTDI_UART

uint32_t uid[3];
static bool modeChanged = false, addressChanged = false;
static uint8_t currentUwbMode;
static uwbServiceFromSerial_t servPacket;
static uwbServiceFromSerial_t* newServicePacket = &servPacket;

StaticSemaphore_t xMutexI2CBuffer;
SemaphoreHandle_t xMutexI2C;


static void restConfig();
static void changeAddress(uint8_t addr);
static void handleSerialInput(char ch);
static void handleButton(void);
static void changeMode(unsigned int newMode);
static void changeRadioMode(unsigned int newMode);
static void changePower(uint8_t power);
static void printModeList();
static void printRadioModeList();
static void printMode();
static void printRadioMode();
static void printPowerHelp();
static void printConfiguratorOptions();
static void help();
// static void bootload(void);

typedef enum {mainMenu, modeMenu, idMenu, radioMenu, powerMenu, posMenu, configuratorMenu, remotePosMenu} Menu_t;
typedef struct {
  bool configChanged;
  Menu_t currentMenu;
  unsigned int tempId;
} MenuState;

static void main_task(void *pvParameters) {
  int i;
  char ch;
  bool selftestPasses = true;

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_USB_DEVICE_Init();

  // Light up all LEDs to test
  ledOn(ledRanging);
  ledOn(ledSync);
  ledOn(ledMode);
  
  buttonInit(buttonIdle);

  clearUart();

  printf("\r\n\r\n====================\r\n");

printf("SYSTEM\t: CPU-ID: ");
for (uint8_t i = 0; i < 3; ++i) {
  printf("%08X", uid[i]); 
}
  printf("\r\n");

// Init Mutex for I2C
 xMutexI2C = xSemaphoreCreateMutexStatic(&xMutexI2CBuffer);
 if(xMutexI2C == NULL) {
  printf("[ERROR]: Error while creating static I2C mutex");
 }

// Init LCD
  if (ssd1306_Init(&hi2c1) != 0) {
     printf("TEST\t: Problem with SSD1306!\r\n");
   }
  ssd1306_GeoscanLogo();
  ssd1306_UpdateScreen(&hi2c1);
  ssd1306_Fill(Black);

  // Init EEPROM
  eepromInit(&hi2c1);
  testSupportPrintStart("EEPROM self-test");
  testSupportReport(&selftestPasses, eepromTest());
  cfgInit();

  // Initialising radio
  testSupportPrintStart("Initialize UWB ");
  uwbInit();
  if (uwbTest()) {
    printf("[OK]\r\n");
  } else {
    printf("[ERROR]: %s\r\n", uwbStrError());
    selftestPasses = false;
  }

  if (!selftestPasses) {
    printf("TEST\t: One or more self-tests failed, blocking startup!\r\n");
    usbcommSetSystemStarted(true);
  }


  // Printing UWB configuration
  struct uwbConfig_s *uwbConfig = uwbGetConfig();

  printf("CONFIG\t: Address is 0x%X\r\n", uwbConfig->address[0]);
  printf("CONFIG\t: Mode is %s\r\n", uwbAlgorithmName(uwbConfig->mode));
  printf("CONFIG\t: Tag mode anchor list (%i): ", uwbConfig->anchorListSize);
  for (i = 0; i < uwbConfig->anchorListSize; i++) {
    printf("0x%02X ", uwbConfig->anchors[i]);
  }
  printf("\r\n");
  printf("CONFIG\t: Anchor position enabled: %s\r\n",
         uwbConfig->positionEnabled?"true":"false");
  if (uwbConfig->positionEnabled) {
    printf("CONFIG\t: Anchor position: %f %f %f\r\n", uwbConfig->position[0],
                                                      uwbConfig->position[1],
                                                      uwbConfig->position[2]);
  }
  printf("CONFIG\t: SmartPower enabled: %s\r\n", uwbConfig->smartPower?"True":"False");
  printf("CONFIG\t: Force TX power: %s\r\n", uwbConfig->forceTxPower?"True":"False");
  if(uwbConfig->forceTxPower) {
    printf("CONFIG\t: TX power setting: %08X\r\n", (unsigned int)uwbConfig->txPower);
  }
  printf("CONFIG\t: Bitrate: %s\r\n", uwbConfig->lowBitrate?"low":"normal");
  printf("CONFIG\t: Preamble: %s\r\n", uwbConfig->longPreamble?"long":"normal");

      HAL_Delay(500);

  ledOff(ledRanging);
  ledOff(ledSync);
  ledOff(ledMode);

  printf("SYSTEM\t: Node started ...\r\n");
  printf("SYSTEM\t: Press 'h' for help.\r\n");

  usbcommSetSystemStarted(true);

  // Starts UWB protocol
  uwbStart();

  // Main loop ...
  while(1) {
    usbcommPrintWelcomeMessage();
    if( xSemaphoreTake(xMutexI2C, portMAX_DELAY) == pdTRUE) {
      printConfigLCD();
      xSemaphoreGive(xMutexI2C);
    }
    ledTick();
    handleButton();

    // Accepts serial commands
#ifdef USE_FTDI_UART
        if (HAL_UART_Receive(&huart2, (uint8_t *)&ch, 1, 0) == HAL_OK)
    {
#else
        if (usbcommRead(&ch, 1)) {
#endif
      handleSerialInput(ch);
    }
  }
}

/* Function required to use "printf" to print on serial console */
int _write (int fd, const void *buf, size_t count)
{
  // stdout
  if (fd == 1) {
    #ifdef USE_FTDI_UART
      HAL_UART_Transmit(&huart2, (uint8_t *)buf, count, HAL_MAX_DELAY);
    #else
      usbcommWrite(buf, count);
    #endif
  }

  // stderr
  if (fd == 2) {
    HAL_UART_Transmit(&huart2, (uint8_t *)buf, count, HAL_MAX_DELAY);
  }

  return count;
}

void clearUart()
{
  printf("\033[2J");
}

/*------------------------------------------------
-----------------HANDLE FUNCTIONS-----------------
--------------------------------------------------*/

static void handleMenuMain(char ch, MenuState* menuState) {
  switch (ch) {                                       // 0-9: ADDRESS 0-9 FAST
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      changeAddress(ch - '0');
      break;
    case 'i':                                         // I: ADDRESS 0-255 
      printf("Type new node ID then enter: ");
      fflush(stdout);
      menuState->currentMenu = idMenu;
      menuState->configChanged = false;
      menuState->tempId = 0;
      break;
    case 'a': changeMode(MODE_TDOA_ANCHOR2); break;   // A: FAST ANCHOR
    case 's': changeMode(MODE_SNIFFER); break;        // S: FAST SNIFFER
    case 'm':                                         // M: MODE SELECT FROM LIST
      printModeList();
      printf("Type 0-9 to choose new mode...\r\n");
      menuState->currentMenu = modeMenu;
      menuState->configChanged = false;
      break;
    case 'c':                                         // C: FAST CONFIGURATOR
      currentUwbMode = uwbGetCurrentMode();
      if (currentUwbMode != MODE_CONFIGURATOR) {
        changeMode(MODE_CONFIGURATOR);
      }
      else {
        clearUart();
        printf("CONFIGURATOR MODE v0.1\r\n");
        printConfiguratorOptions();
        fflush(stdout);
        menuState->currentMenu = configuratorMenu;
        menuState->configChanged = false;
      }
      break;
    case 'r':                                         // R: RADIO SELECT FROM LIST
      printRadioModeList();
      printf("Type 0-9 to choose new mode...\r\n");
      menuState->currentMenu = radioMenu;
      menuState->configChanged = false;
      break;
    case 'd': restConfig(); break;                    // D: RESET CONFIG TO DEFAULTS
    case 'h':                                         // H: PRINT HELP
      help();
      menuState->configChanged = false;
      break;
    case 'b':                                         // B: BINARY MODE FOR SNIFFER
      cfgSetBinaryMode(true);
      menuState->configChanged = false;
      break;
    case 'p':                                         // P: POWER SETTINGS CHANGE
         printPowerHelp();
         menuState->currentMenu = powerMenu;
         menuState->configChanged = false;
         break;
    case 'o':                                         // O: CHANGE ANCHOR POSTION
      printf("Enter anchor position (X Y Z):\r\n");
      printf("X: ");
      fflush(stdout);
      menuState->currentMenu = posMenu;
      menuState->configChanged = false;
      break;
    case 'q':
      printf("RESETTING. . . \r\n");
      NVIC_SystemReset();
      break;
    default:
      menuState->configChanged = false;
      break;
  }
}

static void handleMenuMode(char ch, MenuState* menuState) {
  switch(ch) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      changeMode(ch - '0');
      menuState->currentMenu = mainMenu;
      break;
    default:
      printf("Incorrect mode '%c'\r\n", ch);
      menuState->currentMenu = mainMenu;
      menuState->configChanged = false;
      break;
  }
}

static void handleMenuRadio(char ch, MenuState* menuState) {
  switch(ch) {
    case '0':
    case '1':
    case '2':
    case '3':
      changeRadioMode(ch - '0');
      menuState->currentMenu = mainMenu;
      break;
    default:
      printf("Incorrect mode '%c'\r\n", ch);
      menuState->currentMenu = mainMenu;
      menuState->configChanged = false;
      break;
  }
}

static void handleMenuId(char ch, MenuState* menuState) {
  switch(ch) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      menuState->tempId *= 10;
      menuState->tempId += ch - '0';
      putchar(ch);
      fflush(stdout);
      menuState->configChanged = false;
      break;
    case '\n':
    case '\r':
      printf("\r\n");
      fflush(stdout);
      if (menuState->tempId < 256) {
        printf("Setting node ID to %d\r\n", menuState->tempId);
        changeAddress(menuState->tempId);
      } else {
        printf("Wrong ID '%d', the ID should be between 0 and 255\r\n", menuState->tempId);
        menuState->configChanged = false;
      }
      menuState->currentMenu = mainMenu;
      break;
    default:
      menuState->configChanged = false;
      break;
  }
}

static void handleMenuPower(char ch, MenuState* menuState) {
  switch(ch) {
    case 'f':
      printf("Setting ForceTxPower\r\n");
      cfgWriteU8(cfgSmartPower, 0);
      cfgWriteU8(cfgForceTxPower, 1);
      break;
    case 's':
      printf("Setting SmartPower\r\n");
      cfgWriteU8(cfgSmartPower, 1);
      cfgWriteU8(cfgForceTxPower, 0);
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      //support 10 power levels
      changePower(ch - '0');
      break;
    default:
      menuState->currentMenu = mainMenu;
      menuState->configChanged = false;
      break;
  }
}
// Обработчик для режима ввода позиции
static void handleMenuPos(char ch, MenuState* menuState) {
  static char numBuffer[16] = {0};  // Буфер для временного хранения числа
  static numBufferIndex = 0;
  static float tempX = 0.0, tempY = 0.0, tempZ = 0.0;
  static int posIndex = 0; 
  switch (ch) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case '-': // Добавьте обработку знака минус, если нужно
      numBuffer[numBufferIndex++] = ch;
      putchar(ch);
      fflush(stdout);
      break;
    case ' ': // Разделитель между координатами
    case '\n':
    case '\r': 
      if (numBufferIndex > 0) {
        numBuffer[numBufferIndex] = '\0'; 
        float value = atof(numBuffer);
        switch (posIndex) {
          case 0: tempX = value; printf("\r\nY: "); break;
          case 1: tempY = value; printf("\r\nZ: "); break;
          case 2: tempZ = value; 
            
            // Запись координат в структуру uwbConfig
            struct uwbConfig_s* uwbConfig = uwbGetConfig();
            uwbConfig->position[0] = tempX;
            uwbConfig->position[1] = tempY;
            uwbConfig->position[2] = tempZ;
            uwbConfig->positionEnabled = true;
            
            // Сохранение изменений в EEPROM
            printf("\r\nSetting anchor position to: %04.1f %04.1f %04.1f\r\n", tempX, tempY, tempZ); // Вывод после обновления всех координат

            if( xSemaphoreTake(xMutexI2C, portMAX_DELAY) == pdTRUE) {
              cfgWriteFP32list(cfgAnchorPos, uwbConfig->position, 3);
              xSemaphoreGive(xMutexI2C);
            }
            
            posIndex = -1;  // Последний ++ сделает нам ноль

            menuState->configChanged = true; // Устанавливаем флаг, чтобы сообщить об изменении
            menuState->currentMenu = mainMenu;
            help();
            break; 
        }
        numBufferIndex = 0; // Очистить буфер
        posIndex++;
        fflush(stdout);
      }
      break;
  }
}


static void handleMenuConfigurator(char ch, MenuState *menuState) {
  switch(ch) {
    case '1':
      printf("SET NEW POS\r\n");
      printf("id: ");
      fflush(stdout);
      menuState->currentMenu = remotePosMenu;
      menuState->configChanged = false;
      break;
    case '2':
      printf("RESETTING ALL ANCHORS. . .\r\n");
      newServicePacket->action = LPP_SHORT_REBOOT;
      newServicePacket->destinationAddress = 0xff;
      sendServiceData(newServicePacket);
      clearUart();
      printConfiguratorOptions();
      menuState->configChanged = false;
      break;
    case 'x':
      clearUart();
      menuState->currentMenu = mainMenu;
      help();
      menuState->configChanged = false;
      break;
    default:
      menuState->configChanged = false;
      
  }
}

static void handleMenuRemotePos(char ch, MenuState *menuState) {
  // Буфер для временного хранения числа
  static char numBuffer[16] = {0};
  static int numBufferIndex = 0;
  static float tempX = 0.0, tempY = 0.0, tempZ = 0.0;
  static int posIndex = 0, tempID = 0; 
  switch (ch) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6': 
    case '7':
    case '8':
    case '9':
    case '.':
    case '-': // Добавьте обработку знака минус, если нужно
      numBuffer[numBufferIndex++] = ch;
      putchar(ch);
      fflush(stdout);
      break;
    case ' ': // Разделитель между координатами
    case '\n':
    case '\r': 
      if (numBufferIndex > 0) {
        numBuffer[numBufferIndex] = '\0'; 
        switch (posIndex) {
          case 0: tempID = atoi(numBuffer); printf("\r\nX: "); break;
          case 1: tempX = atof(numBuffer); printf("\r\nY: "); break;
          case 2: tempY = atof(numBuffer); printf("\r\nZ: "); break;
          case 3: tempZ = atof(numBuffer); 

            // Запись координат в структуру uwbServiceFromSerial_s
            newServicePacket->action = LPP_SHORT_ANCHOR_POSITION;
            newServicePacket->position[0] = tempX;
            newServicePacket->position[1] = tempY;
            newServicePacket->position[2] = tempZ;
            newServicePacket->destinationAddress = tempID;

            printf("\r\nSetting anchor %x position to: %04.1f %04.1f %04.1f\r\n", tempID, tempX, tempY, tempZ); // Вывод после обновления всех координат
            sendServiceData(newServicePacket);

            posIndex = -1;

            menuState->configChanged = false; // Устанавливаем флаг, чтобы сообщить об изменении
            menuState->currentMenu = configuratorMenu;

            clearUart();
            printConfiguratorOptions();
            break; 
        }
        
        numBufferIndex = 0; // Очистить буфер 
        posIndex++;
        fflush(stdout);
      }
      break;
  }
}


static void handleSerialInput(char ch)
{
  static MenuState menuState = {
    .configChanged = true,
    .currentMenu = mainMenu,
    .tempId = 0
  };

 // menuState.configChanged = true;

  switch (menuState.currentMenu) {
    case mainMenu:
      handleMenuMain(ch, &menuState);
      break;
    case modeMenu:
      handleMenuMode(ch, &menuState);
      break;
    case radioMenu:
      handleMenuRadio(ch, &menuState);
      break;
    case idMenu:
      handleMenuId(ch, &menuState);
      break;
    case powerMenu:
      handleMenuPower(ch, &menuState);
      break;
    case posMenu:
      handleMenuPos(ch, &menuState); 
      break;
    case configuratorMenu:
      handleMenuConfigurator(ch, &menuState);
      break;
    case remotePosMenu:
      handleMenuRemotePos(ch, &menuState);
      break;
  }

 // menuState.configChanged устанавливается в true только при изменении конфигурации
  if (menuState.configChanged) { 
    printf("EEPROM configuration changed, restart for it to take effect!\r\n");
    menuState.configChanged = false; // Сбрасываем флаг после вывода сообщения
  }
}

static void handleButton(void) {
  ButtonEvent be = buttonGetState();
   static struct uwbConfig_s *config = NULL;
     if (config == NULL)
    {
      config = uwbGetConfig();
    }
  if (be == buttonShortPress) {
    uint8_t new_mode = config->mode + 1 ;
    if (new_mode > 4) 
    {
      new_mode = 0;
    }
    changeMode(new_mode);
    ledBlink(ledRanging, true);
     modeChanged = true;
    // TODO: Implement and remove ledblink
  }  else if (be == buttonLongPress) {
    uint8_t new_address = config->address[0] + 1 ;
    if (new_address > 255) 
    {
      new_address = 0;
    }
    changeAddress(new_address);
    addressChanged = true;
    ledBlink(ledSync, true);
    // TODO: Implement and remove ledblink
  }

  buttonProcess();
}

/*------------------------------------------------
-----------------HANDLE FUNCTIONS END-------------
--------------------------------------------------*/

/*------------------------------------------------
-----------------CHANGE FUNCTIONS-----------------
--------------------------------------------------*/

static void restConfig() {
  printf("Resetting EEPROM configuration...");
  if (cfgReset()) {
    printf("OK\r\n");
  } else {
    printf("ERROR\r\n");
  }
}

static void changeAddress(uint8_t addr) {
  printf("Updating address to 0x%02X\r\n", addr);
  cfgWriteU8(cfgAddress, addr);
  if (cfgReadU8(cfgAddress, &addr)) {
    printf("Device address: 0x%X\r\n", addr);
  } else {
    printf("Device address: Not found!\r\n");
  }
}

static void changeMode(unsigned int newMode) {
    printf("Previous device mode: ");
    printMode();

    cfgWriteU8(cfgMode, newMode);

    printf("New device mode: ");
    printMode();
}

static void changePower(uint8_t power) { //expects [0, POWER_LEVELS-1] interval
  // 000 11111 = 0x1F = 33.5dB = max power
  // 110 00000 = 0xC0 = 0dB = min power
  // first 3 bits can have 7 values 000 - 110  (111 means OFF)

  // convert interval to [1, POWER_LEVELS]
  if(power > POWER_LEVELS-1) {
    power = POWER_LEVELS;
  } else {
    power++;
  }

  float desired_db = (power * 1. / POWER_LEVELS )*33.5;

  //split desired power between amplifier and mixer
  uint8_t db_amp = (uint8_t) roundf( (desired_db * 18/33.5) / 3 ) * 3;  //rounded to 3dB steps (supported by amplifier)
  float db_mix = roundf( (desired_db - db_amp) / 0.5 ) * 0.5; //rounded to 0.5dB steps (supported by mixer)

  // 7=111 minus (normalized db_amp values 1-6), all shifted to first 3 bits:
  uint8_t amp_3bits = 0xE0 & (7-( db_amp / 3 + 1 ))<<5;  // 0xE0 bitmask 11100000
  // mixer is 5 bits:
  uint8_t mix_5bits = 0x1F & ( (uint8_t) (db_mix / 0.5) ); // 0x1F bitmask 00011111

  uint8_t power_bits = amp_3bits | mix_5bits;

  //copy power byte to all 4 bytes of txPower
  uint32_t txPower = power_bits | (power_bits<<8) | (power_bits<<16) | (power_bits<<24);
  printf("Setting txpower to: 0x%lX = %.1fdB\r\n", txPower, db_amp+db_mix);
  cfgWriteU32(cfgTxPower, txPower);
}

static void changeRadioMode(unsigned int newMode)
{
  printf("Previous radio mode: ");
  printRadioMode();

  uint8_t lowBitrate = newMode & 1;
  uint8_t longPreamble = (newMode & 2) / 2;

  cfgWriteU8(cfgLowBitrate, lowBitrate);
  cfgWriteU8(cfgLongPreamble, longPreamble);

  printf("New radio mode: ");
  printRadioMode();
}

/*---------------------------------------------------
-----------------CHANGE FUNCTIONS END----------------
-----------------------------------------------------*/

/*------------------------------------------------
-----------------PRINT FUNCTIONS-----------------
--------------------------------------------------*/

inline void printConfigLCD()
{
  static struct uwbConfig_s *cached_config = NULL;
  if (cached_config == NULL)
  {
    cached_config = uwbGetConfig();
  }
  ssd1306_Fill_Area(0, 0, 128, 47, Black);

  uint8_t device_adress = cached_config->address[0];
  wchar_t address[13];
  wchar_t wmode_str[16];
  wchar_t pos[20];

  swprintf(address, 13, L"Адрес: 0x%X%s\n", device_adress, addressChanged ? L"!" : L"");
  swprintf(wmode_str, 16, L"%s%s", uwbAlgorithmName(cached_config->mode), modeChanged ? L"!" : L"");
  swprintf(pos, 20, L"%04.1f %04.1f %04.1f", cached_config->position[0], cached_config->position[1], cached_config->position[2]);
  ssd1306_draw_string(0, 0, 128, 30, &Font, address, White);
  ssd1306_draw_string(0, 17, 128, 30, &Font, wmode_str, White);
  ssd1306_draw_string(3, 34, 128, 30, &Font, pos, White);
  ssd1306_DrawLine(0, 14, 128, 14, White);
  ssd1306_DrawLine(0, 31, 128, 31, White);
  ssd1306_DrawLine(0, 47, 128, 47, White);
  ssd1306_DrawLine(0, 64, 128, 64, White);
  ssd1306_UpdateScreen(&hi2c1);
}

static void printMode() {
  uint8_t mode;

  if (cfgReadU8(cfgMode, &mode)) {
    printf(uwbAlgorithmName(mode));
  } else {
    printf("Not found!");
  }

  printf("\r\n");
}

static void printRadioMode() {
  uint8_t lowBitrate;
  uint8_t longPreamble;

  cfgReadU8(cfgLowBitrate, &lowBitrate);
  cfgReadU8(cfgLongPreamble, &longPreamble);

  printf("%s bitrate, %s preamble", lowBitrate?"low":"normal", longPreamble?"long":"normal");
  printf("\r\n");
}

static void printRadioModeList()
{
  uint8_t lowBitrate;
  uint8_t longPreamble;

  cfgReadU8(cfgLowBitrate, &lowBitrate);
  cfgReadU8(cfgLongPreamble, &longPreamble);

  printf("-------------------\r\n");
  printf("NOTE: only change if you use TDoA3. Other modes will stop working if changed from the default mode.\r\n");
  printf("\r\n");
  printf("Current mode is "); printRadioMode();
  printf("\r\n");
  printf("Available radio modes:\r\n");
  printf("0: normal bitrate, normal preamble (default)\r\n");
  printf("1: low bitrate, normal preamble\r\n");
  printf("2: normal bitrate, long preamble\r\n");
  printf("3: low bitrate, long preamble\r\n");
}

static void printPowerHelp() {
  printf("Power menu\r\n");
  printf("-------------------\r\n");
  printf("0-9 - to set ForceTxPower power (from 3.5dB to 33.5dB)\r\n");
  printf("f   - for ForceTxPower\r\n");
  printf("s   - for SmartPower\r\n");
}

static void printModeList()
{
  unsigned int count = uwbAlgorithmCount();
  int current_mode = -1;
  uint8_t mode;

  if (cfgReadU8(cfgMode, &mode))
  {
    current_mode = mode;
  }

  printf("-------------------\r\n");
  printf("Available UWB modes:\r\n");
  for (int i = 0; i < count; i++)
  {
    printf(" %d - %s%s\r\n", i, uwbAlgorithmName(i),
           (i == current_mode) ? " (Current mode)" : "");
  }
}

static void printConfiguratorOptions()
{
  printf("-------------------\r\n");
  printf("Choose option to do:\r\n");
  printf("1 - Set Anchor's position \r\n");
  printf("2 - Reboot all anchors\r\n");
  printf("x - Back to main menu\r\n");
}

static void help() {
  printf("Help\r\n");
  printf("-------------------\r\n");
  printf("0-9 - set address (node ID)\r\n");
  printf("i   - set node ID from 0 to 255\r\n");
  printf("a   - anchor mode\r\n");
  printf("c   - USB configurator mode\r\n");
  printf("s   - sniffer mode\r\n");
  printf("m   - List and change mode\r\n");
  printf("r   - List and change UWB radio settings\r\n");
  printf("p   - change power mode\r\n");
  printf("o   - set anchor position (X Y Z)\r\n"); 
  printf("d   - reset configuration\r\n");
  printf("q   - device reset\r\n");
  printf("h   - This help\r\n");
  printf("---- For machine only\r\n");
  printf("b   - Switch to binary mode (sniffer only)\r\n");
}

/*---------------------------------------------------
-----------------PRINT FUNCTIONS END-----------------
-----------------------------------------------------*/

static StaticTask_t xMainTask;
static StackType_t ucMainStack[2*configMINIMAL_STACK_SIZE];

int main() {
  // Reset of all peripherals, Initializes the Flash interface and the Systick.
   HAL_Init();
 
  // Configure the system clock
  SystemClock_Config();

uid[0] = HAL_GetUIDw0();  
uid[1] = HAL_GetUIDw1();
uid[2] = HAL_GetUIDw2();

  // Setup main task
  xTaskCreateStatic( main_task, "main", 2*configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 2, ucMainStack, &xMainTask );

  // Start the FreeRTOS scheduler
  vTaskStartScheduler();
  // Should never reach there
  while(1);

  return 0;
}

// Freertos required callbacks
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  static StaticTask_t xIdleTaskTCB;
  static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  static StaticTask_t xTimerTaskTCB;
  static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
  printf("Assert failed at %s:%lu", pcFileName, ulLine);
  while(1);
}