#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define BT_TXD       2
#define BT_RXD       3
#define NEXTSONGBTN  4 //다음곡 버튼 핀 번호
#define VOLVRREST    A0 //볼륨조절 가변저항 핀번호(아날로그)

SoftwareSerial btSerial(BT_TXD, BT_RXD);
SoftwareSerial mySoftwareSerial(10,11); //RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value); //디버그용 DFPLAYER 상태표시
int feelingState; //(1: Happy, 2: Sad, 3: Normal, 4: Gloomy, 5: Angry)
volatile static uint8_t modifiedValue;
int previous_modifiedValue = -1;


void setup() {
  /*인터럽트 핀 두개 정의해서 버튼 주고 하나는 눌리면 
  btSerial.write('t'); 해주시구요
  하나는 다음곡? 다른곡 재생으로 해주세요(재준)
  */
  Serial.begin(9600);
  btSerial.begin(9600);
}

void loop() {
  /*gsr 함수와 심장박동 함수 잘 불러오시길 ㅠㅠ
  일단 값 받아오는건
  if(btSerial.available){
    btSerial.read();
  }쓰면 될거 같긴 함
  */
  /*분석 코드 (난 감 안잡힘) +  기분 정해주기
  
  
  기분 정해지면 기분 값 맞게 0부터 5까지, 각각 미정, 행복, 슬픔, 화남, 긴장, 평범으로 생각하면 되구요.
  btSerial.write(#); 해주세용
  */
  /* 기분에 따른 노래 틀기 (재준 파트)*/

  /*LED(태환)*/

  /*가변저항 값 읽어서 볼륨 조절(재준), LED밝기 조절(태환)*/
}
