//=========================================================================================
//================================== WiFi / Web Server ====================================
//=========================================================================================

//======================================== setup {} =======================================
//Serial.begin(115200); //---Begin Serial
//delay(1000); //---wait for serial monitor to start
//Serial.println(); //---new line in case junk has been printed
//wifi_start_connect(); //---Start Wifi Connect Sequence
//======================================== loop {} ========================================
////---Listen for incomming HTML requests
//myWebServer.handleClient();
//=================================== create functions ====================================
/*
  //---Add Project specific Web Pages to Web Server with following template
  void webserver_setup_unit() { //---(content =  project specific)
    myWebServer.on("/unit/ledon", myWebServerHandle_ledon); //---Project specific
    myWebServer.on("/unit/ledoff", myWebServerHandle_ledoff); //---Project specific
 }
 
  //---Add appropriate functions for when page is called (functions =  project specific)
  void myWebServerHandle_ledon() {
    ledStatus = LOW;
    Serial.println("Status: ON");
    myWebServer.send(200, "text/html", html_Unit(ledStatus)); 
  }

  void myWebServerHandle_ledoff() {
    ledStatus = HIGH;
    Serial.println("Status: OFF");
    myWebServer.send(200, "text/html", html_Unit(ledStatus)); 
  }

  //---Add function to create HTML text
  String html_Unit(uint8_t ledstat) { //---(content =  project specific)
    String html_text = html_Head();
    html_text +="<body>\n";
    html_text +="<h1>ESP8266 Web Control</h1>\n";
    html_text +="<h3>Switch LED</h3>\n";
      
    if(!ledstat)
      {html_text +="<p>LED Status: ON</p><a class=\"button_large button-off\" href=\"/unit/ledoff\">OFF</a>\n";}
    else
      {html_text +="<p>LED Status: OFF</p><a class=\"button_large button-on\" href=\"/unit/ledon\">ON</a>\n";}
  
    html_text +="<a class=\"button_large\" href=\"/\">Home</a>\n";
    html_text +="</body>\n";
    html_text +="</html>\n";
    return html_text;
  }
  
*/
//================================== Available functions ==================================
//===== Wifi
//--- wifi_start_connect()
//--- wifi_connected()
//--- wifi_connect(const char* wifi_ssid, const char* wifi_password)
//--- wifi_create_ap()
//===== Web Server
//--- webserver_setup()
//===== URL Handlers
//--- myWebServerHandle_Root()
//--- myWebServerHandle_Wifi_Config()
//--- myWebServerHandle_unit()
//===== Webpage HTML String creators
//--- html_Head()
//--- html_Root()
//--- html_Wifi_Config()
//=========================================================================================
//=========================================================================================

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> //---Webserver library for ESP8266 (Built on ESP8266WiFi.h)
ESP8266WebServer myWebServer(80); //---Initialise WebServer on port 80

//=========================================================================================
//========================================= WiFi ==========================================

WiFiClient wifiClient; //---Initialise Wifi object

/*
 * Start Wifi Connect Sequence
 */
void wifi_start_connect() {
  WiFi.mode(WIFI_AP_STA); //---WIFI_STA (1) / (WIFI_AP (2) / WIFI_AP_STA (3) / WIFI_OFF)
  if (wifi_create_ap()) {
    Serial.println("AP Created");
  }
  else {
    Serial.println("AP failed");
  }
  webserver_setup();

  Serial.print("Status: ");
  Serial.print(WiFi.status());
  Serial.print(" AP IP: ");
  Serial.print(WiFi.softAPIP());
  Serial.print(" STAION IP: ");
  Serial.println(WiFi.localIP());
  
}

/*
 * Determine if WiFi is connected
 * return (Boolean)
 */
bool wifi_connected() {
  /*
  * --- WiFi.status() OPTIONS:
  * 0 - WL_IDLE_STATUS: a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED)
  * 1 - WL_NO_SSID_AVAIL: assigned when no SSID are available
  * 2 - WL_SCAN_COMPLETED: assigned when the scan networks is completed
  * 3 - WL_CONNECTED: assigned when connected to a Wi-Fi network
  * 4 - WL_CONNECT_FAILED: assigned when the connection fails for all the attempts
  * 5 - WL_CONNECTION_LOST: assigned when the connection is lost
  * 6 - WL_DISCONNECTED: assigned when disconnected from a network
  * 255 - WL_NO_SHIELD: assigned when no Wi-Fi shield is present
  */
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  else {
    return false;
  }
}

/*
 * Connect ESP8266 to WiFi network
 */
void wifi_connect(const char* wifi_ssid, const char* wifi_password) {
  Serial.print("Attempting WiFi Connection to: ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);

  //---Wait until the connection has been confirmed before continuing
  int conCount = 0;
  int conCountLimit = 10; //---Connection time (x 500ms)
  while (!wifi_connected() and (conCount<conCountLimit)) {
    delay(500);
    Serial.print(".");
    conCount++;
  }

  if (wifi_connected()) { //---If Connection was successful
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); //---Print IP Address
  }
  else {
    Serial.println("WiFi NOT connected...");
  }
}

/*
 * Connect ESP8266 to WiFi network
 */
bool wifi_create_ap() {
  //---Default IP: 192.168.4.1
  const char *ap_ssid = "SetupMyNewUnit";
  return (WiFi.softAP(ap_ssid));
}

//=========================================================================================
//====================================== Web Server =======================================
  
/*
 * Setup Web Sever with URL event handling
 * Call in setup {}
 */
void webserver_setup() {
  myWebServer.on("/", myWebServerHandle_Root);
  myWebServer.on("/wifi_config", myWebServerHandle_Wifi_Config);
  myWebServer.on("/unit", myWebServerHandle_unit);
  //===== Defined by project =============================
  webserver_setup_unit(); //---Add Project specific Web Pages to Web Server
  //======================================================
  myWebServer.onNotFound(myWebServerHandle_Root);
  myWebServer.begin(); //---Start WEB server
  Serial.println("Web Server started");
}

/*
 * Functions for URL root Main Web Page
 */
void myWebServerHandle_Root() {
  myWebServer.send(200, "text/html", html_Root()); 
}

/*
 * Function for Wifi Config Page 
 */
void myWebServerHandle_Wifi_Config() {
  myWebServer.send(200, "text/html", html_Wifi_Config());
  if (myWebServer.arg("doConnect") == "1") { //---Check if should Connect
    //---Define char array for String variables
    char mySSID[sizeof(myWebServer.arg("conSSID"))];
    char myPSW[sizeof(myWebServer.arg("conPSW"))];
    myWebServer.arg("conSSID").toCharArray(mySSID, sizeof(myWebServer.arg("conSSID")));
    myWebServer.arg("conPSW").toCharArray(myPSW, sizeof(myWebServer.arg("conPSW")));
    wifi_connect(mySSID, myPSW); //---Connect Wifi
  }
}

/*
 * URL /unit ***Parameters Could be Project Defined***
 */
 void myWebServerHandle_unit() {
  myWebServer.send(200, "text/html", html_Unit(ledStatus)); 
}

/*
 * Create HTML String for HEAD
 * return (String)
 */
String html_Head() {
  String htmlHead = "<!DOCTYPE html> <html>\n";
    htmlHead +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    htmlHead +="<title>My Web Control</title>\n";
    htmlHead +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    htmlHead +="body{margin-top: 10px;}\n";
    htmlHead +="h1 {color: #444444;margin: 10px;}\n";
    htmlHead +="h3 {color: #444444;margin: 10px;}\n";
    htmlHead +=".button_large {display: block;width: auto;background-color: #1abc9c;border: none;color: white;padding: 15px;text-decoration: none;font-size: 25px;margin: 10px auto 10px;cursor: pointer;border-radius: 4px;}\n";
    htmlHead +=".button_small {display: block;width: auto;background-color: #1abc9c;border: none;color: white;padding: 5px;text-decoration: none;font-size: 15px;margin: 5px auto 5px;cursor: pointer;border-radius: 4px;}\n";
    htmlHead +=".button-on {background-color: #1abc9c;}\n";
    htmlHead +=".button-on:active {background-color: #16a085;}\n";
    htmlHead +=".button-off {background-color: #34495e;}\n";
    htmlHead +=".button-off:active {background-color: #2c3e50;}\n";
    htmlHead +="p {font-size: 15px;color: #888;margin-bottom: 10px;}\n";
    htmlHead +="</style>\n";
    htmlHead +="</head>\n";
  return htmlHead;
}

/*
 * Create HTML String for Root URL "/"
 * return (String)
 */
String html_Root() {
  String html_text = html_Head();
  html_text +="<body>\n";
  html_text +="<h1>Web Control Main</h1>\n";
  html_text +="<a class=\"button_large\" href=\"/wifi_config\">Configure Wifi</a>\n";
  html_text +="<a class=\"button_large\" href=\"/unit\">Unit</a>\n";
  html_text +="</body>\n";
  html_text +="</html>\n";
  return html_text;
}

/*
 * Create HTML String for Wifi Config URL "/wifi_config"
 * return (String)
 */
String html_Wifi_Config() {
  
  String html_text = html_Head();
  html_text +="<body>\n";
  html_text +="<form name='formWifiCon' id='formWifiCon' method='POST' action='/wifi_config'>\n";
  html_text +="<h1>Wifi Config</h1>\n";
  html_text +="<a class=\"button_large\" href=\"/\">Home</a>\n";
  html_text +="<h3>Status:</h3>\n";

  if (WiFi.localIP()[0] == 0) { //---Check if Connected to Station
    html_text +="<p>Not Connected</p>\n";
  }
  else {
    html_text +="<p>Connected to: " + WiFi.SSID() + "</p>\n";
  }
  
  if (myWebServer.arg("doConnect") == "1") { //---Check if should Connect
    html_text +="<head><meta http-equiv='refresh' content='5'></head>";
    html_text +="<p>Attempting to Connect... Auto Refresh in 5s</p>\n";
  }
  
  html_text +="<h3>Available Wifi Networks</h3>\n";

  Serial.print("Status: ");
  Serial.print(WiFi.status());
  Serial.print(" AP IP: ");
  Serial.print(WiFi.softAPIP());
  Serial.print(" STAION IP: ");
  Serial.println(WiFi.localIP());

  if (myWebServer.arg("doScan") == "1") {
    Serial.println("scan for Wifi");
    int n = WiFi.scanNetworks();
    if (n == 0) {
      Serial.println("no networks found");
    } else {
      Serial.print(n);
      Serial.println(" networks found");
      for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.println(": " + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ")");
        html_text +="<input type='button' class='button_small' value='" + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ")' onclick=document.getElementById('conSSID').value='" + WiFi.SSID(i) + "'>";
        delay(10);
      }
    }
  }
  else {
    html_text +="<input type='hidden' name='doScan' id='doScan' value=0>";
    html_text +="<input type='submit' class='button_large' value='Scan' onclick=document.getElementById('doScan').value=1>";
  }
  
  html_text +="SSID: <input type='text' name='conSSID' id='conSSID' value='" + myWebServer.arg("conSSID") + "'><br>\n";
  html_text +="PSW:  <input type='text' name='conPSW' id='conPSW' value='" + myWebServer.arg("conPSW") + "'><br>\n";
  html_text +="<input type='hidden' name='doConnect' id='doConnect' value=0>\n";
  html_text +="<input type='submit' class='button_small' value='Connect' onclick=document.getElementById('doConnect').value=1>\n";
  html_text +="</form>\n";
  html_text +="</body>\n";
  html_text +="</html>\n";
  return html_text;
}
