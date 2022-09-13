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
 * 
 */


#include <stb_mother.h>
#include <stb_keypadCmds.h>
#include <stb_oledCmds.h>

#include "header_st.h"



STB_MOTHER Mother;
int stage = 0;
// doing this so the first time it updates the brains oled without an exta setup line
int lastStage = -1;


void setup() {

    Mother.begin();
    // starts serial and default oled

    Serial.println("WDT endabled");
    wdt_enable(WDTO_8S);

    Mother.rs485SetSlaveCount(1);

    // Mother.setFlag(STB, 0, cmdFlags::LED, true);

    // STB.printSetupEnd();
}


// candidate to be moved to a mother specific part of the keypad lib
bool checkForKeypad() {

    Mother.STB_.dbgln("checkforKeypad");
    Mother.STB_.dbgln(Mother.STB_.rcvdPtr);

    if (passwordMap[stage] < 0) { return false; }

    if (strncmp(keypadCmd.c_str(), Mother.STB_.rcvdPtr, keypadCmd.length()) == 0) {

        Mother.sendAck();
        
        char *cmdPtr = strtok(Mother.STB_.rcvdPtr, KeywordsList::delimiter.c_str());
        cmdPtr = strtok(NULL, KeywordsList::delimiter.c_str());
        int cmdNo;
        sscanf(cmdPtr, "%d", &cmdNo);

        if (cmdNo == KeypadCmds::evaluate) {

            cmdPtr = strtok(NULL, KeywordsList::delimiter.c_str());
            Serial.println("password is: ");
            Serial.println(cmdPtr);
            delay(500);
            // TODO: error handling here in case the rest of the msg is lost?
            if (!(cmdPtr != NULL)) {
                // send NACK? this isnt in the control flow yet
                return false;
            }
            // TODO: check for ACK handling on Brain
            // maybe have optional bool for NACK?
            
            char msg[10] = "";
            strcpy(msg, keypadCmd.c_str());
            strcat(msg, KeywordsList::delimiter.c_str());
            char noString[3];
            if (strncmp(passwords[stage], cmdPtr, strlen(passwords[stage]) ) == 0) {
                sprintf(noString, "%d", KeypadCmds::correct);
                strcat(msg, noString);
                Mother.dbg("increased stage to");
                Mother.dbgln(String(stage));
                stage++;
            } else {
                sprintf(noString, "%d", KeypadCmds::wrong);
                strcat(msg, noString);
                Mother.dbg("wrong pass");
                Mother.dbgln(String(stage));
            }
            // slaveNo needs to be really optional at this point. the default shall be polledslave
            Mother.sendCmdToSlave(msg);
        }
        
        return true;
    }
    return false;
}


// again good candidate for a mother specific lib
bool checkForRfid() {
    if (passwordMap[stage] < 0) { return false; }
    // not matching the keyword
    if (strncmp(KeywordsList::rfidKeyword.c_str(), Mother.STB_.rcvdPtr, KeywordsList::rfidKeyword.length() ) != 0) {
        return false;
    } 
    Mother.STB_.rs485SendAck();
    char *cmdPtr = strtok(Mother.STB_.rcvdPtr, KeywordsList::delimiter.c_str());
    cmdPtr = strtok(NULL, KeywordsList::delimiter.c_str());
    if (strncmp(passwords[passwordMap[stage]], cmdPtr, strlen(passwords[passwordMap[stage]]) ) == 0 ) {
        // send some form of validation to trigger buzzer
        stage++;
    }
    return true;
}


void interpreter() {
    while (Mother.nextRcvdLn()) {
        if (checkForKeypad()) {continue;};
        if (checkForRfid()) {continue;};
    }
}

void stageUpdate() {
    if (lastStage == stage) { return; }
    char msg[32];
    strcpy(msg, oledHeaderCmd.c_str());
    strcat(msg, KeywordsList::delimiter.c_str());
    strcat(msg, stageTexts[stage]); 
    Mother.sendCmdToSlave(msg);
    lastStage = stage;

    // update flags
}


void loop() {
    Mother.rs485PerformPoll();
    interpreter();
    stageUpdate();
    wdt_reset();

    delay(500);
    // Mother.sendCmdToSlave(1, msg);
}




