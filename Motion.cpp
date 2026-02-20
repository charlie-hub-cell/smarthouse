#include <Arduino.h>
#include "Motion.h"

#define PIRPIN 3
#define BUZZERPIN 4

void setupMotion() {
  pinMode(PIRPIN, INPUT);
  pinMode(BUZZERPIN, OUTPUT);
  digitalWrite(BUZZERPIN, LOW);

  Serial.println("PIR system ready...");
}

void checkMotion() {
  int pirState = digitalRead(PIRPIN);

  if (pirState == HIGH) {
    Serial.println("Motion detected!");
    
    // Simple beep pattern: 3 beeps
    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZERPIN, HIGH);
      delay(200); // ON for 200ms
      digitalWrite(BUZZERPIN, LOW);
      delay(200); // OFF for 200ms
    }

    // Wait until motion ends to avoid immediate repeat
    while (digitalRead(PIRPIN) == HIGH) {
      delay(50);
    }
  }
}