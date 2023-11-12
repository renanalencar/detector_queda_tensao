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
 * @file utilities.h
 * @brief Header responsible for setting up the SIM800H modem.
 * @author Renan Alencar
 * Contact: https://linktr.ee/mr.costaalencar
 *
 */

#include <Wire.h>

#if defined(SIM800H_IP5306_VERSION_20200811)

#define MODEM_RST             5
#define MODEM_PWRKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

#define I2C_SDA              21
#define I2C_SCL              22
#define LED_GPIO             13
#define LED_ON               HIGH
#define LED_OFF              LOW

#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

#endif

bool setupPMU()
{
  bool en = true;
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_SYS_CTL0);
  if (en) {
    Wire.write(0x37); // set bit1: 1 enable 0 disable boost keep on
  } else {
    Wire.write(0x35); // 0x37 is default reg value
  }
  return Wire.endTransmission() == 0;
}

void setupModem() {
#ifdef MODEM_RST
  pinMode(MODEM_RST, OUTPUT);
  digitalWrite(MODEM_RST, HIGH);
#endif
  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_POWER_ON, HIGH);
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(100);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, HIGH);
  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, LED_OFF);
}
