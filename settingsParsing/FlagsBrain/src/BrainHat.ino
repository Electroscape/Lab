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

SSD1306AsciiWire secondOled;


void setup() {
    
    STB.begin();

    STB.rs485SetSlaveAddr(0);
    BRAIN.receiveFlags(STB);
    STB.defaultOled.setScrollMode(SCROLL_MODE_AUTO);

    BRAIN.receiveSettings(STB);

    /*
    somehow row 1 is a duplicate of row, some inor bug here

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
    */

    if (BRAIN.flags[ledFlag]) {
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
