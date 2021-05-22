/***************************************************** 
* ESP32 Pololu proximity sensor Test

* 
*****************************************************/


int sensorPin = 4;

void setup()
{
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Proximity Test");
  pinMode(sensorPin, INPUT);
}

void loop()
{
  Serial.println(analogRead(sensorPin));  // get value using T0 
  delay(100);
}
