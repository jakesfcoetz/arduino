//=========================================================================================
//================================== WiFi / Web Server ====================================
//=========================================================================================

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>     //---Webserver library for ESP8266 (Built on ESP8266WiFi.h)
ESP8266WebServer myWebServer(80); //---Initialise WebServer on port 80

//=========================================================================================
//========================================= WiFi ==========================================

WiFiClient wifiClient; //---Initialise Wifi object

/*
 * Start Wifi Connect Sequence
 */
void wifi_start()
{
  WiFi.mode(WIFI_AP); //---WIFI_STA (1) / (WIFI_AP (2) / WIFI_AP_STA (3) / WIFI_OFF)
  if (wifi_create_ap())
  {
    Serial.print("AP Created with IP: ");
    Serial.println(WiFi.softAPIP());
    webserver_setup();
  }
  else
  {
    Serial.println("AP failed");
  }
}

/*
 * Connect ESP8266 to WiFi network
 */
bool wifi_create_ap()
{
  //---Default IP: 192.168.4.1
  const char *ap_ssid = "CoetzeeHorseFeeder";
  return (WiFi.softAP(ap_ssid));
}

//=========================================================================================
//====================================== Web Server =======================================

/*
 * Setup Web Sever with URL event handling
 * Call in setup {}
 */
void webserver_setup()
{
  //--- JSON
  myWebServer.on("/api/get", HTTP_GET, myWebServerHandle_get);
  myWebServer.on("/api/set", HTTP_POST, myWebServerHandle_set);
  myWebServer.on("/api/trig", HTTP_GET, myWebServerHandle_trig);
  myWebServer.onNotFound(sendCrossOriginHeader); //--- All other including OPTIONS requests
  myWebServer.begin();                           //---Start WEB server
  Serial.println("Web Server started");
}

/*
 * Helper Functions
 */

//--- CORS Headers
void corsHeaders()
{
  myWebServer.sendHeader(F("access-control-allow-credentials"), F("false"));
  myWebServer.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
  myWebServer.sendHeader(F("Access-Control-Max-Age"), F("600"));
  myWebServer.sendHeader(F("Access-Control-Allow-Methods"), F("PUT,POST,GET,OPTIONS"));
  myWebServer.sendHeader(F("Access-Control-Allow-Headers"), F("*"));
}

//--- Respond to requests
void httpRespondSettings()
{
  response = createJSONResponse();
  corsHeaders();
  myWebServer.send(200, "application/json", response);
  Serial.println("====================================");
}

/*
 * Functions for URL handlers
 */

void sendCrossOriginHeader()
{
  Serial.println("Send Options");
  corsHeaders();
  myWebServer.send(204);
}

void myWebServerHandle_get()
{
  Serial.println("Request: Get Settings");
  httpRespondSettings();
}

void myWebServerHandle_set()
{
  Serial.println("Request: Set Settings");
  bodyParse(myWebServer.arg("plain"));
  httpRespondSettings();
}

void myWebServerHandle_trig()
{
  Serial.println("Request: Trigger");
  setToTrig();
  httpRespondSettings();
}
