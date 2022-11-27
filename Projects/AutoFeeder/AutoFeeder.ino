/*
 * ESP01
 * Basic Webserver to switch Pin 2 LED (Active Low) with MQTT and WEB interface
 * ( Can be used on program jig )*/

//===== Variables
const int trigPin = 2; //---Connect led to pin 3 (GPIO2) (Active LOW)
bool trigStatus = LOW; //---OFF
bool runStatus = LOW; //---Set action run OFF
int trigDuration = 5; //---Trigger Duration per channel in seconds
int lastTrig_H = 0;
int lastTrig_m = 0;
int schedTrig[][4] = {{6,30,7,0}, {16,30,7,0}, {21,30,3,0}}; //---{H,m,duration(s),ISSET}

//===== Time Variables
unsigned long time_Now = 0; //---seconds since boot
unsigned long time_Pre = 0;
unsigned long trigTime_start = 0;
int time_s = 0;
int time_m = 0;
int time_H = 0;
