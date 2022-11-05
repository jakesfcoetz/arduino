void loop()
{
  //---Update Time
  timeKeeper();

  //---Listen for incoming HTML requests
  myWebServer.handleClient();

  //---Check Trigger Schedule
  sched_Trigger();

  //---Trigger Received
  if (trigStatus)
  {
    Serial.print("Triggered Time --> ");
    Serial.print(lastTrig_H);
    Serial.print(":");
    Serial.println(lastTrig_m);
    //---Set run start
    runStatus = HIGH;
    trigTime_start = time_Now;
    //---Switch ON
    digitalWrite(trigPin1, LOW);
    digitalWrite(trigPin2, LOW);
    //---Trigger done
    trigStatus = LOW;
  }

  //---Check Run Status
  checkAction();
}
