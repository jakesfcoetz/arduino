/*
 * test nrf24L01 connected to ESP32 Wroom 
 * Arduino Pro Mini Receiver
 * SCK  -> 13
 * MISO -> 12
 * MOSI -> 11
 * CSN  -> 10
 * CE   -> 8
*/

//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
//---Pins for Pro mini
RF24 radio(8, 10);  // CE, CSN

//---Unit Settings
unsigned int thisUnit = 1; //---( 1-5 )

//address through which two modules communicate.
const byte unitAddress[][6] = {"Home0","Home1","Home2","Home3","Home4","Home5"};

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate( RF24_2MBPS  );
  radio.setChannel(76);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, unitAddress[thisUnit]);
  radio.startListening();
  Serial.println("Pro Mini RX Ready");
}

void loop()
{
  //Read the data if available in buffer
  if (radio.available())
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}
