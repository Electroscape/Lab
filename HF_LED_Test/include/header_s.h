/**
* To configure a relay:
*   - rename REL_X_PIN to informative name
*   - rename and set REL_X_INIT with your init value
*   - create array with the used pins and benefit from loops :D
*/
#pragma once

String title = "HF_LED_TEST";
String versionDate = "9.11.2021";
String version = "version 1.0ST";
String brainName = String("BrCOLOR");
String relayCode = String("HID");
const unsigned long heartbeatFrequency = 5000;



// Passwort

#define PWM_1_PIN 3                       // Predefined by STB design
#define PWM_2_PIN 5                       // Predefined by STB design
#define PWM_3_PIN 6                       // Predefined by STB design
#define PWM_4_PIN 9                       // Predefined by STB design

#define MAX485_READ LOW
#define MAX485_WRITE HIGH

// Standards der Adressierung (Konvention)
#define RELAY_I2C_ADD 0x3F   // Relay Expander																							*/
#define OLED_I2C_ADD 0x3C    // Ist durch Hardware des OLEDs vorgegeben
#define KEYPAD_I2C_ADD 0x39  // möglich sind 0x38, 39, 3A, 3B, 3D

#define LED_TYPE_5V WS2811
#define LED_TYPE_12V WS2812B 
#define NUM_LEDS_5V 60
#define NUM_LEDS_12V 60

const byte KEYPAD_ROWS = 4;  // Zeilen
const byte KEYPAD_COLS = 3;  // Spalten
byte KeypadRowPins[KEYPAD_ROWS] = {1, 6, 5, 3};  // Zeilen  - Messleitungen
byte KeypadColPins[KEYPAD_COLS] = {2, 0, 4};     // Spalten - Steuerleitungen (abwechselnd HIGH)




