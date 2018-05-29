#include <DFRobot_Heartrate.h>
#include <TMCHeartBeat.h>

#define BPMSENSORPIN  0
#define GSRSENSORPIN  1

static uint8_t startTime;
static byte states[3] = {0};
static uint8_t initialGSR;

void setup() {
  // put your setup code here, to run once:
  startTime = millis();
  Serial.begin(115200);
  digitalWrite(13, LOW);
  delay(5000);
  initialGSR = getGSR(GSRSENSORPIN);
  Serial.print("InitialGSR: ");
  Serial.println(initialGSR);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t BPM = getBPM(BPMSENSORPIN);
  states[0] = getHRState(BPM);
  Serial.print("\nBPM: ");
  Serial.println(BPM);
  Serial.print("BPMState: ");
  Serial.println(states[0]);
  uint8_t GSR = getGSR(GSRSENSORPIN);
  Serial.print("\nGSR: ");
  Serial.println(GSR);    
}
