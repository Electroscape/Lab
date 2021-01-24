/*==========================================================================================================*/
/*		2CP - TeamEscape - Engineering
 *		by Robert Schloezer
 *
 *		v1.0
 *		- Verschiedene LED Funktionen
 *		- Steuerung über Buttons am ARDUINO
 *    - Steuerung über serielle Schnittstelle
 */
/*==========================================================================================================*/

const String title = String("TEST-LedFunctions v1.0");


/*==INCLUDE=================================================================================================*/
// FastLED
#include "FastLED.h"

/*==DEFINE==================================================================================================*/
// LED
#define CHIPSET       WS2812B       // LED Chip
#define COLOR_ORDER   GRB           // Farbreihenfolge - STD: GRB

#define DATA_PIN        3           // PWM Pin
#define NUM_LEDS       64           // Anzahl der LED
#define BRIGHTNESS    255           // Maximale Helligkeit

// BUTTON
#define BUTTON_ANZAHL  4

#define BUTTON_0_PIN   2
#define BUTTON_1_PIN  10
#define BUTTON_2_PIN  11
#define BUTTON_3_PIN  12

// FEUER
#define FRAMES_PER_SECOND 50
#define COOLING 100
#define SPARKING 100

/*==BUTTONS=================================================================================================*/
const byte buttonList[BUTTON_ANZAHL] = {BUTTON_0_PIN, BUTTON_1_PIN, BUTTON_2_PIN, BUTTON_3_PIN};

/*==LED=====================================================================================================*/
// Flags
bool light_on  = false;             // Ist das Licht schon an
bool light_off = true;              // Ist das Licht ausgeschaltet

// HSV Werte - global
const byte satur_min  =   0;        // Minimale Saettigung bei Farbwechsel
const byte bright_min = 150;        // Minimale Helligkeit bei Farbwechsel
      byte color      =   0;        // Farbe
      byte satur      =   0;        // Saettigung
      byte bright     =   0;        // Helligkeit

// Farbeinstellungen
byte warmweiss[3]   = { 20, 130, 255};
byte rot[3]         = {  0, 255, 255};
byte gelb[3]        = { 64, 255, 255};
byte blau[3]        = {160, 255, 255};
byte gruen[3]       = { 96, 255, 255};
byte lila[3]        = {192, 255, 255};

// Led Typ
CRGB leds[NUM_LEDS];

// Feuer
CRGBPalette16 gPal;
bool gReverseDirection = false;


/*============================================================================================================
//===SETUP====================================================================================================
//==========================================================================================================*/
void setup() {
  // Serielle Schnittstelle
  Serial.begin(115200);
  if( serialInit(title) ) {Serial.println("ok");}

  // Buttons initialisieren
  Serial.print("Buttons: ");
  if( buttonInit(BUTTON_ANZAHL) ) {Serial.println("ok");}

  // LED initialisieren
  Serial.print("LED: ");
  if( ledInit() ) {Serial.println("ok");}
  //gPal = HeatColors_p;
  // These are other ways to set up the color palette for the 'fire'.
  // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::OrangeRed, CRGB::Orange);

  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
  // gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);

  // Third, here's a simpler, three-step gradient, from black to red to white
//   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);

  // Setup Ende
  Serial.println("Setup abgeschlossen");
  Serial.println();
  delay(2000);
}

/*============================================================================================================
//===LOOP=====================================================================================================
//==========================================================================================================*/
void loop() {

  serialLicht();

  buttonLicht();

  //Feuer();

}

/*============================================================================================================
//===LOOP-FUNCTIONS===========================================================================================
//==========================================================================================================*/
void serialLicht() {
  if( Serial.available() ) {
    byte nr = Serial.read();
    Serial.print("Licht ");
    if (light_on == 0) {Serial.println("ist aktuell aus.");}
    else {Serial.println("ist aktuell an.");}
    Serial.print(F("Code ")); Serial.print(nr, DEC); Serial.print(F(": Licht "));

    if( nr == 49 ) {
      Serial.print(" aus - ");
      if( licht_aus() ) {Serial.println("ausgefuehrt"); Serial.println("");}
    }
    else if( nr == 50 ) {
      Serial.println("warmweiss - ");
      if( lichtsteuerung( warmweiss[0], warmweiss[1], warmweiss[2]) ) {Serial.println("ausgefuehrt"); Serial.println("");}
    }
    else if( nr == 51 ) {
      Serial.println("rot - ");
      if( lichtsteuerung( rot[0], rot[1], rot[2]) ) {Serial.println("ausgefuehrt"); Serial.println("");}
    }
    else if( nr == 52 ) {
      Serial.println("gelb - ");
      if( lichtsteuerung( gelb[0], gelb[1], gelb[2]) ) {Serial.println("ausgefuehrt"); Serial.println("");}
    }
    else if( nr == 53 ) {
      Serial.println("blau - ");
      if( lichtsteuerung( blau[0], blau[1], blau[2]) ) {Serial.println("ausgefuehrt"); Serial.println("");}
    }
    else if( nr == 54 ) {
      Serial.println("gruen - ");
      if( lichtsteuerung( gruen[0], gruen[1], gruen[2]) ) {Serial.println("ausgefuehrt"); Serial.println("");}
    }
    else if( nr == 55 ) {
      Serial.println("lila - ");
      if( lichtsteuerung( lila[0], lila[1], lila[2]) ) {Serial.println("ausgefuehrt"); Serial.println("");}
    }
    else if( nr == 56 ) {
      Serial.println("zufaellig - ");
      if( lichtsteuerung( random(0, 255), 255, 255) ) {Serial.println("ausgefuehrt"); Serial.println("");}
    }
    else if( nr == 57 ) {
      Serial.println("mit Colorshift - ");
      if( colorshift() ) {Serial.println("ausgefuehrt"); Serial.println("");}
    }
    else if( nr == 97 ) {
      Serial.println("Feuer - ");
      while (!Serial.available()){
      Feuer();
    }
    return;
    }
  }
}

void buttonLicht() {
  if     ( digitalRead(BUTTON_0_PIN) ) { licht_aus();                                 }
  else if( digitalRead(BUTTON_1_PIN) ) { lichtsteuerung( warmweiss[0], warmweiss[1], warmweiss[2]); }
  else if( digitalRead(BUTTON_2_PIN) ) { lichtsteuerung(       rot[0],       rot[1],       rot[2]); }
  else if( digitalRead(BUTTON_3_PIN) ) { lichtsteuerung(      gelb[0],      gelb[1],      gelb[2]); }
}

/*============================================================================================================
//===FUNCTIONS================================================================================================
//==========================================================================================================*/
bool lichtsteuerung( byte color_par, byte satur_par, byte bright_par ) {
  byte color_old = color;
  byte satur_old = satur;
  byte bright_old = bright;

  color = color_par;
  satur = satur_par;
  bright = bright_par;

  if( !light_on ) {
    for(int i=0; i<=bright; i+=5) {
      FastLED.showColor( CHSV(color, satur, i) );     // Warmes Weiß
      delay(10);
    }
    light_on = true;
  }
  else if( light_on ) {
    // Helligkeit reduzieren
    for(int a=bright_old; a>=bright_min; a-=5) {
      FastLED.showColor( CHSV(color_old, satur_old, a) );     // Warmes Weiß
      delay(10);
    }
    // Helligkeit erhoehen
    for(int d=30; d<=bright/2; d+=10) {
      FastLED.showColor( CHSV(color_old, satur_old, d) );     // Warmes Weiß
      delay(10);
    }
    // Farbe wechseln
    for(int e=color_old; e<=255; e+=5) {
        FastLED.showColor( CHSV(e, satur, bright/2) );     // Warmes Weiß
        delay(10);
      }
      for(int e=255; e>=0; e-=5) {
        FastLED.showColor( CHSV(e, satur, bright/2) );     // Warmes Weiß
        delay(10);
      }
      for(int e=0; e<=color; e+=1) {
        FastLED.showColor( CHSV(e, satur, bright/2) );     // Warmes Weiß
        delay(20);
      }
    // Helligkeit erhoehen
    for(int d=bright/2; d<=bright; d+=1) {
      FastLED.showColor( CHSV(color, satur, d) );     // Warmes Weiß
      delay(10);
    }
    Serial.println("Lichtfarbe wurde gewechselt");
  }
  else {Serial.println("Licht ist schon eingeschaltet");
  }
  return true;
}

bool colorshift() {
  byte color_old = color;
  byte satur_old = satur;
  byte bright_old = bright;

  color = color + 16;

  if( !light_on ) {
    for(int i=0; i<=bright; i+=5) {
      FastLED.showColor( CHSV(color, satur, i) );     // Warmes Weiß
      delay(10);
    }
    light_on = true;
  }
  else if( light_on ) {
    // Farbe wechseln
    if (color < color_old) {
      for(byte e=color_old; e<255; e+=1) {
          FastLED.showColor( CHSV(e, satur, bright) );     // Warmes Weiß
          delay(40);
        }
      FastLED.showColor( CHSV(0, satur, bright) );     // Warmes Weiß
      color_old = 0;
    }
    else {
      for(byte e=color_old; e<=color; e+=1) {
          FastLED.showColor( CHSV(e, satur, bright) );     // Warmes Weiß
          delay(40);
      }
    }
    Serial.println("Lichtfarbe wurde gewechselt");
  }
  else {Serial.println("Licht ist schon eingeschaltet");
  }
  return true;
}

bool licht_aus() {
  if( light_on ) {
    for(int i=255; i>-1; i-=5) {
      FastLED.showColor( CHSV(color, satur, i) );     // aus
      delay(10);
    }
  }
  light_on = false;
  return true;
}

void Feuer() {
  //for(int i=0; i<20000; i++) {
  random16_add_entropy( random());

  // Fourth, the most sophisticated: this one sets up a new palette every
  // time through the loop, based on a hue that changes every time.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  //
  //   static uint8_t hue = 0;
  //   hue++;
  //   CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
  //   CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
  //   gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);


  Fire2012WithPalette(); // run simulation frame, using palette colors

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
//}
}

void Fire2012WithPalette()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

/*============================================================================================================
//===INIT=====================================================================================================
//==========================================================================================================*/
bool serialInit(String progTitle) {
  delay(100);
  Serial.println(F("+-----------------------------------+"));
  Serial.println(F("|    TeamEscape HH&S ENGINEERING    |"));
  Serial.println(F("+-----------------------------------+"));
  Serial.println();
  Serial.println(progTitle);
  Serial.println();
  Serial.print("Serial: ");
  return true;
}

bool buttonInit(byte buttonCount) {
  delay(100);
  for( int i=0; i<buttonCount; i++ ) {
    pinMode(buttonList[i], INPUT);
  }
  return true;
}

bool ledInit() {
  delay(100);
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.setBrightness(BRIGHTNESS);

  // Aufblenden
  for(int i=0; i<=255; i++) {
    FastLED.showColor( CHSV(0, 255, i) );
  }
  // Farbe wechseln
  for(int i=0; i<=255; i++) {
    FastLED.showColor( CHSV(i, 255, 255) );
    delay(5);
  }
  // Abblenden
  for(int i=255; i>=0; i--) {
    FastLED.showColor( CHSV(0, 255, i) );
  }
  return true;
}
