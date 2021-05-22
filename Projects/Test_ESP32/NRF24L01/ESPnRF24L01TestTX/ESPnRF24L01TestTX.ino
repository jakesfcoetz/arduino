/*
 * test nrf24L01 connected to ESP32 Wroom 
 * ESP Transmit Pinout
 * MOSI -> D23
 * CE   -> D22
 * CSN  -> D21
 * MISO -> D19
 * SCK  -> D18
*/

//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(22, 21);  //--- CE, CSN --- ESP32
//RF24 radio(9, 8);  //--- CE, CSN --- Uno

//address through which two modules communicate.
const byte address[6] = "Home1";

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate( RF24_2MBPS  );
  radio.setChannel(76);
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(address);
  radio.stopListening();
  radio.printDetails();
  Serial.println("ESP TX Ready");
}

void loop()
{
  Serial.println("send ...");
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  delay(1000);
}
