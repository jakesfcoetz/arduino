void setup() {
  
  //---Set Pins
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, HIGH);
  
  Serial.begin(115200); //---Begin Serial
  delay(1000); //---wait for serial monitor to start
  Serial.println(); //---new line in case junk has been printed

  wifi_start_connect(); //---Start Wifi Connect Sequence

  timeClient.begin(); //---Set NTP time client
  timeNTPUpdate(); //---Update time

  Serial.println("Ready...");
    
}
