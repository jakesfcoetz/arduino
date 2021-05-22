void loop() {

  //---Update Time
  timeKeeper();
  
  //---Listen for incomming HTML requests
  myWebServer.handleClient();

  //---MQTT
  mqtt_check_connection();

  sched_Trigger(); //---Check Trigger Schedule
  
  //---Trigger Received
  if(trigStatus) {
    lastTrig_H = time_H;
    lastTrig_m = time_m;
    Serial.print("Triggered Time --> ");
    Serial.print(lastTrig_H);
    Serial.print(":");
    Serial.println(lastTrig_m);
    digitalWrite(trigPin, LOW);
    delay(3000);
    digitalWrite(trigPin, HIGH);
    trigStatus = LOW;
  }

}
