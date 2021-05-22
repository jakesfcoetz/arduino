/*
 * ESP8266 4 GPIO Test
 * Add these lines to remove RX/TX function and replace with GPIO
 * pinMode(led3, FUNCTION_3); //GPIO 1 swap the pin to a GPIO.
 * pinMode(led4, FUNCTION_3); //GPIO 3 swap the pin to a GPIO.
 *
 * GPIO 3 (RX) - Active HIGH
 * GPIO 0/1/2  - Active LOW
 */

//=========================================================================================
//================================== Functions for 4GPIO ==================================

//---Pin Definitions
const int MY_GPIO_1 = 3; //---pin 7 = GPIO 3 (RX)
const int MY_GPIO_2 = 0; //---pin 5 = GPIO 0
const int MY_GPIO_3 = 2; //---pin 3 = GPIO 2
const int MY_GPIO_4 = 1; //---pin 2 = GPIO 1 (TX)

//void setGPIO(int pinNR, bool pinSet) {
//  if (pinNR == 3) { //---GPIO 3 is active HIGH ( on is on )
//    
//  } else {
//    
//  }
//}

//=========================================================================================
//======================================== Setup ==========================================

void setup() {

  delay(3000); //---wait 3s after boot to Test initial state
  
  //---Change Pin Function (Make RX/TX into GPIO pins)
  pinMode(1, FUNCTION_3); //---GPIO 1 (TX) swap the pin to a GPIO
  pinMode(3, FUNCTION_3); //---GPIO 3 (RX) swap the pin to a 

  //---Set Pins
  pinMode(MY_GPIO_1, OUTPUT);
  pinMode(MY_GPIO_2, OUTPUT);
  pinMode(MY_GPIO_3, OUTPUT);
  pinMode(MY_GPIO_4, OUTPUT);
  
  //---Switch the LED off to start with
  digitalWrite(MY_GPIO_1, LOW);
  digitalWrite(MY_GPIO_2, HIGH);
  digitalWrite(MY_GPIO_3, HIGH);
  digitalWrite(MY_GPIO_4, HIGH);
  
}

//=========================================================================================
//========================================= Loop ==========================================

void loop() {

  digitalWrite(MY_GPIO_1, HIGH);
  delay(500);
  digitalWrite(MY_GPIO_1, LOW);
  delay(500);

  digitalWrite(MY_GPIO_2, LOW);
  delay(500);
  digitalWrite(MY_GPIO_2, HIGH);
  delay(500);

  digitalWrite(MY_GPIO_3, LOW);
  delay(500);
  digitalWrite(MY_GPIO_3, HIGH);
  delay(500);

  digitalWrite(MY_GPIO_4, LOW);
  delay(500);
  digitalWrite(MY_GPIO_4, HIGH);
  delay(500);

}
