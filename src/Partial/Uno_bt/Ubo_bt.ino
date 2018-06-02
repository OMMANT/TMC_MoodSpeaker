#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Adafruit_NeoPixel.h"
#include "math.h"
#include "TMCHeartBeat.h"
#ifdef __AVR__
  #include "avr/power.h"
#endif

//핀번호 설정
#define BT_TXD       2
#define BT_RXD       3
#define NEOPIN       9
#define MP3_TXD		 10
#define MP3_RXD		 11
#define VOLVRREST    A1 //볼륨조절 가변저항 핀번호(아날로그)
#define LEDVRREST	 A2
#define RE_MEASURE	 12

//네오픽셀 설정사항(픽셀 수)
#define NUMPIXELS    16

//Before void setup() -- 블루투스, MP3모듈 부분
SoftwareSerial btSerial(BT_TXD, BT_RXD);
SoftwareSerial mp3Serial(MP3_TXD, MP3_RXD); //RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value); //디버그용 DFPLAYER 상태표시
static byte feelingState; //(1: Happy, 2: Sad, 3: Normal, 4: Gloomy, 5: Angry)
volatile static uint8_t modifiedValue;
int previous_modifiedValue = -1;

//Before void setup() -- LED 부분
static const int delayTime = 500;
static uint8_t wakeTime = 0;
static float period = 4.0;

volatile byte colors[3] = {55, 157, 196};
const byte ColorHOS[3] = {55, 157, 196};
static int targetMin = 45;
static int targetMax = 185;

void setColor(byte color[3]) {
  wakeTime = millis();
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, colors[0], colors[1], colors[2]);
    pixels.show();
  }
}

uint8_t getLEDBright(int maxLight) {
  uint8_t uReturn;
  uReturn = round((exp(sin(millis() / (500.0 * period) * 3.141592)) - 0.36787944) * (maxlight / 2.3504024));
  return uReturn;
}

void setup() {

  /*인터럽트 핀 두개 정의해서 버튼 주고 하나는 눌리면 
  btSerial.write('t'); 해주시구요
  하나는 다음곡? 다른곡 재생으로 해주세요(재준)
  */
  
  Serial.begin(9600);
  btSerial.begin(9600);

  //void setup() -- MP3모듈 부분
  mp3Serial.begin(9600);
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Hello, AI Jukebox is Preparing for a Start-Up!"));
  Serial.println(F("Initializing the jukebox ... (Waie a little while)"));

  if(!myDFPlayer.begin(mp3Serial)) {
    Serial.println(F("LOST CONNECTION -- Please Try Again!"));
    while(true) {
      delay(0);
    }
  }

  Serial.println(F("AI Jukebox Connected :: Sense your Feelings!"));
  randomSeed(analogRead(0));
  
  //void setup() -- LED 부분
  #if defined(_AVR_ATtiny85_)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  wakeTime = millis();
  period = 2;	//주기: 2초
  pinMode(RE_MEASURE, INPUT);
}

void loop() {

  //void loop() -- Bluetooth 부분
  int hun = -1;
  int ten = -1;
  int one = -1;

  while(feelingState == -1) {
    if(btSerial.available()) {
      feelingState = (btSerial.read());
      Serial.print("state = ");
      Serial.println(feelingState);
    }
  }
  while(hun = -1) {
    if(btSerial.available()) {
      hun = (btSerial.read());
      Serial.print("hun = ");
      Serial.println(hun);
    }
  }
  while(ten = -1) {
    if(btSerial.available()) {
      ten = (btSerial.read());
      Serial.print("ten = ");
      Serial.println(ten);
    }
  }
  while(one = -1){
    if(btSerial.available() {
      one = (btSerial.read());
      Serial.print("one = ");
      Serial.println(one);
      bpm = 100 * hun + 10 * ten + one;
    }
  }

  //void loop() -- MP3모듈 부분
  int sensorValue;
  sensorValue = analogRead(VOLVRREST);
  modifiedValue = int(30*sensorValue/1023);

  if(previous_modifiedValue != modifiedValue) {
    Serial.print("Current Volume:");
    Serial.print("  ");
    Serial.println(modifiedValue);
    myDFPlayer.volume(modifiedValue);
    previous_modifiedValue = modifiedValue;
  }

  if(digitalRead(NEXTSONGBTN)==HIGH) {
    if(feelingState == HAPPY) {
      myDFPlayer.playFolder(1,random(1,50)); //'기쁨폴더(01)'에서 50개 노래 중 랜덤재생
    }
    else if(feelingState == SAD) {
      myDFPlayer.playFolder(2,random(1,30)); //'슬픔폴더(02)'에서 30개 노래 중 랜덤재생
    }
    else if(feelingState = NORMAL) {
      myDFPlayer.playFolder(3,random(1,30)); //'평상폴더(03)'에서 30개 노래 중 랜덤재생
    }
    else if(feelingState == GLOOMY) {
      myDFPlayer.playFolder(4,random(1,30)); //'우울폴더(04)'에서 30개 노래 중 랜덤재생
    }
    else if(feelingState == ANGRY){
      myDFPlayer.playFolder(5,random(1,30)); //'화남폴더(05)'에서 30개 노래 중 랜덤재
    }
	else ;
  }

   //void loop() -- LED부분
   for(int i=0; i<NUMPIXELS; i++) {
    colors[0] = getLEDBright(55);
    colors[1] = getLEDBright(157);
    colors[2] = getLEDBright(196);
    pixels.setPixelColor(i, colors[0], colors[1], colors[2]);
    pixels.show();
   }
   
   if(digtalRead(RE_MEASURE) == HIGH){
	   if(btSerial.available()){
		   byte a = 'K';
		   a = btSerial.read();
		   while(a != 'o'){
			   delay(0);
		   }
	   }
   }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}