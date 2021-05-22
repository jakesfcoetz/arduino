void setup() {
  
  //---Set Pins
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  Serial.begin(115200); //---Begin Serial
  delay(1000); //---wait for serial monitor to start
  Serial.println(); //---new line in case junk has been printed

  wifi_start_connect(); //---Start Wifi Connect Sequence

  Serial.println("Ready...");
    
}
