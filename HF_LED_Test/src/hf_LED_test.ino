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

// LEDS
CRGB leds0[NUM_LEDS];
CRGB leds1[NUM_LEDS];
CRGB leds2[15];
CRGB leds3[15];

struct CRGB * ledarray[] ={leds0, leds1, leds2, leds3};

static uint menuStage = 0;

/*
- Choice of LED (12v vs. 5v vs. both)
- Intensity 
- Colour
- Interval (blinking)
- "Flackern"
*/
#define STAGECOUNT 5
// TODO: use enum here for each stage so we have a name?
static const char *menutexts[STAGECOUNT] = {
    "Led selection\n 1-4\n 5: all\n",
    "Brightness\n from 0 to 255\n",
    "Red\n",
    "Green\n",
    "Blue\n",
};

static char inputRaw[8] = "\0";

static int charpos = 0;
static int inputs[STAGECOUNT] = {}; 




Keypad_I2C MyKeypad( makeKeymap(KeypadKeys), KeypadRowPins, KeypadColPins, KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_I2C_ADD, PCF8574);

void setup() {

    Serial.begin(115200);

    Serial.println("WTF");
    delay(2000);
    Wire.begin();
    Wire.setClock(100000);
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
}

void set_led_clrs(CRGB clr, int stripe_no) {
    // Serial.print("setLedClrs");
    fill_solid(ledarray[stripe_no], NUM_LEDS, clr);
    FastLED.show();
    delay(20);
};

void ledInit() {

    FastLED.addLeds<LED_TYPE_12V,PWM_4_PIN,BRG>(leds0, NUM_LEDS);
    FastLED.addLeds<LED_TYPE_12V,PWM_3_PIN,BRG>(leds1, NUM_LEDS);
    FastLED.addLeds<LED_TYPE_12V,PWM_2_PIN,BRG>(leds2, NUM_LEDS);
    FastLED.addLeds<LED_TYPE_12V,PWM_1_PIN,BRG>(leds3, NUM_LEDS);
    FastLED.setBrightness(255);

    CRGB someclr = CRGB(255,255,255);

    for (int i=0; i<4; i++) {
        set_led_clrs(CRGB(0,0,0), i);
    }
    
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

    if (inputs[0] == 0) {
        return;
    }
    if (inputs[0] < 4) {
        set_led_clrs(selectedClr, inputs[0]);
    } else {
        for (int i=0; i<4; i++) {
            set_led_clrs(CRGB(0,0,0), i);
        }
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






