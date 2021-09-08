//=== ATTiny blink sketch
//--- Can be used in UNO programmer board

int led_pin = 4;

void setup() {
  pinMode(led_pin,OUTPUT);
}

void loop(){
  digitalWrite(led_pin,HIGH);
  delay(1000);
  digitalWrite(led_pin,LOW);
  delay(1000);
}
