//===== Display ATtiny85 analog temp sensor value on LCD
const int tempProbe = 0;                    //--- Uno pin A0
float R1 = 10000;                           //--- Volt divider fixed res value (Ohm)
float tempProbeVolt;                        //--- Divider center volt value
long thermRes;                              //--- Thermistor Resistance (Ohm)
float curTemp;                              //--- Temperature in C

//--- Temperature value array
const int firstDegree = -25;                //--- First array element degree value
const long resValPerDegree[] = {            //--- List of Res values per degree increase
  133500,
  125672,
  118350,
  111498,
  105084,
  99077,
  93447,
  88175,
  83230,
  78591,
  74238,
  70153,
  66316,
  62712,
  59325,
  56142,
  53148,
  50331,
  47680,
  45184,
  42834,
  40620,
  38533,
  36566,
  34710,
  32960,
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
  2066,
  1992,
  1927,
  1861,
  1798,
  1738,
  1679,
  1624,
  1570,
  1518,
  1473,
  1420,
  1374,
  1330,
  1287,
  1246,
  1207,
  1169,
  1132,
  1097,
  1062,
  1030,
  998,
  967,
  938,
  909,
  882,
  856,
  830,
  805,
  782,
  759,
  737,
  715,
  694,
  674,
  655,
  636,
  618,
  601,
  584,
  568,
  552,
  537,
  522,
  508,
  494,
  480,
  467,
  455,
  443,
  431,
  419,
  408,
  398,
  387,
  377,
  367,
  358,
  349,
  340
};
int totalArrLength = sizeof(resValPerDegree)/sizeof(int);

void setup() {
  Serial.begin(9600);
}

void loop() {
  //--- Loop variables
  int index = 0;

  //--- Get thermistor Res value
  tempProbeVolt = analogRead(tempProbe);
  thermRes = (long)((((float)tempProbeVolt/1023) * R1) / (1 - ((float)tempProbeVolt/1023)));
  
  //--- Evaluate thermRes
  if (thermRes > resValPerDegree[0]) {
    Serial.println("TOO COLD");
  }
  else if (thermRes < resValPerDegree[totalArrLength - 1]) {
    Serial.println("TOO HOT");
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

    Serial.println(curTemp);
    
  } 
  
  delay(1000);
}
