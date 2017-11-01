// Makerfair 2017 demo reel

#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define PIN_PATTERN   6
#define PIN_TOUCH     0
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    32
CRGB leds_pattern[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

void setup() {
  delay(3000); // 3 second delay for recovery
  
  FastLED.addLeds<LED_TYPE,PIN_PATTERN,COLOR_ORDER>(leds_pattern, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(PIN_TOUCH, INPUT);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, confetti, sinelon, juggle, bpm, sinelon_red, sinelon_blue, juggle_red, juggle_blue};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  if(digitalRead(PIN_TOUCH==LOW)) { nextPattern(); }
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
} 

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}



void juggle_redblue() 
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds_pattern, NUM_LEDS, 20);
  for( int i = 0; i < 8; i++) {
    leds_pattern[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(0, 255, 192);
    leds_pattern[NUM_LEDS-beatsin16(i+7,0,NUM_LEDS)] |= CHSV(160, 255, 192);
  }
}

