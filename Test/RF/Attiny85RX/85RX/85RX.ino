//=== Attiny85 RX from Uno

#include <Manchester.h>

#define RX_PIN 2 //---Pin 2 - Physical PIN 7 on ATTiny85
#define LED_PIN 0 //---Pin 0 - Physical PIN 5 on ATTiny85
#define LED_PIN_RX 4 //---Pin 4 - Physical PIN 3 on ATTiny85
#define RX_BUFFER_SIZE 20 //---MAX Message received length
uint8_t RX_BUFFER[RX_BUFFER_SIZE]; //---Message received array
uint8_t RX_ReceivedSize = 0; //---Size of message received array
String MSG_Received = ""; //---Message received String
uint8_t i = 0; //---For counter
String Trig1 = "Hello123";

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN_RX, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_PIN_RX, LOW);
  man.setupReceive(RX_PIN, MAN_1200);
  man.beginReceiveArray(RX_BUFFER_SIZE, RX_BUFFER);
}

void loop() {
  if (man.receiveComplete()) { //---Wait for message to receive
    digitalWrite(LED_PIN, HIGH);
    getSensorData();
    if (MSG_Received == Trig1) {
      digitalWrite(LED_PIN_RX, HIGH);
      delay(50); //---only for LED_RX
      digitalWrite(LED_PIN_RX, LOW);
    }
    delay(50); //---only for LED
    digitalWrite(LED_PIN, LOW);
    delay(50); //---only for LED
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
