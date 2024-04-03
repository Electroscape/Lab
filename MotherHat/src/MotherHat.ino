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
    Mother.relayInit();
    Serial.println("WDT endabled");
}


void loop() {
    Mother.rs485PerformPoll();
}
