/*
Arduino Turn LED (13) On/Off using Software Serial Comms

Possible string values:
a (to turn the LED on)
b (tor turn the LED off)
*/

//---Includes
#include <SoftwareSerial.h>


char junk;
String inputString="";
SoftwareSerial MyBlue(2, 3); // RX | TX 


void setup()                    // run once, when the sketch starts
{
 Serial.begin(9600);           // set the baud rate to 9600, same should be of your Serial Monitor
 MyBlue.begin(9600); 
 pinMode(13, OUTPUT);
 Serial.println("Ready");
}

void loop()
{
  if(MyBlue.available()){
    Serial.println("Read");
    while(MyBlue.available()) {
      char inChar = (char)MyBlue.read(); //read the input
      inputString += inChar;        //make a string of the characters coming on serial
    }
    Serial.println(inputString);
    if (inputString == "a") {         //in case of 'a' turn the LED on
      Serial.println("turn the LED on");
      digitalWrite(13, HIGH);  
    } else if (inputString == "b") {   //incase of 'b' turn the LED off
      Serial.println("turn the LED off");
      digitalWrite(13, LOW);
    } else {
      Serial.println("Dont know what to do");
    }
    inputString = "";
  }
}
