/*
 * ESP01
 * Basic Webserver to switch Pin 2 LED (Active Low)
 * ( Can be used on program jig )
*/

#include <ESP8266WiFi.h>
 
const char* ssid = "CoetzeeAPn";
const char* password = "OnlyByGrace";
 
int ledPin = 2;
const char* ledState = "OFF";

WiFiServer myWEBserver(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
   
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   
  // Start the server
  myWEBserver.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Server IP ");
  Serial.println(WiFi.localIP());
    
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = myWEBserver.available();
  if (!client) {
    return;
  }
   
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
   
  // Match the request
 
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, LOW);
    ledState = "ON";
  }
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(ledPin, HIGH);
    ledState = "OFF";
  }
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
   
  client.print("Led pin is now: ");
  client.print(ledState);  
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 2 ON<br>");
  client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 2 OFF<br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
