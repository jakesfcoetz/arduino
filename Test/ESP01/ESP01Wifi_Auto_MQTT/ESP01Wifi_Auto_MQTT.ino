/*
 * ESP01
 * Basic Webserver to switch Pin 2 LED (Active Low)
 * ( Can be used on program jig )
*/

//===== Variables
const int ledPin = 2; //---Connect led to pin 3 (GPIO2) (Active LOW)
const int buttonPin = 0; //---Connect button to pin 5 (GPIO0)
bool ledStatus = HIGH;

//=====MQTT subscription topic declarations (project specific)
const char* mqtt_topic_but = "MY_ESP8266_BUT";
const char* mqtt_topic_led = "MY_ESP8266_LED";
