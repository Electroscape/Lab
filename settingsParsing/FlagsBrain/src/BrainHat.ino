/**
 * @file BrainHat.ino
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-05-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**************************************************************************/


// Setting Configurations
#include "header_st.h"

#include <stb_common.h>
#include <stb_brain.h>
#include <stb_led.h>
#include <stb_rfid.h>
#include <PCF8574.h>

STB STB;
STB_BRAIN BRAIN;
STB_LED LEDS;

PCF8574 button;

SSD1306AsciiWire secondOled;

int stage = 0;
int modi = 0;
int const modiCnt = 3;

long int clrSets[4][modiCnt] = {
    {LEDS.Strips[0].Color(120, 0, 0), LEDS.Strips[0].Color(75, 75, 75), LEDS.Strips[0].Color(60, 0, 0)},
    {LEDS.Strips[0].Color(200, 255, 220), LEDS.Strips[0].Color(40, 52, 44), LEDS.Strips[0].Color(143, 160, 180)},
    {LEDS.Strips[0].Color(40, 50, 255), LEDS.Strips[0].Color(255, 0, 0), LEDS.Strips[0].Color(143, 160, 180)},
    {LEDS.Strips[0].Color(205, 225, 255), LEDS.Strips[0].Color(205, 225, 255), LEDS.Strips[0].Color(0, 0, 0)}
};


void setup() {
    
    STB.begin();

    STB.rs485SetSlaveAddr(0);
    // BRAIN.receiveFlags(STB);
    STB.defaultOled.setScrollMode(SCROLL_MODE_AUTO);

    // BRAIN.receiveSettings(STB);

    // col 0 is the cmd type 0 is for setLedamount aka settingCmds::ledCount;
    BRAIN.settings[0][0] = settingCmds::ledCount;
    // col 1 is the PWM index
    BRAIN.settings[0][1] = 0;
    // col 2 is the amount of leds
    BRAIN.settings[0][2] = 60;
    BRAIN.settings[1][0] = settingCmds::ledCount;
    // col 1 is the PWM index
    BRAIN.settings[1][1] = 1;
    // col 2 is the amount of leds
    BRAIN.settings[1][2] = 60;
    BRAIN.settings[2][0] = settingCmds::ledCount;
    // col 1 is the PWM index
    BRAIN.settings[2][1] = 2;
    // col 2 is the amount of leds
    BRAIN.settings[2][2] = 60;
    BRAIN.settings[3][0] = settingCmds::ledCount;
    // col 1 is the PWM index
    BRAIN.settings[3][1] = 3;
    // col 2 is the amount of leds
    BRAIN.settings[3][2] = 60;

    if (BRAIN.flags[ledFlag] || true) {
        STB.dbgln("doing LED init v2");
        LEDS.ledInit(BRAIN.settings);
        delay(100);
        // LEDS.setAllStripsToClr(LEDS.Strips[0].Color(205, 225, 255));
        LEDS.setStripToClr(0, LEDS.Strips[0].Color(120, 0, 0));
        LEDS.setStripToClr(1, LEDS.Strips[1].Color(200, 255, 220));
        LEDS.setStripToClr(2, LEDS.Strips[2].Color(40, 50, 255));
        LEDS.setStripToClr(3, LEDS.Strips[3].Color(205, 225, 255));
    }

    if (BRAIN.flags[rfidFlag]) {
        STB.dbgln("doing RFID init");
        Adafruit_PN532 RFID_0(PN532_SCK, PN532_MISO, PN532_MOSI, RFID_1_SS_PIN);
        Adafruit_PN532 RFID_READERS[1] = {RFID_0};
    }

    if (BRAIN.flags[oledFlag]) {
        STB.dbgln("doing Oled init");
        STB_OLED::oledInit(&secondOled , SH1106_128x64, 61);
    }

    STB.dbgln("setup ended");
    wdt_disable();

    button.pinMode(0, INPUT);
    button.pinMode(1, INPUT);
    button.begin(0x38);
}


/*======================================
//===LOOP==============================
//====================================*/
void loop() {

    if (button.digitalRead(0) == HIGH) {
        Serial.print("stage incremented to: ");
        Serial.println(String(stage));
        toggleLight(stage);
        stage++;
        delay(200);
    }

    if (button.digitalRead(1) == HIGH) {
        Serial.print("modi incremented to: ");
        Serial.println(String(modi));
        toggleModi();
        delay(200);
    }


}


void toggleLight(int index) {
    LEDS.setAllStripsToClr(LEDS.Strips[0].Color(0, 0, 0));
    if (modi < modiCnt) {
        switch (index) {
            case 0: LEDS.setStripToClr(index, clrSets[index][modi]); break;
            case 1: LEDS.setStripToClr(index, clrSets[index][modi]); break;
            case 2: LEDS.setStripToClr(index, clrSets[index][modi]); break;
            case 3: LEDS.setStripToClr(index, clrSets[index][modi]); break;
            case 4: 
                Serial.println("doing case 4");
                LEDS.setStripToClr(0, clrSets[0][modi]);
                delay(15);
                LEDS.setStripToClr(1, clrSets[1][modi]);
                delay(15);
                LEDS.setStripToClr(2, clrSets[2][modi]);
                delay(15);
                LEDS.setStripToClr(3, clrSets[3][modi]); 
            break;
            default: break;
        }
    } else {
        Serial.println("toggling whites");
        if (index < 4) {
            LEDS.setStripToClr(index, LEDS.Strips[0].Color(255, 255, 255));
        }
    }
   
    if (stage > 4) {
        stage = -1;
    }
    delay(100);
}


void toggleModi() {
    modi++;
    switch (modi) {
        case 3: LEDS.setAllStripsToClr(LEDS.Strips[0].Color(255, 255, 255)); break;
        default: 
            stage = 4;
            toggleLight(stage);
        break;
    }
    if (modi > 3) {
        modi = 0;
    }
    delay(100);
}