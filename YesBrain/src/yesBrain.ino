/**
 * @file m_Acces.ino
 * @author Martin Pek (martin.pek@web.de)
 * @brief access module supports RFID and Keypad authentication with oled&buzzer feedback
 * @version 0.1
 * @date 2022-09-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "header_st.h"                                     
#include <Keypad_I2C.h>
#include <stb_brain.h>
#include <avr/wdt.h>
#include <PCF8574.h> /* https://github.com/skywodd/pcf8574_arduino_library - modifiziert!  */

#include <Password.h>
#include <stb_rfid.h>
#include <stb_keypadCmds.h>
#include <stb_oledCmds.h>


/*
build for lib_arduino 0.6.7 onwards
TODO:
 - periodic updates on the password? everytime its polled? 

🔲✅
Fragen and access module Requirements
 - ✅ Dynamischer Headline text wie "Enter Code", "Welcome" etc over cmd from Mother
 - ✅ Necessity toggle between RFID and Keypad
 - ✅  wrong rfid may trigger twice
*/


STB_BRAIN Brain;


// only use software SPI not hardware SPI
Adafruit_PN532 RFID_0(PN532_SCK, PN532_MISO, PN532_MOSI, RFID_1_SS_PIN);
Adafruit_PN532 RFID_READERS[1] = {RFID_0};
uint8_t data[16];
unsigned long lastRfidCheck = millis();

Keypad_I2C Keypad(makeKeymap(KeypadKeys), KeypadRowPins, KeypadColPins, KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_ADD, PCF8574_MODE);

// the Evaluation is done on the Mother, may be kept for convenience or removed later
Password passKeypad = Password(dummyPassword);
unsigned long lastKeypadAction = millis();

unsigned long lastOledAction = millis();
// technically only the 3rd line i will update
bool oledHasContent = false;
// freq is unsinged int, durations are unsigned long
// frequency, ontime, offtime
unsigned int buzzerFreq[BuzzerMaxStages] = {0};
unsigned long buzzerOn[BuzzerMaxStages] = {0};
unsigned long buzzerOff[BuzzerMaxStages] = {0};
unsigned long buzzerTimeStamp = millis();

int buzzerStage = -1;


void setup() {
    
    // starts serial and default oled
    
    Brain.begin();
    Brain.STB_.dbgln("v0.09");

    Brain.setSlaveAddr(0);
}


void loop() {

    while (Brain.STB_.rcvdPtr != NULL) {
        // Serial.println("Brain.STB_.rcvdPtr");
        interpreter();
        Brain.nextRcvdLn();
    }
}


void interpreter() {
    Brain.sendAck();
}

