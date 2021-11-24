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

/*
- Choice of LED (12v vs. 5v vs. both)
- Intensity 
- Colour
- Interval (blinking)
- "Flackern"
*/
#define STAGECOUNT 7
// TODO: use enum here for each stage so we have a name?
static const char *menutexts[STAGECOUNT] = {
    "Led selection\n 1: 12V, 2: 5V, 3: Both",
    "Brightness from 0 to 255",
    "Red",
    "Green",
    "Blue",
    "Time on duration in 100ms",
    "Time off duration in 100ms"};
static char inputs[STAGECOUNT][4] = {}; 
static char currentInput[4] = "";
static int currentInputIndex = 0;

// first is power on Period second is power off period
static int ledPeriods[2][2] = {{1,0},{1,0}};
// 0 is on 1 is off
static int ledStage[2] = {0, 0};
static int ledCLRS[2] = {CRGB::Black, CRGB::Black};
unsigned long ledTimer[2] = {millis(), millis()};



char KeypadKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};


Keypad_I2C MyKeypad(makeKeymap(KeypadKeys), KeypadRowPins, KeypadColPins,
                    KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_I2C_ADD, PCF8574);


void setup() {
    brainSerialInit();
    Serial.println(title);
    Serial.println(version);

    ledInit();
    i2cScanner();

    OLED_Init();


    Keypad_Init();

    delay(2000);
    printWithHeader("Setup Complete", "SYS");
}

void loop() {
    Keypad_Update();
    ledFlicker();
}

void set_led_clrs(CRGB clr, CRGB stripe) {
    int led_count = sizeof(stripe);
    for(size_t i = 0; i < led_count; i++) {
        stripe[i] = clr;
    };
    FastLED.show();
};

void ledInit() {
    LEDS.addLeds<LED_TYPE_12V,PWM_1_PIN,GRB>(leds[0], NUM_LEDS_12V);
    LEDS.addLeds<LED_TYPE_5V,PWM_2_PIN,GRB>(leds[1], NUM_LEDS_5V);
    LEDS.setBrightness(255);
    set_led_clrs(CRGB::Magenta, 0);
    set_led_clrs(CRGB::Magenta, 1);
    FastLED.show();
}

// TODO: WIP really
void ledFlicker() {
    for(int i = 0; i<2; i++) {
        if (millis() - ledTimer[i] > ledPeriods[i][ledStage[i]]) {
            if (ledStage[i] == 0) {
                ledStage[i]++;
                set_led_clrs(ledCLRS[i], i);
            } else {
                set_led_clrs(CRGB::Black, i);
                ledStage[i]=0;
            }
        }
    }

   
    // ledPeriods
    // ledStage
    // ledTimer 
}


void OLED_Init() {
    oled.begin(&SH1106_128x64, OLED_I2C_ADD);
    oled.set400kHz();
    oled.setScroll(true);
    oled.setFont(Arial_bold_14);
    oled.println("\n\n   booting.... \n");
}


void menuReset() {
    menuStage = 0;
    for(size_t i = 0; i <= STAGECOUNT; i++) {
        strcpy(inputs[i], "");
    }
    // keypadevent already will trigger iirc
    // OLED_main_menu();
}

void OLED_main_menu() {
    if (menuStage > STAGECOUNT) {
        menuStage = 0;
        oled.print("Aplying input");
        delay(500);
        evaluate_inputs();
    };
    oled.clear();
    oled.setFont(Adafruit5x7);
    oled.print("   # =Confirm  * =back \n hold * = Reset\n");
    oled.setFont(Arial_bold_14);
    oled.println(menutexts[menuStage]);
    oled.println(currentInput);
}


// TODO: seperate the clr values input
void evaluate_inputs() {
    LEDS.setBrightness(uint_fast8_t(inputs[1]));
          
    CRGB selectedClr = CRGB(uint_fast8_t(inputs[2]), uint_fast8_t(inputs[3]), uint_fast8_t(inputs[4]));

    int brightness;
    // 5V LED
    if (int(inputs[0]) >= 2) {
        brightness = int(inputs[1]);
        ledCLRS[1] = selectedClr;
        set_led_clrs(selectedClr, 1);
        ledPeriods[1][0] = int(inputs[6]);
        ledPeriods[1][1] = int(inputs[7]);
    }   
    // 12V LED
    if ( int(inputs[0]) != 2) {
        brightness = int(inputs[1]);
        ledCLRS[0] = selectedClr;
        set_led_clrs(selectedClr, 0);
        ledPeriods[0][0] = int(inputs[6]);
        ledPeriods[0][1] = int(inputs[7]);
    }
};


void Keypad_Init() {
    MyKeypad.addEventListener(keypadEvent);  // Event Listener erstellen
    MyKeypad.begin(makeKeymap(KeypadKeys));
    MyKeypad.setHoldTime(2500);
    MyKeypad.setDebounceTime(20);
}


void Keypad_Update() {
    MyKeypad.getKey();
}


void keypadEvent(KeypadEvent eKey) {
    switch (MyKeypad.getState()) {
        case PRESSED:

            switch (eKey) {
                case '#': 
                    menuStage++; 
                    strcpy(inputs[menuStage], currentInput);
                    currentInputIndex = 0;
                    break;
                case '*': 
                    menuStage--; 
                    currentInputIndex = 0;
                    break;
                default:
                    if (currentInputIndex > 3) break;
                    char newStr[4] = "";
                    currentInput[currentInputIndex++] = eKey;
                    currentInput[currentInputIndex] = '\\';
                    break;
            }
            break;
        case HOLD:
            switch (eKey) {
                case '*':
                    currentInputIndex = 0;
                    menuReset();
                    break;
                break;
            }
            break;
    }
    OLED_main_menu();
}






