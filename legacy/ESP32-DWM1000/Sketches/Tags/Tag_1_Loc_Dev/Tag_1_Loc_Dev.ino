#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000.h"
#include "PosEstimator.h"


//#define DEBUG_DIST
 
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4
 
// connection pins
const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4;   // spi select pin
 
// TAG antenna delay defaults to 16384
// leftmost two bytes below will become the "short address"
char tag_addr[] = "7D:01:22:EA:82:60:3B:9C";


// variables for position determination
PosEstimator estimator;

#define N_ANCHORS 4
#define ANCHOR_DISTANCE_EXPIRED 5000   //measurements older than this are ignore (milliseconds) 

uint32_t last_anchor_update[N_ANCHORS] = {0}; //millis() value last time anchor was seen
double last_anchor_distance[N_ANCHORS] = {0.0}; //most recent distance reports
double *current_tag_position; //global current position (meters with respect to anchor origin)
 

void setup()
{
  Serial.begin(115200);
  delay(1000);
 
  //init the configuration
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
 
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);
 
// start as tag, do not assign random short address
  DW1000Ranging.startAsTag(tag_addr, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
}
 
void loop()
{
  DW1000Ranging.loop();
}
 
void newRange()
{
  int i;  //index of this anchor, expecting values 1 to 7
  int index = DW1000Ranging.getDistantDevice()->getShortAddress() & 0x07;

  if (index > 0) {
    last_anchor_update[index - 1] = millis();  //decrement index for array index
    float range = DW1000Ranging.getDistantDevice()->getRange();
    last_anchor_distance[index - 1] = range;
    if (range < 0.0 || range > 30.0)     last_anchor_update[index - 1] = 0;  //error or out of bounds, ignore this measurement
  }

  int detected = 0;

  //reject old measurements
  for (i = 0; i < N_ANCHORS; i++) {
    if (millis() - last_anchor_update[i] > ANCHOR_DISTANCE_EXPIRED) last_anchor_update[i] = 0; //not from this one
    if (last_anchor_update[i] > 0) detected++;
  }

#ifdef DEBUG_DIST
    // print distance and age of measurement
    uint32_t current_time = millis();
    for (i = 0; i < N_ANCHORS; i++) {
      Serial.print(i+1); //ID
      Serial.print("> ");
      Serial.print(last_anchor_distance[i]);
      Serial.print("\t");
      Serial.println(current_time - last_anchor_update[i]); //age in millis
    }
#endif

  if ( detected == 4) { //four measurements minimum

    estimator.setAnchorsDist(last_anchor_distance);
    estimator.posCalc();
    current_tag_position = estimator.getMobPos();

    //output the values (X, Y, Z)
    Serial.print("P= ");
    Serial.print(current_tag_position[0]);
    Serial.write(',');
    Serial.print(current_tag_position[1]);
    Serial.write(',');
    Serial.println(current_tag_position[2]);
  }
} 
 
void newDevice(DW1000Device *device)
{
  Serial.print("Device added: ");
  Serial.println(device->getShortAddress(), HEX);
}
 
void inactiveDevice(DW1000Device *device)
{
  Serial.print("delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}
