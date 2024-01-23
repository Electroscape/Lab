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
#include <stb_brain.h>
#include <stb_led.h>
#include <stb_rfid.h>

#define MAX_CTRL_PIN A0
#define T1 A1
#define T2 A2
#define T3 A3
#define T4 A4
#define T5 A5
#define T6 D7
#define T7 D8



STB STB;
int rcvd;
unsigned int writedelay = 50;





void setup() {
    
    // STB.begin();
    pinMode(A0, OUTPUT);
    pinMode(T1, OUTPUT);
    pinMode(T2, OUTPUT);
    pinMode(T3, OUTPUT);
    pinMode(T4, OUTPUT);
    pinMode(T5, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);

    // STB.rs485SetSlaveAddr(0);
}


/*======================================
//===LOOP==============================
//====================================*/
void loop() {
    
    digitalWrite(A0, HIGH);
    digitalWrite(T1, HIGH);
    digitalWrite(T2, HIGH);
    digitalWrite(T3, HIGH);
    digitalWrite(T4, HIGH);
    digitalWrite(T5, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    

    /*
    digitalWrite(A0, LOW);
    digitalWrite(T1, LOW);
    digitalWrite(T2, LOW);
    digitalWrite(T3, LOW);
    digitalWrite(T4, LOW);
    digitalWrite(T5, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    */

    delay(5000);
    /*

    digitalWrite(MAX_CTRL_PIN, MAX485_WRITE);
    delay(writedelay);
    Serial.println("bufferOutA1");
    delay(writedelay);
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);
    digitalWrite(auxPin, MAX485_WRITE);
    delay(writedelay);
    Serial.println("bufferOutA2");
    delayMicroseconds(writedelay);
    digitalWrite(auxPin, MAX485_READ);
    delay(writedelay);
    */
    
}
