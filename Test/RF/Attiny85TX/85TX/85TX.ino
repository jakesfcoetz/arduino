//=== Attiny85 TX to Uno

#include <Manchester.h>

#define TX_PIN 2 //---Pin 2 - Physical PIN 7 on ATTiny85
#define LED_PIN 0 //---Pin 0 - Physical PIN 5 on ATTiny85

String data2send = "Hello";
uint8_t datalength = data2send.length();
uint8_t data[20]; //---MAX Message length 20
uint8_t i = 0; //---For counter

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  man.setupTransmit(TX_PIN, MAN_1200);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  readSensorData();
  man.transmitArray(data[0], data);
  digitalWrite(LED_PIN, LOW);
  delay(900);
}

void readSensorData() { //---Populate data array - data
  data2send = "Hello123"; //---get value
  datalength = data2send.length();
  memset(data, 0, sizeof(data));
  data[0] = datalength + 1;
  for(i = 0; i < datalength; i++) {
    data[i+1] = data2send[i];
  }
}
