void loop() {
  
  //---Listen for incomming HTML requests
  myWebServer.handleClient();

  //---Update LED Status
  if(ledStatus)
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW);

//  unsigned long currentTime = millis();
//  if (currentTime - previousTime >= intervalTime) {
//
//    previousTime = currentTime;
//  }
  
}
