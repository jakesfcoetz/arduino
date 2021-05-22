/*
 * Add Project specific Web Pages to Web Server
 */
void webserver_setup_unit() {
  myWebServer.on("/unit/ledon", myWebServerHandle_ledon); //---Project specific
  myWebServer.on("/unit/ledoff", myWebServerHandle_ledoff); //---Project specific
}

/*
 * URL /unit/ledon
 */
void myWebServerHandle_ledon() {
  ledStatus = LOW;
  Serial.println("Status: ON");
  myWebServer.send(200, "text/html", html_Unit(ledStatus)); 
}

/*
 * URL /ledoff
 */
void myWebServerHandle_ledoff() {
  ledStatus = HIGH;
  Serial.println("Status: OFF");
  myWebServer.send(200, "text/html", html_Unit(ledStatus)); 
}

/*
 * Create HTML String
 * requires (Any value needed to create HTML text)
 * return (String)
 */
String html_Unit(uint8_t ledstat) {
  String html_text = html_Head();
  html_text +="<body>\n";
  html_text +="<h1>Web Control</h1>\n";
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
