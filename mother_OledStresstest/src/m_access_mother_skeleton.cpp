/**
 * @file m_access_mother_skeleton.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-09-09
 * 
 * @copyright Copyright (c) 2022
 * 
 *  TODO: 
 *  - check passwords and passwordsmap usages 
 *  - consider a generic code/combination evaluation
 *  - map actions to passwords, change stage in these actions
 *  - consider non blocking 
 */


#include <stb_mother.h>
#include <stb_keypadCmds.h>
#include <stb_oledCmds.h>

#include "header_st.h"



STB_MOTHER Mother;



    
void setup() {

    Mother.begin();
    // starts serial and default oled

    Serial.println("WDT endabled");
    wdt_enable(WDTO_8S);

    Mother.rs485SetSlaveCount(1);

    // STB.printSetupEnd();
}


void stageUpdate(int sleepdurtation) {
    char msg[32];
    strcpy(msg, oledHeaderCmd.c_str());
    strcat(msg, KeywordsList::delimiter.c_str());
    strcat(msg, "Sleep ");
    char noString[5] = ""; 
    sprintf(noString, "%d", sleepdurtation);
    strcat(msg, noString);
    Mother.sendCmdToSlave(msg, 0);
    delay((unsigned long) sleepdurtation);

}


void loop() {
    Mother.rs485PerformPoll();
    for (int i=1000; i>=0; i -= 10) {
        stageUpdate(i);
        wdt_reset();
    }

    // Mother.sendCmdToSlave(1, msg);
}




