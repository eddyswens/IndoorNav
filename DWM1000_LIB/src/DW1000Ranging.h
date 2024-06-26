/*
 * Copyright (c) 2015 by Thomas Trojer <thomas@trojer.net> and Leopold Sayous <leosayous@gmail.com>
 * Decawave DW1000 library for arduino.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file DW1000Ranging.h
 * Arduino global library (header file) working with the DW1000 library
 * for the Decawave DW1000 UWB transceiver IC.
 *
 * @TODO
 * - remove or debugmode for Serial.print
 * - move strings to flash to reduce ram usage
 * - do not safe duplicate of pin settings
 * - maybe other object structure
 * - use enums instead of preprocessor constants
 */

#include "DW1000.h"
#include "DW1000Time.h"
#include "DW1000Device.h" 
#include "DW1000Mac.h"

// messages used in the ranging protocol
#define POLL 0
#define POLL_ACK 1
#define RANGE 2
#define RANGE_REPORT 3
#define RANGE_FAILED 255
#define BLINK 4
#define RANGING_INIT 5

#define LEN_DATA 90

//Max devices we put in the networkDevices array ! Each DW1000Device is 74 Bytes in SRAM memory for now.
#define MAX_DEVICES 4

//Default Pin for module:
#define DEFAULT_RST_PIN 9
#define DEFAULT_SPI_SS_PIN 10

//Default value
//in ms
#define DEFAULT_RESET_PERIOD 200
//in us
#define DEFAULT_REPLY_DELAY_TIME 7000

//sketch type (anchor or tag)
#define TAG 0
#define ANCHOR 1

//default timer delay
#define DEFAULT_TIMER_DELAY 80

//debug mode
#ifndef DEBUG
#define DEBUG false
#endif


class DW1000RangingClass {
public:
	//variables
	// data buffer
	static byte data[LEN_DATA];
	
	//initialisation
	static void    initCommunication(uint8_t myRST = DEFAULT_RST_PIN, uint8_t mySS = DEFAULT_SPI_SS_PIN, uint8_t myIRQ = 2);
	static void    configureNetwork(uint16_t deviceAddress, uint16_t networkId, const byte mode[]);
	static void    generalStart();
	static void    startAsAnchor(char address[], const byte mode[], const bool randomShortAddress = true);
	static void    startAsTag(char address[], const byte mode[], const bool randomShortAddress = true);
	static boolean addNetworkDevices(DW1000Device* device, boolean shortAddress);
	static boolean addNetworkDevices(DW1000Device* device);
	static void    removeNetworkDevices(int16_t index);
	
	//setters
	static void setReplyTime(uint16_t replyDelayTimeUs);
	static void setResetPeriod(uint32_t resetPeriod);
	
	//getters
	static byte* getCurrentAddress() { return _currentAddress; };
	
	static byte* getCurrentShortAddress() { return _currentShortAddress; };
	
	static uint8_t getNetworkDevicesNumber() { return _networkDevicesNumber; };
	
	//ranging functions
	static int16_t detectMessageType(byte datas[]); // TODO check return type
	static void loop();
	static void useRangeFilter(boolean enabled);
	// Used for the smoothing algorithm (Exponential Moving Average). newValue must be >= 2. Default 15.
	static void setRangeFilterValue(uint16_t newValue);
	
	//Handlers:
	static void attachNewRange(void (* handleNewRange)(void)) { _handleNewRange = handleNewRange; };
	
	static void attachBlinkDevice(void (* handleBlinkDevice)(DW1000Device*)) { _handleBlinkDevice = handleBlinkDevice; };
	
	static void attachNewDevice(void (* handleNewDevice)(DW1000Device*)) { _handleNewDevice = handleNewDevice; };
	
	static void attachInactiveDevice(void (* handleInactiveDevice)(DW1000Device*)) { _handleInactiveDevice = handleInactiveDevice; };
	
	
	
	static DW1000Device* getDistantDevice();
	static DW1000Device* searchDistantDevice(byte shortAddress[]);
	
	//FOR DEBUGGING
	static void visualizeDatas(byte datas[]);


private:
	//other devices in the network
	static DW1000Device _networkDevices[MAX_DEVICES];
	static volatile uint8_t _networkDevicesNumber;
	static int16_t      _lastDistantDevice;
	static byte         _currentAddress[8];
	static byte         _currentShortAddress[2];
	static byte         _lastSentToShortAddress[2];
	static DW1000Mac    _globalMac;
	static int32_t      timer;
	static int16_t      counterForBlink;
	
	//Handlers:
	static void (* _handleNewRange)(void);
	static void (* _handleBlinkDevice)(DW1000Device*);
	static void (* _handleNewDevice)(DW1000Device*);
	static void (* _handleInactiveDevice)(DW1000Device*);
	
	//sketch type (tag or anchor)
	static int16_t          _type; //0 for tag and 1 for anchor
	// TODO check type, maybe enum?
	// message flow state
	static volatile byte    _expectedMsgId;
	// message sent/received state
	static volatile boolean _sentAck;
	static volatile boolean _receivedAck;
	// protocol error state
	static boolean          _protocolFailed;
	// reset line to the chip
	static uint8_t     _RST;
	static uint8_t     _SS;
	// watchdog and reset period
	static uint32_t    _lastActivity;
	static uint32_t    _resetPeriod;
	// reply times (same on both sides for symm. ranging)
	static uint16_t     _replyDelayTimeUS;
	//timer Tick delay
	static uint16_t     _timerDelay;
	// ranging counter (per second)
	static uint16_t     _successRangingCount;
	static uint32_t    _rangingCountPeriod;
	//ranging filter
	static volatile boolean _useRangeFilter;
	static uint16_t         _rangeFilterValue;
	//_bias correction
	static char  _bias_RSL[17]; // TODO remove or use
	//17*2=34 bytes in SRAM
	static int16_t _bias_PRF_16[17]; // TODO remove or use
	//17 bytes in SRAM
	static char  _bias_PRF_64[17]; // TODO remove or use
	
	
	//methods
	static void handleSent();
	static void handleReceived();
	static void noteActivity();
	static void resetInactive();
	
	//global functions:
	static void checkForReset();
	static void checkForInactiveDevices();
	static void copyShortAddress(byte address1[], byte address2[]);
	
	//for ranging protocole (ANCHOR)
	static void transmitInit();
	static void transmit(byte datas[]);
	static void transmit(byte datas[], DW1000Time time);
	static void transmitBlink();
	static void transmitRangingInit(DW1000Device* myDistantDevice);
	static void transmitPollAck(DW1000Device* myDistantDevice);
	static void transmitRangeReport(DW1000Device* myDistantDevice);
	static void transmitRangeFailed(DW1000Device* myDistantDevice);
	static void receiver();
	
	//for ranging protocole (TAG)
	static void transmitPoll(DW1000Device* myDistantDevice);
	static void transmitRange(DW1000Device* myDistantDevice);
	
	//methods for range computation
	static void computeRangeAsymmetric(DW1000Device* myDistantDevice, DW1000Time* myTOF);
	static double getrangebias(byte chan, float range, byte prf);
	
	static void timerTick();
	
	//Utils
	static float filterValue(float value, float previousValue, uint16_t numberOfElements);


#define NUM_16M_OFFSET  (37)
#define NUM_16M_OFFSETWB  (68)
#define NUM_64M_OFFSET  (26)
#define NUM_64M_OFFSETWB  (59)

#define NUM_CH_SUPPORTED 8  //supported channels are '0', 1, 2, 3, 4, 5, '6', 7

static constexpr byte chan_idxnb[NUM_CH_SUPPORTED] = {0, 0, 1, 2, 0, 3, 0, 0}; //only channels 1,2,3 and 5 are in the narrow band tables
static constexpr byte chan_idxwb[NUM_CH_SUPPORTED] = {0, 0, 0, 0, 0, 0, 0, 1}; //only channels 4 and 7 are in in the wide band tables
	//---------------------------------------------------------------------------------------------------------------------------
// Range Bias Correction TABLES of range values in integer units of 25 CM, for 8-bit unsigned storage, MUST END IN 255 !!!!!!
//---------------------------------------------------------------------------------------------------------------------------

// offsets to nearest centimeter for index 0, all rest are +1 cm per value

#define CM_OFFSET_16M_NB    (-23)   // for normal band channels at 16 MHz PRF
#define CM_OFFSET_16M_WB    (-28)   // for wider  band channels at 16 MHz PRF
#define CM_OFFSET_64M_NB    (-17)   // for normal band channels at 64 MHz PRF
#define CM_OFFSET_64M_WB    (-30)   // for wider  band channels at 64 MHz PRF


//---------------------------------------------------------------------------------------------------------------------------
// range25cm16PRFnb: Range Bias Correction table for narrow band channels at 16 MHz PRF, NB: !!!! each MUST END IN 255 !!!!
//---------------------------------------------------------------------------------------------------------------------------

static constexpr byte range25cm16PRFnb[4][NUM_16M_OFFSET] =
{
    // ch 1 - range25cm16PRFnb
    {
           1,
           3,
           4,
           5,
           7,
           9,
          11,
          12,
          13,
          15,
          18,
          20,
          23,
          25,
          28,
          30,
          33,
          36,
          40,
          43,
          47,
          50,
          54,
          58,
          63,
          66,
          71,
          76,
          82,
          89,
          98,
         109,
         127,
         155,
         222,
         255,
         255
    },

    // ch 2 - range25cm16PRFnb
    {
           1,
           2,
           4,
           5,
           6,
           8,
           9,
          10,
          12,
          13,
          15,
          18,
          20,
          22,
          24,
          27,
          29,
          32,
          35,
          38,
          41,
          44,
          47,
          51,
          55,
          58,
          62,
          66,
          71,
          78,
          85,
          96,
         111,
         135,
         194,
         240,
         255
    },

    // ch 3 - range25cm16PRFnb
    {
           1,
           2,
           3,
           4,
           5,
           7,
           8,
           9,
          10,
          12,
          14,
          16,
          18,
          20,
          22,
          24,
          26,
          28,
          31,
          33,
          36,
          39,
          42,
          45,
          49,
          52,
          55,
          59,
          63,
          69,
          76,
          85,
          98,
         120,
         173,
         213,
         255
    },

    // ch 5 - range25cm16PRFnb
    {
           1,
           1,
           2,
           3,
           4,
           5,
           6,
           6,
           7,
           8,
           9,
          11,
          12,
          14,
          15,
          16,
          18,
          20,
          21,
          23,
          25,
          27,
          29,
          31,
          34,
          36,
          38,
          41,
          44,
          48,
          53,
          59,
          68,
          83,
         120,
         148,
         255
    }
}; // end range25cm16PRFnb


//---------------------------------------------------------------------------------------------------------------------------
// range25cm16PRFwb: Range Bias Correction table for wide band channels at 16 MHz PRF, NB: !!!! each MUST END IN 255 !!!!
//---------------------------------------------------------------------------------------------------------------------------

static constexpr byte range25cm16PRFwb[2][NUM_16M_OFFSETWB] =
{
    // ch 4 - range25cm16PRFwb
    {
           7,
           7,
           8,
           9,
           9,
          10,
          11,
          11,
          12,
          13,
          14,
          15,
          16,
          17,
          18,
          19,
          20,
          21,
          22,
          23,
          24,
          26,
          27,
          28,
          30,
          31,
          32,
          34,
          36,
          38,
          40,
          42,
          44,
          46,
          48,
          50,
          52,
          55,
          57,
          59,
          61,
          63,
          66,
          68,
          71,
          74,
          78,
          81,
          85,
          89,
          94,
          99,
         104,
         110,
         116,
         123,
         130,
         139,
         150,
         164,
         182,
         207,
         238,
         255,
         255,
         255,
         255,
         255
    },

    // ch 7 - range25cm16PRFwb
    {
           4,
           5,
           5,
           5,
           6,
           6,
           7,
           7,
           7,
           8,
           9,
           9,
          10,
          10,
          11,
          11,
          12,
          13,
          13,
          14,
          15,
          16,
          17,
          17,
          18,
          19,
          20,
          21,
          22,
          23,
          25,
          26,
          27,
          29,
          30,
          31,
          32,
          34,
          35,
          36,
          38,
          39,
          40,
          42,
          44,
          46,
          48,
          50,
          52,
          55,
          58,
          61,
          64,
          68,
          72,
          75,
          80,
          85,
          92,
         101,
         112,
         127,
         147,
         168,
         182,
         194,
         205,
         255
    }
}; // end range25cm16PRFwb

//---------------------------------------------------------------------------------------------------------------------------
// range25cm64PRFnb: Range Bias Correction table for narrow band channels at 64 MHz PRF, NB: !!!! each MUST END IN 255 !!!!
//---------------------------------------------------------------------------------------------------------------------------

static constexpr byte range25cm64PRFnb[4][NUM_64M_OFFSET] =
{
    // ch 1 - range25cm64PRFnb
    {
           1,
           2,
           2,
           3,
           4,
           5,
           7,
          10,
          13,
          16,
          19,
          22,
          24,
          27,
          30,
          32,
          35,
          38,
          43,
          48,
          56,
          78,
         101,
         120,
         157,
         255
    },

    // ch 2 - range25cm64PRFnb
    {
           1,
           2,
           2,
           3,
           4,
           4,
           6,
           9,
          12,
          14,
          17,
          19,
          21,
          24,
          26,
          28,
          31,
          33,
          37,
          42,
          49,
          68,
          89,
         105,
         138,
         255
    },

    // ch 3 - range25cm64PRFnb
    {
           1,
           1,
           2,
           3,
           3,
           4,
           5,
           8,
          10,
          13,
          15,
          17,
          19,
          21,
          23,
          25,
          27,
          30,
          33,
          37,
          44,
          60,
          79,
          93,
         122,
         255
    },

    // ch 5 - range25cm64PRFnb
    {
           1,
           1,
           1,
           2,
           2,
           3,
           4,
           6,
           7,
           9,
          10,
          12,
          13,
          15,
          16,
          17,
          19,
          21,
          23,
          26,
          30,
          42,
          55,
          65,
          85,
         255
    }
}; // end range25cm64PRFnb

//---------------------------------------------------------------------------------------------------------------------------
// range25cm64PRFwb: Range Bias Correction table for wide band channels at 64 MHz PRF, NB: !!!! each MUST END IN 255 !!!!
//---------------------------------------------------------------------------------------------------------------------------

static constexpr byte range25cm64PRFwb[2][NUM_64M_OFFSETWB] =
{
    // ch 4 - range25cm64PRFwb
    {
           7,
           8,
           8,
           9,
           9,
          10,
          11,
          12,
          13,
          13,
          14,
          15,
          16,
          16,
          17,
          18,
          19,
          19,
          20,
          21,
          22,
          24,
          25,
          27,
          28,
          29,
          30,
          32,
          33,
          34,
          35,
          37,
          39,
          41,
          43,
          45,
          48,
          50,
          53,
          56,
          60,
          64,
          68,
          74,
          81,
          89,
          98,
         109,
         122,
         136,
         146,
         154,
         162,
         178,
         220,
         249,
         255,
         255,
         255
    },

    // ch 7 - range25cm64PRFwb
    {
           4,
           5,
           5,
           5,
           6,
           6,
           7,
           7,
           8,
           8,
           9,
           9,
          10,
          10,
          10,
          11,
          11,
          12,
          13,
          13,
          14,
          15,
          16,
          16,
          17,
          18,
          19,
          19,
          20,
          21,
          22,
          23,
          24,
          25,
          26,
          28,
          29,
          31,
          33,
          35,
          37,
          39,
          42,
          46,
          50,
          54,
          60,
          67,
          75,
          83,
          90,
          95,
         100,
         110,
         135,
         153,
         172,
         192,
         255
    }
}; // end range25cm64PRFwb
};

extern DW1000RangingClass DW1000Ranging;
