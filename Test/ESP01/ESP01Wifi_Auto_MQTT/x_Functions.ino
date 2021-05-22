/*
 * Subscribe to MQTT Topics
 */
void mqtt_subscriptions() {
  client.subscribe(mqtt_topic_led);
}

/*
 * function called when MQTT subscription message is received
 */
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

/*
 * Add Project specific Web Pages to Web Server
 */
void webserver_setup_unit() {
  myWebServer.on("/unit/ledon", myWebServerHandle_ledon); //---Project specific
  myWebServer.on("/unit/ledoff", myWebServerHandle_ledoff); //---Project specific
}

/*
 * URL /unit/ledon
 */
void myWebServerHandle_ledon() {
  ledStatus = LOW;
  Serial.println("Status: ON");
  myWebServer.send(200, "text/html", html_Unit(ledStatus)); 
}

/*
 * URL /ledoff
 */
void myWebServerHandle_ledoff() {
  ledStatus = HIGH;
  Serial.println("Status: OFF");
  myWebServer.send(200, "text/html", html_Unit(ledStatus)); 
}

/*
 * Create HTML String
 * requires (Any value needed to create HTML text)
 * return (String)
 */
String html_Unit(uint8_t ledstat) {
  String html_text = html_Head();
  html_text +="<body>\n";
  html_text +="<h1>Web Control</h1>\n";
  html_text +="<h3>Switch LED</h3>\n";
    
  if(!ledstat)
    {html_text +="<p>LED Status: ON</p><a class=\"button_large button-off\" href=\"/unit/ledoff\">OFF</a>\n";}
  else
    {html_text +="<p>LED Status: OFF</p><a class=\"button_large button-on\" href=\"/unit/ledon\">ON</a>\n";}

  html_text +="<a class=\"button_large\" href=\"/\">Home</a>\n";
  html_text +="</body>\n";
  html_text +="</html>\n";
  return html_text;
}
