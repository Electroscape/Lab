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
