#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const int Temp_Probe = 0;  //Analog pin 0

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
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
