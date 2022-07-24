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

#include <stb_oled.h>
#include <stb_common.h>
#include <stb_brain.h>


STB STB;
STB_BRAIN BRAIN;


void setup() {
    STB.begin();
    // STB.defaultOled.setRow(uint8_t row);
    STB.defaultOled.clear();
    STB.defaultOled.println("what the fuck");
    STB.defaultOled.println("Testq2");
    STB.defaultOled.println("Testq3");
    STB.defaultOled.println("Testq4");
    STB.defaultOled.println("Testq5");
    // STB.defaultOled.setScroll(false);

    STB_OLED::clearAbove(STB.defaultOled, (uint8_t) 3);
    // without teh space the w hat is split into 2 lines ...
    STB.defaultOled.println(" what the fuck");
}


/*======================================
//===LOOP==============================
//====================================*/
void loop() {
    // STB.defaultOled.setRow((uint8_t) 1);
    // STB.defaultOled.println("newline");
    delay(500);
}