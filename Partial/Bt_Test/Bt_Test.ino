#include <SoftwareSerial.h>
//핀 설정
#define BT_TXD 2
#define BT_RXD 3

//소프트웨어 UART 설정
SoftwareSerial btSerial(BT_TXD, BT_RXD);

void setup(){
   Serial.begin(9600);
   btSerial.begin(9600);
}

void loop(){
   if (btSerial.available()){
      Serial.write(btSerial.read());
   }
   if (Serial.available()){
      btSerial.write(Serial.read());
   }
}