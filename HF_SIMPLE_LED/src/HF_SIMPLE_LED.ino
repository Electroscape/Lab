/**
 * @file BREAKOUT.ino
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 6.04.2022
 * 
 */

String versionDate = "13.04.2022";
String version = "0.0.1";

#include "header_st.h"
// I2C Port Expander

#include <stb_common.h>
#include <stb_led.h>


Adafruit_NeoPixel LED_Strips[STRIPE_CNT];
const long int white = LED_Strips[0].Color(255,255,255);
const long int black = LED_Strips[0].Color(0,0,0);


void setup() {

    STB::begin();
    STB_LED::ledInit(LED_Strips, ledCnts, ledPins, NEO_BRG);

}

void loop() {
    STB_LED::setAllStripsToClr(LED_Strips, white);
    delay(5000);
    STB_LED::setAllStripsToClr(LED_Strips, black);
    delay(5000);
}

