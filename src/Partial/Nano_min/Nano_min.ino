#include <DFRobot_Heartrate.h>
#include <LiquidCrystal_I2C.h>
#include <TMCHeartBeat.h>
#include <SoftwareSerial.h>
#include <Wire.h>      //저번에 이게 (0x3F 처리해 주는 거라 했던거 같은데 없어져있길래 추가)

#define BPMSENSORPIN  A3
#define GSRSENSORPIN  A2
#define BT_TXD 12      //서로 핀번호 바뀌어 있길래 수정
#define BT_RXD 11      //

volatile static uint8_t BPM, GSR;
static uint8_t startTime;
static byte states[3] = { 0 };
static uint8_t initialGSR;
static boolean isGetState = false;

LiquidCrystal_I2C lcd(0x3F, 16, 2);      //이건 함수임. static 주는거 아님, SCL = A5, SDA = A4 핀 주기
SoftwareSerial btSerial(BT_TXD, BT_RXD);//블루투스 통신 몇번 핀으로 해줄건지 정해주는 함수

void setup() {
   // put your setup code here, to run once:
   startTime = millis();
   Serial.begin(115200);
   btSerial.begin(9600);      //블루투스 통신 주파수 9600, 바꾸고 싶으면 블루투스 세팅도 바꿔야 하니깐 바꾸고 싶으면 나한테 말하고 블투에 주파수 맞춰줘야함
   digitalWrite(13, LOW);
   lcd.init();
   lcd.backlight();
   lcd.setCursor(4, 0);
   lcd.print("Measuring");
   lcd.setCursor(13, 0);
   delay(5000);
   initialGSR = getGSR(GSRSENSORPIN);
   Serial.print("InitialGSR: ");
   Serial.println(initialGSR);
   while (!isGetState) {
      BPM = getBPM(BPMSENSORPIN, &lcd, startTime);
      states[0] = getHRState(BPM);
      Serial.print("\nBPM: ");
      Serial.println(BPM);
      Serial.print("BPMState: ");
      Serial.println(states[0]);
      GSR = getGSR(GSRSENSORPIN);
      states[1] = getGRState(GSR, initialGSR);
      Serial.print("\nGSR: ");
      Serial.println(GSR);
      Serial.print("GSRState: ");
      Serial.println(states[1]);
      states[2] = getStateValue(states[0], states[1]);
      Serial.print("Final State: ");
      Serial.println(states[2]);
      if (states[2] != (byte)-1) {
         isGetState = true;
      }
      digitalWrite(5, LOW);
      Serial.println(isGetState);
      Serial.println("ALL State Calculated!");
   }
   
   btSerial.write(states[2]);      //state값 보내기
   btSerial.write(BPM);         //BPM값 보내기, (이부분 실험 필요할듯. 세개가 한번에 보내지기는 하는데 이렇게 했을때 우노에서 함수처리하고 기다리는 시간이 충분한지 테스트 필요.)
                           //아마 문제 없을거 같기는 함.

   lcd.setCursor(0, 0);
   lcd.print("Heartrate: ");
   lcd.setCursor(10, 0);
   int temp = ((int)BPM % 1000) / 100;
   if (temp)
      lcd.print(temp);
   else
      lcd.print(" ");
   temp = ((int)BPM % 100) / 10;
   lcd.print(temp);
   temp = (int)BPM % 10;
   lcd.print(temp);
   lcd.setCursor(13, 0);
   lcd.print("bpm");
   lcd.setCursor(0, 1);
   lcd.print("Feeling: ");
   lcd.setCursor(10, 1);
   switch (states[2]) {
   case GLOOMY:
      lcd.print("GLOOMY");
      break;
   case SAD:
      lcd.print("   SAD");
      break;
   case NORMAL:
      lcd.print("NORMAL");
      break;
   case HAPPY:
      lcd.print(" HAPPY");
      break;
   case ANGRY:
      lcd.print(" ANGRY");
      break;
   default:
      lcd.print(" ERROR");
   }
}

void loop() {      //loop문 내가 짠거니깐 확인 해봐,
   if (btSerial.available()) {
      if (btSerial.read() == 'm') {
         btSerial.write('?');      //확인 했다는 거 알리면 ?를 보내라고 했는데 이거는 그냥 형이 맞는걸로 바꿔줘
         remeasure();
         btSerial.write(states[2]);
         btSerial.write(BPM);
      }
   }
   delay(100);
}