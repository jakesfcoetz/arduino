//=========================================================================================
//===================================== Time Keeper =======================================
//=========================================================================================

//======================================= Requires ========================================
//----- wifiClient - from ESP_Wifi_Web.ino module
//======================================== Setup {} =======================================
/*
  timeClient.begin(); //---Set NTP time client
  timeNTPUpdate(); //---Update time
*/
//======================================== Loop {} ========================================
/*
  //---Update Time
  timeKeeper();
  timeClient.update();
*/
//=================================== create functions ====================================
/*
  //---Create or add to existing function
  void webserver_setup_unit() {
    myWebServer.on("/time", myWebServerHandle_time);
 }
*/
//=================================== Available functions =================================
//----- timeKeeper()
//=========================================================================================
//=========================================================================================

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7200);

unsigned long time_Now = 0;
unsigned long time_Pre = 0;
int time_s = 0;
int time_m = 0;
int time_H = 0;

const unsigned long intervalTime = (1000 * 60 * 60); //---Interval in mil sec (1000 = 1s)
unsigned long previousTime = 0;

/*
 * TimeKeeper
 */
void timeKeeper() {
  time_Now = millis(); // the number of milliseconds that have passed since boot
  time_s = (time_Now/1000) - time_Pre; //the number of seconds that have passed since the last time 60 seconds was reached.
  
  if (time_s >= 60) { // Every minute
    time_Pre = time_Pre + 60;
    time_m += 1;
//    Serial.println("Current Time - " + String(time_H) + ":" + String(time_m) + ":" + String(time_s) );
  }
  
  if (time_m >= 60){ // Every hour
    time_m = time_m % 60;
    time_H += 1;
  }
  
  if (time_H == 24)
    time_H = 0;

  if (time_Now - previousTime >= intervalTime) { // Run NTP Update every 'intervalTime'
//    timeNTPUpdate();
    previousTime = time_Now;
  }
}

/*
 * Get Time from NTP
 */
void timeNTPUpdate() {
  timeClient.update();
  time_H = timeClient.getHours();
  time_m = timeClient.getMinutes();
  time_s = timeClient.getSeconds();
  Serial.println("Time set to NTP time: " + timeClient.getFormattedTime());
}

/*
 * URL /time
 */
 void myWebServerHandle_time() {
  myWebServer.send(200, "text/html", html_Time()); 
}

/*
 * URL /gettime
 */
void myWebServerHandle_gettime() {
  timeNTPUpdate();
  myWebServer.send(200, "text/html", html_Time()); 
}

/*
 * Create HTML String
 * requires (Any value needed to create HTML text)
 * return (String)
 */
String html_Time() {
  String html_text = html_Head();
  html_text +="<body>\n";
  html_text +="<h1>Web Control - Time</h1>\n";
  html_text +="<h3>Time</h3>\n";

  if (myWebServer.arg("doSetTime") == "1") { //---Check if should Connect
    int new_time_H = myWebServer.arg("time_H").toInt();
    int new_time_m = myWebServer.arg("time_m").toInt();
    if ((new_time_H >= 0) and (new_time_H <= 23) and (new_time_m >= 0) and (new_time_m <= 59)) {
      time_H = new_time_H;
      time_m = new_time_m;
      Serial.print("Time set to: ");
      Serial.print(time_H);
      Serial.print(":");
      Serial.println(time_m);
    }
  }
  
  html_text +="<form name='formTime' id='formTime' method='POST' action='/time'>\n";
  html_text +="<input type='text' size=2 name='time_H' id='time_H' value='";
  html_text += time_H;
  html_text +="'> : ";
  html_text +="<input type='text' size=2 name='time_m' id='time_m' value='";
  html_text += time_m;
  html_text += "'>";
  html_text +="<input type='hidden' name='doSetTime' id='doSetTime' value=0>\n";
  html_text +="<input type='submit' class='button_small' value='Set Time' onclick=document.getElementById('doSetTime').value=1>\n";
  html_text +="</form><br>\n";
  html_text +="<a class=\"button_large button\" href=\"/gettime\">Get Time</a>\n";
  html_text +="<a class=\"button_large\" href=\"/\">Home</a>\n";
  html_text +="</body>\n";
  html_text +="</html>\n";
  return html_text;
}
