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

#define STRIPE_CNT             1
// doublecheck if this is inside the stb_led
#define RFID_1_LED_PIN          9 

int ledCnts[STRIPE_CNT] = {15};
int ledPins[STRIPE_CNT] = {RFID_1_LED_PIN};

Adafruit_NeoPixel LED_Strips[STRIPE_CNT];


STB STB;

void setup() {

    STB.begin();
    STB_LED::ledInit(LED_Strips, STRIPE_CNT, ledCnts, ledPins, NEO_GRB);
    STB.dbgln("HELLO");
}




/*======================================
//===LOOP==============================
//====================================*/
void loop() {
    

    uint32_t clrs[3] = {LED_Strips[0].Color(0, 255, 0), LED_Strips[0].Color(255, 0, 0), LED_Strips[0].Color(0, 0, 255)};
    STB_LED::setStripToClrs(LED_Strips[0], clrs, 3);

    delay(1000);
    STB_LED::setStripToClr(LED_Strips[0], LED_Strips[0].Color(0, 0, 255));
    delay(1000);
}