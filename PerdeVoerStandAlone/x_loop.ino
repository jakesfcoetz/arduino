void loop() {

  //---Update Time
  timeKeeper();
  
  //---Listen for incomming HTML requests
  myWebServer.handleClient();

  //---Check Trigger Schedule
  sched_Trigger();
  
  //---Trigger Received
  if(trigStatus) {
    lastTrig_H = time_H;
    lastTrig_m = time_m;
    Serial.print("Triggered Time --> ");
    Serial.print(lastTrig_H);
    Serial.print(":");
    Serial.println(lastTrig_m);
    digitalWrite(trigPin1, LOW);
    digitalWrite(trigPin2, LOW);
    delay(trigDuration * 1000);
    digitalWrite(trigPin1, HIGH);
    digitalWrite(trigPin2, HIGH);
    trigStatus = LOW;
  }

}
