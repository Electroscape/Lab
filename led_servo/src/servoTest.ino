#include <Servo.h>
#include <stb_led.h>
#include <Adafruit_NeoPixel.h>  

#define stripeCnt 1

Adafruit_NeoPixel LED_Strips[1];
const long int darked = LED_Strips[0].Color(255,255,255);
const long int green = LED_Strips[0].Color(0,100,0);


Servo servo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(115200);
  Serial.println("LedInit");
  LED_Strips[0] = Adafruit_NeoPixel(1, 6, (NEO_RGB + NEO_KHZ800));
  LED_Strips[0].begin();
  delay(100);
  LED_Strips[0].setPixelColor(0, darked);
  LED_Strips[0].show();
  delay(1000);
  servo.attach(9);  // attaches the servo on pin 9 to the servo object
  servo.write(0);
  delay(100);
}

void loop() {


  servo.write(90);
  delay(500);
  Serial.println("loop");
  LED_Strips[0].setPixelColor(0, darked);
  LED_Strips[0].show();
  
  delay(10);

  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
  delay(500);

  Serial.println("loopmid");
  
  
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
  LED_Strips[0].setPixelColor(0, green);
  LED_Strips[0].show();


  Serial.println("loopend");
  
  delay(500);
}