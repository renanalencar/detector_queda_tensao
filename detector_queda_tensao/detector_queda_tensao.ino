#define SIM800L_IP5306_VERSION_20200811

#include "utilities.h"

#define SerialMon  Serial
#define SerialAT  Serial1

static const uint32_t AT_BAUD  = 115200;
static const uint32_t MON_BAUD = 115200;

const char* SUBJECT = "AIRE INFORMA: FOI DETECTADA UMA QUEDA DE TENSAO! ENTRE EM CONTATO COM A NEOENERGIA PERNAMBUCO 116.";
const char* SMS_TARGETS[] = {"+5581982335881", "+5581994243992"};

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

  xTaskCreatePinnedToCore(triggerAlert, "Task1", 10000, NULL, 1, &Task1, 0); // atribuir a tarefa de enviar a mensagem ao CORE_0 da ESP32 para alivar o CORE_1
}

void triggerAlert(void * pvParameters) {
  for (;;) {
    if (AmpsRMS < 0.05) {
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
  Voltage = getVPP(); // função que lê o valor analógico e transforma em valor de tensão
  VRMS = (Voltage / 2.0) * 0.707;  
  AmpsRMS = ((VRMS * 1000) / mVperAmp) - 0.50; // calcula a corrente percorrida, dado o datasheet do sensor. Pode ser necessário corrigir este valor, 
  Watt = (AmpsRMS * 240 / 1.2);                // basta alterar o valor "-0.50" para outro quando o circuito está berto (sensor não conectado ao equipamento a medir)

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
