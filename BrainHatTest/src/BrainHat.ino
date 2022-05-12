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
#include <stb_led.h>
#include <avr/wdt.h>


STB STB;

PCF8574 relay;

Adafruit_NeoPixel LED_Strips[STRIPE_CNT];

const long int red = LED_Strips[0].Color(255,0,0);
const long int green = LED_Strips[0].Color(0,255,0);
const long int blue = LED_Strips[0].Color(0,0,255);


void setup() {
    STB.begin();
    STB.dbgln("v1");
    Serial.println("WDT endabled");
    wdt_enable(WDTO_8S);
    if (STB_LED::ledInit(LED_Strips, ledCnts, ledPins, NEO_BRG)) {Serial.println("LED: OK!");} else {Serial.println("LED: FAILED!");}

    STB.i2cScanner();

    wdt_reset();

    STB_LED::setAllStripsToClr(LED_Strips, green);
    delay(2000);
    STB_LED::setAllStripsToClr(LED_Strips, red);
    delay(2000);
    STB_LED::setAllStripsToClr(LED_Strips, blue);
    delay(2000);
    
    STB.printSetupEnd();
    Serial.setTimeout(100);
    Serial.begin(9600);
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);
}

/*======================================
//===LOOP==============================
//====================================*/
void loop() {
    // STB.defaultOled.println(String(Serial.available()));
    if (Serial.available() > 0) {
        STB.defaultOled.println("Receiving: ");
        STB.defaultOled.println(String(Serial.read()));
    }
    // STB.defaultOled.println(String(Serial.read()));
    // if (Serial.available ()) {
    //    STB.dbgln("stuff available");
    

    wdt_reset();
}
