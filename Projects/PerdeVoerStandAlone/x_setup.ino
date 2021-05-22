void setup() {
  
  //---Set Pins
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  digitalWrite(trigPin1, HIGH);
  digitalWrite(trigPin2, HIGH);
  
  Serial.begin(115200); //---Begin Serial
  delay(1000); //---wait for serial monitor to start
  Serial.println(); //---new line in case junk has been printed

  wifi_start_connect(); //---Start Wifi Connect Sequence

  Serial.println("Ready...");
    
}
