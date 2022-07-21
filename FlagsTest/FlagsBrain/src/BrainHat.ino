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

Adafruit_NeoPixel LED_Strips[STRIPE_CNT];
const long int green = LED_Strips[0].Color(0,255,0);

SSD1306AsciiWire secondOled;

int ledCnts[STRIPE_CNT] = {100};
int ledPins[STRIPE_CNT] = {RFID_1_LED_PIN};


void setup() {
    
    STB.begin();
    STB.rs485SetSlaveAddr(0);
    BRAIN.receiveFlags(STB);

    if (BRAIN.flags[LED]) {
        STB.dbgln("doing LED init");
        STB_LED::ledInit(LED_Strips, 1, ledCnts, ledPins, NEO_BRG);
        STB_LED::setAllStripsToClr(LED_Strips, 1, green);
    }

    if (BRAIN.flags[RFID]) {
        STB.dbgln("doing RFID init");
        Adafruit_PN532 RFID_0(PN532_SCK, PN532_MISO, PN532_MOSI, RFID_1_SS_PIN);
        Adafruit_PN532 RFID_READERS[1] = {RFID_0};
    }

    if (BRAIN.flags[OLED]) {
        STB.dbgln("doing Oled init");
        STB_OLED::oledInit(&secondOled , SH1106_128x64, 61);
    }

}


/*======================================
//===LOOP==============================
//====================================*/
void loop() {

}
