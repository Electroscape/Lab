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

#include <Adafruit_PN532.h>
#include <stb_oled.h>

Adafruit_PN532 reader(RFID_SSPins[0]);
SSD1306AsciiWire defaultOled;

void setup() {


    Serial.begin(115200);

    STB_OLED::oledInit(&defaultOled , SH1106_128x64);
    defaultOled.setFont(Adafruit5x7);

    reader.begin();
    reader.setPassiveActivationRetries(5);

    int retries = 0;
    while (true) {
        uint32_t versiondata = reader.getFirmwareVersion();
        if (!versiondata) {
            Serial.print(F("Didn't find PN53x board\n"));
            if (retries > 5) {
                Serial.print(F("PN532 startup timed out"));
                delay(5);
            }
        } else {
            Serial.print(F("Found chip PN5"));
            Serial.println((versiondata >> 24) & 0xFF, HEX);
            Serial.print(F("Firmware ver. "));
            Serial.print((versiondata >> 16) & 0xFF, DEC);
            Serial.print('.');
            Serial.println((versiondata >> 8) & 0xFF, DEC);
            break;
        }
        retries++;
    }
    reader.SAMConfig();
    delay(50);
    Serial.print(F("\nRFID init ... Successful \n"));
}


bool cardRead() {
    uint8_t data[16];
    uint8_t success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;
    uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    memset(data, 0, 16);
    uint16_t timeout = 150;
    uint32_t blockNumber = 1;
    uint8_t datablock = 1;

    success = reader.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, timeout);
    if (!success) {
        defaultOled.println("No card");
        return false;
    }

    success = reader.mifareclassic_AuthenticateBlock(uid, uidLength, blockNumber, 0, keya);
    if (!success) {
        defaultOled.println("Auth failed"); return false;
    }

    success = reader.mifareclassic_ReadDataBlock(datablock, data);
    if (!success) {
        defaultOled.println("read failed"); return false;
    }
    if (strlen((char *) data) == 0) {
        defaultOled.println("read empty datablock -> invalid");
        return false;
    } 
    defaultOled.println((char *) data);
    return true;
}


/*======================================
//===LOOP==============================
//====================================*/
void loop() {
  cardRead();
}