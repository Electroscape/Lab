<<<<<<< Updated upstream
/**
*   2CP - TeamEscape - Engineering
*   Author Martin Pek
*
*/

/**************************************************************************/
// Setting Configurations
#include "header_st.h"

#include <stb_mother.h>
#include <stb_mother_ledCmds.h>
#include <stb_mother_IO.h>

STB_MOTHER Mother;
STB_MOTHER_IO MotherIO;

int inc = 0;


void setup() {
    Mother.begin();
    MotherIO.ioInit();
    Mother.relayInit(relayPinArray, relayInitArray, REL_AMOUNT);
    Serial.println("WDT endabled");
}


void loop() {
    Mother.rs485PerformPoll();
    delay(1000);
            /*
    Mother.relayWrite(REL_0_PIN, HIGH);
    Mother.relayWrite(REL_1_PIN, HIGH);
    Mother.relayWrite(REL_2_PIN, HIGH);
    Mother.relayWrite(REL_3_PIN, HIGH);
    Mother.relayWrite(REL_4_PIN, HIGH);
    Mother.relayWrite(REL_5_PIN, HIGH);
    Mother.relayWrite(REL_6_PIN, HIGH);
    Mother.relayWrite(REL_7_PIN, HIGH);
    */

    delay(1000);
}
=======
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

// PCF8574 relay;
PCF8574 reset;

void setup() {
    STB.begin();
    STB.rs485SetToMaster();
    Serial.println("WDT endabled");
    wdt_enable(WDTO_8S);

    STB.i2cScanner();
    reset.begin(0x3D);
    for (int i = 0; i < 8; i++) {
        reset.pinMode(i, OUTPUT);
    }
    
    wdt_reset();
    STB.printSetupEnd();
    STB.dbgln("MOther");
}

/*======================================
//===LOOP==============================
//====================================*/
void loop() {
    STB.rs485PerformPoll();
    wdt_reset();
}
>>>>>>> Stashed changes
