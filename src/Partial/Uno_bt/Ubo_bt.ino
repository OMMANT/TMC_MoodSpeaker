#include <DFPlayer_Mini_Mp3.h>
#include <TMCHeartBeat.h>
#include <math.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>

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
SoftwareSerial mp3Serial(MP3_TXD, MP3_RXD);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);
DFRobotDFPlayerMini myDFPlayer;

static byte BPM = 0, feelingState = 0;
static uint8_t a = 0;
volatile static uint8_t modifiedValue;
int previous_modifiedValue = -1;

static const int delayTime = 500;
static uint8_t wakeTime = 0;
static float period = 4.0;

const byte colorHOS[3] = {55, 157, 196};
volatile byte colors[3] = {55, 157, 196};

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
  // put your setup code here, to run once:
  Serial.begin(115200);
  btSerial.begin(19200);
  mp3_set_serial(Serial);
  pinMode(RE_MEASURE, INPUT_PULLUP);
  randomSeed(analogRead(0));
  #if defined(_AVR_ATtiny85_)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  wakeTime = millis();
  period = 2;  //주기: 2초
  Serial.println("Ready~!");
  setValueFromBT();
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue;
  sensorValue = analogRead(VOLVRREST);
  modifiedValue = (int)(30 * sensorValue / 1023);

  if(previous_modifiedValue != modifiedValue) { 
    Serial.print("Current Volume:  ");
    Serial.println(modifiedValue);
    mp3_set_volume(modifiedValue);
    previous_modifiedValue = modifiedValue;
  }
   //void loop() -- LED부분
   for(int i=0; i<NUMPIXELS; i++) {
    colors[0] = getLEDBright(55);
    colors[1] = getLEDBright(157);
    colors[2] = getLEDBright(196);
    pixels.setPixelColor(i, colors[0], colors[1], colors[2]);
    pixels.show();
  }
  //void loop() -- 재측정버튼부분
  if(digitalRead(RE_MEASURE) == LOW){
    Serial.println("Pressed!");
    byte a = 'm';
    btSerial.write(a);
    BPM = 0;
    feelingState = 0;
    setValueFromBT();
    while(1){
      if(digitalRead(RE_MEASURE) == HIGH)
        break;
    }
  }
}

void setValueFromBT(){
  while(1){
    if(btSerial.available()){
      Serial.println("Received!");
      feelingState = btSerial.read();
      Serial.print("state = ");
      Serial.println((int)feelingState);
      break;
      
    }
  }
  while(1){
    if(btSerial.available()){
      Serial.println("Received!");
      BPM = btSerial.read();
      Serial.print("BPM = ");
      Serial.println((int)BPM);
      break;
    }
  }
  startMusic();
}

void startMusic(){
  Serial.println("playing Music~");
  Serial.print("Feeling State: ");
  Serial.println(feelingState);
  if(feelingState == HAPPY)
    mp3_play_file_in_folder(1, random(1,50)); //'기쁨폴더(01)'에서 50개 노래 중 랜덤재생
  
  else if(feelingState == SAD) 
    mp3_play_file_in_folder(2, random(1,30)); //'슬픔폴더(02)'에서 30개 노래 중 랜덤재생
  
  else if(feelingState = NORMAL) 
    mp3_play_file_in_folder(3, random(1,30)); //'평상폴더(03)'에서 30개 노래 중 랜덤재생
  
  else if(feelingState == GLOOMY) 
    mp3_play_file_in_folder(4, random(1,30)); //'우울폴더(04)'에서 30개 노래 중 랜덤재생
  
  else if(feelingState == ANGRY)
    mp3_play_file_in_folder(5, random(1,30)); //'화남폴더(05)'에서 30개 노래 중 랜덤재
}
