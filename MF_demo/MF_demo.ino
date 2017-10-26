/* MakeFashion LED control example
 * -------------------------------
 * This software demos LED control using Seeed Studio's ultrasonic range finder module to toggle power to the LEDs
 * and using a sound sensor to switch LED modes.
 * 
 * Requires:
 * ---------
 *  MakeFashion controller board
 *  SeeedStudio Ultrasonic Ranger ( http://wiki.seeed.cc/Grove-Ultrasonic_Ranger/ )
 *  SeeedStudio Touch Sensor ( http://wiki.seeed.cc/Grove-Touch_Sensor/ )
 *  FastLED-compatible LEDs ( https://github.com/FastLED/FastLED/wiki/Overview#chipsets )
 *  
 * Dependencies:
 * -------------
 *  FastLED ( Available via Arduino package manager )
 *  SeeedStudio Ultrasonic library ( https://github.com/Seeed-Studio/Grove_Ultrasonic_Ranger/archive/master.zip )
 */

#include <Ultrasonic.h>
#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define PIN_LEDARRAY1    6
#define PIN_LEDARRAY2    5
#define PIN_ULTRASONIC   0
#define PIN_TOUCH        1

#define LED_TYPE           WS2812B
#define COLOR_ORDER        GRB
#define NUM_LEDS1          32
#define NUM_LEDS2          32
#define BRIGHTNESS         96
#define FRAMES_PER_SECOND  120

#define THRESHOLD_RANGE 10.0 //centimeters
#define THRESHOLD_SOUND 1000

CRGB leds_array1[NUM_LEDS1];
CRGB leds_array2[NUM_LEDS2];
Ultrasonic ultrasonic(PIN_ULTRASONIC);

// function prototypes
void rainbow(CRGB *gArray, int gSize);
void rainbowWithGlitter(CRGB *gArray, int gSize);
void confetti(CRGB *gArray, int gSize);
void sinelon(CRGB *gArray, int gSize);
void juggle(CRGB *gArray, int gSize);
void bpm(CRGB *gArray, int gSize);
void juggle(CRGB *gArray, int gSize);
void juggle_red(CRGB *gArray, int gSize);
void juggle_blue(CRGB *gArray, int gSize);
void juggle_redblue(CRGB *gArray, int gSize);
void blinky(CRGB *gArray, int gSize);


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList)(CRGB *, int);
SimplePatternList gPatterns[] = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gArray1_PatternNumber = 0; // Pattern selector array 1
uint8_t gArray2_PatternNumber = 0; // Pattern selector array 2
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t gBlinky = 0;
long ultrasonic_range;

void setup() {
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, PIN_LEDARRAY1, COLOR_ORDER>(leds_array1, NUM_LEDS1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, PIN_LEDARRAY2, COLOR_ORDER>(leds_array2, NUM_LEDS2).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  
  pinMode(PIN_TOUCH, INPUT);
}

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gArray1_PatternNumber](leds_array1, NUM_LEDS1);
  gPatterns[gArray2_PatternNumber](leds_array2, NUM_LEDS2);
  ultrasonic_range = ultrasonic.MeasureInCentimeters();

  if (ultrasonic_range < 4.0) {
    FastLED.show();  // send the leds array out to the actual LED strip
  } else {
    FastLED.clear();
  }
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  int touched = digitalRead(PIN_TOUCH);
  if(touched) { nextPattern(); }
  //EVERY_N_SECONDS( 10 ) { nextPattern(); }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gArray1_PatternNumber = (gArray1_PatternNumber + 1) % ARRAY_SIZE(gPatterns);
  gArray2_PatternNumber = random(0, ARRAY_SIZE(gPatterns));
}

void rainbow(CRGB *gArray, int gSize)
{
  // FastLED's built-in rainbow generator
  
  fill_rainbow( gArray, gSize, gHue, 7);
}

void rainbowWithGlitter(CRGB *gArray, int gSize)
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow(gArray, gSize);
  addGlitter(gArray, gSize);
}

void addGlitter(CRGB *gArray, int gSize)
{
  if ( random8() < 80) {
    gArray[ random16(gSize) ] += CRGB::White;
  }
}

void confetti(CRGB *gArray, int gSize)
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( gArray, gSize, 10);
  int pos = random16(gSize);
  gArray[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon(CRGB *gArray, int gSize)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( gArray, gSize, 20);
  int pos = beatsin16(13, 0, gSize);
  gArray[pos] += CHSV( gHue, 255, 192);
}

void sinelon_redblue(CRGB *gArray, int gSize)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( gArray, gSize, 20);
  int pos1 = beatsin16(13, 0, gSize);
  int pos2 = gSize - pos1;
  gArray[pos1] += CHSV( 0, 255, 192);
  gArray[pos2] += CHSV( 160, 255, 192);
}

void sinelon_red(CRGB *gArray, int gSize)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( gArray, gSize, 20);
  int pos = beatsin16(13, 0, gSize);
  gArray[pos] += CHSV( 0, 255, 192);
}

void sinelon_blue(CRGB *gArray, int gSize)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( gArray, gSize, 20);
  int pos = beatsin16(13, 0, gSize);
  gArray[pos] += CHSV(160, 255, 192);
}

void bpm(CRGB *gArray, int gSize)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < gSize; i++) { //9948
    gArray[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle(CRGB *gArray, int gSize) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( gArray, gSize, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    gArray[beatsin16(i + 7, 0, gSize)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void juggle_red(CRGB *gArray, int gSize) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( gArray, gSize, 20);
  for ( int i = 0; i < 8; i++) {
    gArray[beatsin16(i + 7, 0, gSize)] |= CHSV(0, 255, 192);
  }
}

void juggle_blue(CRGB *gArray, int gSize) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( gArray, gSize, 20);
  for ( int i = 0; i < 8; i++) {
    gArray[beatsin16(i + 7, 0, gSize)] |= CHSV(160, 255, 192);
  }
}

void juggle_redblue(CRGB *gArray, int gSize) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( gArray, gSize, 20);
  for ( int i = 0; i < 8; i++) {
    gArray[beatsin16(i + 7, 0, gSize)] |= CHSV(0, 255, 192);
    gArray[gSize - beatsin16(i + 7, 0, gSize)] |= CHSV(160, 255, 192);
  }
}

void blinky(CRGB *gArray, int gSize) {
  gBlinky = not gBlinky;
  CRGB color = CRGB::Red;
  if ( gBlinky ) {
    color = CRGB::Black;
  } else {
    color = CRGB::Red;
  }
  for ( int i = 0; i < gSize; i++ ) {
    gArray[i] = color;
  }
}


