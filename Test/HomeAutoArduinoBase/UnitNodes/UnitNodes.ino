
//---Unit Settings
unsigned int thisUnit = 1; //---( 1-5 )

//---=== NOT USED YET ===---
//---Pins (D2-7) (A0-3)
//--- 0 - Not connected
//--- 1 - Active HIGH Relay
//--- 2 - Active LOW Relay
//--- 3 - Analogue OUTPUT
//--- 4 - Analogue INPUT
//const int pinLayout[] = {2,3,4,5,6,7,A0,A1,A2,A3};
//const int pinType[] = {0,0,2,2,0,0,0,0,0,0};
//---=== NOT USED YET ===---

const int relayPin1 = 4; //---Active LOW Relay 1
const int relayPin2 = 5; //---Active LOW Relay 2

//====================================================================================
//---Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//====================================================================================
//---nRF24L01 Addresses
const byte homeAddress[6] = "Home0";
const byte unitAddress[][6] = {"Home1","Home2","Home3","Home4","Home5"};

//---Global variables
const int doSerialPrint = 1; //---to print to Serial Monitor 1=True 0=False
char dataReceived[32] = {0};
char dataToSend[32] = {0};
bool checkData = false;

//---create an RF24 object
//---Pins for Pro mini
RF24 radio(11, 10);  // CE, CSN
//---Pins for Uno
//RF24 radio(9, 8);  // CE, CSN

//====================================================================================
void setup()
{
  if (doSerialPrint) Serial.begin(9600);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);

  
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.setRetries(3,15); // delay, count

  //---Pipes swapped compared to the master
  radio.openWritingPipe(homeAddress); //---Sending to Home
  radio.openReadingPipe(1, unitAddress[thisUnit - 1]); //---Receiving own
  radio.startListening(); //---Set as Receiver

//  radio.powerUp();
//  radio.printDetails();

  if (doSerialPrint) Serial.println("Arduino Ready");
}

//====================================================================================
void loop()
{

  //---Read
  if (radio.available()){
    radio.read( &dataReceived, sizeof(dataReceived));
    checkData = true;
    if (doSerialPrint) Serial.print ("Data received:");
    if (doSerialPrint) Serial.print (dataReceived);
    if (doSerialPrint) Serial.println (":");
  }

  //---Evaluate Data
  if (checkData) {
    checkData = false;
    delay(100);

    //---Switch On Relay 1
    if (strcmp(dataReceived,"Relay 1 ON") == 0) {
      if (doSerialPrint) Serial.println ("Do Switch Relay 1 ON");
      strcpy(dataToSend,"Relay 1 is ON");
      if (sendMSGHome()) { //---Reply received - Do event
        digitalWrite(relayPin1, LOW);
      }
      digitalWrite(relayPin1, LOW);
    }

    //---Switch Off Relay 1
    else if (strcmp(dataReceived,"Relay 1 OFF") == 0) {
      if (doSerialPrint) Serial.println ("Do Switch Relay 1 OFF");
      strcpy(dataToSend,"Relay 1 is OFF");
      if (sendMSGHome()) { //---Reply received - Do event
        digitalWrite(relayPin1, HIGH);
      }
      digitalWrite(relayPin1, HIGH);
    }

    //---
  }
  

}

//====================================================================================
bool sendMSGHome () {
  bool sentACK;
  radio.stopListening();
  sentACK = radio.write( &dataToSend, sizeof(dataToSend) );
  radio.startListening();
  if (doSerialPrint) {
    Serial.print ("Data sent to : ");
    int i;
    for(i=0; i<5; i++){
      Serial.write((char)homeAddress[i]);
    }
    Serial.print (" : ");
    Serial.println (dataToSend);
  }
  return sentACK;
}
