/*
 * Add Project specific Web Pages to Web Server
 */
void webserver_setup_unit() {
  myWebServer.on("/unit/trigger", myWebServerHandle_trigger); //---Project specific
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
  html_text += "<body>";
  html_text += "<h1>Web Control - Unit</h1>";
  html_text += "<form name='formUnit' id='formUnit' method='POST' action='/unit'>";
  html_text += "<h3>Trigger</h3>";
  html_text += "<p>Current Time: " + String(time_H) + ":" + String(time_m) + "</p>";
  html_text += "<p>Last Triggered: " + String(lastTrig_H) + ":" + String(lastTrig_m) + "</p>";
  html_text += "Trigger Duration: ";

  if (myWebServer.arg("doSetTrigDur") == "1") { //---Check if Triggrd Duration has been changed
    int new_trig_duration = myWebServer.arg("trig_duration").toInt();
    if ((new_trig_duration > 0) and           //---Min value validation
        (new_trig_duration < 30) and          //---Max value validation
        (new_trig_duration != trigDuration)   //---If different
       ) {
        Serial.println("Trigger Duration set to: " + String(trigDuration));
        trigDuration = new_trig_duration;
    }
  }
  
  html_text += "<input type='text' size=2 name='trig_duration' id='trig_duration' value='" + String(trigDuration) + "'>s";
  html_text += "<input type='hidden' name='doSetTrigDur' id='doSetTrigDur' value=0>";
  html_text += "<input type='submit' class='button_small' value='Set Duration' onclick=document.getElementById('doSetTrigDur').value=1>";
  html_text += "<br>";
  html_text += "<h3>Schedule</h3>";

  for (int i = 0; i < (sizeof(schedTrig) / sizeof(schedTrig[0])); i++ ) {
    if (myWebServer.arg("doSetSched") == "1") { //---Check if sched has been changed
      int new_sched_time_H = myWebServer.arg("time_H" + String(i)).toInt();
      int new_sched_time_m = myWebServer.arg("time_m" + String(i)).toInt();
      if ((new_sched_time_H >= 0) and (new_sched_time_H <= 23) and                          //---Hour Validation
          (new_sched_time_m >= 0) and (new_sched_time_m <= 59) and                          //---Min Validation
          ((new_sched_time_H != schedTrig[i][0]) or (new_sched_time_m != schedTrig[i][1]))  //---If different
          ) {
        schedTrig[i][0] = new_sched_time_H;
        schedTrig[i][1] = new_sched_time_m;
        Serial.println("Shedule " + String(i + 1) + " set to: " + String(new_sched_time_H) + ":" + String(new_sched_time_m));
      }
    }
    
    html_text += String(i + 1);
    html_text += " <input type='text' size=2 name='time_H" + String(i) + "' id='time_H" + String(i) + "' style='text-align:right' value='" + String(schedTrig[i][0]) + "'>";
    html_text += " : ";
    html_text += "<input type='text' size=2 name='time_m" + String(i) + "' id='time_m" + String(i) + "' value='" + String(schedTrig[i][1]) + "'>";
    html_text += "<br>";
  }
  html_text += "<input type='hidden' name='doSetSched' id='doSetSched' value=0>";
  html_text += "<input type='submit' class='button_small' value='Set Schedule' onclick=document.getElementById('doSetSched').value=1>";
  html_text += "</form><br>";
  html_text += "<a class=\"button_large button\" href=\"/unit/trigger\">Trigger</a>";
  html_text += "<a class=\"button_large\" href=\"/\">Home</a>";
  html_text += "</body>";
  html_text += "</html>";
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
