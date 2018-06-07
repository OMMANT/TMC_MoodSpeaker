#include <DFRobot_Heartrate.h>
#include <LiquidCrystal_I2C.h>
#include <TMCHeartBeat.h>
#include <SoftwareSerial.h>

#define BPMSENSORPIN  A3
#define GSRSENSORPIN  A2
#define BT_TXD  12
#define BT_RXD  11

volatile static uint8_t BPM, GSR;
volatile static byte states[3] = {0};
static uint8_t initialGSR;
static boolean isGetState = false;
static LiquidCrystal_I2C lcd(0x3F, 16, 2);
SoftwareSerial btSerial(BT_TXD, BT_RXD);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  btSerial.begin(19200);
  digitalWrite(13, LOW);
  initMeasure();
}

void loop() {
  // put your main code here, to run repeatedly:
  byte temp = 0;
  if(btSerial.available()){
    temp = btSerial.read();
    if(temp == 'm'){      
      Serial.println("\nRemeasuring...");
      initMeasure();
    }      
    temp = 0;
  }
}

void initMeasure() {
  lcd.init();
  lcd.backlight();      
  isGetState = false;
  lcd.setCursor(4, 1);
  lcd.print("Measuring");
  lcd.setCursor(13, 1);
  delay(4500);
  initialGSR = getGSR(GSRSENSORPIN);
  Serial.print("InitialGSR: ");
  Serial.println(initialGSR);
  while(!isGetState){
    BPM = getBPM(BPMSENSORPIN, &lcd);
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
    if(states[2] != (byte)-1){
      isGetState = true;
    }
    Serial.println(isGetState);
    Serial.println("ALL State Calculated!");
  }
  lcd.setCursor(0, 0);
  lcd.print("Heartrate: ");
  lcd.setCursor(10, 0);
  int temp = ((int)BPM % 1000) / 100;
  if(temp)
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
  lcd.setCursor(10,1);
  switch(states[2]){
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
  btSerial.write(states[2]);
  delay(100);
  btSerial.write(BPM);
  Serial.println("Sent");
}
