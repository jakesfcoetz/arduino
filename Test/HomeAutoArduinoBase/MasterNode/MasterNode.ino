//---Include Libraries
#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//---nRF24L01 Addresses
const byte homeAddress[6] = "Home0";
const byte unitAddress[][6] = {"Home1","Home2","Home3","Home4","Home5"};

//---Global variables
String blueInputString = ""; //---Data received from Bluetooth
SoftwareSerial MyBlue(2, 3); //---Bluetooth Software Serial Pins (RX, TX) 
int doSerialPrint = 1; //---to print to Serial Monitor 1=True 0=False
char dataReceived[32] = ""; //---Data Received
char dataToSend[32] = ""; //---Data to Send
const int rfSendLED = 5;

//---create an RF24 object
//---Pins for Pro mini
RF24 radio(11, 10);  // CE, CSN
//---Pins for Uno
//RF24 radio(9, 8);  // CE, CSN

//====================================================================================
void setup()
{
  if (doSerialPrint) Serial.begin(9600);

  pinMode(rfSendLED, OUTPUT);
  digitalWrite(rfSendLED, LOW);
  
  MyBlue.begin(9600); 
  
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.setRetries(5,15); // delay, count
  radio.openReadingPipe(1, homeAddress); //---Receiving own
  radio.startListening(); //---Set as Receiver
//  radio.powerUp();
//  radio.printDetails();
  
  if (doSerialPrint) Serial.println("Arduino Ready");
}

//====================================================================================
void loop()
{
  
  //---Read Wifi network
//  if (radio.available()) {
//    radio.read( &dataReceived, sizeof(dataReceived));
//    if (doSerialPrint) Serial.print ("RF received: ");
//    if (doSerialPrint) Serial.println (dataReceived);
//  }

  if(MyBlue.available()){
    if (doSerialPrint) Serial.print("BT Received :");
    while(MyBlue.available()) {
      char inChar = (char)MyBlue.read(); //---read the input
      blueInputString += inChar;         //---make a string of the characters coming on serial
    }
    if (doSerialPrint) Serial.println(blueInputString);
    if (blueInputString == "a") {         //---in case of 'a' turn the LED on
      if (doSerialPrint) Serial.println("DO turn Relay 1 on");
      strcpy(dataToSend,"Relay 1 ON");
      sendMSG(1);
    } else if (blueInputString == "b") {   //---incase of 'b' turn the LED off
      if (doSerialPrint) Serial.println("DO turn Relay 1 off");
      strcpy(dataToSend,"Relay 1 OFF");
      sendMSG(1);
    } else {
      if (doSerialPrint) Serial.println("Dont know what to do");
    }
    blueInputString = "";
  }

}

//====================================================================================
void sendMSG (int rxAddressID) { //---(Home Unit Number)
  digitalWrite(rfSendLED, HIGH);
  bool sentACK = false;
  int timesTry = 0;
  radio.stopListening();
  radio.openWritingPipe(unitAddress[rxAddressID - 1]);
  if (doSerialPrint) Serial.print ("RF Sending :");
  while ((!sentACK) && (++timesTry <= 3)) {
    if (doSerialPrint) Serial.print (timesTry);
    sentACK = radio.write( &dataToSend, sizeof(dataToSend) );
  }
  if (!sentACK) {
    if (doSerialPrint) Serial.println (" X");
  } else {
    if (doSerialPrint) {
      Serial.println (" Sent");
      Serial.print ("Data sent to : ");
      int i;
      for(i=0; i<5; i++){
        Serial.write((char)unitAddress[rxAddressID - 1][i]);
      }
      Serial.print (" : ");
      Serial.println (dataToSend);
    }
  }
  
  radio.startListening();
  digitalWrite(rfSendLED, LOW);
}
