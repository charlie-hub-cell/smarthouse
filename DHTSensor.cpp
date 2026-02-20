#include <Arduino.h>
#include <U8g2lib.h>
#include <DHT.h>
#include "DHTSensor.h"

// ---------- DHT ----------
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------- OLED ----------
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// ---------- Timing ----------
unsigned long lastRead = 0;
const unsigned long interval = 2000;

float temperature = 0;
float humidity = 0;

void setupDHT() {
  dht.begin();
  u8g2.begin();
}

void readDHT() {

  // ----- Read every 2 seconds -----
  if (millis() - lastRead >= interval) {
    lastRead = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      temperature = t;
      humidity = h;

      Serial.print("Temp: ");
      Serial.print(temperature);
      Serial.print(" C  |  Hum: ");
      Serial.println(humidity);
    } else {
      Serial.println("DHT read failed");
    }
  }

  // ----- OLED Display -----
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.drawStr(0, 15, "Fablab Winam");

  char buf[20];

  dtostrf(temperature, 4, 1, buf);
  u8g2.drawStr(0, 35, "Temp:");
  u8g2.drawStr(60, 35, buf);
  u8g2.drawStr(100, 35, "C");

  dtostrf(humidity, 4, 1, buf);
  u8g2.drawStr(0, 55, "Hum:");
  u8g2.drawStr(60, 55, buf);
  u8g2.drawStr(100, 55, "%");

  u8g2.sendBuffer();
}