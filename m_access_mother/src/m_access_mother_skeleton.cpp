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
int stage = stage1;
// doing this so the first time it updates the brains oled without an exta setup line
int lastStage = -1;


void setup() {

    Mother.begin();
    // starts serial and default oled

    Serial.println("WDT endabled");
    wdt_enable(WDTO_8S);

    Mother.rs485SetSlaveCount(1);

    // STB.printSetupEnd();
}


bool passwordInterpreter(char* password) {
    for (int passNo; passNo < PasswordAmount; passNo++) {
        if (passwordMap[passNo] & stage) {
            Serial.print("activ pass:");
            Serial.println(passwords[passNo]);
            // get MSB here 
            if (strncmp(passwords[stage], password, strlen(passwords[stage]) ) == 0) {
                stage = stage << 1;
                delay(6000);
                return true;
            }
        }
    }
    delay(6000);
    return false;
}


// candidate to be moved to a mother specific part of the keypad lib
bool checkForKeypad() {

    Mother.STB_.dbgln("checkforKeypad");
    Mother.STB_.dbgln(Mother.STB_.rcvdPtr);

    if (strncmp(keypadCmd.c_str(), Mother.STB_.rcvdPtr, keypadCmd.length()) != 0) {
        return false;
    }
    Mother.sendAck();

    char *cmdPtr = strtok(Mother.STB_.rcvdPtr, KeywordsList::delimiter.c_str());
    cmdPtr = strtok(NULL, KeywordsList::delimiter.c_str());
    int cmdNo;
    sscanf(cmdPtr, "%d", &cmdNo);

    // no evaluation requiested
    if (cmdNo != KeypadCmds::evaluate) { return true; }

    cmdPtr = strtok(NULL, KeywordsList::delimiter.c_str());
    Serial.println("password is: ");
    Serial.println(cmdPtr);
    delay(500);

    // TODO: error handling here in case the rest of the msg is lost?
    if (!(cmdPtr != NULL)) {
        // send NACK? this isnt in the control flow yet or simply eof?
        return false;
    }

    // prepare return msg with correct or incorrect
    char msg[10] = "";
    char noString[3];
    strcpy(msg, keypadCmd.c_str());
    strcat(msg, KeywordsList::delimiter.c_str());
            
    if (passwordInterpreter(cmdPtr)) {
        sprintf(noString, "%d", KeypadCmds::correct);
        strcat(msg, noString);
    } else {
        sprintf(noString, "%d", KeypadCmds::wrong);
        strcat(msg, noString);
    }


    strcat(msg, noString);
    Mother.sendCmdToSlave(msg);
    return true;
}


// again good candidate for a mother specific lib
bool checkForRfid() {

    if (strncmp(KeywordsList::rfidKeyword.c_str(), Mother.STB_.rcvdPtr, KeywordsList::rfidKeyword.length() ) != 0) {
        return false;
    } 
    char *cmdPtr = strtok(Mother.STB_.rcvdPtr, KeywordsList::delimiter.c_str());
    cmdPtr = strtok(NULL, KeywordsList::delimiter.c_str());
    
    // return msg with correct or incorrect
    // could be simply universal CodeCorrect
    char msg[10] = "";
    strcpy(msg, keypadCmd.c_str());
    strcat(msg, KeywordsList::delimiter.c_str());
    char noString[3];

    if (passwordInterpreter(cmdPtr)) {
        sprintf(noString, "%d", KeypadCmds::correct);
        strcat(msg, noString);
    } else {
        sprintf(noString, "%d", KeypadCmds::wrong);
        strcat(msg, noString);
    }
    Mother.sendCmdToSlave(msg);
    // blocking
    delay(5000);
    return true;
}


void interpreter() {
    while (Mother.nextRcvdLn()) {
        if (checkForKeypad()) {continue;};
        if (checkForRfid()) {continue;};
        Serial.println("interpreter");
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
    // Mother.sendCmdToSlave(1, msg);
}




