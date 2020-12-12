/*
 SerialPassthrough sketch
 with SoftwareSeral option

 created in August 2019 for WiFiEspAT library
 by Juraj Andrassy https://github.com/jandrassy
 */

//#define SAMD_FLOW_CONTROL

// Emulate Serial1 on pins 6/7 if not present
#include "SoftwareSerial.h"
SoftwareSerial SerialAT(2, 3); // RX, TX
#define AT_BAUD_RATE 9600

void setup() {

  Serial.begin(AT_BAUD_RATE);
  while (!Serial);

  SerialAT.begin(AT_BAUD_RATE);
}

void loop() {
  while (Serial.available()) {
    SerialAT.write(Serial.read());
  }
  while (SerialAT.available()) {
    Serial.write(SerialAT.read());
  }
}
