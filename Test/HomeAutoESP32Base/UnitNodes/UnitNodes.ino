/*
 * Arduino Pro Mini nrf24L01 Unit for Home Automation
 * -=Pinout=-
 * SCK  -> 13
 * MISO -> 12
 * MOSI -> 11
 * CSN  -> 10
 * CE   -> A0
*/

//====================================================================================
//---Unit Specific Settings
unsigned int thisUnit = 1; //---( 1-5 )
const int relayPin1 = 4; //---Active LOW Relay 1
const int relayPin2 = 5; //---Active LOW Relay 2

bool relay1Status = false;
bool relay2Status = false;

//====================================================================================
//---Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//====================================================================================
//---nRF24L01 Addresses
const byte unitAddress[][6] = {"Home0","Home1","Home2","Home3","Home4","Home5"};

//---Global variables
const int doSerialPrint = 1; //---to print to Serial Monitor 1=True 0=False
char dataReceived[32] = {0};
char dataToSend[32] = {0};

//---create an RF24 object
RF24 radio(A0, 10);  // CE, CSN

//====================================================================================
void setup()
{
  //---Unit Pin Setup
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);

  //---Radio Setup
  radio.begin();
  radio.setDataRate(RF24_2MBPS);
  radio.setRetries(3,15); // delay, count
  radio.setChannel(76);
  radio.setPALevel(RF24_PA_MIN);

  //---Pipes
  radio.openWritingPipe(unitAddress[0]); //---Sending to Home
  radio.openReadingPipe(1, unitAddress[thisUnit]); //---Receiving own
  radio.startListening(); //---Set as Receiver

  //---Serial monitor
  if (doSerialPrint) {
    Serial.begin(9600);
    delay(1000);
    Serial.print("Home Unit ");
    Serial.print(thisUnit);
    Serial.println(" Ready");
  }
}

//====================================================================================
void loop()
{
  //---Set Relay Pins
  if (relay1Status) digitalWrite(relayPin1, LOW); else digitalWrite(relayPin1, HIGH);
  if (relay2Status) digitalWrite(relayPin2, LOW); else digitalWrite(relayPin2, HIGH);
  
  //---Read
  if (radio.available()) {
    radio.read( &dataReceived, sizeof(dataReceived));
    if (doSerialPrint) {
      Serial.print ("RF received <=== ");
      Serial.println (dataReceived);
    }

    //===Evaluate Data===
    
    //---Send Status
    if (strcmp(dataReceived,"Get Status") == 0) {
      if (doSerialPrint) Serial.println ("Send Status");
      sendMSGHome();
    }

    //---Switch On Relay 1
    else if (strcmp(dataReceived,"Relay 1 ON") == 0) {
      if (doSerialPrint) Serial.println ("Do Switch Relay 1 ON");
      relay1Status = true;
      sendMSGHome();
    }

    //---Switch Off Relay 1
    else if (strcmp(dataReceived,"Relay 1 OFF") == 0) {
      if (doSerialPrint) Serial.println ("Do Switch Relay 1 OFF");
      relay1Status = false;
      sendMSGHome();
    }

    //---Switch On Relay 2
    else if (strcmp(dataReceived,"Relay 2 ON") == 0) {
      if (doSerialPrint) Serial.println ("Do Switch Relay 2 ON");
      relay2Status = true;
      sendMSGHome();
    }

    //---Switch Off Relay 2
    else if (strcmp(dataReceived,"Relay 2 OFF") == 0) {
      if (doSerialPrint) Serial.println ("Do Switch Relay 2 OFF");
      relay2Status = false;
      sendMSGHome();
    }

    //---
    
  }
  
}

//====================================================================================
void sendMSGHome () { //---Always send back current status
  dataToSend[0] = 'U';
  dataToSend[1] = '1'; //---UnitNumber
  dataToSend[2] = 'S';
  if (relay1Status) dataToSend[3] = '1'; else dataToSend[3] = '0';
  if (relay2Status) dataToSend[4] = '1'; else dataToSend[4] = '0';
  dataToSend[5] = '\0';
  bool sentACK;
  radio.stopListening();
  sentACK = radio.write( &dataToSend, sizeof(dataToSend) );
  radio.startListening();
  if (doSerialPrint) {
    Serial.print ("RF Sending ===> ");
    int i;
    for(i=0; i<5; i++){
      Serial.write((char)unitAddress[0][i]);
    }
    Serial.print (" : ");
    if (sentACK) { 
      Serial.println (dataToSend);
    } else {
      Serial.println ("Acknowledge not Received");
    }
  }
}
