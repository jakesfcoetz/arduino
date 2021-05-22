//=== Arduino Uno RX from Attiny85

#include <Manchester.h>

#define RX_PIN 7 //---Pin 2 - Physical PIN 7 on ATTiny85
#define LED_PIN 4 //---Pin 0 - Physical PIN 5 on ATTiny85
#define RX_BUFFER_SIZE 20 //---MAX Message received length
uint8_t RX_BUFFER[RX_BUFFER_SIZE]; //---Message received array
uint8_t RX_ReceivedSize = 0; //---Size of message received array
String MSG_Received = ""; //---Message received String
uint8_t i = 0; //---For counter

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  man.setupReceive(RX_PIN, MAN_1200);
  man.beginReceiveArray(RX_BUFFER_SIZE, RX_BUFFER);
}

void loop() {
  if (man.receiveComplete()) { //---Wait for message to receive
    digitalWrite(LED_PIN, HIGH);
    getSensorData();
    Serial.println(MSG_Received);
    delay(100); //---only for LED
    digitalWrite(LED_PIN, LOW);
  }
}

void getSensorData() { //---Populate data String - MSG_Received
  RX_ReceivedSize = RX_BUFFER[0];
  MSG_Received = "";
  for(i = 1; i < RX_ReceivedSize; i++) {
    MSG_Received += (char)RX_BUFFER[i];
  }
  man.beginReceiveArray(RX_BUFFER_SIZE, RX_BUFFER);
}
