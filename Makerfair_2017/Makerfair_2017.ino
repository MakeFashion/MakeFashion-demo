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

  if(digitalRead(PIN_TOUCH)) { nextPattern(); }
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds_pattern, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds_pattern[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds_pattern, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds_pattern[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds_pattern, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds_pattern[pos] += CHSV( gHue, 255, 192);
}

void sinelon_redblue()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds_pattern, NUM_LEDS, 20);
  int pos1 = beatsin16(13,0,NUM_LEDS);
  int pos2 = NUM_LEDS - pos1;
  leds_pattern[pos1] += CHSV( 0, 255, 192);
  leds_pattern[pos2] += CHSV( 160, 255, 192);
}

void sinelon_red()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds_pattern, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds_pattern[pos] += CHSV( 0 + gHue % 16, 255, 192);
}

void sinelon_blue()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds_pattern, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds_pattern[pos] += CHSV(160 + gHue % 16, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds_pattern[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds_pattern, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds_pattern[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void juggle_red() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds_pattern, NUM_LEDS, 20);
  for( int i = 0; i < 8; i++) {
    leds_pattern[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(0 + gHue % 16, 255, 192);
  }
}

void juggle_blue() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds_pattern, NUM_LEDS, 20);
  for( int i = 0; i < 8; i++) {
    leds_pattern[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(160 + gHue % 16, 255, 192);
  }
}

void juggle_redblue() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds_pattern, NUM_LEDS, 20);
  for( int i = 0; i < 8; i++) {
    leds_pattern[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(0, 255, 192);
    leds_pattern[NUM_LEDS-beatsin16(i+7,0,NUM_LEDS)] |= CHSV(160, 255, 192);
  }
}



