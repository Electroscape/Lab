/**
*   2CP - TeamEscape - Engineering
*   Author Martin Pek
*
*/

/**************************************************************************/
// Setting Configurations
#include "header_st.h"

#include <stb_common.h>
#include <avr/wdt.h>


STB STB;

PCF8574 relay;
PCF8574 reset;

void setup() {
    STB.begin();
    STB.rs485SetToMaster();
    STB.dbgln("sth sth");
    Serial.println("WDT endabled");
    wdt_enable(WDTO_8S);

    STB.i2cScanner();
    reset.begin(0x3D);
    for (int i = 0; i < 8; i++) {
        reset.pinMode(i, OUTPUT);
    }

    STB.relayInit(relay, relayPinArray, relayInitArray, REL_AMOUNT);
    
    wdt_reset();
    STB.printSetupEnd();
}

/*======================================
//===LOOP==============================
//====================================*/
void loop() {
    STB.rs485PerformPoll();
    wdt_reset();
}
