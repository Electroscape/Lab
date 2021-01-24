/*==========================================================================================================*/
/*		2CP - TeamEscape - Engineering
 *		by Abdullah Saei & Robert Schloezer
 *
 *		v1.0
 *		- Watchdog timer
 *		- Servo Motor working
 *		- 07.11.2019
 *    - No 1 sec release
 */
/*==========================================================================================================*/

/*==INCLUDE=================================================================================================*/
#include <Wire.h>                     /* TWI / I2C                                                          */
//Watchdog timer
#include <avr/wdt.h>
// I2C Port Expander
#include "PCF8574.h"


// AUDIO
#include "AltSoftSerial.h"
//#include "NeoSWSerial.h"
#include "DFRobotDFPlayerMini.h"
//Servo
#include <Servo_I2C.h> //
//#include <Servo.h> //pins connected to Attiny PIN D

/*==DEFINE==================================================================================================*/
// LED
// PIN
enum PWM_PIN{
  PWM_1_PIN = 3,                           // Predefined by STB design
  PWM_2_PIN = 5,                           // Predefined by STB design
  PWM_3_PIN = 6,                           // Predefined by STB design
  PWM_4_PIN = 9,                           // Predefined by STB design
  PWM_5_PIN = 12                           // Servo
};

#define INPUT_CNT 4
enum INPUT_PIN{
  SERVO_CMD_0,                             //  0 Phone pickup button
  SERVO_CMD_1,                             //  1 Coin Detector
  SERVO_CMD_2,                             //  2
  SERVO_CMD_3                             //  3
};
const int inputs[] = {SERVO_CMD_0, SERVO_CMD_1, SERVO_CMD_2, SERVO_CMD_3};


// I2C ADRESSES
#define RELAY_I2C_ADD     	   0x3F         // Relay Expander
//#define OLED_I2C_ADD         0x3C         // Predefined by hardware
#define LCD_I2C_ADD					  0x27          //0x16         // Predefined by hardware
#define KEYPAD_I2C_ADD        0x38         // Telephone Keypad
#define INPUT_I2C_ADD         0x39         // coin detector and Phone speaker
#define SERVO_I2C_ADD         0x16         //0x13         // From Attiny code


int servo_default = 40;
int servo_remove = 25;

Expander_PCF8574 iTrigger;
// Servo
Servo_I2C myservo;

/*===Servo==================================================================================================*/
bool servo_Init() {
  myservo.begin(SERVO_I2C_ADD);
  delay(20);
  myservo.write(1);
  myservo.write(50);
  delay(2000);
  myservo.write(100);

  return true;
}

/*==INPUTS==================================================================================================*/
bool input_Init() {
    Serial.println("input_init...");
    iTrigger.begin(INPUT_I2C_ADD);
    for(int i = 0; i<sizeof(inputs); i++) {
        iTrigger.pinMode(inputs[i], INPUT);
    }
    return true;
}


/*============================================================================================================
//===BASICS===================================================================================================
//==========================================================================================================*/

void i2c_scanner() {
  Serial.println (F("I2C scanner:"));
  Serial.println (F("Scanning..."));
  byte wire_device_count = 0;

  for (byte i = 8; i < 120; i++) {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0) {
      Serial.print   (F("Found address: "));
      Serial.print   (i, DEC);
      Serial.print   (F(" (0x"));
      Serial.print   (i, HEX);
      Serial.print (F(")"));
      if (i == 39) Serial.print(F(" -> LCD"));
      if (i == 56) Serial.print(F(" -> LCD-I2C-Board"));
      if (i == 57) Serial.print(F(" -> Input-I2C-board"));
      if (i == 60) Serial.print(F(" -> Display"));
      if (i == 63) Serial.print(F(" -> Relay"));
      if (i == 22) Serial.print(F(" -> Servo-I2C-Board"));
      Serial.println();
      wire_device_count++;
      delay (1);
    }
  }
  Serial.print   (F("Found "));
  Serial.print   (wire_device_count, DEC);
  Serial.println (F(" device(s)."));

  Serial.println();

  delay(500);
}


/*============================================================================================================
//===SETUP====================================================================================================
//==========================================================================================================*/
void setup() {
  wdt_disable();
  Serial.begin(115200);
  Serial.println(); Serial.println("==============Sweet Revange 12.08.2019=============");

  i2c_scanner();
  if ( input_Init()  ) {Serial.println("Inputs:  ok"); }
  if ( servo_Init() )    {Serial.println("Servo:   ok");	}


  delay(100);

  Serial.println("WDT endabled");
  wdt_enable(WDTO_8S);
  Serial.println(); Serial.println("===================START====================="); Serial.println();
}

void manage_inputs() {
    for (int i=0; i<sizeof(inputs); i++) {
            Serial.print("servoCMDs: "); Serial.print(iTrigger.digitalRead(i)); Serial.print(", ");
        myservo.write(byte(servo_default + iTrigger.digitalRead(i) * servo_remove));
        delay(20);
    }
    Serial.println();
}

/*============================================================================================================
//===LOOP=====================================================================================================
//==========================================================================================================*/
void loop() {
    Serial.println("lööps");
    manage_inputs();
    delay(50);
}


/*============================================================================================================
//===INIT=====================================================================================================
//==========================================================================================================*/




void software_Reset() {
  Serial.println(F("Restarting in"));
  delay(250);
  for (byte i = 3; i>0; i--) {
    Serial.println(i);
    delay(100);
  }
  asm volatile ("  jmp 0");
}
