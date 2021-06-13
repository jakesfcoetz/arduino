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
    {                           //---Trigger Time is Now
      if (schedTrig[i][2] == 0) //---Check if not yet Done, Do Trigger
      {
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
 * Setup trigger flag to start trigger sequence
 */
void setToTrig()
{
  lastTrig_H = time_H;
  lastTrig_m = time_m;
  trigStatus = HIGH;
}

/*
 * Parse settings received via JSON Body
 */
void bodyParse(String body)
{
  DeserializationError error = deserializeJson(doc, body); //--- Parse body string to JSON doc

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  const char *curTime = doc["curTime"];
  const char *sched1 = doc["sched"]["sched1"];
  const char *sched2 = doc["sched"]["sched2"];
  const char *sched3 = doc["sched"]["sched3"];
  const char *sched4 = doc["sched"]["sched4"];
  const char *sched5 = doc["sched"]["sched5"];

  //--- Set Current Time
  updateTime((curTime[0] - '0') * 10 + (curTime[1] - '0'), (curTime[3] - '0') * 10 + (curTime[4] - '0'));

  //--- Update Trigger Duration
  updateTrigDur(doc["trigDur"].as<int>());

  //--- Update New Schedule 1
  if (String(sched1) != "")
  {
    newSchedTrig[0][0] = (sched1[0] - '0') * 10 + (sched1[1] - '0');
    newSchedTrig[0][1] = (sched1[3] - '0') * 10 + (sched1[4] - '0');
  }
  else
  {
    newSchedTrig[0][0] = -1;
    newSchedTrig[0][1] = 0;
  }

  //--- Update New Schedule 2
  if (String(sched2) != "")
  {
    newSchedTrig[1][0] = (sched2[0] - '0') * 10 + (sched2[1] - '0');
    newSchedTrig[1][1] = (sched2[3] - '0') * 10 + (sched2[4] - '0');
  }
  else
  {
    newSchedTrig[1][0] = -1;
    newSchedTrig[1][1] = 0;
  }

  //--- Update New Schedule 3
  if (String(sched3) != "")
  {
    newSchedTrig[2][0] = (sched3[0] - '0') * 10 + (sched3[1] - '0');
    newSchedTrig[2][1] = (sched3[3] - '0') * 10 + (sched3[4] - '0');
  }
  else
  {
    newSchedTrig[2][0] = -1;
    newSchedTrig[2][1] = 0;
  }

  //--- Update New Schedule 4
  if (String(sched4) != "")
  {
    newSchedTrig[3][0] = (sched4[0] - '0') * 10 + (sched4[1] - '0');
    newSchedTrig[3][1] = (sched4[3] - '0') * 10 + (sched4[4] - '0');
  }
  else
  {
    newSchedTrig[3][0] = -1;
    newSchedTrig[3][1] = 0;
  }

  //--- Update New Schedule 5
  if (String(sched5) != "")
  {
    newSchedTrig[4][0] = (sched5[0] - '0') * 10 + (sched5[1] - '0');
    newSchedTrig[4][1] = (sched5[3] - '0') * 10 + (sched5[4] - '0');
  }
  else
  {
    newSchedTrig[4][0] = -1;
    newSchedTrig[4][1] = 0;
  }

  updateSched();
}

/*
 * Update Time
 */
void updateTime(int new_time_H, int new_time_m)
{
  Serial.print("Check Current Time: ");
  if ((new_time_H != time_H) and (new_time_m != time_m))
  {
    if ((new_time_H >= 0) and (new_time_H <= 23) and (new_time_m >= 0) and (new_time_m <= 59))
    {
      time_H = new_time_H;
      time_m = new_time_m;
      Serial.print("set to: ");
      Serial.print(time_H);
      Serial.print(":");
      Serial.println(time_m);
    }
    else
    {
      Serial.println("UNCHANGED - invalid");
    }
  }
  else
  {
    Serial.println("UNCHANGED");
  }
}

/*
 * Update Trigger Duration
 */
void updateTrigDur(int new_trig_duration)
{
  Serial.print("Check Trigger Duration: ");
  if ((new_trig_duration > 0) and         //---Min value validation
      (new_trig_duration < 30) and        //---Max value validation
      (new_trig_duration != trigDuration) //---If different
  )
  {
    trigDuration = new_trig_duration;
    Serial.println("set to: " + String(trigDuration));
  }
  else
  {
    Serial.println("UNCHANGED");
  }
}

/*
 * Update Schedule - Only for JSON body parse
 */
void updateSched()
{
  for (int i = 0; i < (sizeof(schedTrig) / sizeof(schedTrig[0])); i++)
  {
    Serial.print("Check Shedule: " + String(i + 1) + " ");
    Serial.print(String(schedTrig[i][0]) + ":" + String(schedTrig[i][1]));
    Serial.print(" --> ");
    Serial.print(String(newSchedTrig[i][0]) + ":" + String(newSchedTrig[i][1]) + " ");

    if ((newSchedTrig[i][0] != schedTrig[i][0]) or (newSchedTrig[i][1] != schedTrig[i][1])) //---If different
    {
      if ((newSchedTrig[i][0] >= 0) and (newSchedTrig[i][0] <= 23) and //---Hour Validation
          (newSchedTrig[i][1] >= 0) and (newSchedTrig[i][1] <= 59)     //---Min Validation
      )
      {
        Serial.print("UPDATE ");
        schedTrig[i][0] = newSchedTrig[i][0];
        schedTrig[i][1] = newSchedTrig[i][1];
      }
      else
      {
        Serial.print("CLEAR ");
        schedTrig[i][0] = -1;
        schedTrig[i][1] = 0;
      }
      Serial.println("set to: " + String(schedTrig[i][0]) + ":" + String(schedTrig[i][1]));
    }
    else
    {
      Serial.println("UNCHANGED");
    }
  }
}

/*
 * Create JSON Response
 * {
 *   response: boolean,
 *   data: {
 *     curTime: string = (hh:mm)
 *     trigDur: number
 *     lastTrig: string = (hh:mm)
 *     sched: {
 *       sched1: string = (hh:mm)
 *       sched2: string = (hh:mm)
 *       sched3: string = (hh:mm)
 *       sched4: string = (hh:mm)
 *       sched5: string = (hh:mm)
 *     }
 *   }
 * }
 */

String createJSONResponse()
{
  String jsonResponse = "";

  sprintf(tempTimeVar, "%02d:%02d", time_H, time_m);
  doc["curTime"] = tempTimeVar;
  doc["trigDur"] = trigDuration;
  sprintf(tempTimeVar, "%02d:%02d", lastTrig_H, lastTrig_m);
  doc["lastTrig"] = tempTimeVar;
  if (schedTrig[0][0] >= 0)
  {
    sprintf(tempTimeVar, "%02d:%02d", schedTrig[0][0], schedTrig[0][1]);
    doc["sched"]["sched1"] = tempTimeVar;
  }
  else
  {
    doc["sched"]["sched1"] = "";
  }
  if (schedTrig[1][0] >= 0)
  {
    sprintf(tempTimeVar, "%02d:%02d", schedTrig[1][0], schedTrig[1][1]);
    doc["sched"]["sched2"] = tempTimeVar;
  }
  else
  {
    doc["sched"]["sched2"] = "";
  }
  if (schedTrig[2][0] >= 0)
  {
    sprintf(tempTimeVar, "%02d:%02d", schedTrig[2][0], schedTrig[2][1]);
    doc["sched"]["sched3"] = tempTimeVar;
  }
  else
  {
    doc["sched"]["sched3"] = "";
  }
  if (schedTrig[3][0] >= 0)
  {
    sprintf(tempTimeVar, "%02d:%02d", schedTrig[3][0], schedTrig[3][1]);
    doc["sched"]["sched4"] = tempTimeVar;
  }
  else
  {
    doc["sched"]["sched4"] = "";
  }
  if (schedTrig[4][0] >= 0)
  {
    sprintf(tempTimeVar, "%02d:%02d", schedTrig[4][0], schedTrig[4][1]);
    doc["sched"]["sched5"] = tempTimeVar;
  }
  else
  {
    doc["sched"]["sched5"] = "";
  }

  serializeJson(doc, jsonResponse);

  return "{ \"response\": \"true\", \"data\": " + jsonResponse + "}";
}
