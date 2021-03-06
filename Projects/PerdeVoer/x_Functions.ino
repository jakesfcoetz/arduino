/*
 * Subscribe to MQTT Topics
 */
void mqtt_subscriptions() {
  client.subscribe(mqtt_topic_trig);
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
    trigStatus = HIGH;
    Serial.println("MQTT Trigger Set");
  }
}

/*
 * Add Project specific Web Pages to Web Server
 */
void webserver_setup_unit() {
  myWebServer.on("/unit/trigger", myWebServerHandle_trigger); //---Project specific
  myWebServer.on("/time", myWebServerHandle_time);
}

/*
 * URL /unit/trigger
 */
void myWebServerHandle_trigger() {
  trigStatus = HIGH;
  myWebServer.send(200, "text/html", html_Unit()); 
}

/*
 * Create HTML String
 * requires (Any value needed to create HTML text)
 * return (String)
 */
String html_Unit() {
  String html_text = html_Head();
  html_text +="<body>\n";
  html_text +="<h1>Web Control</h1>\n";
  html_text +="<h3>Trigger</h3>\n";
  html_text +="<p>Last Triggered: ";
  html_text += lastTrig_H;
  html_text +=":";
  html_text += lastTrig_m;
  html_text +="</p>";
  html_text +="<p>Current Time: ";
  html_text += time_H;
  html_text +=":";
  html_text += time_m;
  html_text +="</p>\n";
  html_text +="<h3>Schedule</h3>\n";
  html_text +="<form name='formSched' id='formSched' method='POST' action='/unit'>\n";
  for (int i = 0; i < (sizeof(schedTrig) / sizeof(schedTrig[0])); i++ ) {
    html_text += i+1;
    html_text +=" <input type='text' size=2 name='time_H";
    html_text += i;
    html_text +="' id='time_H";
    html_text += i;
    html_text +="' style='text-align:right' value='";
    html_text += schedTrig[i][0];
    html_text +="'> : ";
    html_text +="<input type='text' size=2 name='time_m";
    html_text += i;
    html_text +="' id='time_m";
    html_text += i;
    html_text +="' value='";
    html_text += schedTrig[i][1];
    html_text += "'><br>";
  }
  html_text +="<input type='hidden' name='doSetSched' id='doSetSched' value=0>\n";
  html_text +="<input type='submit' class='button_small' value='Set Schedule' onclick=document.getElementById('doSetSched').value=1>\n";
  html_text +="</form><br>\n";
  html_text +="<a class=\"button_large button\" href=\"/unit/trigger\">Trigger</a>\n";
  html_text +="<a class=\"button_large\" href=\"/\">Home</a>\n";
  html_text +="</body>\n";
  html_text +="</html>\n";
  return html_text;
}

/*
 * Check Schedule to Trigger
 */
void sched_Trigger() {
  for ( int i = 0; i < (sizeof(schedTrig) / sizeof(schedTrig[0])); i++ ) {
    if ((time_H == schedTrig[i][0]) and (time_m == schedTrig[i][1])) { //---Trigger Time is Now
      if (schedTrig[i][2] == 0) { //---Check if not yet Done, Do Trigger
        schedTrig[i][2] = 1; //---Mark as Done
        trigStatus = HIGH;
      }
    }
    else { //---Not Trigger Time, set as Not Done
      if (schedTrig[i][2] == 1)
        schedTrig[i][2] = 0;
    }
  } 
}
