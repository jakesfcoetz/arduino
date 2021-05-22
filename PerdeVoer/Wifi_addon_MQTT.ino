//=========================================================================================
//========================================= MQTT ==========================================
//=========================================================================================

//=================================== Available functions =================================
//----- bool mqtt_connected()
//----- void mqtt_callback(char* topic, byte* payload, unsigned int length)
//----- bool mqtt_connect()
//----- bool mqtt_check_connection()
//----- void mqtt_publish(const char* mqtt_pub_toppic, const char* mqtt_pub_payload)
//=================================== Test on MQTT Server =================================
//----- mosquitto_sub -d -u coetzee_mqtt -P coetzeemqtt -t Voer/Perde/Done
//----- mosquitto_pub -d -u coetzee_mqtt -P coetzeemqtt -t Voer/Perde/Trig -m "1"
//=========================================================================================
//=========================================================================================

#include <PubSubClient.h> //---MQTT broker communication
const char* mqtt_server = "192.168.10.3"; //---MQTT broker server IP
const char* mqtt_username = "coetzee_mqtt"; //---MQTT broker username
const char* mqtt_password = "coetzeemqtt"; //---MQTT broker password
const char* mqttClientID = Unit_MAC_ID; //---The client id identifies the device.

const unsigned long intervalTime_checkMQTT = (1000 * 60 * 5); //---Interval in mil sec (1000 = 1s)
unsigned long previousTime_checkMQTT = 0;

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
    //---Set Callback to listen for incomming publications
    client.setCallback(mqtt_callback);
    mqtt_subscriptions();
    return true;
  } else {
    Serial.println("failed");
    return false;
  }
}

/*
 * Check Connction to MQTT Broker
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
void mqtt_publish(const char* mqtt_pub_toppic, const char* mqtt_pub_payload) {
  if (client.publish(mqtt_pub_toppic, mqtt_pub_payload)) {
    Serial.println("Message sent!");
  }
  //---If the message failed, try again
  else {
    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
    mqtt_connect();
    delay(10);
    client.publish(mqtt_pub_toppic, mqtt_pub_payload);
  }
}
