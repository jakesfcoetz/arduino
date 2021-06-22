/*
 * TimeKeeper
 */
void timeKeeper() {
  time_Now = (millis()/1000); //--- the number of milliseconds that have passed since boot
  time_s = time_Now - time_Pre; //--- the number of seconds that have passed since the last time 60 seconds was reached.
  
  if (time_s >= 60) {
    time_Pre = time_Pre + 60;
    time_m += 1;
  }
  
  if (time_m >= 60){ 
    time_m = time_m % 60;
    time_H += 1;
  }
  
  if (time_H == 24)
    time_H = 0;

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

/*
 * Check if feeding sequence should still be running
 */
void checkAction() {
  if (runStatus) { //---Still Running
    if ((time_Now - trigTime_start) > trigDuration) { //---if time done, switch off
      Serial.println("Switch Off");
      runStatus = LOW;
      digitalWrite(trigPin1, HIGH);
      digitalWrite(trigPin2, HIGH);
    }
  }
}
