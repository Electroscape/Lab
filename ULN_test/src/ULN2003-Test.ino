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


#include <stb_common.h>
#include <stb_brain.h>
#include <stb_led.h>
#include <stb_rfid.h>

#define MAX_CTRL_PIN A0
#define T1 A1
#define T2 A2
#define T3 A3
#define T4 2
#define T5 3
#define T6 4
#define T7 5
#define T8 6



STB STB;
int rcvd;
unsigned int writedelay = 50;





void setup() {
    
    Serial.begin(57600);
    pinMode(A0, OUTPUT);
    pinMode(T1, OUTPUT);
    pinMode(T2, OUTPUT);
    pinMode(T3, OUTPUT);
    pinMode(T4, OUTPUT);
    pinMode(T5, OUTPUT);
    pinMode(T6, OUTPUT);
    pinMode(T7, OUTPUT);
    pinMode(T8, OUTPUT);
    Serial.println("V1");


    // STB.rs485SetSlaveAddr(0);
}


/*======================================
//===LOOP==============================
//====================================*/
void loop() {
    
    digitalWrite(A0, HIGH);

    digitalWrite(T1, HIGH);
    delay(200);
    digitalWrite(T2, HIGH);
    delay(200);
    digitalWrite(T3, HIGH);
    delay(200);
    digitalWrite(T4, HIGH);
    delay(200);
    digitalWrite(T6, HIGH);
    delay(200);
    digitalWrite(T7, HIGH);
    delay(200);
    digitalWrite(T8, HIGH);
    delay(2000);

    digitalWrite(T1, LOW);
    delay(200);
    digitalWrite(T2, LOW);
    delay(200);
    digitalWrite(T3, LOW);
    delay(200);
    digitalWrite(T4, LOW);
    delay(200);
    digitalWrite(T6, LOW);
    delay(200);
    digitalWrite(T7, LOW);
    delay(200);
    digitalWrite(T8, LOW);
    delay(2000);
    

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
    Serial.println("bufferOutA1");
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
