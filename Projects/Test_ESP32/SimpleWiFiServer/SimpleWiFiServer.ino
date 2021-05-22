/*
* Basic Webserver to switch Pin 2 and 5 LED (Connect to Router OR AP Mode)
*/

#include <WiFi.h>
#include <WebServer.h>

bool RouterAP = true; //---True = connect to router, False = be AP

//---To use if on Router Mode
const char* Rssid = "CoetzeeAPn";  // Enter SSID here
const char* Rpassword = "OnlyByGrace";  //Enter Password here

//---To use if on AP Mode
const char* APssid = "ESP32";  // Enter SSID here
const char* APpassword = "12345678";  //Enter Password here
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

//---declare an object of WebServer library
WebServer server(80);

const char* ssid = "";  // Enter SSID here
const char* password = "";  //Enter Password here

uint8_t LED1pin = 2;
bool LED1status = LOW;

uint8_t LED2pin = 5;
bool LED2status = LOW;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);

  if (RouterAP) {
    ssid = Rssid;
    password = Rpassword;
    Serial.print("---=== Connecting to router SSID : "); Serial.print(ssid); Serial.println(" ===---");
    Serial.println(ssid);
    WiFi.begin(ssid, password); //---connect to your local wi-fi network
    
    while (WiFi.status() != WL_CONNECTED) {
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
      delay(100);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected..!");
    Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  } else {
    ssid = APssid;
    password = APpassword;
    Serial.print("---=== Setting up AP SSID : "); Serial.print(ssid); Serial.print(" ===---");
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);
  }

  /*
   * Specify which function to call on URL
   * server.on( URL , FUNCTION );
   * If URL is invalid
   * server.onNotFound(handle_NotFound);
  */
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.begin(); //---Start Server
  Serial.println("HTTP server started");
}


void loop() {
  server.handleClient();
  if(LED1status) {
    digitalWrite(LED1pin, HIGH);
  } else {
    digitalWrite(LED1pin, LOW);
  }
  
  if(LED2status) {
    digitalWrite(LED2pin, HIGH);
  }
  else {
    digitalWrite(LED2pin, LOW);
  }
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO2 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Jakes ESP Server</h1>\n";
  ptr +="<h3>Switching LED</h3>\n";
  
   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
