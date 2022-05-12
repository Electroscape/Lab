/**
*   2CP - TeamEscape - Engineering
*   Author Martin Pek & Abdullah Saei
*
*   - use namespace
*   - Modified Serial outputs
*   - Optimize initialization delay to smooth restarts.
*   - Locking after correct solution.
*
*/

/**************************************************************************/
// Setting Configurations
#include "header_st.h"

#include <stb_common.h>
#include <avr/wdt.h>


STB STB;

PCF8574 relay;


void setup() {
    STB.begin();
    STB.dbgln("new Stuff");
    Serial.println("WDT endabled");
    wdt_enable(WDTO_8S);

    STB.i2cScanner();

    wdt_reset();
    STB.printSetupEnd();
    Serial.setTimeout(100);
    Serial.begin(9600);
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);
}

/*======================================
//===LOOP==============================
//====================================*/
void loop() {
    STB.defaultOled.println(String(Serial.available()));
    if (Serial.available() > 0) {
        STB.defaultOled.println("Receiving: ");
        STB.defaultOled.println(String(Serial.read()));
    }
    // STB.defaultOled.println(String(Serial.read()));
    // if (Serial.available ()) {
    //    STB.dbgln("stuff available");
    

    wdt_reset();
}
