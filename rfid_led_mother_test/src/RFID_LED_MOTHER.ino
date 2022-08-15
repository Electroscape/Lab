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


STB STB;
STB_MOTHER Mother;


void setup() {
    STB.begin();

    STB.i2cScanner();

    STB.rs485SetToMaster();
    STB.rs485SetSlaveCount(4);

    Serial.println("WDT endabled");
    wdt_enable(WDTO_8S);
    
    wdt_reset();

    /*
    for (int brainNo=0; brainNo<STB.rs485getSlaveCnt(); brainNo++) {
        Mother.setFlag(STB, brainNo, cmdFlags::ledFlag, true);
        Mother.setFlag(STB, brainNo, cmdFlags::rfidFlag, true);
        Mother.setFlag(STB, brainNo, cmdFlags::oledFlag, true);
        Mother.flagsCompleted(STB, brainNo);
    }

    
    int argsCnt = 2;
    // first value is the index of the Stripe, second value is the amount of LEDs on said Stripe
    int ledCount[argsCnt] = {0, 2};
    Mother.sendSetting(STB, 0, settingCmds::ledCount, ledCount, argsCnt);
    ledCount[0] = 1;
    ledCount[1] = 1;
    Mother.sendSetting(STB, 0, settingCmds::ledCount, ledCount, argsCnt);
    ledCount[0] = 2;
    ledCount[1] = 3;
    Mother.sendSetting(STB, 0, settingCmds::ledCount, ledCount, argsCnt);
    ledCount[0] = 3;
    ledCount[1] = 3;
    Mother.sendSetting(STB, 0, settingCmds::ledCount, ledCount, argsCnt);
    Mother.settingsCompleted(STB, 0);
    */

    STB.dbgln("Completed settings");
    STB.printSetupEnd();
}


void loop() {
    wdt_reset();
    RFIDPolling();
}


void RFIDPolling() {
    int lineCnt = 0;
    STB.rs485PerformPoll();

    while (STB.rs485RcvdNextLn() && lineCnt++ < 5) {

        STB.dbgln(STB.rcvdPtr);

        if (strncmp(Mother.keyWords.rfidKeyword, STB.rcvdPtr, strlen(Mother.keyWords.rfidKeyword)) != 0) {
            STB.dbgln("continuing");
            continue; // skip all the other checks since its not RFID cmd
        }

        char* ptr = strtok(STB.rcvdPtr, "_");
        ptr = strtok(NULL, "_");
        if (ptr != NULL) {
            STB.dbgln("card detected: "); 
            STB.dbgln(ptr);
        }

        if (strncmp(rfidSolutions[STB.rs485getPolledSlave()], ptr, strlen(rfidSolutions[STB.rs485getPolledSlave()])) == 0) {    
            STB.dbg("correct card on slave");
            STB.dbgln(String(STB.rs485getPolledSlave()));
        }
    }

    lineCnt = 0;
    wdt_reset();
}