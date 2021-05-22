
const int Temp_Probe = 0; //Analog pin 0

void setup() {
  Serial.begin(9600);
}

void loop() { 

int therm = analogRead(Temp_Probe);

Serial.print(" Temp: ");
Serial.print(therm-238);
Serial.println(" Deg");

delay(1000);

}
 
