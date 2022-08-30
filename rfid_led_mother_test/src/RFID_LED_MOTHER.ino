/**
*   2CP - TeamEscape - Engineering
*   Author Martin Pek
*   @date 30.06.2022
*   build with lib_arduino v0.6.2
*/

/**************************************************************************/
// Setting Configurations
#include "header_st.h"

#include <stb_common.h>
#include <avr/wdt.h>
#include <stb_mother.h>
// #include <stb_mother_ledCmds.h>

STB_MOTHER Mother;

void setup() {
    // STB.rs485SetToMaster();
    Mother.rs485SetSlaveCount(1);

    Serial.println(F("WDT endabled"));
    wdt_enable(WDTO_8S);
    
    wdt_reset();

    int argsCnt = 2;
    // first value is the index of the Stripe, second value is the amount of LEDs on said Stripe
    int ledCount[argsCnt] = {0, 2};
    for (int brainNo=0; brainNo<Mother.rs485getSlaveCnt(); brainNo++) {
        Mother.setFlag(brainNo, cmdFlags::ledFlag, true);
        Mother.setFlag(brainNo, cmdFlags::rfidFlag, true);
        Mother.setFlag(brainNo, cmdFlags::oledFlag, true);
        Mother.flagsCompleted(brainNo);
        Mother.sendSetting(brainNo, settingCmds::ledCount, ledCount, argsCnt);
        Mother.settingsCompleted(brainNo);
    }

    Mother.dbgln(F("Completed settings"));
    initLeds();
}


void loop() {
    wdt_reset();
    RFIDPolling();
}


void initLeds() {
    for (int i=0; i<RFID_LED_AMOUNT; i++) {
        // LED_CMDS::setToClr(STB, i, LED_CMDS::clrRed, 50);
    }
}


/**
 * @brief handles outputs from Brain with RFIDs, sets them to green when a predefined card has beeen presented
 */
void RFIDPolling() {
    int lineCnt = 0;
    Mother.rs485PerformPoll();

    while (Mother.nextRcvdLn() && lineCnt++ < 5) {

        Mother.dbgln(Mother.STB_.rcvdPtr);

        if (strncmp(KeywordsList::rfidKeyword.c_str(), Mother.STB_.rcvdPtr, KeywordsList::rfidKeyword.length()) != 0) {
            Mother.dbgln("continuing");
            continue; // skip all the other checks since its not RFID cmd
        }

        char* ptr = strtok(Mother.STB_.rcvdPtr, "_");
        ptr = strtok(NULL, "_");
        if (ptr != NULL) {
            Mother.dbgln("card detected: "); 
            Mother.dbgln(ptr);
        }

        if (strncmp(rfidSolutions[Mother.rs485getPolledSlave()], ptr, strlen(rfidSolutions[Mother.rs485getPolledSlave()])) == 0) {    
            Mother.dbg("correct card on slave");
            Mother.dbgln(String(Mother.rs485getPolledSlave()));
            // LED_CMDS::setToClr(STB, Mother.rs485getPolledSlave(), LED_CMDS::clrGreen, 50);
        }
    }

    lineCnt = 0;
    wdt_reset();
}