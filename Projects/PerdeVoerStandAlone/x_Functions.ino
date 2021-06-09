/*
 * TimeKeeper
 */
void timeKeeper()
{
  time_Now = (millis() / 1000); //--- the number of milliseconds that have passed since boot
  time_s = time_Now - time_Pre; //--- the number of seconds that have passed since the last time 60 seconds was reached.

  if (time_s >= 60)
  {
    time_Pre = time_Pre + 60;
    time_m += 1;
  }

  if (time_m >= 60)
  {
    time_m = time_m % 60;
    time_H += 1;
  }

  if (time_H == 24)
    time_H = 0;
}

/*
 * Check Schedule to Trigger
 */
void sched_Trigger()
{
  for (int i = 0; i < (sizeof(schedTrig) / sizeof(schedTrig[0])); i++)
  {
    if ((time_H == schedTrig[i][0]) and (time_m == schedTrig[i][1]))
    { //---Trigger Time is Now
      if (schedTrig[i][2] == 0)
      {                      //---Check if not yet Done, Do Trigger
        schedTrig[i][2] = 1; //---Mark as Done
        trigStatus = HIGH;
      }
    }
    else
    { //---Not Trigger Time, set as Not Done
      if (schedTrig[i][2] == 1)
        schedTrig[i][2] = 0;
    }
  }
}

/*
 * Check if feeding sequence should still be running
 */
void checkAction()
{
  if (runStatus)
  { //---Still Running
    if ((time_Now - trigTime_start) > trigDuration)
    { //---if time done, switch off
      Serial.println("Switch Off");
      runStatus = LOW;
      digitalWrite(trigPin1, HIGH);
      digitalWrite(trigPin2, HIGH);
    }
  }
}

/*
 * Update Time
 */
void updateTime(int new_time_H, int new_time_m)
{
  if ((new_time_H >= 0) and (new_time_H <= 23) and (new_time_m >= 0) and (new_time_m <= 59))
  {
    time_H = new_time_H;
    time_m = new_time_m;
    Serial.print("Time set to: ");
    Serial.print(time_H);
    Serial.print(":");
    Serial.println(time_m);
  }
}

/*
 * Setup trigger flag to start trigger sequence
 */
void setToTrig()
{
    lastTrig_H = time_H;
    lastTrig_m = time_m;
    trigStatus = HIGH;
}

/*
 * Update Trigger Duration
 */
void updateTrigDur(int new_trig_duration)
{
  if ((new_trig_duration > 0) and         //---Min value validation
      (new_trig_duration < 30) and        //---Max value validation
      (new_trig_duration != trigDuration) //---If different
  )
  {
    Serial.println("Trigger Duration set to: " + String(trigDuration));
    trigDuration = new_trig_duration;
  }
}

/*
 * Update Schedule ---<> still to do
 */
void updateSched()
{
  for (int i = 0; i < (sizeof(schedTrig) / sizeof(schedTrig[0])); i++)
  {
    int new_sched_time_H = 0;                                                            //---<> Get New Val
    int new_sched_time_m = 0;                                                            //---<> Get New Val
    if ((new_sched_time_H >= 0) and (new_sched_time_H <= 23) and                         //---Hour Validation
        (new_sched_time_m >= 0) and (new_sched_time_m <= 59) and                         //---Min Validation
        ((new_sched_time_H != schedTrig[i][0]) or (new_sched_time_m != schedTrig[i][1])) //---If different
    )
    {
      schedTrig[i][0] = new_sched_time_H;
      schedTrig[i][1] = new_sched_time_m;
      Serial.println("Shedule " + String(i + 1) + " set to: " + String(schedTrig[i][0]) + ":" + String(schedTrig[i][1]));
    }
  }
}

/*
 * Create JSON Response
 * {
 *   response: boolean,
 *   data: {
 *     curTime: string = (hh:mm)
 *     sched: string[] = [(hh:mm), (hh:mm), ...]
 *     trigDur: number
 *     lastTrig: string = (hh:mm)
 *   }
 * }
 */

String createJSONResponse()
{
  String jsonResponse = "{ \"response\": \"true\", \"data\": {";
  sprintf(tempTimeVar, "%02d:%02d", time_H, time_m);
  jsonResponse += "\"curTime\": \"" + String(tempTimeVar) + "\", ";
  jsonResponse += "\"sched\": [";
  sprintf(tempTimeVar, "%02d:%02d", schedTrig[0][0], schedTrig[0][1]);
  jsonResponse += "\"" + String(tempTimeVar) + "\", ";
  sprintf(tempTimeVar, "%02d:%02d", schedTrig[1][0], schedTrig[1][1]);
  jsonResponse += "\"" + String(tempTimeVar) + "\" ";
  jsonResponse += "], ";
  jsonResponse += "\"trigDur\": " + String(trigDuration) + ", ";
  sprintf(tempTimeVar, "%02d:%02d", lastTrig_H, lastTrig_m);
  jsonResponse += "\"lastTrig\": \"" + String(tempTimeVar) + "\" ";
  jsonResponse += "}}";
  return jsonResponse;
}
