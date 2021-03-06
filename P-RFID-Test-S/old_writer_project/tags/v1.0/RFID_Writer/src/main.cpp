/*==========================================================================================================*/
/*		2CP - TeamEscape - Engineering
 *		by Martin Pek & Stephan Christoph & Robert Schloezer
 *
 *		v1.1 beta rebuild stuttgart
 *		- Bis zu 3 MFRC522 Reader, inkl. LED Strip/Reader fuer Feedback
 *		- OLED Display ueber I2C
 *		- Schaltet Deckenlicht und Schwarzlicht ein und aus
 */
/*==========================================================================================================*/
// #include <string>
// const String title = String("RFID Multi-RFID v1.4 beta");


/*==INCLUDE=================================================================================================*/

// RFID
#include <SPI.h>                      /* Standardbibliothek                                                 */
#include <Wire.h>                     /* TWI / I2C                                                          */
#include <MFRC522.h>                  /* https://github.com/miguelbalboa/rfid                               */

// NeoPixel
// #include <Adafruit_NeoPixel.h>        /* Ueber Bibliotheksverwalter                                         */

// OLED
#include "SSD1306Ascii.h"             /* https://github.com/greiman/SSD1306Ascii                            */
#include "SSD1306AsciiWire.h"         /* https://github.com/greiman/SSD1306Ascii                            */

// I2C Port Expander
#include "PCF8574.h"                  /* https://github.com/skywodd/pcf8574_arduino_library - modifiziert!  */
                                      /* Der Klassenname wurde geaendert, da es sonst Namenskonflikte gibt! */

/*==WIRING ESSENTIALS===============================================================================*/

#define PWM_1_PIN 9                       // Predefined by STB design
#define PWM_2_PIN 6                       // Predefined by STB design
#define PWM_3_PIN 5                       // Predefined by STB design
#define PWM_4_PIN 3                       // Predefined by STB design

#define RFID_NR_OF_READERS      1
#define RFID_RST_PIN            10     /* Per Konvention auf 10 festgelegt                                   */

#define RFID_1_SS_PIN           8     /* Per Konvention ist dies RFID-Port 1                                */
#define RFID_2_SS_PIN           7     /* Per Konvention ist dies RFID-Port 2                                */
#define RFID_3_SS_PIN           4     /* Per Konvention ist dies RFID-Port 3                                */
#define RFID_4_SS_PIN           2     /* Per Konvention ist dies RFID-Port 4                                */

#define RFID_1_LED_PIN          9     /* Per Konvention ist dies RFID-Port 1                                */
#define RFID_2_LED_PIN          6     /* Per Konvention ist dies RFID-Port 2                                */
#define RFID_3_LED_PIN          5     /* Per Konvention ist dies RFID-Port 3                                */
#define RFID_4_LED_PIN          3     /* Per Konvention ist dies RFID-Port 4                                */

byte    RFID_SS_PINS[] = { RFID_1_SS_PIN };     // RFID_3_SS_PIN, RFID_4_SS_PIN
byte    RFID_LEDPins[] = { RFID_1_LED_PIN };

// I2C ADRESSES
#define RELAY_I2C_ADD     	 0x3F         // Relay Expander
#define OLED_I2C_ADD         0x3C         // Predefined by hardware
// #define LCD_I2C_ADD					 0x27 // Predefined by hardware
// #define KEYPAD_I2C_ADD       0x38         // Keypad

Expander_PCF8574 relay;

// === RFIDs ===

bool RFID_presence[RFID_NR_OF_READERS] = {false};

MFRC522 RFID_Reader[RFID_NR_OF_READERS];
MFRC522::MIFARE_Key RFID_Key;
// returncode or read func
MFRC522::StatusCode RFID_Read_RC;

SSD1306AsciiWire oled;

// == constants

String title = "Standarcode Example";
String versionDate = "42.2.4242";
String version = "version 0.0";

const int sectorsCount = 16;
const int blocksPerSector = 4;
const int totalBlocks = sectorsCount * blocksPerSector;

void string2ByteArray(char* input, byte* output) {
    int loop;
    int i;

    loop = 0;
    i = 0;

    while(input[loop] != '\0')
    {
        output[i++] = input[loop++];
    }
};

/*============================================================================================================
//===Functions====================================================================================================
//==========================================================================================================*/

/*============================================================================================================
//===INIT=====================================================================================================
//==========================================================================================================*/

void dbg_println(String print_dbg) {
    Serial.println(print_dbg);
    oled.println(print_dbg);
}

void print_logo_infos(String progTitle) {
    Serial.println(F("+-----------------------------------+"));
    Serial.println(F("|    TeamEscape HH&S ENGINEERING    |"));
    Serial.println(F("+-----------------------------------+"));
    Serial.println();
    Serial.println(progTitle);
    Serial.println();
    delay(200);
}

void print_serial_header() {
    Serial.begin(115200);
    print_logo_infos(title);

    dbg_println("!header_begin");
    dbg_println(title);
    dbg_println(versionDate);
    dbg_println(version);
    dbg_println("!header_end");
    dbg_println("!setup_begin");
}

void OLED_Init() {
    Wire.begin();
    // oled.begin(&Adafruit128x64, OLED_I2C_ADD);
    oled.set400kHz();
    oled.setScroll(true);
    oled.setFont(System5x7);
}

void dbg_init() {
    Serial.begin(115200);
    Serial.println("dbg_init");
    OLED_Init();
    print_serial_header();
}

// do not blink faster than 100ms otherwise may be hard to see
void blink_RFIDled(int blink_speed) {
    for (int repeats = 0; 3 > repeats; repeats++) {
        for (int reader_nr = 0; reader_nr<RFID_NR_OF_READERS; reader_nr++) {
            digitalWrite(RFID_LEDPins[reader_nr], HIGH);
        }
        delay(blink_speed);
        for (int reader_nr = 0; reader_nr<RFID_NR_OF_READERS; reader_nr++) {
            digitalWrite(RFID_LEDPins[reader_nr], LOW);
        }
    }
}

bool RFID_Init() {

    Serial.println("initializing RFID readers");

    // SS Pins auf HIGH ziehen, um SPI und alle Reader initialisieren zu k�nnen
    for (uint8_t reader_nr = 0; reader_nr < RFID_NR_OF_READERS; reader_nr++) {
        pinMode(RFID_SS_PINS[reader_nr], OUTPUT);
        digitalWrite(RFID_SS_PINS[reader_nr], HIGH);
        pinMode(RFID_LEDPins[reader_nr], OUTPUT);
        digitalWrite(RFID_LEDPins[reader_nr], HIGH);
        RFID_Reader[reader_nr].PICC_HaltA();
        RFID_Reader[reader_nr].PCD_StopCrypto1();
        RFID_Reader[reader_nr].PCD_Init();
    }

    Serial.println("b0");
    SPI.begin();
    Serial.println("b1");

    for (uint8_t reader_nr = 0; reader_nr < RFID_NR_OF_READERS; reader_nr++) {
        RFID_Reader[reader_nr].PCD_Init(RFID_SS_PINS[reader_nr], RFID_RST_PIN);
        Serial.print(F("Reader["));
        Serial.print(reader_nr + 1);
        Serial.println(F("] init."));
    }

    Serial.println("b2");

    for (uint8_t reader_nr = 0; reader_nr < RFID_NR_OF_READERS; reader_nr++) {
        Serial.print(F("Reader[")); Serial.print(reader_nr + 1); Serial.print(F("] "));
        RFID_Reader[reader_nr].PCD_DumpVersionToSerial();
        //Here we need a function which controls our Dump, because of errors while initialisation!!!!!!
        delay(500);
    }
    Serial.println();

    //Prepare RFID_Key - All Keys are set to FFFFFFFFFFFFh at chip delivery from the factory!
    //We will not change it!!! We use this Key!!!
    for (byte i = 0; i < 6; i++) RFID_Key.keyByte[i] = 0xFF;

    blink_RFIDled(200);

    return true;
}



/*============================================================================================================
//===SETUP====================================================================================================
//==========================================================================================================*/

void setup() {
    dbg_init();

    /*
    char testString[] = "Hello world!";
    byte testBytes[strlen(testString)];
    string2ByteArray(testString, testBytes);

    int i;
    for(i=0; i<strlen(testString); i++)
    {
        Serial.print(testString[i]);
        Serial.print(" - ");
        Serial.println(testBytes[i]);
    }
    RFID_writeToBlock(byte blockStart, byte content[], int reader_nr)
    //bool RFID_writeToBlock(4, testBytes, 0);
    RFID_Reader[0].MIFARE_Read(4, testBytes, strlen(testString));

    for(i=0; i<strlen(testString); i++)
    {
        Serial.print(testString[i]);
        Serial.print(" - ");
        Serial.println(testBytes[i]);
    }
    */

    if (RFID_Init())	{Serial.println("Relay:   ok");	}
    delay(50);
    // i2c_scanner();
    Serial.println("!setup_end");

    Serial.println("===================date 03.09.2019====================="); Serial.println();
    Serial.println("===================START====================="); Serial.println();

}


/*============================================================================================================
//===RFID=====================================================================================================
//==========================================================================================================*/

static byte buffer[32];

bool RFID_readBlock(MFRC522 Reader, byte blockStart, byte readlen) {


    //also make or go for a check with end of adressspace?
    if (readlen > sizeof(buffer)) {
        dbg_println("Invalid buffersize, cannot read");
        // Todo check what 0 actually is
        return false;
    }

    RFID_Read_RC = Reader.MIFARE_Read(blockStart, buffer, readlen);
    if (!(RFID_Read_RC == MFRC522::STATUS_OK || RFID_Read_RC == 255)) {
        Serial.print("read returncode no success ");
        Serial.print(RFID_Read_RC); Serial.print(" ");
        Serial.println(Reader.GetStatusCodeName(RFID_Read_RC));
    } else {
        for (int byteNo = 0; byteNo < readlen; byteNo++) {
            dbg_println(buffer[byteNo]);
        }
    }
    return true;
}

// 16 blocks 	4 sectors each
bool RFID_writeToBlock(MFRC522 Reader, byte blockStart, byte content[]) {
    if ((uint32_t) blockStart + sizeof(content) > totalBlocks) {
        dbg_println("content reaching out of index, cannot write");
        blink_RFIDled(100);
        return false;
    }

    MFRC522::StatusCode status = Reader.MIFARE_Write(blockStart, content, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(Reader.GetStatusCodeName(status));
    }

    blink_RFIDled(200);
    return true;
}

/*
void RFID_checkup(MFRC522 Reader) {

    for (int reader_nr = 0; reader_nr<RFID_NR_OF_READERS; reader_nr++) {

        bool isNewCard = RFID_Reader[reader_nr].PICC_IsNewCardPresent();
        Serial.println (RFID_Reader[reader_nr].PICC_ReadCardSerial());


        RFID_Read_RC = RFID_Reader[reader_nr].MIFARE_Read(4, buffer_read, strlen("hello world!"));
        if (RFID_Read_RC == MFRC522::STATUS_OK || RFID_Read_RC == 255) {
            dbg_println("Card UID:");
            Serial.println("read success");
            String str = "";
            for (int i = 0; i < 16; i++) {
                if (buffer_read[i] == 0xFF) {
                    break;
                }
                Serial.println(buffer_read[i]);
                str += char(buffer_read[i]);
            }

            Serial.println(str);
        } else {
            Serial.print("read returncode no success ");
            Serial.print(RFID_Read_RC); Serial.print(" ");
            Serial.println(RFID_Reader[reader_nr].GetStatusCodeName(RFID_Read_RC));
        }
        //  Remember to call this function after communicating with an authenticated PICC - otherwise no new communications can start.
        RFID_Reader[reader_nr].PCD_StopCrypto1();
    }
}
*/

/*
void RFID_Read_Handler() {

    bool RFIDsuccess = false;
    byte readBufferAAA[128];

    for (int reader_nr = 0; reader_nr<RFID_NR_OF_READERS; reader_nr++) {
        if (RFID_Reader[reader_nr].PICC_IsNewCardPresent() && !RFID_presence[reader_nr]) {
            RFID_presence[reader_nr] = true;
            RFIDsuccess = RFID_readBlock(RFID_Reader[0], 4);
        } else {
            RFID_presence[reader_nr] = false;
        }
    }


    RFIDsuccess = RFID_readBlock(RFID_Reader[0], 4, strlen("Hello World!"), readBuffer);
    if (RFIDsuccess) {
        dump_byte_array(readBuffer, sizeof(readBuffer));
    }
    RFID_Status = RFID_Reader[reader_nr].PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &RFID_Key, &(RFID_Reader[reader_nr].uid));

    if (RFID_Status == MFRC522::STATUS_OK) {
        Serial.println ("RFID okay.. ");
    }
    if (RFID_Status != MFRC522::STATUS_OK) {
        Serial.println ("Problem with RFID reader status restarting... ");
        software_Reset();
    }





    //RFID_checkup();
}
*/

/*============================================================================================================
//===LOOP=====================================================================================================
//==========================================================================================================*/

void loop() {

    Serial.println("looping");
    //RFID_Read_Handler();

    bool re = RFID_readBlock(RFID_Reader[0], 4, strlen("Hello World!"));

    delay(500);
    for (int reader_nr = 0; reader_nr<RFID_NR_OF_READERS; reader_nr++) {
        digitalWrite(RFID_LEDPins[reader_nr], LOW);
    }

  // RFID_Loop();

}

void software_Reset()
{
  Serial.println(F("writing in"));
  delay(250);
  for (byte i = 3; i>0; i--) {
    Serial.println(i);
    delay(1000);
  }
  asm volatile ("  jmp 0");
}
