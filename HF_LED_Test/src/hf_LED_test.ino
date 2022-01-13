/*=======================================================*/
/**		2CP - TeamEscape - Engineering
 *		by Martin Pek
/*=======================================================*/


#include <Arduino.h>
#include "header_s.h"
#include <stb_namespace.h>
using namespace stb_namespace;

/*==INCLUDE==============================================*/



// Keypad
#include <Keypad.h>
#include <Keypad_I2C.h>
#include <Wire.h>

// I2C Port Expander
#include <PCF8574.h>

#include <FastLED.h>

// OLED
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>


/*==OLED======================================*/
SSD1306AsciiWire oled;
unsigned long lastHeartbeat = millis();

// LEDS
CRGB leds12V[NUM_LEDS];
CRGB leds5V[NUM_LEDS];

struct CRGB * ledarray[] ={leds12V, leds5V};

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
    "Led selection\n 1: 12V, 2: 5V,\n 3: Both\n",
    "Brightness\n from 0 to 255\n",
    "Red\n",
    "Green\n",
    "Blue\n",
    "Time on duration\n in 100ms\n",
    "Time off duration\n in 100ms\n"};
static char inputRaw[8] = "\0";

static int charpos = 0;
static int inputs[STAGECOUNT] = {}; 

// first is power on Period second is power off period
static unsigned long  ledPeriods[2][2] = {{1,0},{1,0}};
// 0 is on 1 is off
static int ledStage[2] = {0, 0};
// static int ledCLRS[2][3] = {{0,0,0}, {0,0,0}};
static CRGB ledCLRS[2] = {CRGB::Pink, CRGB::Pink};
static unsigned long ledTimer[2] = {millis(), millis()};

// fill_solid();




Keypad_I2C MyKeypad( makeKeymap(KeypadKeys), KeypadRowPins, KeypadColPins, KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_I2C_ADD, PCF8574);

void setup() {
    brainSerialInit();
    Serial.println(title);
    Serial.println(version);

    i2cScanner();

    Serial.print("Oled ...");
    OLED_Init();
    Serial.println("done");

    
    ledInit();


    Serial.print("Keypad ...");
    Keypad_Init();
    Serial.print("done");

    printWithHeader("Setup Complete", "SYS");
    menuReset();
    OLED_main_menu();
}

void loop() {
    Keypad_Update();
    ledFlicker();
}

void set_led_clrs(CRGB clr, int stripe_no) {
    // Serial.print("setLedClrs");
    fill_solid(ledarray[stripe_no], NUM_LEDS, clr);
    FastLED.show();
};

void ledInit() {
    FastLED.addLeds<LED_TYPE_12V,PWM_4_PIN,BRG>(leds12V, NUM_LEDS);
    FastLED.addLeds<LED_TYPE_5V,PWM_3_PIN,GRB>(leds5V, NUM_LEDS);
    FastLED.setBrightness(255);

    CRGB someclr = CRGB(255,255,255);
    Serial.println(someclr.r);
    Serial.println(someclr.b);
    Serial.print(someclr.g);


    fill_solid(ledarray[0], NUM_LEDS, someclr);
    fill_solid(leds5V, NUM_LEDS, CRGB(255,255,255));
    FastLED.show();

    delay(500);
    fill_solid(leds12V, NUM_LEDS, CRGB::Red);
    // fill_solid(leds5V, NUM_LEDS, CRGB::Red);
    FastLED.show();
    delay(500);

    set_led_clrs(CRGB::Green, 0);
    FastLED.show();
    delay(599);
    // set_led_clrs(CRGB::Green, 1);

    
}

void ledFlicker() {
    
    for(int i = 0; i<2; i++) {
        if (ledPeriods[i][1] == 0 || ledPeriods[i][0] == 0) continue;
        // Serial.print("flicker check for"); Serial.println(i);
        if (millis() - ledTimer[i] > 100 * ledPeriods[i][ledStage[i]]) {
            ledTimer[i] = millis();
            
            if (ledStage[i] > 0) {
                ledStage[i] = 0;
                set_led_clrs(ledCLRS[i], i);
            } else {
                set_led_clrs(CRGB::Black, i);
                ledStage[i]++;
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
    Serial.println("menureset");
    menuStage = 0;
    for(size_t i = 0; i < STAGECOUNT; i++) {
        inputs[i] = 0;
    }
    // keypadevent already will trigger iirc
    OLED_main_menu();
}

void OLED_main_menu() {
    delay(500);
    if (menuStage >= STAGECOUNT) {
        menuStage = 0;
        oled.clear();
        oled.print("\n\n    Applying input");
        applyInputs();
        delay(2000);
        menuReset();
    };
    oled.clear();
    oled.setFont(Adafruit5x7);
    oled.println("# =Confirm  * =back\n hold * =Reset");
    // oled.setFont(Arial_bold_14);
    oled.println(menutexts[menuStage]);
    oled.print("   ");
}


// TODO: seperate the clr values input
void applyInputs() {
    
    // fastles brighness is universal, applies to both led strips sadly 
    Serial.println("inputs are");
    for (int i = 0; i < STAGECOUNT; i++) {
        Serial.print(menutexts[i]); Serial.println(inputs[i]);
    }
    Serial.println();

    float brightness = (float) inputs[1] / int(255);
    CRGB selectedClr = CRGB(uint(inputs[2]*brightness), uint(inputs[3]*brightness), uint(inputs[4]*brightness));
    Serial.println(uint_fast8_t(inputs[2]*brightness));
    Serial.println('%f', brightness);
    Serial.println("final clr is in rgb");
    Serial.println(selectedClr.r);
    Serial.println(selectedClr.g);
    Serial.println(selectedClr.b);

    // 5V LED
    if (inputs[0] >= 2) {
        Serial.println("setting 5V Leds");
        // fill_solid(leds5V, NUM_LEDS/2, selectedClr);
        // FastLED.show();
        set_led_clrs(selectedClr, 1);
        ledCLRS[1] = selectedClr;
        ledPeriods[1][0] = int(inputs[5]);
        ledPeriods[1][1] = int(inputs[6]);
        Serial.print("power on period ");
        Serial.println(ledPeriods[1][0]*100);
        Serial.print("power off period ");
        Serial.println(ledPeriods[1][1]);
    }   
    // 12V LED
    if ( inputs[0] != 2) {
        Serial.println("setting 12V Leds");
  
        // fill_solid(leds12V, NUM_LEDS, selectedClr);
        // FastLED.show();
        set_led_clrs(selectedClr, 0);
        ledCLRS[0] = selectedClr;

        ledPeriods[0][0] = int(inputs[5]);
        ledPeriods[0][1] = int(inputs[6]);

        Serial.print("power on period ");
        Serial.println(ledPeriods[0][0]*100);
        Serial.print("power off period ");
        Serial.println(ledPeriods[0][1]);
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
    Serial.print("keypad event");
    char key = MyKeypad.getKey();
    Serial.println(key);
    switch (MyKeypad.getState()) {
        case PRESSED:
            switch (eKey) {
                case '#': 
                    inputs[menuStage] = String(inputRaw).toInt();
                    Serial.print("inputs is: "); Serial.println(inputs[menuStage]);
                    inputRaw[0] = '\0';
                    menuStage++; 
                    charpos = 0;
                    OLED_main_menu();
                    break;
                case '*': 
                    inputRaw[0] = '\0';
                    charpos = 0;
                    OLED_main_menu();
                    break;
                default:
                    Serial.println("default action");
                    inputRaw[charpos] = eKey;
                    oled.print(inputRaw[charpos]);
                    charpos++;
                    inputRaw[charpos]='\0';
                    
                    if (charpos > 2) {
                        inputs[menuStage] = String(inputRaw).toInt();
                        charpos = 0;
                        inputRaw[charpos]='\0';
                        menuStage++;
                        OLED_main_menu();
                    }
                    break;
            }
            break;
        case HOLD:
            switch (eKey) {
                case '*':
                    menuReset();
                    break;
                break;
                default: break;
            }
            break;
    }
    // delay(20);
}






