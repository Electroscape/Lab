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
#include <stb_rfid.h>
#include <stb_common.h>
#include <stb_brain.h>

STB STB;
STB_BRAIN BRAIN;


void setup() {
    
    STB.begin();
    STB.rs485SetSlaveAddr(0);
    BRAIN.receiveFlags(STB);
    delay(9000);
}


/*======================================
//===LOOP==============================
//====================================*/
void loop() {
}
