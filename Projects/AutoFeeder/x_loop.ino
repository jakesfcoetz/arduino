void loop() {

  //---Listen for incoming HTML requests
  myWebServer.handleClient();

  sched_Trigger(); //---Check Trigger Schedule
  
  //---Trigger Received
  if(trigStatus) {
    lastTrig_H = time_H;
    lastTrig_m = time_m;
    Serial.print("Triggered Time --> ");
    Serial.print(lastTrig_H);
    Serial.print(":");
    Serial.println(lastTrig_m);
    //---Set run start
    runStatus = HIGH;
    trigTime_start = time_Now;
    //---Switch ON
    digitalWrite(trigPin, LOW);
    //---Trigger done
    trigStatus = LOW;
  }

  //---Check Run Status
  checkAction();

}
