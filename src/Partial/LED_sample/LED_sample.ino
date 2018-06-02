#include <Adafruit_NeoPixel.h>
#include <math.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//핀번호 설정
#define NEOPIN    3

//네오픽셀 설정사항(픽셀 수, 최대밝기)
#define NUMPIXELS 16
#define MAXLIGHT  150

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

static const int delayTime = 500;
static uint8_t wakeTime = 0;
static float period = 4.0;

volatile byte colors[3] = {55, 157, 196};
const byte ColorHOS[3] = {55, 157, 196};
static int targetMin = 45;
static int targetMax = 185;

void setColor(byte color[3]){
  wakeTime = millis();
  for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, colors[0], colors[1], colors[2]);
    pixels.show();
  }
}

uint8_t getLEDBright(int maxlight){
  uint8_t uReturn;
  uReturn = round((exp(sin(millis() / (500.0 * period) * 3.141592)) - 0.36787944) * (maxlight / 2.3504024));
  return uReturn;
}



void setup(){
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  pixels.begin(); // This initializes the NeoPixel library.
  wakeTime = millis();
  period = 2;
}

void loop(){
  for(int i = 0; i < NUMPIXELS; i++){
    colors[0] = getLEDBright(55);
    colors[1] = getLEDBright(157);
    colors[2] = getLEDBright(196);
    pixels.setPixelColor(i, colors[0], colors[1], colors[2]);
    pixels.show();
  }
  delay(10);
}

