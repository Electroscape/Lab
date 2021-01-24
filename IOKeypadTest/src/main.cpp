#include <avr/wdt.h>
#include <Wire.h>                     /* TWI / I2C                                                          */
#include "PCF8574.h"

#include <Keypad.h>                   /* Standardbibliothek                                                 */
#include <Keypad_I2C.h>               /*                                                                    */
#include <Password.h>

enum PWM_PIN {
    PWM_1_PIN = 3,                           // Predefined by STB design
    PWM_2_PIN = 5,                           // Predefined by STB design
    PWM_3_PIN = 6,                           // Predefined by STB design
    PWM_4_PIN = 9,                           // Predefined by STB design
};

#define RELAY_I2C_ADD     	 0x3F         // Relay Expander
#define OLED_I2C_ADD         0x3C         // Predefined by hardware
#define LCD_I2C_ADD					 0x27 // Predefined by hardware
#define KEYPAD_I2C_ADD       0x38         // Keypad
#define KEYPAD_IO_ADD        0x39         // Fuses

#define ON_BOARD_LED_PIN            13

/*==KEYPAD I2C==============================================================================================*/
const byte KEYPAD_ROWS            = 4;
const byte KEYPAD_COLS            = 3;
const byte KEYPAD_CODE_LENGTH     = 4;
const byte KEYPAD_CODE_LENGTH_MAX = 7;
const byte KeypadRowPins[KEYPAD_ROWS] = {1, 6, 5, 3}; 	// Measure
const byte KeypadColPins[KEYPAD_COLS] = {2, 0, 4};
// since we have pullup vs pulldown and we nneed tu put some current through...
// its necessary to shift the channels around to get them next to each other
// therefore the cables need to be crossed

const byte IOpadRowPins[KEYPAD_ROWS] = {0, 1, 2, 3}; 	// Measure
const byte IOpadColPins[KEYPAD_COLS] = {4, 5, 6};


const char KeypadKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};
const char IOpadKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};


Keypad_I2C MyKeypad( makeKeymap(KeypadKeys), KeypadRowPins, KeypadColPins, KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_I2C_ADD, PCF8574);
Keypad_I2C MyIOKeypad(makeKeymap(IOpadKeys), IOpadRowPins, IOpadColPins, KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_IO_ADD, PCF8574);
Password pass_fusebox = Password( "2517");   //

/*===KEYPAD=================================================================================================*/
void keypadEvent(KeypadEvent eKey) {
    Serial.println("Event Happened");
    Serial.print(eKey);
    Serial.print(MyKeypad.getState());
}

bool Keypad_Init() {
	MyKeypad.addEventListener(keypadEvent);    // Event Listener erstellen
    delay(100);
	MyKeypad.begin( makeKeymap(KeypadKeys) );
	MyKeypad.setHoldTime(5000);
	MyKeypad.setDebounceTime(20);

	return true;
}


/*============================================================================================================
//===BASICS===================================================================================================
//==========================================================================================================*/
void print_logo_infos(String progTitle) {
    Serial.println(F("+-----------------------------------+"));
    Serial.println(F("|    TeamEscape HH&S ENGINEERING    |"));
    Serial.println(F("+-----------------------------------+"));
    Serial.println();
    Serial.println(progTitle);
    Serial.println();
    delay(750);
}

void i2c_scanner() {
    Serial.println (F("I2C scanner:"));
    Serial.println (F("Scanning..."));
    byte wire_device_count = 0;

    for (byte i = 8; i < 120; i++) {
        Wire.beginTransmission (i);

        if (Wire.endTransmission () == 0) {
            Serial.print   (F("Found address: "));
            Serial.print   (i, DEC);
            Serial.print   (F(" (0x"));
            Serial.print   (i, HEX);
            Serial.print (F(")"));
            if (i == 39) Serial.print(F(" -> LCD"));
            if (i == 56) Serial.print(F(" -> LCD-I2C-Board"));
            if (i == 57) Serial.print(F(" -> Input-I2C-board"));
            if (i == 60) Serial.print(F(" -> Display"));
            if (i == 63) Serial.print(F(" -> Relay"));
            Serial.println();
            wire_device_count++;
            delay (1);
        }
    }

    Serial.print   (F("Found "));
    Serial.print   (wire_device_count, DEC);
    Serial.println (F(" device(s)."));

    Serial.println();

    delay(500);
}

void blink_onBoardled(uint8_t delay_ms){
	pinMode(ON_BOARD_LED_PIN, OUTPUT);
	digitalWrite(ON_BOARD_LED_PIN, HIGH);
	delay(delay_ms);
	digitalWrite(ON_BOARD_LED_PIN, LOW);
	delay(delay_ms);
	digitalWrite(ON_BOARD_LED_PIN, HIGH);
	delay(delay_ms);
	digitalWrite(ON_BOARD_LED_PIN, LOW);
}

void software_Reset() {
#if DEBUG_MODE
    Serial.println(F("Restarting in"));
    for (byte i = RESTARTTIME; i>0; i--) {
        //wdt_reset();
        Serial.println(i);
        delay(1000);
    }
#endif
    blink_onBoardled(50);
    asm volatile ("  jmp 0");
}

void setup() {
    Serial.begin(115200);
    Serial.println("!header_begin");
    delay(50);

    if (Keypad_Init()) {Serial.println("Keypad: ok");	}

    i2c_scanner();
}

void loop() {

    char key = MyKeypad.getKey();

    if (key != NO_KEY){
        Serial.println("regular I2c Keypad");
        Serial.println(key);
    }

    key = MyIOKeypad.getKey();

    if (key != NO_KEY){
        Serial.println("IOpad");
        Serial.println(key);
    }
}
