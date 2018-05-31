#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

/****핀 번호 정의*****/
#define NEXTSONGBTN 2   //다음곡 버튼 핀 번호
#define VOLVRREST   A0   //볼륨조절 가변저항 핀번호(아날로그)

SoftwareSerial mySoftwareSerial(10,11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;       
void printDetail(uint8_t type, int value);    //디버그용; DFPLYAER 상태 표시(시리얼)
int feelingState; //(1: Happy, 2: Sad, 3: Normal, 4: Gloomy, 5: Angry)
volatile static uint8_t modifiedValue;
int previous_modifiedValue = -1;


void setup() {
  // Button Pin Settings--
  pinMode(NEXTSONGBTN, INPUT_PULLUP);

  //--MP3 Module Settings--
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("Hello, AI Jukebox is Preparing for Start-Up!"));
  Serial.println(F("Initializing the jukebox ... (Wait a little while)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("LOST CONNECTION - Please Try Again!"));
    while(true) {
      delay(0);
    }
  }
  Serial.println(F("AI Jukebox Connected :: Sense your Feelings!"));
  
  randomSeed(analogRead(0));
}


void loop() {
  int sensorValue;
  sensorValue = analogRead(VOLVRREST);
  modifiedValue = int(30*sensorValue/1023);
    
  //--볼륨조절 (가변저항#2)--
  if (previous_modifiedValue != modifiedValue) {
    Serial.print("Current Volume:");
    Serial.print("  ");
    Serial.println(modifiedValue);
    myDFPlayer.volume(modifiedValue);
    previous_modifiedValue = modifiedValue;
  }
  
  //--다음곡 (버튼#2)--
  if(digitalRead(NEXTSONGBTN)==HIGH){
    if(feelingState == 1) {
      myDFPlayer.playFolder(1,random(1,50)); //'기쁨폴더(01)'에서 50개 노래 중 랜덤재생
    }
    else if(feelingState == 2) {
      myDFPlayer.playFolder(2,random(1,30)); //'슬픔폴더(02)'에서 30개 노래 중 랜덤재생
    }
    else if(feelingState == 3) {
      myDFPlayer.playFolder(3,random(1,30)); //'평상폴더(03)'에서 30개 노래 중 랜덤재생
    }
    else if(feelingState == 4) {
      myDFPlayer.playFolder(4,random(1,30)); //'우울폴더(04)'에서 30개 노래 중 랜덤재생
    }
    else {
      myDFPlayer.playFolder(5,random(1,30)); //'화남폴더(05)'에서 30개 노래 중 랜덤재생
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
