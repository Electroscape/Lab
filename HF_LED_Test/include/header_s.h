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



// RELAY
enum REL_PIN {
    REL_0_PIN,          // 0 First room light
    REL_1_PIN,          // 1 Light 2nd room
    REL_2_PIN,          // 2 UV Light
    REL_3_PIN,          // 3 Alarm
    REL_4_PIN,          // 4 Empty
    REL_5_PIN,          // 5 Fireplace valve
    REL_SAFE_PIC_PIN,  // 6 valve holding the painting
    REL_7_PIN           // 7 Exit door lock
};

enum REL_INIT {
    REL_0_INIT = 1,                    // DESCRIPTION OF THE RELAY WIRING
    REL_1_INIT = 1,                    // DESCRIPTION OF THE RELAY WIRING
    REL_2_INIT = 1,                    // DESCRIPTION OF THE RELAY WIRING
    REL_3_INIT = 1,                    // DESCRIPTION OF THE RELAY WIRING
    REL_4_INIT = 1,                    // DESCRIPTION OF THE RELAY WIRING
    REL_5_INIT = 1,                    // DESCRIPTION OF THE RELAY WIRING
    REL_SAFE_PIC_INIT = SAFE_HIDDEN,  // NC = Empty | COM = 24V  | NO = Valve
    REL_7_INIT = 1                     // DESCRIPTION OF THE RELAY WIRING
};


