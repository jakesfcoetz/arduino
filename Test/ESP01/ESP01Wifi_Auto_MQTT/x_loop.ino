void loop() {
  
  //---Listen for incomming HTML requests
  myWebServer.handleClient();

  //---Update LED Status
  if(ledStatus)
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW);

  //---MQTT
  client.loop(); //---Check incoming MQTT messages
  unsigned long currentTime = millis();
  if (currentTime - previousTime_MQTT >= intervalTime_MQTT) {
    if (!mqtt_connected()) {
      mqtt_connect();
    }
    previousTime_MQTT = currentTime;
  }

  //---Check if the pushbutton is pressed. (LOW on pin GPIO0)
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("Button LOW");
    mqtt_publish(mqtt_topic_but, "Button pressed!");
    delay(500); //---delay to stop button from transmitting multiple times
  }
  
}
