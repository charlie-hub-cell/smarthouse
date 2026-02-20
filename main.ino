#include <Arduino.h>
#include "DHTSensor.h"
#include "Motion.h"

void setup() {
  Serial.begin(9600);
  delay(1000);

  setupDHT();
  setupMotion();
}

void loop() {

  // Read DHT every 2 seconds (non-blocking)
  readDHT();

  // Check motion and control buzzer
  checkMotion();

  delay(50); // small stability delay
}