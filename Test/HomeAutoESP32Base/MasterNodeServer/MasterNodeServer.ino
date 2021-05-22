/*
 * ESP32 as Base Unit for Home Automation
 * ESP32 Connect to Router and host webserver to switch Pins
 * -=Pinout=-
 * MOSI -> D23
 * CE   -> D22
 * CSN  -> D21
 * MISO -> D19
 * SCK  -> D18
*/

//====================================================================================
//---Unit Specific Settings
int Trigger1 = 4; //---ESP32 Pin D4
int Trigger2 = 5; //---ESP32 Pin D5
int RF_LED_PIN = 2; //---LED

//---Router Settings
const char* ssid = "CoetzeeAPn";  // Enter SSID here
const char* password = "OnlyByGrace";  //Enter Password here

//====================================================================================
//---Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <WiFi.h>
#include <WebServer.h>

//====================================================================================
//---nRF24L01 Addresses
const byte unitAddress[][6] = {"Home0","Home1","Home2","Home3","Home4","Home5"};

//---Global variables
const int doSerialPrint = 1; //---to print to Serial Monitor 1=True 0=False
char dataReceived[32] = ""; //---Data Received
char dataToSend[32] = ""; //---Data to Send

//---Unit Statuses
bool R1status = false;
bool R2status = false;

//---create an RF24 object
RF24 radio(22, 21);  // CE, CSN

//---declare an object of WebServer library
WebServer server(80);

//====================================================================================
void setup()
{
  //---Unit Pin Setup
  pinMode(Trigger1, INPUT);
  pinMode(Trigger2, INPUT);
  pinMode(RF_LED_PIN, OUTPUT);
  digitalWrite (RF_LED_PIN, LOW);

  //---Radio Setup
  radio.begin();
  radio.setDataRate(RF24_2MBPS);
  radio.setRetries(3,15); // delay, count
  radio.setChannel(76);
  radio.setPALevel(RF24_PA_MIN);

  //---Pipes
  radio.openReadingPipe(1, unitAddress[0]); //---Receiving own
  radio.startListening(); //---Set as Receiver

  //---Serial monitor and StartUp Info
  Serial.begin(9600);
  delay(1000);
  Serial.println("");
  Serial.println("======================================================================");
  Serial.println("---=== RF Tranceiver Detail ===---"); 
  radio.printDetails();
  Serial.println("======================================================================");
  Serial.print("---=== Connecting to router SSID : "); 
  Serial.print(ssid); 
  Serial.println(" ===---");
  

  //---Connect to Wifi
  WiFi.begin(ssid, password); //---Start wi-fi connection
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
    delay(200);
    Serial.print(".");
  }

  /*
   * Specify which function to call on URL
   * server.on( URL , FUNCTION );
   * If URL is invalid
   * server.onNotFound(handle_NotFound);
  */
  server.on("/", handle_OnConnect);
  server.on("/R1on", handle_R1on);
  server.on("/R1off", handle_R1off);
  server.on("/R2on", handle_R2on);
  server.on("/R2off", handle_R2off);
  server.onNotFound(handle_NotFound);
  server.begin(); //---Start Server
  
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("IP: ");  Serial.println(WiFi.localIP());
  Serial.println("Home Base Unit Ready");
  Serial.println("======================================================================");
  
}

//====================================================================================
void loop()
{
  
  //---Read RF
  if (radio.available()) {
    radio.read( &dataReceived, sizeof(dataReceived));
    readRFMSG();
  }
  
  //---Trigger 1
//  if (digitalRead(Trigger1) == HIGH) {
//    doSwitch(1, 1, true);
//    delay(200);
//  }

  //---Trigger 2
//  if (digitalRead(Trigger2) == HIGH) {
//    doSwitch(1, 1, false);
//    delay(200);
//  }

  //---Handle Web Server Clients
  server.handleClient();
  
}

//====================================================================================
/*
 * Send the data through the RF module to a specific Unit Address
 * Requires 
 *    int UnitNumber //---Home Unit Number - used in unitAddress[]
 */
bool sendRFMSG(int UnitNumber) {
  bool sentACK;
  digitalWrite (RF_LED_PIN, HIGH);
  radio.stopListening();
  radio.openWritingPipe(unitAddress[UnitNumber]);
  sentACK = radio.write( &dataToSend, sizeof(dataToSend) );
  radio.startListening();
  if (doSerialPrint) {
    Serial.print ("RF Sending ===> ");
    int i;
    for(i=0; i<5; i++){
      Serial.write((char)unitAddress[UnitNumber][i]);
    }
    Serial.print (" : ");
    if (sentACK) { 
      Serial.println (dataToSend);
    } else {
      Serial.println ("Acknowledge not Received");
    }
  }
  digitalWrite (RF_LED_PIN, LOW);
  return sentACK;
}

/*
 * Read and decode the data received through the RF module
 */
void readRFMSG() {
  if (dataReceived[3] == '1') R1status = true; else R1status = false; //---Unit Switch 1
  if (dataReceived[4] == '1') R2status = true; else R2status = false; //---Unit Switch 2
  if (doSerialPrint) {
    Serial.print ("RF received <=== ");
    Serial.print (dataReceived);
    Serial.print (" >> ");
    Serial.print ("Unit Number:");
    Serial.print (dataReceived[1]);
    Serial.print (" Switch State 1:");
    if (R1status) Serial.print ("ON"); else Serial.print ("OFF");
    Serial.print (" 2:");
    if (R2status) Serial.println ("ON"); else Serial.println ("OFF");
  }
}

/*
 * Get Current Status from Units
 * Requires 
 *    int UnitNumber //---Home Unit Number - used in unitAddress[]
 */
void getUnitRStatus(int UnitNumber) {
  strcpy(dataToSend,"Get Status");
  sendRFMSG(UnitNumber);
}

/*
 * Attemp to DO Switch
 * Requires 
 *    int UnitNumber //---Home Unit Number - used in unitAddress[]
 *    int switchNR //---Switch number to change state
 *    bool switchState //---True = Switch ON, False = Switch OFF
 */
void doSwitch(int UnitNumber, int switchNR, bool switchState) {

  if (switchNR == 1) {
    if (switchState) strcpy(dataToSend, "Relay 1 ON" );
    else strcpy(dataToSend, "Relay 1 OFF");
  } else {
    if (switchState) strcpy(dataToSend, "Relay 2 ON" );
    else strcpy(dataToSend, "Relay 2 OFF");
  }
  
  if (sendRFMSG(UnitNumber)) { //---If message was sent
    //---Wait for responce
    unsigned long startedWaiting = millis();
    if (doSerialPrint) Serial.print ("Wait for response");
    while( (!radio.available()) && (millis() - startedWaiting <= 1000) ) {
      if (doSerialPrint) Serial.print (".");
    }
    if (doSerialPrint) Serial.println ("");
    //---Read Responce if avialable  
    if (radio.available()) {
      radio.read( &dataReceived, sizeof(dataReceived));
      readRFMSG();
    }
  }
    
}

//====================================================================================
/*
 * Webserver functions to determine action from URL
 */
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML()); 
}
  
void handle_R1on() {
  doSwitch(1, 1, true);
  server.send(200, "text/html", SendHTML());
}

void handle_R1off() { 
  doSwitch(1, 1, false);
  server.send(200, "text/html", SendHTML());
}

void handle_R2on() {
  doSwitch(1, 2, true);
  server.send(200, "text/html", SendHTML()); 
}

void handle_R2off() {
  doSwitch(1, 2, false);
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Page Not found");
}

/*
 * Webserver - Generate html/text for publishing
 * Requires
 *    bool R1stat //---Status of R1
 *    bool R2stat //---Status of R2
 */
String SendHTML() {
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
  ptr +="<h1>Home ESP Server</h1>\n";
  ptr +="<h3>Switching Relay</h3>\n";
  
  if (R1status) { ptr +="<p>R1 Status: ON</p><a class=\"button button-off\" href=\"/R1off\">OFF</a>\n"; }
  else { ptr +="<p>R1 Status: OFF</p><a class=\"button button-on\" href=\"/R1on\">ON</a>\n"; }

  if (R2status) { ptr +="<p>R2 Status: ON</p><a class=\"button button-off\" href=\"/R2off\">OFF</a>\n"; }
  else { ptr +="<p>R2 Status: OFF</p><a class=\"button button-on\" href=\"/R2on\">ON</a>\n"; }

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
