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

PCF8574 relay;

Adafruit_NeoPixel LED_Strips[STRIPE_CNT];

#define rfidDisable 1

Adafruit_PN532 RFID_0(RFID_SSPins[0]);
Adafruit_PN532 RFID_READERS[1] = {RFID_0};


const long int red = LED_Strips[0].Color(255,0,0);
const long int green = LED_Strips[0].Color(0,255,0);
const long int blue = LED_Strips[0].Color(0,0,255);

/*==KEYPAD I2C============================================================*/
const byte KEYPAD_ROWS = 4;  // Zeilen
const byte KEYPAD_COLS = 3;  // Spalten
const byte KEYPAD_CODE_LENGTH = 4;
const byte KEYPAD_CODE_LENGTH_MAX = 7;

char KeypadKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

byte KeypadRowPins[KEYPAD_ROWS] = {1, 6, 5, 3};  // Zeilen  - Messleitungen
byte KeypadColPins[KEYPAD_COLS] = {2, 0, 4};     // Spalten - Steuerleitungen (abwechselnd HIGH)

unsigned long updateTimer = millis();
const int keypadResetInterval = 3000;

Keypad_I2C Keypad(makeKeymap(KeypadKeys), KeypadRowPins, KeypadColPins, KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_ADD, PCF8574_MODE);

// Passwort
Password passKeypad = Password(secret_password);


void setup() {
    STB.begin();
    STB.dbgln("v1");
    Serial.println("WDT endabled");
    wdt_enable(WDTO_8S);
    if (STB_LED::ledInit(LED_Strips, ledCnts, ledPins, NEO_BRG)) {Serial.println("LED: OK!");} else {Serial.println("LED: FAILED!");}

    STB.i2cScanner();

    if (keypadInit()) {
        Serial.print(F(" ok\n"));
    }

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

bool checkPassword() {
    if (strlen(passKeypad.guess) < 1) return false;
    bool result = passKeypad.evaluate();
    if (result) {
        STB.printWithHeader("!Correct", relayCode);

        STB.defaultOled.clear();
        STB.defaultOled.setFont(Adafruit5x7);
        STB.defaultOled.print("\n\n\n");
        STB.defaultOled.setFont(Verdana12_bold);
        STB.dbgln("   ACCESS GRANTED!");

        //0.1 sec delay between correct msg and relay switch
        delay(100);
        // Block code
        while (true) {
            wdt_reset();
            delay(500);
        }
    } else {
        // printWithHeader("!Wrong", relayCode);
        STB.dbgln("    ACCESS DENIED!");
        // Wait to show wrong for a second
        delay(1000);
        passwordReset();
    }
    return result;
}


void keypadEvent(KeypadEvent eKey) {
    KeyState state = IDLE;

    state = Keypad.getState();

    switch (state) {
        case PRESSED:
            updateTimer = millis();
            switch (eKey) {
                case '#':
                    checkPassword();
                    break;

                case '*':
                    passwordReset();
                    break;

                default:
                    passKeypad.append(eKey);
                    STB.defaultOled.clear();
                    STB.defaultOled.setFont(Adafruit5x7);
                    STB.defaultOled.print("\n\n\n");
                    STB.defaultOled.setFont(Verdana12_bold);
                    STB.defaultOled.print("         ");
                    STB.dbgln(passKeypad.guess);
                    // printWithHeader(passKeypad.guess, relayCode);
                    break;
            }
            break;

        default:
            break;
    }
}

bool keypadInit() {
    Keypad.addEventListener(keypadEvent);  // Event Listener erstellen
    Keypad.begin(makeKeymap(KeypadKeys));
    Keypad.setHoldTime(5000);
    Keypad.setDebounceTime(20);
    return true;
}

void passwordReset() {
    if (strlen(passKeypad.guess) > 0) {
        passKeypad.reset();
        // printWithHeader("!Reset", relayCode);
        oledHomescreen();
    }
}

void keypadReset() {
    if (millis() - updateTimer >= keypadResetInterval) {
        updateTimer = millis();

        if (strlen(passKeypad.guess) > 0) {
            checkPassword();
            // printWithHeader("!Reset", relayCode);
            Serial.println("!Timeout");
        } else {
            // Act as heartbeat pulse
            // printWithHeader("", relayCode);
        }
        passwordReset();
    }
}

void oledHomescreen() {
    Serial.println("Homescreen");
    STB.defaultOled.clear();
    STB.defaultOled.setFont(Adafruit5x7);
    STB.defaultOled.print("\n\n\n");
    STB.defaultOled.setFont(Verdana12_bold);
    STB.defaultOled.println("  Type your code..");
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

    Keypad.getKey();
    keypadReset();

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
