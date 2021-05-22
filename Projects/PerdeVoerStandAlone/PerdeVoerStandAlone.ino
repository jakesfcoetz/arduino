/*
 * ESP01
 * Auto Feeder Web Server with motor control for 2 ports
*/

//===== Variables
const int trigPin1 = 2; //---Connect motor1 to pin 3 (GPIO2) (Active LOW)
const int trigPin2 = 0; //---Connect motor2 to pin 5 (GPIO0) (Active LOW)
bool trigStatus = LOW; //---OFF
int trigDuration = 5; //---Trigger Duration per channel in seconds
int lastTrig_H = 0;
int lastTrig_m = 0;
int schedTrig[][3] = {{3,0,0}, {10,0,0}}; //---{H,m,ISSET}

//===== Time
unsigned long time_Now = 0;
unsigned long time_Pre = 0;
int time_s = 0;
int time_m = 0;
int time_H = 0;
