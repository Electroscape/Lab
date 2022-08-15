#pragma once

// --- LED settings --- 
#define NR_OF_LEDS             1  /* Anzahl der Pixel auf einem Strang (Test 1 Pixel)                   */
#define STRIPE_CNT             3

int ledCnts[STRIPE_CNT] = {9};

char rfidSolutions[4][5] = {"AH\0", "RES\0", "GF\0", "SD\0"};


// RFIDs
#define RFID_AMOUNT 8

// == constants
// RELAY
enum REL_PIN {
    REL_0_PIN,        // 0 Exit door
    REL_1_PIN,  
    REL_2_PIN,  
    REL_3_PIN,        
    REL_4_PIN,        
    REL_5_PIN,        
    REL_6_PIN,       
    REL_7_PIN        
};
enum REL_INIT {                 // DESCRIPTION OF THE RELAY WIRING
    REL_0_INIT = 1,                
    REL_1_INIT = 1,                 
    REL_2_INIT = 1,  
    REL_3_INIT = 1,                
    REL_4_INIT = 1,                
    REL_5_INIT = 1,              
    REL_6_INIT = 1,              // NC = Empty | COM = 24V  | NO = Valve
    REL_7_INIT = 1                
};

#define REL_AMOUNT 8

int relayPinArray[REL_AMOUNT] = {
    REL_0_PIN,
    REL_1_PIN,
    REL_2_PIN,
    REL_3_PIN,
    REL_4_PIN,
    REL_5_PIN,
    REL_6_PIN,
    REL_7_PIN
};
int relayInitArray[REL_AMOUNT] = {
    REL_0_INIT,
    REL_1_INIT,
    REL_2_INIT,
    REL_3_INIT,
    REL_4_INIT,
    REL_5_INIT,
    REL_6_INIT,
    REL_7_INIT
};
