#include <SoftwareSerial.h>

#define BT_TXD 2
#define BT_RXD 3

SoftwareSerial btSerial(BT_TXD, BT_RXD);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  btSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte alpha = 123;
  if(Serial.available()){
    if(Serial.read() == 'm')
      btSerial.write(alpha);
  }
  if(btSerial.available()){
    Serial.write(btSerial.read());
  }
}
