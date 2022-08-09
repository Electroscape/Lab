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
}


/*======================================
//===LOOP==============================
//====================================*/
void loop() {
    if (Serial.available()) {
        Serial.write(Serial.read());
    }
}
