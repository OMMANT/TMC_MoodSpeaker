#include <DFPlayer_Mini_Mp3.h>
#include <TMCHeartBeat.h>
#include <math.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Pin definition
#define BT_TXD 2
#define BT_RXD 3
#define NEOPIN 9
#define MP3_TXD 10
#define MP3_RXD 11
#define VOLVRREST A1
#define LEDVRREST A2
#define RE_MEASURE 12

//For NEOPIXEL Ring~
#define NUMPIXELS 16

//Software UART connect
SoftwareSerial btSerial(BT_TXD, BT_RXD);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

static byte BPM = 0, feelingState = 0;
static uint8_t a = 0;
volatile static uint8_t modifiedValue;
int previous_modifiedValue = -1;

static const int delayTime = 500;
static uint8_t wakeTime = 0;
static float period = 4.0;

const byte colorSEO[3] = {172, 138, 0};   //When NORMAL
const byte colorDAL[3] = {0, 114, 206};   //When SAD
const byte colorGLA[3] = {56, 20, 96};    //When GLOOMY
const byte colorYOU[3] = {255, 2, 2};     //When ANGRY
const byte colorOMM[3] = {28, 255, 5};    //When HAPPY
const byte colorINI[3] = {175, 39, 47};
volatile static byte colors[3] = {175, 39, 47};
volatile static byte target[3] = {175, 39, 47};

void setColor(byte color[3]){
  wakeTime = millis();
  for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, colors[0], colors[1], colors[2]);
    pixels.show();
  }
}

uint8_t getLEDBright(int maxLight) {
  uint8_t uReturn;
  uReturn = round((exp(sin(millis() / (500.0 * period) * 3.141592)) - 0.36787944) * (maxLight / 2.3504024));
  return uReturn;
}

void setup() {
  Serial.begin(9600);  
  btSerial.begin(19200);
  mp3_set_serial(Serial);
  delay(1);
  mp3_set_volume(10);
  pinMode(RE_MEASURE, INPUT_PULLUP);
  randomSeed(analogRead(0));
  #if defined(_AVR_ATtiny85_)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  wakeTime = millis();
  period = 4;  //주기: 4초
  setColor(colors);
  setValueFromBT();
}

void loop() {
  //void loop() -- MP3
  int sensorValue;
  sensorValue = analogRead(VOLVRREST);
  modifiedValue = (int)(30 * sensorValue / 1023);

  if(previous_modifiedValue != modifiedValue) { 
    mp3_set_volume(modifiedValue);
    previous_modifiedValue = modifiedValue;
  }
   //void loop() -- LED부분
   targetChange();
   colors[0] = getLEDBright(target[0]);
   colors[1] = getLEDBright(target[1]);
   colors[2] = getLEDBright(target[2]);
   for(int i=0; i<NUMPIXELS; i++) {
     pixels.setPixelColor(i, colors[0], colors[1], colors[2]);
     pixels.show();
  }
  //void loop() -- 재측정버튼부분
  if(digitalRead(RE_MEASURE) == LOW){
    byte a = 'm';
    btSerial.write(a);
    BPM = 0;
    feelingState = 0;
    setValueFromBT();
    target[0] = colorINI[0];
    target[1] = colorINI[1];
    target[2] = colorINI[2];
    colors = target;
    while(1){
      colors[0] = getLEDBright(target[0]);
      colors[1] = getLEDBright(target[1]);
      colors[2] = getLEDBright(target[2]);
      if(digitalRead(RE_MEASURE) == HIGH)
        break;
    }
  }
}

void setValueFromBT(){
  while(1){
    if(btSerial.available()){
      feelingState = btSerial.read();
      break;
      
    }
  }
  while(1){
    if(btSerial.available()){
      BPM = btSerial.read();
      break;
    }
  }
  startMusic();
}

void startMusic(){
  if(feelingState == HAPPY){
    uint8_t feelA = random(1,50);
    mp3_play(feelA);
  }
  else if(feelingState == SAD){
    uint8_t feelB = random(51,80);
    mp3_play(feelB);
  }
  else if(feelingState == NORMAL){
    uint8_t feelC = random(81,110);
    mp3_play(feelC);
  }
  else if(feelingState == GLOOMY){
    uint8_t feelD = random(111,140);
    mp3_play(feelD);
  }
  else if(feelingState == ANGRY){
    uint8_t feelE = random(141,170);
    mp3_play(feelE);
  }
  else ;
}

void targetChange(){
  if(feelingState == NORMAL){
    target[0] = colorSEO[0];
    target[1] = colorSEO[1];
    target[2] = colorSEO[2];
  }
  else if(feelingState == SAD){
    target[0] = colorDAL[0];
    target[1] = colorDAL[1];
    target[2] = colorDAL[2];
  }
  else if(feelingState == HAPPY){
    target[0] = colorOMM[0];
    target[1] = colorOMM[1];
    target[2] = colorOMM[2];
  }
  else if(feelingState == GLOOMY){
    target[0] = colorGLA[0];
    target[1] = colorGLA[1];
    target[2] = colorGLA[2];
  }
  else if(feelingState == ANGRY){
    target[0] = colorYOU[0];
    target[1] = colorYOU[1];
    target[2] = colorYOU[2];
  }
  else ;
}
