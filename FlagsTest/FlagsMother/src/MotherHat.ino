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


    /*
    resetPCF.begin(0x3D);
    for (int i = 0; i < 8; i++) {
        resetPCF.pinMode(i, OUTPUT);
        resetPCF.digitalWrite(i,1);
    }
    */
    
    wdt_reset();



    Mother.setFlag(STB, 0, cmdFlags::LED, true);
    Mother.setFlag(STB, 0, cmdFlags::RFID, true);
    Mother.setFlag(STB, 0, cmdFlags::OLED, true);
    Mother.flagsCompleted(STB, 0);

    // using the wdt to reboot
    delay(9000);
    
    STB.printSetupEnd();
}


void loop() {
    STB.rs485PerformPoll();
}
