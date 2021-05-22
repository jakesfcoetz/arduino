/*
 * ESP01
 * Basic Webserver to switch Pin 2 LED (Active Low)
 * ( Can be used on program jig )
*/

//===== Variables
const int trigPin1 = 2; //---Connect led to pin 3 (GPIO2) (Active LOW)
const int trigPin2 = 0; //---Connect led to pin 5 (GPIO0) (Active LOW)
bool trigStatus = LOW; //---OFF
int trigDuration = 5; //---Trigger Duration per channel in seconds
int lastTrig_H = 0;
int lastTrig_m = 0;
int schedTrig[][3] = {{3,0,0}, {10,0,0}}; //---{H,m,ISSET}
