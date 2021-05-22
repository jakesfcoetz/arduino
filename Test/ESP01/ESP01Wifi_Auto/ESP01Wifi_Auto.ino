/*
 * ESP01
 * Basic Webserver to switch Pin 2 LED (Active Low)
 * ( Can be used on program jig )
*/

//===== Variables
int ledPin = 2;
bool ledStatus = HIGH;
const char* ledState = "OFF";

const unsigned long intervalTime = 1000; //---Interval in mil sec (1000 = 1s)
unsigned long previousTime = 0;
