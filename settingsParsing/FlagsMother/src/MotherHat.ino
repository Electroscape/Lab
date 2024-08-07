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
    STB.rs485SetSlaveCount(1);

    Serial.println("WDT endabled");
    wdt_enable(WDTO_8S);
    
    wdt_reset();

    /*
    Mother.setFlag(STB, 0, cmdFlags::ledFlag, true);
    Mother.setFlag(STB, 0, cmdFlags::rfidFlag, true);
    Mother.setFlag(STB, 0, cmdFlags::oledFlag, true);
    Mother.flagsCompleted(STB, 0);
    */

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

    STB.dbgln("Completed settings");

    // using the wdt to reboot
    delay(9000);
    
    STB.printSetupEnd();
}


void loop() {
    STB.rs485PerformPoll();
}
