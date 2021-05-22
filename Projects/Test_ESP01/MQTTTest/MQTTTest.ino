/*
 * ESP8266 MQTT Client Test
 * Used to subscribe to broker
 *    publish button pressed
 *    subscribe to led switch on
 *    
 * Commands for mosquitto
 *    subscribe for button: mosquitto_sub -d -u coetzee_mqtt -P coetzeemqtt -t MY_ESP8266_BUT
 *    publish to led: mosquitto_pub -d -u coetzee_mqtt -P coetzeemqtt -t MY_ESP8266_LED -m "1"
 * 
 * Can be tested in programming jig
 */

#include <ESP8266WiFi.h> //---ESP8266 WiFi
#include <PubSubClient.h> //---MQTT broker communication

const int ledPin = 2; //---Connect led to pin 3 (GPIO2) (Active LOW)
const int buttonPin = 0; //---Connect button to pin 5 (GPIO0)

//===== WiFi
const char* ssid = "CoetzeeHome"; //---WiFi AP SSID
const char* wifi_password = "OnlyByGrace"; //---WiFi AP Pasword

//===== MQTT
const char* mqtt_server = "192.168.10.3"; //---MQTT broker server IP
const char* mqtt_username = "coetzee_mqtt"; //---MQTT broker username
const char* mqtt_password = "coetzeemqtt"; //---MQTT broker password
const char* clientID = "ESP01"; //---The client id identifies the device

//===== MQTT topics
const char* mqtt_topic_but = "MY_ESP8266_BUT";
const char* mqtt_topic_led = "MY_ESP8266_LED";

//=========================================================================================
//========================================= WiFi ==========================================
//=== Requires:
//--- #include <ESP8266WiFi.h> //---ESP8266 WiFi
//--- const char* ssid = "CoetzeeHome"; //---WiFi AP SSID
//--- const char* wifi_password = "OnlyByGrace"; //---WiFi AP Pasword
//================

WiFiClient wifiClient; //---Initialise Wifi object

/*
 * Determine if WiFi is connected
 * return (Boolean)
 */
bool wifi_connected() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  else {
    return false;
  }
}

/*
 * Connect ESP8266 to WiFi network
 */
void wifi_connect() {
  delay(10);
  Serial.println("Attempting WiFi Connection...");
  Serial.print("Connect to: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifi_password);

  //---Wait until the connection has been confirmed before continuing
  while (!wifi_connected()) {
    delay(500);
    Serial.print(".");
  }

  //---Print IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//=========================================================================================
//========================================= MQTT ==========================================
//=== Requires:
//--- #include <PubSubClient.h> //---MQTT broker communication
//--- const char* mqtt_server = "192.168.10.3"; //---MQTT broker server IP
//--- const char* mqtt_username = "coetzee_mqtt"; //---MQTT broker username
//--- const char* mqtt_password = "coetzeemqtt"; //---MQTT broker password
//--- const char* clientID = "ESP01"; //---The client id identifies the device.
//=== topics (dependant on project)
//--- const char* mqtt_topic_but = "MY_ESP8266_BUT";
//--- const char* mqtt_topic_led = "MY_ESP8266_LED";
//=== add subscriptions to setup (dependant on project)
//--- client.subscribe(mqtt_topic_led);
//================

//---Initialise MQTT Client object (IP, port, WiFiClient)
PubSubClient client(mqtt_server, 1883, wifiClient);

/*
 * Determine if connected to MQTT Broker
 * return (Boolean)
 */
bool mqtt_connected() {
  if (client.connected()) {
    return true;
  }
  else {
    return false;
  }
}

/*
 * function called when MQTT subscription message is received
 */
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    //---turn LED on:
    digitalWrite(ledPin, LOW);
    Serial.println("LED ON");
  } else {
    //---turn LED off:
    digitalWrite(ledPin, HIGH);
    Serial.println("LED OFF");
  }
}

/*
 * Connect to MQTT Broker
 */
void mqtt_connect() {
  //---Loop until connected
  while (!mqtt_connected()) {
    Serial.print("Attempting MQTT connection...");
    //---Attempt to connect
    if (client.connect(clientID, mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT Broker!");
      //---Set Callback to listen for incomming publications
      client.setCallback(mqtt_callback);
    } else {
      // Wait 5 seconds before retrying
      Serial.println("Connection to MQTT Broker failed");
      Serial.println("Attempting again in 5 seconds...");
      delay(5000);
    }
  }
}

/*
 * PUBLISH to the MQTT Broker
 */
void mqtt_publish() {
  if (client.publish(mqtt_topic_but, "Button pressed!")) {
    Serial.println("Message sent!");
  }
  //---If the message failed, try again
  else {
    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
    mqtt_connect();
    delay(10);
    client.publish(mqtt_topic_but, "Button pressed!");
  }
}

//=========================================================================================
//========================================= Setup =========================================

void setup() {
  //---Set Pins
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  //---Switch the LED off to start with
  digitalWrite(ledPin, HIGH);

  //---Begin Serial
  Serial.begin(115200);
  delay(1000); //---wait for serial monitor to start
  Serial.println(); //---new line in case junk has been printed
  
  //---Connect Wifi and MQTT
  wifi_connect();
  mqtt_connect();
  Serial.println("Ready...");

  //---Subscribe to MQTT Topics
  client.subscribe(mqtt_topic_led);
  
}

//=========================================================================================
//========================================= Loop= =========================================

void loop() {

  //---Check incoming messages
  client.loop();
  
  //---Check if the pushbutton is pressed. (LOW on pin GPIO0)
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("Button LOW");
    mqtt_publish();
    delay(500); //---delay to stop button from transmitting multiple times
  }

}
