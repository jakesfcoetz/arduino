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
  }
  else
  {
    Serial.println("AP failed");
  }
  webserver_setup();
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
  myWebServer.on("/api/get", HTTP_OPTIONS, sendCrossOriginHeader);
  myWebServer.on("/api/set", HTTP_POST, myWebServerHandle_set);
  myWebServer.on("/api/set", HTTP_OPTIONS, sendCrossOriginHeader);
  myWebServer.on("/api/trig", HTTP_GET, myWebServerHandle_trig);
  myWebServer.on("/api/trig", HTTP_OPTIONS, sendCrossOriginHeader);
  //--- HTML
  myWebServer.on("/", myWebServerHandle_Root);
  myWebServer.on("/unit", myWebServerHandle_unit);
  myWebServer.on("/time", myWebServerHandle_time);
  myWebServer.on("/unit/trigger", myWebServerHandle_trigger);
  myWebServer.onNotFound(myWebServerHandle_Root);
  myWebServer.begin(); //---Start WEB server
  Serial.println("Web Server started");
}

/*
 * Functions for URL handlers
 */
//--- HTML
void myWebServerHandle_Root()
{
  myWebServer.send(200, "text/html", html_Root());
}

void myWebServerHandle_unit()
{
  myWebServer.send(200, "text/html", html_Unit());
}

void myWebServerHandle_time()
{
  myWebServer.send(200, "text/html", html_Time());
}

void myWebServerHandle_trigger()
{
  setToTrig();
  myWebServer.send(200, "text/html", html_Unit());
}

//--- CORS
void corsHeaders()
{
  myWebServer.sendHeader(F("access-control-allow-credentials"), F("false"));
  myWebServer.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
  myWebServer.sendHeader(F("Access-Control-Max-Age"), F("600"));
  myWebServer.sendHeader(F("Access-Control-Allow-Methods"), F("PUT,POST,GET,OPTIONS"));
  myWebServer.sendHeader(F("Access-Control-Allow-Headers"), F("*"));
}

void sendCrossOriginHeader()
{
  corsHeaders();
  myWebServer.send(204);
  Serial.println("Send Options");
  Serial.println("==================================================================");
}

//---JSON Only
void myWebServerHandle_get()
{
  response = createJSONResponse();
  Serial.println("Get Response: " + response);
  Serial.println("==================================================================");
  corsHeaders();
  myWebServer.send(200, "application/json", response);
}

void myWebServerHandle_set()
{
  Serial.println("===== Update Settings ============================================");
  Serial.println("Request: " + myWebServer.arg("plain"));
  bodyParse(myWebServer.arg("plain"));
  response = createJSONResponse();
  Serial.println("Response: " + response);
  Serial.println("==================================================================");
  corsHeaders();
  myWebServer.send(200, "application/json", response);
}

void myWebServerHandle_trig()
{
  setToTrig();
  response = createJSONResponse();
  Serial.println("Trig Response: " + response);
  Serial.println("==================================================================");
  corsHeaders();
  myWebServer.send(200, "application/json", response);
}
