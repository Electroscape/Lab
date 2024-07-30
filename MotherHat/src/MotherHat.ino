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
    // Mother.rs485PerformPoll();
    // Mother.motherRelay.digitalWrite()
    MotherIO.setOuput(1 << inc);

    int res = MotherIO.getInputs();
    Serial.println(res);
    inc++;
    if (inc > 8) {
        inc = 0;
    }

    delay(1000);
}
