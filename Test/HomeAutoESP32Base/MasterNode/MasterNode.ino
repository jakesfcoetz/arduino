/*
 * ESP32 as Base Unit for Home Automation
 * Triggers (D4, D5) send "Relay ON" / "Relay OFF" through RF
 * -=Pinout=-
 * MOSI -> D23
 * CE   -> D22
 * CSN  -> D21
 * MISO -> D19
 * SCK  -> D18
*/

//====================================================================================
//---Unit Specific Settings
int Trigger1 = 4; //---ESP32 Pin D4
int Trigger2 = 5; //---ESP32 Pin D5
int RF_LED_PIN = 2; //---LED

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
char dataReceived[32] = ""; //---Data Received
char dataToSend[32] = ""; //---Data to Send
bool dataRX = false;

//---create an RF24 object
RF24 radio(22, 21);  // CE, CSN

//====================================================================================
void setup()
{
  //---Unit Pin Setup
  pinMode(Trigger1, INPUT);
  pinMode(Trigger2, INPUT);
  pinMode(RF_LED_PIN, OUTPUT);
  digitalWrite (RF_LED_PIN, LOW);

  //---Radio Setup
  radio.begin();
  radio.setDataRate( RF24_2MBPS );
  radio.setRetries(5,15); // delay, count
  radio.setChannel(76);
  radio.setPALevel(RF24_PA_MAX);

  //---Pipes
  radio.openReadingPipe(1, unitAddress[0]); //---Receiving own
  radio.startListening(); //---Set as Receiver

  //---Serial monitor
  if (doSerialPrint) {
    Serial.begin(9600);
    delay(1000);
    Serial.println("");
    radio.printDetails();
    Serial.println("Home Base Unit Ready");
  }
}

//====================================================================================
void loop()
{
  
  //---Read
  if (radio.available()) {
    radio.read( &dataReceived, sizeof(dataReceived));
    dataRX = true;
    if (doSerialPrint) {
      Serial.print ("RF received <=== ");
      Serial.println (dataReceived);
    }
  }

  //---Trigger 1
  if (digitalRead(Trigger1) == HIGH) {
    if (doSerialPrint) Serial.println("DO switch Relay 1 ON");
    strcpy(dataToSend, "Relay 1 ON" );
    sendMSG(1);
    delay(200);
  }

  //---Trigger 2
  if (digitalRead(Trigger2) == HIGH) {
    if (doSerialPrint) Serial.println("DO switch Relay 1 OFF");
    strcpy(dataToSend,"Relay 1 OFF");
    sendMSG(1);
    delay(200);
  }
  
}

//====================================================================================
bool sendMSG (int rxAddressID) { //---(Home Unit Number)
  bool sentACK;
  digitalWrite (RF_LED_PIN, HIGH);
  radio.stopListening();
  radio.openWritingPipe(unitAddress[rxAddressID]);
  sentACK = radio.write( &dataToSend, sizeof(dataToSend) );
  radio.startListening();
  if (doSerialPrint) {
    Serial.print ("RF Sending ===> ");
    int i;
    for(i=0; i<5; i++){
      Serial.write((char)unitAddress[rxAddressID][i]);
    }
    Serial.print (" : ");
    if (sentACK) { 
      Serial.println (dataToSend);
    } else {
      Serial.println ("Acknowledge not Received");
    }
  }
  digitalWrite (RF_LED_PIN, LOW);
  return sentACK;
}
