#include <LiquidCrystal_I2C_attiny85.h>

LiquidCrystal_I2C_attiny85 lcd(0x27,16,2);  // set address & 16 chars / 2 lines
const int Temp_Probe = 3;  //Analog pin 3

void setup()
{
  TinyWireM.begin();                    // initialize I2C lib
  lcd.init();                           // initialize the lcd 
  lcd.backlight(); 
  lcd.clear();
  lcd.setCursor(0,0);
  delay(100);
  lcd.print("Temp: ");
  lcd.setCursor(5,1);
  delay(100);
  lcd.print("C");
}

void loop()
{
  int therm = analogRead(Temp_Probe);
  therm = therm - 238;
  lcd.setCursor(0,1);
  delay(100);
  lcd.print(therm / 10);
  lcd.print(".");
  lcd.print(therm % 10);
  delay(1000);
}
