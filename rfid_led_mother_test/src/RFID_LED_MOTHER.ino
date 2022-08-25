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
#include <stb_mother_ledCmds.h>


STB STB;
STB_MOTHER Mother;


void setup() {
    STB.begin();

    STB.i2cScanner();

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

    STB.dbgln(F("Completed settings"));
    initLeds();
    STB.printSetupEnd();
}


void loop() {
    wdt_reset();
    RFIDPolling();
}


void initLeds() {
    for (int i=0; i<RFID_LED_AMOUNT; i++) {
        LED_CMDS::setToClr(STB, i, LED_CMDS::clrRed, 50);
    }
}


/**
 * @brief handles outputs from Brain with RFIDs, sets them to green when a predefined card has beeen presented
 */
void RFIDPolling() {
    int lineCnt = 0;
    Mother.rs485PerformPoll();

    while (STB.rs485RcvdNextLn() && lineCnt++ < 5) {

        STB.dbgln(STB.rcvdPtr);

        if (strncmp(KeywordsList::rfidKeyword, STB.rcvdPtr, strlen(KeywordsList::rfidKeyword)) != 0) {
            STB.dbgln("continuing");
            continue; // skip all the other checks since its not RFID cmd
        }

        char* ptr = strtok(STB.rcvdPtr, "_");
        ptr = strtok(NULL, "_");
        if (ptr != NULL) {
            STB.dbgln("card detected: "); 
            STB.dbgln(ptr);
        }

        if (strncmp(rfidSolutions[Mother.rs485getPolledSlave()], ptr, strlen(rfidSolutions[Mother.rs485getPolledSlave()])) == 0) {    
            STB.dbg("correct card on slave");
            STB.dbgln(String(Mother.rs485getPolledSlave()));
            LED_CMDS::setToClr(STB, Mother.rs485getPolledSlave(), LED_CMDS::clrGreen, 50);
        }
    }

    lineCnt = 0;
    wdt_reset();
}