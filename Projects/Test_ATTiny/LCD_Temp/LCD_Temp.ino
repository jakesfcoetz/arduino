//===== Display ATtiny85 analog temp sensor value on LCD
//--- Vcc (pin8) <-----> Vcc
//--- SCL (pin7) <-----> SCL
//--- SDA (pin5) <-----> SDA
//--- GND (pin4)<-----> GND

#include <LiquidCrystal_I2C_attiny85.h>
#include <TinyWireM.h>

LiquidCrystal_I2C_attiny85 lcd(0x27,16,2);  //--- set address & 16 chars / 2 lines
const int tempProbe = 3;                    //--- IC pin 2
float R1 = 10000;                           //--- Volt divider fixed res value (Ohm)
float tempProbeVolt;                        //--- Divider center volt value
long thermRes;                              //--- Thermistor Resistance (Ohm)
float curTemp;                              //--- Temperature in C

//--- Temperature value array
const int firstDegree = 1;                  //--- First array element degree value
const int resValPerDegree[] = {             //--- List of Res values per degree increase
31308,
29749,
28276,
26885,
25570,
24327,
23152,
22041,
20989,
19993,
19051,
18158,
17312,
16511,
15751,
15031,
14347,
13699,
13083,
12499,
11944,
11417,
10916,
10440,
10000,
9557,
9147,
8758,
8387,
8034,
7698,
7377,
7072,
6781,
6504,
6239,
5987,
5746,
5517,
5298,
5088,
4888,
4697,
4515,
4341,
4174,
4015,
3862,
3716,
3588,
3443,
3315,
3193,
3076,
2963,
2856,
2753,
2654,
2559,
2468,
2381,
2297,
2217,
2140,
2066
};
int totalArrLength = sizeof(resValPerDegree)/sizeof(int);

void setup()
{
  TinyWireM.begin();
  lcd.init();
  lcd.backlight(); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp:");
}

void loop()
{
  //--- Loop variables
  int index = 0;

  //--- Get thermistor Res value
  tempProbeVolt = analogRead(tempProbe);
  thermRes = (long)((((float)tempProbeVolt/1023) * R1) / (1 - ((float)tempProbeVolt/1023)));
  
  //--- Evaluate thermRes
  if (thermRes > resValPerDegree[0]) {
    lcd.setCursor(0,1);
    lcd.print("TOO COLD");
  }
  else if (thermRes < resValPerDegree[totalArrLength - 1]) {
    lcd.setCursor(0,1);
    lcd.print("TOO HOT ");
  } else {
    //--- Get corresponding value from temperature array
    while (thermRes < (resValPerDegree[index])) {
      index++;
    }
    //--- Get temperature
    curTemp = (
        (firstDegree + index - 1)
        + 
        (
          (float)(thermRes - resValPerDegree[index])
          /
          (float)(resValPerDegree[index - 1] - resValPerDegree[index])
        )
      );
  
    lcd.setCursor(0,1);
    lcd.print(curTemp);
    lcd.print(" C     ");
  }  
  
  delay(1000);
}
