//=========================================================================================
//========================================= MQTT ==========================================
//=========================================================================================

//======================================= Requires ========================================
//----- wifiClient - from ESP_Wifi_Web.ino module
/*
  //---MQTT subscription topic declarations (project specific)
  const char* mqtt_topic_but = "MY_ESP8266_BUT";
  const char* mqtt_topic_led = "MY_ESP8266_LED";
*/
//======================================== Loop {} ========================================
/*
  //---MQTT
  mqtt_check_connection();

  //---Check if the pushbutton is pressed. (LOW on pin GPIO0)
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("Button LOW");
    mqtt_publish(mqtt_topic_but, "Button pressed!");
    delay(500); //---delay to stop button from transmitting multiple times
  }
  
*/
//=================================== create functions ====================================
/*
  //---Subscribe to MQTT Topics
  void mqtt_subscriptions() { //---(content =  project specific)
    client.subscribe(mqtt_topic_led);
  }

  
  //---What to do when MQTT subscription message is received
  void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    char myPayload[length]; //---convert byte array to char array
    Serial.print("Message received [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i=0;i<length;i++) {
      myPayload[i] = (char)payload[i];
      Serial.print((char)payload[i]);
    }
    Serial.println();
    
    if (strcmp(myPayload,"1")==0) {
      //---turn LED on:
      ledStatus = LOW;
      Serial.println("LED ON");
    } else {
      //---turn LED off:
      ledStatus = HIGH;
      Serial.println("LED OFF");
    }
  }

*/
//=================================== Available functions =================================
//----- bool mqtt_connected()
//----- void mqtt_callback(char* topic, byte* payload, unsigned int length)
//----- bool mqtt_connect()
//----- bool mqtt_check_connection()
//----- void mqtt_publish(const char* mqtt_pub_topic, const char* mqtt_pub_payload)
//=================================== Test on MQTT Server =================================
//----- mosquitto_sub -d -u coetzee_mqtt -P coetzeemqtt -t MY_ESP8266_BUT
//----- mosquitto_pub -d -u coetzee_mqtt -P coetzeemqtt -t MY_ESP8266_LED -m "0"
//=========================================================================================
//=========================================================================================

#include <PubSubClient.h> //---MQTT broker communication
const char* mqtt_server = "192.168.10.3"; //---MQTT broker server IP
const char* mqtt_username = "coetzee_mqtt"; //---MQTT broker username
const char* mqtt_password = "coetzeemqtt"; //---MQTT broker password
const char* mqttClientID = "ESP01"; //---The client id identifies the device.

const unsigned long intervalTime_MQTT = (1000 * 1); //---Interval in mil sec (1000 = 1s)
unsigned long previousTime_MQTT = 0;

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
 * Connect to MQTT Broker
 */
bool mqtt_connect() {
  Serial.print("Attempting MQTT connection...");
  if (client.connect(mqttClientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected");
    //---Set Callback to listen for incoming publications
    client.setCallback(mqtt_callback);
    mqtt_subscriptions();
    return true;
  } else {
    Serial.println("failed");
    return false;
  }
}

/*
 * Check Connection to MQTT Broker
 */
bool mqtt_check_connection() {
  client.loop(); //---Check incoming MQTT messages
  unsigned long currentTime = millis();
  if (currentTime - previousTime_checkMQTT >= intervalTime_checkMQTT) {
    if (!mqtt_connected()) {
      mqtt_connect();
    }
    previousTime_checkMQTT = currentTime;
  }
}

/*
 * PUBLISH to the MQTT Broker
 */
void mqtt_publish(const char* mqtt_pub_topic, const char* mqtt_pub_payload) {
  if (client.publish(mqtt_pub_topic, mqtt_pub_payload)) {
    Serial.println("Message sent!");
  }
  //---If the message failed, try again
  else {
    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
    mqtt_connect();
    delay(10);
    client.publish(mqtt_pub_topic, mqtt_pub_payload);
  }
}
