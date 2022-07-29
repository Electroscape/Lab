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

STB STB;
STB_BRAIN BRAIN;
STB_LED LEDS;

/*
Adafruit_NeoPixel Strips[STRIPE_CNT];
const long int green = Strips[0].Color(30,30,30);
*/

SSD1306AsciiWire secondOled;


void setup() {
    
    STB.begin();

    /*
    delay(10);
    Strips[0] = Adafruit_NeoPixel((uint16_t) 3, (int16_t) 9, (NEO_BRG + NEO_KHZ800));
    Strips[0].begin();
    delay(50);
    Strips[0].setPixelColor(0, Strips[0].Color(0, 0, 50));
    Strips[0].setPixelColor(1, Strips[0].Color(50, 50, 50));
    Strips[0].setPixelColor(2, Strips[0].Color(100, 0, 0));
    Strips[0].show();

    delay(1500);
    */

    STB.dbgln("v.0.0.11");
    STB.rs485SetSlaveAddr(0);
    LEDS.begin();
    // BRAIN.receiveFlags(STB);
    STB.defaultOled.setScrollMode(SCROLL_MODE_AUTO);

    BRAIN.receiveSettings(STB);


    /*
    STB.dbgln("received settings");
    STB.dbgln(String(BRAIN.settings[5][0]));
    */

    STB.dbgln("settings matrix");
    STB.dbgln(String(BRAIN.settings[0][0]));
    STB.dbgln(String(BRAIN.settings[0][1]));
    STB.dbgln(String(BRAIN.settings[0][2]));
    STB.dbgln("Setting Row 1");
    STB.dbgln(String(BRAIN.settings[1][0]));
    STB.dbgln(String(BRAIN.settings[1][1]));
    STB.dbgln(String(BRAIN.settings[1][2]));
    STB.dbgln("Setting Row 3");
    STB.dbgln(String(BRAIN.settings[3][0]));
    STB.dbgln(String(BRAIN.settings[3][1]));
    STB.dbgln(String(BRAIN.settings[3][2]));

    BRAIN.settings[0][0] = 0;
    BRAIN.settings[0][1] = 0;
    BRAIN.settings[0][2] = 6;


    if (BRAIN.flags[ledFlag] || true) {
        STB.dbgln("doing LED init");
        LEDS.ledInit(BRAIN.settings);
        delay(100);
        // LEDS.setAllStripsToClr(LEDS.Strips[0].Color(100, 100,100));
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
}


/*======================================
//===LOOP==============================
//====================================*/
void loop() {

}
