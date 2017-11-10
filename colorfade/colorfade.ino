#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define PIN_LEDARRAY1    5
#define PIN_TOUCH        1

#define LED_TYPE           WS2812B
#define COLOR_ORDER        GRB
#define NUM_LEDS1          21
#define FRAMES_PER_SECOND  24

CRGB leds_array1[NUM_LEDS1];
//CRGB RGBcolors[]  = {CRGB::Pink, CRGB::Red, CRGB::Purple, CRGB::Blue, CRGB::Lime, CRGB::Turquoise, CRGB::Orange, CRGB::Yellow};
int HSVcolors[] = {224, 0, 192, 160, 80, 128, 32, 64}; //equivalent HSV hues

byte state_now;
byte state_last = HIGH;
byte state_toggle = 1;
byte ledpower = 1;
unsigned long time_start = 0;
unsigned long time_interval = 0;
byte colorSelect = 0;
int brightness = 10;
int fadeAmount = 5;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setup()
{
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, PIN_LEDARRAY1, COLOR_ORDER>(leds_array1, NUM_LEDS1).setCorrection(TypicalLEDStrip);
  pinMode(PIN_TOUCH, INPUT);
  digitalWrite(PIN_TOUCH, HIGH);
}

void loop()
{
  for(int i = 0; i < NUM_LEDS1; i++ ) 
  {
    leds_array1[i] = CHSV(HSVcolors[colorSelect], 255, 255);
    //leds_array1[i] = RGBcolors[colorSelect]; //uncomment to use RGB color values
  }

  brightness += fadeAmount;
  if(brightness == 10 || brightness == 255) {fadeAmount = -fadeAmount;}
  FastLED.setBrightness(brightness);
  
  if (ledpower) { FastLED.show(); }
  else { FastLED.clear(); }
  
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  state_now = digitalRead(PIN_TOUCH);
  if(state_now == LOW && state_last == HIGH && (millis() - time_start > 200)) { time_start = millis(); }

  time_interval = millis() - time_start;
  if (time_interval > 50) {
    if (state_now == HIGH && state_last == LOW) {
      state_toggle = 1;
      if(time_interval < 1000) { colorSelect = (colorSelect + 1) % ARRAY_SIZE(HSVcolors); }
    } else if (state_now == LOW && state_last == LOW && state_toggle) {
      if (time_interval > 3000 && ledpower) {
        ledpower = 0;
        state_toggle = 0;
      } else if (time_interval > 3000 && not ledpower) {
        ledpower = 1;
        state_toggle = 0;
      }
    }
  }
  state_last = state_now;
  
}





