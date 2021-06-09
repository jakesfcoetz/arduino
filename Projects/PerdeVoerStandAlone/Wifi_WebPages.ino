/*
 * =========================================
 * Create HTML String for HEAD
 * return (String)
 * =========================================
 */
String html_Head()
{
  String htmlHead = "<!DOCTYPE html> <html>\n";
  htmlHead += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  htmlHead += "<title>My Web Control</title>\n";
  htmlHead += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  htmlHead += "body{margin-top: 10px;}\n";
  htmlHead += "h1 {color: #444444;margin: 10px;}\n";
  htmlHead += "h3 {color: #444444;margin: 10px;}\n";
  htmlHead += ".button_large {display: block;width: auto;background-color: #1abc9c;border: none;color: white;padding: 15px;text-decoration: none;font-size: 25px;margin: 10px auto 10px;cursor: pointer;border-radius: 4px;}\n";
  htmlHead += ".button_small {display: block;width: auto;background-color: #1abc9c;border: none;color: white;padding: 5px;text-decoration: none;font-size: 15px;margin: 5px auto 5px;cursor: pointer;border-radius: 4px;}\n";
  htmlHead += ".button {background-color: #1abc9c;}\n";
  htmlHead += ".button:active {background-color: #16a085;}\n";
  htmlHead += "p {font-size: 15px;color: #888;margin-bottom: 10px;}\n";
  htmlHead += "</style>\n";
  htmlHead += "</head>\n";
  return htmlHead;
}

/*
 * =========================================
 * Create HTML String for Root URL "/"
 * return (String)
 * =========================================
 */
String html_Root()
{
  String html_text = html_Head();
  html_text += "<body>\n";
  html_text += "<h1>Coetzee Perde Voer</h1>\n";
  html_text += "<a class=\"button_large\" href=\"/time\">Time</a>\n";
  html_text += "<a class=\"button_large\" href=\"/unit\">Unit</a>\n";
  html_text += "</body>\n";
  html_text += "</html>\n";
  return html_text;
}

/*
 * =========================================
 * Create HTML String for Unit URL "/unit"
 * return (String)
 * =========================================
 */
String html_Unit()
{
  String html_text = html_Head();
  html_text += "<body>";
  html_text += "<h1>Web Control - Unit</h1>";
  html_text += "<form name='formUnit' id='formUnit' method='POST' action='/unit'>";
  html_text += "<h3>Trigger</h3>";
  html_text += "<p>Current Time: " + String(time_H) + ":" + String(time_m) + "</p>";
  html_text += "<p>Last Triggered: " + String(lastTrig_H) + ":" + String(lastTrig_m) + "</p>";
  html_text += "Trigger Duration: ";

  if (myWebServer.arg("doSetTrigDur") == "1")
  { //---Check if Triggered Duration has been changed
    updateTrigDur(myWebServer.arg("trig_duration").toInt());
  }

  html_text += "<input type='text' size=2 name='trig_duration' id='trig_duration' value='" + String(trigDuration) + "'>s";
  html_text += "<input type='hidden' name='doSetTrigDur' id='doSetTrigDur' value=0>";
  html_text += "<input type='submit' class='button_small' value='Set Duration' onclick=document.getElementById('doSetTrigDur').value=1>";
  html_text += "<br>";
  html_text += "<h3>Schedule</h3>";

  for (int i = 0; i < (sizeof(schedTrig) / sizeof(schedTrig[0])); i++)
  {
    if (myWebServer.arg("doSetSched") == "1")
    { //---Check if sched has been changed
      int new_sched_time_H = myWebServer.arg("time_H" + String(i)).toInt();
      int new_sched_time_m = myWebServer.arg("time_m" + String(i)).toInt();
      if ((new_sched_time_H >= 0) and (new_sched_time_H <= 23) and                         //---Hour Validation
          (new_sched_time_m >= 0) and (new_sched_time_m <= 59) and                         //---Min Validation
          ((new_sched_time_H != schedTrig[i][0]) or (new_sched_time_m != schedTrig[i][1])) //---If different
      )
      {
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
 * =========================================
 * Create HTML String for Unit URL "/time"
 * return (String)
 * =========================================
 */
String html_Time()
{
  String html_text = html_Head();
  html_text += "<body>\n";
  html_text += "<h1>Web Control - Time</h1>\n";
  html_text += "<h3>Time</h3>\n";

  if (myWebServer.arg("doSetTime") == "1")
  {
    updateTime(myWebServer.arg("time_H").toInt(), myWebServer.arg("time_m").toInt());
  }

  html_text += "<form name='formTime' id='formTime' method='POST' action='/time'>\n";
  html_text += "<input type='text' size=2 name='time_H' id='time_H' value='";
  html_text += time_H;
  html_text += "'> : ";
  html_text += "<input type='text' size=2 name='time_m' id='time_m' value='";
  html_text += time_m;
  html_text += "'>";
  html_text += "<input type='hidden' name='doSetTime' id='doSetTime' value=0>\n";
  html_text += "<input type='submit' class='button_small' value='Set Time' onclick=document.getElementById('doSetTime').value=1>\n";
  html_text += "</form><br>\n";
  html_text += "<a class=\"button_large\" href=\"/\">Home</a>\n";
  html_text += "</body>\n";
  html_text += "</html>\n";
  return html_text;
}
