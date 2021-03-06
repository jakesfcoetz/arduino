/*
 * ESP01
 * Basic Webserver to switch Pin 2 LED (Active Low)
 * ( Can be used on program jig )
*/

//===== Variables
const int trigPin = 2; //---Connect led to pin 3 (GPIO2) (Active LOW)
bool trigStatus = LOW; //---OFF
int lastTrig_H = 0;
int lastTrig_m = 0;
int schedTrig[][3] = {{6,0,0}, {14,0,0}, {22,0,0}}; //---{H,m,ISSET}

//=====MQTT subscription topic declarations (project specific)
const char* mqtt_topic_trig = "Voer/Perde/Trig";
