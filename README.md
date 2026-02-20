# smarthouse

🌡️ DHT11 + OLED + PIR Motion + Buzzer System




This project combines:

DHT11 sensor → temperature & humidity

OLED Display (SH1106 128x64) → shows real-time readings

PIR Motion Sensor → detects motion

Buzzer → plays a 3-beep alert on motion

Designed for Arduino Mega / Uno, modular, and easy to expand.

📦 Components & Wiring
Component	Connection	Notes
Arduino Mega / Uno	–	Microcontroller
DHT11	Data → Pin 2, VCC → 5V, GND → GND	Read temp & humidity
OLED SH1106	SDA → SDA, SCL → SCL, VCC → 5V, GND → GND	Displays readings
PIR Sensor	OUT → Pin 3, VCC → 5V, GND → GND	Detects motion
Buzzer	Signal → Pin 4, GND → GND	Plays alert

Important: Make sure all grounds are common.

⚡ Power Setup

During testing, powering all components directly from Arduino was insufficient. To solve this:

Used a 12V DC adapter → connected to a 5V booster to supply the sensors (DHT11, OLED, PIR, Buzzer).

Arduino is still powered via USB → allows Serial monitoring.

Ensured common ground between Arduino, sensors, and booster.

💡 This setup provides stable 5V to all sensors and avoids brownout issues.
💡 PIR requires 30–60 seconds warm-up after power-on.

Wiring Diagram (Simplified)
Arduino Mega / Uno
+-----------------------------+
|                             |
|  Pin 2   -> DHT11 DATA      |
|  5V      -> DHT11 VCC       |
|  GND     -> DHT11 GND       |
|                             |
|  SDA      -> OLED SDA       |
|  SCL      -> OLED SCL       |
|  5V       -> OLED VCC       |
|  GND      -> OLED GND       |
|                             |
|  Pin 3    -> PIR OUT        |
|  5V       -> PIR VCC        |
|  GND      -> PIR GND        |
|                             |
|  Pin 4    -> BUZZER SIGNAL |
|  GND      -> BUZZER GND     |
+-----------------------------+

Sensors powered from 5V booster (fed by 12V adapter)

Arduino powered via USB

All grounds connected together

🗂️ Code Structure
/project
│
├── main.cpp         // Initializes system & calls loops
├── DHTSensor.h      // DHT11 + OLED declarations
├── DHTSensor.cpp    // DHT11 + OLED logic
├── Motion.h         // PIR + buzzer declarations
└── Motion.cpp       // PIR + buzzer logic
📄 main.cpp
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
  readDHT();      // Update temperature & humidity on OLED
  checkMotion();  // Detect motion and beep buzzer
}
📄 DHTSensor.h
#ifndef DHTSENSOR_H
#define DHTSENSOR_H

#include <Arduino.h>

void setupDHT();
void readDHT();

#endif
📄 DHTSensor.cpp
#include "DHTSensor.h"
#include <U8g2lib.h>
#include <DHT.h>

// DHT11
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// OLED
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// Timing
unsigned long lastRead = 0;
const unsigned long interval = 2000;

float temperature = 0;
float humidity = 0;

void setupDHT() {
  dht.begin();
  u8g2.begin();
}

void readDHT() {
  if (millis() - lastRead >= interval) {
    lastRead = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      temperature = t;
      humidity = h;

      Serial.print("Temp: ");
      Serial.print(temperature);
      Serial.print(" C | Hum: ");
      Serial.println(humidity);
    } else {
      Serial.println("DHT read failed");
    }
  }

  // OLED display
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 15, "DHT11 Sensor");

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
📄 Motion.h
#ifndef MOTION_H
#define MOTION_H

#include <Arduino.h>

void setupMotion();
void checkMotion();

#endif
📄 Motion.cpp
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
      delay(200); // ON
      digitalWrite(BUZZERPIN, LOW);
      delay(200); // OFF
    }

    // Wait until motion ends to avoid immediate repeat
    while (digitalRead(PIRPIN) == HIGH) {
      delay(50);
    }
  }
}
🔧 Usage Instructions

Connect components per wiring diagram.

Power sensors with 12V adapter → 5V booster.

Power Arduino via USB for programming & Serial.

Upload all .cpp and .h files.

Open Serial Monitor (9600 baud).

Move in front of PIR → buzzer beeps 3 times → OLED updates every 2s.

💡 Notes

PIR may require H mode jumper for retriggering.

Wait 30–60s for PIR warm-up.

Ensure common ground between Arduino, sensors, and booster.

12V → 5V booster provides stable power for all components.
