//=== Arduino Uno TX to Attiny85

#include <Manchester.h>

#define TX_PIN 7 //---Pin 2 - Physical PIN 7 on ATTiny85
#define TX_LED_PIN 4 //---Pin 0 - Physical PIN 5 on ATTiny85

String data2send = "Hello";
uint8_t datalength = 5;
uint8_t data[20]; //---MAX Message length 20
uint8_t i = 0; //---For counter

String Trig1_ON = "Trig101"; //---Trig 1 On
String Trig1_OFF = "Trig100"; //---Trig 1 Off
String Trig2_ON = "Trig201"; //---Trig 2 On
String Trig2_OFF = "Trig200"; //---Trig 2 Off

void SendData( String SendThisNow ) { //---Populate data array - data
  digitalWrite(TX_LED_PIN, HIGH);
  datalength = SendThisNow.length();
  memset(data, 0, sizeof(data));
  data[0] = datalength + 1;
  for(i = 0; i < datalength; i++) {
    data[i+1] = SendThisNow[i];
  }
  man.transmitArray(data[0], data);
  delay(100);
  digitalWrite(TX_LED_PIN, LOW);
  delay(100);
}

void setup() {
//  Serial.begin(9600);
  pinMode(TX_LED_PIN, OUTPUT);
  digitalWrite(TX_LED_PIN, LOW);
  man.setupTransmit(TX_PIN, MAN_1200);
}

void loop() {
//  digitalWrite(TX_LED_PIN, HIGH);
  SendData(Trig1_ON);
  delay(10);
  SendData(Trig1_ON);
  delay(10);
  SendData(Trig1_ON);
  delay(1000);
  SendData(Trig2_ON);
  delay(10);
  SendData(Trig2_ON);
  delay(10);
  SendData(Trig2_ON);
  delay(1000);
  SendData(Trig1_OFF);
  delay(10);
  SendData(Trig1_OFF);
  delay(10);
  SendData(Trig1_OFF);
  delay(1000);
  SendData(Trig2_OFF);
  delay(10);
  SendData(Trig2_OFF);
  delay(10);
  SendData(Trig2_OFF);
  delay(1000);
//  Serial.println(DataSent);
//  digitalWrite(TX_LED_PIN, LOW);
}
