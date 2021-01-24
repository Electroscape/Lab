/*
        2CP - TeamEscape - Engineering
        Author Martin Pek

        changelog
        v0.1

        Todo:
        -hello!
*/

/**************************************************************************/
// I2C Port Expander
#include "PCF8574.h"
#include <Wire.h>
#include <SPI.h>

//Watchdog timer
#include <avr/wdt.h>

String title = "PN532 Example";
String versionDate = "22.01.2020";
String version = "version 0.1wip";

#include <FastLED.h>

#define RELAY_I2C_ADD     	0x3F         // Relay Expander
#define OLED_I2C_ADD        0x3C         // Predefined by hardware
#define LCD_I2C_ADD         0x27         // Predefined by hardware

// uncomment to use
#define DEBUGMODE           1


#define REL_AMOUNT      8

#define NR_OF_LEDS              1     /* Anzahl der Pixel auf einem Strang (Test 1 Pixel)                   */
// BE CAREFUL WHEN SETTING ALL LEDS to a colour use the socket count for this
#define LED_STRIP_TYPE          WS2811
#define COLOR_ORDER             GRB // BRG

#define RFID_1_LED_PIN          9     /* Per Konvention ist dies RFID-Port 1                                */
#define RFID_2_LED_PIN          6     /* Per Konvention ist dies RFID-Port 2                                */
#define RFID_3_LED_PIN          5     /* Per Konvention ist dies RFID-Port 3                                */
#define RFID_4_LED_PIN          3     /* Per Konvention ist dies RFID-Port 4                                */

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK               13
#define PN532_MOSI              11
#define PN532_MISO              12

#define RFID_1_SS_PIN           8     /* Per Konvention ist dies RFID-Port 1                                */
#define RFID_2_SS_PIN           7     /* Per Konvention ist dies RFID-Port 2                                */
#define RFID_3_SS_PIN           4     /* Per Konvention ist dies RFID-Port 3                                */
#define RFID_4_SS_PIN           2     /* Per Konvention ist dies RFID-Port 4                                */

const byte RFID_SSPins[]  = {RFID_1_SS_PIN,  RFID_2_SS_PIN,  RFID_3_SS_PIN,  RFID_4_SS_PIN};


// == PN532 imports and setup
#include <Adafruit_PN532.h>

// only a max of 3 PWMS possible hence 3 stripes max, any further LEDs need to be addressed trhough the 3rd stipe
// here we go: 'LED_PINS' is not usable in a constant expression,
// cannot use arrays bec thats too newschool.
// MODIFIY IN LED_INIT ASWELL
// very manual but ... its C its gonna be bitching when it doesnt know during compilte time
// uncomment as needed

#define STRIPE_CNT  8
#define RFID_AMOUNT 8

CRGB LED_STRIPE_1[NR_OF_LEDS];
CRGB LED_STRIPE_2[NR_OF_LEDS];
CRGB LED_STRIPE_3[NR_OF_LEDS];
CRGB LED_STRIPE_4[NR_OF_LEDS];
CRGB LED_STRIPE_5[NR_OF_LEDS];
CRGB LED_STRIPE_6[NR_OF_LEDS];
CRGB LED_STRIPE_7[NR_OF_LEDS];
CRGB LED_STRIPE_8[NR_OF_LEDS];

static CRGB LED_STRIPES[STRIPE_CNT] = {LED_STRIPE_1, LED_STRIPE_2, LED_STRIPE_3, LED_STRIPE_4, LED_STRIPE_5, LED_STRIPE_6, LED_STRIPE_7, LED_STRIPE_8};





/*==PCF8574=================================================================================================*/
Expander_PCF8574 relay;

void setup() {

    Serial.println("WDT endabled");
    wdt_enable(WDTO_8S);

    communication_init();

    Serial.println();
    Serial.println("LED: ... ");
    if (LED_init()) {Serial.println("LED: OK!");} else {Serial.println("LED: FAILED!");};

}

void loop() {



    for (int stripe_nr = 0; stripe_nr < 8; stripe_nr++) {
        Serial.print("loop LED stripe: "); Serial.println(stripe_nr);
        led_set_clrs(stripe_nr, CRGB::Red, NR_OF_LEDS);
        delay(500);
        led_set_clrs(stripe_nr, CRGB::Green, NR_OF_LEDS);
        delay(500);
        led_set_clrs(stripe_nr, CRGB::Black, NR_OF_LEDS);
    }

    delay(20);


    wdt_reset();
}

bool LED_init() {

    // yep, exactly what it looks like...
    // we need to do it manually since we cannot define arrays easily for the preprocessor
    LEDS.addLeds<LED_STRIP_TYPE, RFID_1_LED_PIN, COLOR_ORDER>(LED_STRIPE_1, NR_OF_LEDS);
    LEDS.addLeds<LED_STRIP_TYPE, RFID_2_LED_PIN, COLOR_ORDER>(LED_STRIPE_2, NR_OF_LEDS);
    LEDS.addLeds<LED_STRIP_TYPE, RFID_3_LED_PIN, COLOR_ORDER>(LED_STRIPE_3, NR_OF_LEDS);
    LEDS.addLeds<LED_STRIP_TYPE, RFID_4_LED_PIN, COLOR_ORDER>(LED_STRIPE_4, NR_OF_LEDS);
    LEDS.addLeds<LED_STRIP_TYPE, RFID_1_SS_PIN, COLOR_ORDER>(LED_STRIPE_5, NR_OF_LEDS);
    LEDS.addLeds<LED_STRIP_TYPE, RFID_2_SS_PIN, COLOR_ORDER>(LED_STRIPE_6, NR_OF_LEDS);
    LEDS.addLeds<LED_STRIP_TYPE, RFID_3_SS_PIN, COLOR_ORDER>(LED_STRIPE_7, NR_OF_LEDS);
    LEDS.addLeds<LED_STRIP_TYPE, RFID_4_SS_PIN, COLOR_ORDER>(LED_STRIPE_8, NR_OF_LEDS);
    LEDS.setBrightness(100);
    delay(20);

    for (int stripe_nr = 0; stripe_nr < 8; stripe_nr++) {
        Serial.print("LED stripe: "); Serial.println(stripe_nr);
        led_set_clrs(stripe_nr, CRGB::Red, NR_OF_LEDS);
        delay(500);
        led_set_clrs(stripe_nr, CRGB::Green, NR_OF_LEDS);
        delay(500);
        led_set_clrs(stripe_nr, CRGB::Black, NR_OF_LEDS);
        wdt_reset();
    }
    delay(20);

    return true;
}

void led_set_all_clrs(CRGB clr, int led_cnt) {
    for(int stripe_nr=0; stripe_nr<RFID_AMOUNT; stripe_nr++) {
        led_set_clrs(stripe_nr, clr, NR_OF_LEDS);
    }
}

void led_set_clrs(int stripe_nr, CRGB clr, int led_cnt) {
    for(size_t i = 0; i < led_cnt; i++) {
        switch(stripe_nr) {
            case 0:
                LED_STRIPE_1[i] = clr; break;
            case 1:
                LED_STRIPE_2[i] = clr; break;
            case 2:
                LED_STRIPE_3[i] = clr; break;
            case 3:
                LED_STRIPE_4[i] = clr; break;
            case 4:
                LED_STRIPE_5[i] = clr; break;
            case 5:
                LED_STRIPE_6[i] = clr; break;
            case 6:
                LED_STRIPE_7[i] = clr; break;
            case 7:
                LED_STRIPE_8[i] = clr; break;
            default: Serial.println("wrong led selection"); break;
        }
    }
    FastLED.show();
    delay(10*led_cnt);
}


void print_serial_header() {
    Serial.println("!header_begin");
    Serial.println(title);
    Serial.println(versionDate);
    Serial.println(version);
}

void communication_init() {
    Wire.begin();
    Serial.begin(115200);
    delay(20);
    print_serial_header();
}

void print_setup_end() {
    Serial.println("!setup_end");
    Serial.println(); Serial.println("===================START====================="); Serial.println();
}

/*==RESET==================================================================================================*/

void software_Reset() {
    Serial.println(F("Restarting in"));
    delay(50);
    for (byte i = 3; i>0; i--) {
        Serial.println(i);
        delay(100);
    }
    asm volatile ("  jmp 0");
}
