/*=======================================================*/
/**		2CP - TeamEscape - Engineering
 *		by Martin Pek
/*=======================================================*/
#include <stb_namespace.h>

#include "header_s.h"
using namespace stb_namespace;

/*==INCLUDE==============================================*/
// I2C Port Expander
#include <PCF8574.h>

// OLED
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

// Keypad
#include <Keypad.h>
#include <Keypad_I2C.h>
#include <Wire.h>

#include <FastLED.h>

/*==OLED======================================*/
SSD1306AsciiWire oled;
unsigned long lastHeartbeat = millis();

// LEDS
CRGB leds[2][NUM_LEDS_5V + NUM_LEDS_12V];

static uint menuStage = 0;
static uint ledSelection = 0;

const byte KEYPAD_ROWS = 4;  // Zeilen
const byte KEYPAD_COLS = 3;  // Spalten
byte KeypadRowPins[KEYPAD_ROWS] = {1, 6, 5, 3};  // Zeilen  - Messleitungen
byte KeypadColPins[KEYPAD_COLS] = {2, 0, 4};     // Spalten - Steuerleitungen (abwechselnd HIGH)


char KeypadKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};


unsigned long oledLastUpdate = millis();

Keypad_I2C MyKeypad(makeKeymap(KeypadKeys), KeypadRowPins, KeypadColPins,
                    KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_I2C_ADD, PCF8574);


void setup() {
    brainSerialInit();
    Serial.println(title);
    Serial.println(version);

    i2cScanner();

    OLED_Init();


    Keypad_Init();
    ledInit();

    delay(2000);
    printWithHeader("Setup Complete", "SYS");
}

void loop() {
    // Heartbeat message
    if (millis() - lastHeartbeat >= heartbeatFrequency) {
        lastHeartbeat = millis();
        printWithHeader("stuff", relayCode);
    }
    Keypad_Update();
}

void set_led_clrs(CRGB clr, CRGB stripe, int led_count) {
    led_count = max(0, min(sizeof(stripe), led_count));
    for(size_t i = 0; i < led_count; i++) {
        stripe[i] = clr;
    };
    FastLED.show();
};

void ledInit() {
    LEDS.addLeds<LED_TYPE_12V,PWM_1_PIN,GRB>(leds[0], NUM_LEDS_12V);
    LEDS.addLeds<LED_TYPE_5V,PWM_2_PIN,GRB>(leds[1], NUM_LEDS_5V);
    LEDS.setBrightness(255);
    set_led_clrs(CRGB::Gold, 0, NUM_LEDS_12V);
    set_led_clrs(CRGB::Gold, 1, NUM_LEDS_5V);
    FastLED.show();
}


void OLED_Init() {
    oled.begin(&SH1106_128x64, OLED_I2C_ADD);
    oled.set400kHz();
    oled.setScroll(true);
    oled.setFont(Arial_bold_14);
    oled.println("\n\n   booting.... \n");
}

void OLED_main_menu() {
    oled.clear();
    oled.setFont(Adafruit5x7);
    oled.print("\n\n");
    oled.setFont(Arial_bold_14);
    oled.println("  select LED: ");
    oled.print("  press 3 for both");
    oledLastUpdate = millis();
}


void Keypad_Init() {
    MyKeypad.addEventListener(keypadEvent);  // Event Listener erstellen
    MyKeypad.begin(makeKeymap(KeypadKeys));
    MyKeypad.setHoldTime(5000);
    MyKeypad.setDebounceTime(20);
}

void Keypad_Update() {
}


void keypadEvent(KeypadEvent eKey) {
    switch (MyKeypad.getState()) {
        case PRESSED:

            switch (eKey) {
                default:
                    return eKey;
                    break;
            }
            break;
    }
}






