/**
* To configure a relay:
*   - rename REL_X_PIN to informative name
*   - rename and set REL_X_INIT with your init value
*   - create array with the used pins and benefit from loops :D
*/
#pragma once

String title = "ENT_ST_BREAKOUT";
// String brainName = String("BrCOLOR");

// --- LED settings --- 
#define NR_OF_LEDS             31  /* Anzahl der Pixel auf einem Strang (Test 1 Pixel)                   */
#define STRIPE_CNT             2

#define RFID_1_LED_PIN          9     /* Per Konvention ist dies RFID-Port 1                                */
#define RFID_2_LED_PIN          6     /* Per Konvention ist dies RFID-Port 2                                */
#define RFID_3_LED_PIN          5     /* Per Konvention ist dies RFID-Port 3                                */
#define RFID_4_LED_PIN          3     /* Per Konvention ist dies RFID-Port 4   */

int ledCnts[STRIPE_CNT] = {70};
int ledPins[STRIPE_CNT] = {RFID_1_LED_PIN, RFID_2_LED_PIN};

