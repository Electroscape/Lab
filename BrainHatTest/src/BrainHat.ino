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


STB STB;

PCF8574 relay;

Adafruit_NeoPixel LED_Strips[STRIPE_CNT];

#define rfidDisable 1

Adafruit_PN532 RFID_0(RFID_SSPins[0]);
Adafruit_PN532 RFID_READERS[1] = {RFID_0};


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

#ifndef rfidDisable
    STB_RFID::RFIDInit(RFID_0);
    wdt_reset();
#endif

/*
    STB_LED::setAllStripsToClr(LED_Strips, green);
    delay(2000);
    STB_LED::setAllStripsToClr(LED_Strips, red);
    delay(2000);
    STB_LED::setAllStripsToClr(LED_Strips, blue);
    delay(2000);
    */
    
    STB.printSetupEnd();
    Serial.setTimeout(100);
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);
    wdt_reset();
}

/*======================================
//===LOOP==============================
//====================================*/
void loop() {

#ifndef rfidDisable
    uint8_t data[16];
    if (STB_RFID::cardRead(RFID_READERS[0], data, 1)) {
        STB.dbgln((char *) data);
    }
#endif

    /*
    digitalWrite(MAX_CTRL_PIN, MAX485_WRITE);
    Serial.write("FUCK");
    Serial.flush();
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);
    delay(5000);
    */
    /*
    while (Serial.available() > 0) {
        STB.defaultOled.println("Receiving: ");
        STB.defaultOled.println(String(Serial.readString()));
    }
    */

    // STB.defaultOled.println(String(Serial.read()));
    // if (Serial.available ()) {
    //    STB.dbgln("stuff available");
    
    delay(100);
    wdt_reset();
}
