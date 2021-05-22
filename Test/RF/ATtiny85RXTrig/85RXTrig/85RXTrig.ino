//=== Attiny85 RX from Uno

#include <Manchester.h>

#define RX_PIN 2 //---Pin 2 - Physical PIN 7 on ATTiny85
#define LED_PIN 0 //---Pin 0 - Physical PIN 5 on ATTiny85
#define RX_TRIG_1 3 //---Pin 3 - Physical PIN 2 on ATTiny85
#define RX_TRIG_2 4 //---Pin 4 - Physical PIN 3 on ATTiny85
#define RX_BUFFER_SIZE 20 //---MAX Message received length
uint8_t RX_BUFFER[RX_BUFFER_SIZE]; //---Message received array
uint8_t RX_ReceivedSize = 0; //---Size of message received array
String MSG_Received = ""; //---Message received String
uint8_t i = 0; //---For counter

String Trig1_ON = "Trig101"; //---Trig 1 On
String Trig1_OFF = "Trig100"; //---Trig 1 Off
String Trig2_ON = "Trig201"; //---Trig 2 On
String Trig2_OFF = "Trig200"; //---Trig 2 Off

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RX_TRIG_1, OUTPUT);
  pinMode(RX_TRIG_2, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(RX_TRIG_1, LOW);
  digitalWrite(RX_TRIG_2, LOW);
  man.setupReceive(RX_PIN, MAN_1200);
  man.beginReceiveArray(RX_BUFFER_SIZE, RX_BUFFER);
}

void loop() {
  if (man.receiveComplete()) { //---Wait for message to receive
    digitalWrite(LED_PIN, HIGH);
    getSentData();
    if (MSG_Received == Trig1_ON) digitalWrite(RX_TRIG_1, HIGH);
    else if (MSG_Received == Trig1_OFF) digitalWrite(RX_TRIG_1, LOW);
    else if (MSG_Received == Trig2_ON) digitalWrite(RX_TRIG_2, HIGH);
    else if (MSG_Received == Trig2_OFF) digitalWrite(RX_TRIG_2, LOW);
    delay(50); //---only for LED
    digitalWrite(LED_PIN, LOW);
  }
}

void getSentData() { //---Populate data String - MSG_Received
  RX_ReceivedSize = RX_BUFFER[0];
  MSG_Received = "";
  for(i = 1; i < RX_ReceivedSize; i++) {
    MSG_Received += (char)RX_BUFFER[i];
  }
  man.beginReceiveArray(RX_BUFFER_SIZE, RX_BUFFER);
}
