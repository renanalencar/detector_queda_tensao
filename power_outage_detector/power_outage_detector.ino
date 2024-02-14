/* Copyright (C) 2023 Renan Alencar - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GNU license, which unfortunately won't be
 * written for another century.
 *
 * You should have received a copy of the GNU license with
 * this file. If not, please write to: renan.costaalencar@gmail.com,
 * or visit : https://spdx.org/licenses/GPL-3.0-or-later.html
 *
 * Project based on DETETOR DE FALHA DE ENERGIA published by 
 * Electrofun Blog (https://www.electrofun.pt/blog/detetor-de-falha-de-energia-com-esp32/)
 */

/**
 * @file power_outage_detector.ino
 * @brief Code responsible for detecting power outage and sending
 * an SMS to the target recipients.
 * @author Renan Alencar
 * Contact: https://linktr.ee/mr.costaalencar
 *
 */

#define SIM800H_IP5306_VERSION_20200811

#include "utilities.h"
#include "variables.h"

#define SerialMon  Serial
#define SerialAT  Serial1

static const uint32_t AT_BAUD  = 115200;
static const uint32_t MON_BAUD = 115200;

const char* SUBJECT = MESSAGE;
const auto SMS_TARGETS = NUMBERS;

const int SENSOR_IN = 34;
int mVperAmp        = 100;
int Watt            = 0;
double Voltage      = 0;
double VRMS         = 0;
double AmpsRMS      = 0;

TaskHandle_t Task1;

void setup() {
  SerialMon.begin(MON_BAUD, SERIAL_8N1);
  SerialAT.begin(AT_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
  pinMode(SENSOR_IN, INPUT);
  setupPMU();
  setupModem();
  delay(5000);

  // Assign the task of sending the message to CORE_0 of ESP32 to relieve CORE_1
  xTaskCreatePinnedToCore(triggerAlert, "Task1", 10000, NULL, 1, &Task1, 0);
}

void triggerAlert(void * pvParameters) {
  for (;;) {
    while (AmpsRMS < 0.05) {
      for (int i = 0; i < sizeof(SMS_TARGETS) / sizeof(char*); i++) {
        sendSMS(SMS_TARGETS[i]);
        updateSerialAT();
        delay(1000);
      }
    }
  }
  vTaskDelay(100 / portTICK_PERIOD_MS);
}

void loop() {
  Serial.println ("");
  Voltage = getVPP(); // Function that reads the analog value and converts it into a voltage value
  VRMS    = (Voltage / 2.0) * 0.707;  
  AmpsRMS = ((VRMS * 1000) / mVperAmp) - 0.50; // Calculate the current flow, given the sensor datasheet. It might be necessary to correct this value,
  Watt    = (AmpsRMS * 240 / 1.2);             // simply by changing the value "-0.50" to another when the circuit is open (sensor not connected to the measuring equipment).

  Serial.print(AmpsRMS);
  Serial.print(" Amps RMS  ---  ");
  Serial.print(Watt);
  Serial.println(" Watts");
  delay(100);
}

void updateSerialAT() {
  while (SerialMon.available())
    SerialAT.write(SerialMon.read());
  while (SerialAT.available())
    SerialMon.write(SerialAT.read());
}

void sendSMS(const char* recipient) {
  SerialAT.println("AT+CMGF=1");
  delay(100);

  SerialAT.print("AT+CMGS=\"");
  SerialAT.print(recipient);
  SerialAT.println("\"");
  delay(200);

  SerialAT.print(SUBJECT);
  delay(500);

  SerialAT.print((char)26);
  delay(500);

  SerialAT.println();
}

float getVPP() {
  float result;
  int readValue;
  int maxValue = 0;
  int minValue = 4096;
  uint32_t start_time = millis();

  while ((millis() - start_time) < 1000)
  {
    readValue = analogRead(SENSOR_IN);
    if (readValue > maxValue)
      maxValue = readValue;
    if (readValue < minValue)
      minValue = readValue;
  }

  result = ((maxValue - minValue) * 5) / 4096.0;

  return result;
}
