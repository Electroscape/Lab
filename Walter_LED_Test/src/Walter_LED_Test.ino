/**
*   2CP - TeamEscape - Engineering
*   Author Martin Pek
*   @date 30.06.2022
*   updated to lib_arduino version v 0.6.6
*/

/**************************************************************************/
// Setting Configurations
#include "header_st.h"

#include <stb_common.h>
#include <avr/wdt.h>
#include <stb_mother.h>
#include <stb_mother_ledCmds.h>

STB_MOTHER Mother;

void setup() {
    Mother.begin();
    Mother.rs485SetSlaveCount(4);
    // Mother.relayInit(Mother.motherRelay, relayPinArray, relayInitArray);

    Serial.println(F("WDT endabled"));
    wdt_enable(WDTO_8S);
    
    wdt_reset();

    /*
    int argsCnt = 2;
    // first value is the index of the Stripe, second value is the amount of LEDs on said Stripe
    int ledCount[argsCnt] = {0, 3};
    for (int brainNo=0; brainNo<Mother.rs485getSlaveCnt(); brainNo++) {
        Mother.setFlag(brainNo, cmdFlags::ledFlag, true);
        Mother.setFlag(brainNo, cmdFlags::rfidFlag, true);
        Mother.setFlag(brainNo, cmdFlags::oledFlag, true);
        Mother.flagsCompleted(brainNo);
        Mother.sendSetting(brainNo, settingCmds::ledCount, ledCount, argsCnt);
        Mother.settingsCompleted(brainNo);
    }
    Mother.dbgln(F("Completed settings"));
    */

    initLeds();
}


void loop() {
    wdt_reset();
}


void initLeds() {
    for (int i=0; i<RFID_LED_AMOUNT; i++) {
        delay(5);
        LED_CMDS::setToClr(Mother, i, LED_CMDS::clrRed, 50);
    }
}
