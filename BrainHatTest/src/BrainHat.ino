/**
 * @file BrainHat.ino
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-05-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**************************************************************************/
// Setting Configurations
#include "header_st.h"

#include <stb_common.h>
#include <stb_led.h>
#include <avr/wdt.h>
#include <stb_rfid.h>
#include <Keypad_I2C.h>
#include <Password.h> 


STB STB;

int testVal = 0;

void setup() {
    STB.begin();
    STB.dbgln("v1.1");
    Serial.println("WDT endabled");
    // wdt_enable(WDTO_8S);

    STB.i2cScanner();
    STB.rs485SetSlaveAddr(0);
    
    STB.printSetupEnd();
    Serial.setTimeout(100);
    wdt_reset();
}



/*======================================
//===LOOP==============================
//====================================*/
void loop() {
    
    // this can be used to fwd the serial input for the master to an usb port on the RPI
    if (Serial.available()) {
        Serial.write(Serial.read());
    }

    /*
    delay(100);
    
    STB.rs485Write("Brain response" + String(testVal));
    testVal++;

    if (testVal > 1000) {
        testVal = 0;
    }
    */

    // wdt_reset();
}