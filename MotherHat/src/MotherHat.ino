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
PCF8574 reset;

void setup() {
    STB.begin();
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
    digitalWrite(MAX_CTRL_PIN, MAX485_WRITE);
}

/*======================================
//===LOOP==============================
//====================================*/
void loop() {
    Serial.println("\n");
    Serial.println("Mother");
    Serial.println("Tell your children\n");
    Serial.println("Not to walk my way\n");
    Serial.flush();
    Serial.println("Tell your children\n");
    Serial.println("not to hear my words\n");
    Serial.flush();
    wdt_reset();
    delay(1000);
}
