#include "TMCHeartBeat.h"

/*****Circular List를 위한 count를 초기화 시키기 위함*****/
void setCount(byte *co){
  if(*co >= 5){
    *co = 0;
  }
}

uint8_t getBPM(uint8_t pin, LiquidCrystal_I2C* pLCD){
  /**********변수 선언**********
   * BPM값(임시) 측정마다 바뀔 수 있음
   * 신뢰할만 한 값을 가져왔는지 판단
   * 임시값을 저장하기 위한 배열, 4번 같으면 신뢰하는 것으로 판단
   * Ciurcular List를 위한 count
   *******************************/
  uint8_t rateValue, lcd_counter = 0;
  uint8_t startTime = millis();
  boolean isGetValue = true;
  uint8_t values[4] = {1, 2, 3, 4};
  byte count = 0;
  DFRobot_Heartrate heartrate(DIGITAL_MODE);
  
  do{
    heartrate.getValue(pin);
    rateValue = heartrate.getRate();
    if(rateValue){
      Serial.print("BPM(temp): ");
	  Serial.println(rateValue);
      values[count++] = rateValue;
      setCount(&count);
      for(int i = 0; i < 3; i++){
        if(values[i] != values[i + 1])
          isGetValue = false;
		else
          isGetValue = true;
	  }
    }
	if(millis() - startTime > 600){
		lcd_counter++;
		pLCD->setCursor(13, 1);
		switch(lcd_counter % 3){
			case 0:
				pLCD->print(".  ");
				break;
			case 1:
				pLCD->print(".. ");
				break;
			default:
				pLCD->print("...");
				break;
		}
		startTime = millis();
	}		
  }while(!isGetValue || !rateValue);
  
  return rateValue;
}

byte getHRState(uint8_t BPM){//심박수에 따라 state_hrt 분류
	byte state_hrt;

	if(BPM <= 55) 
		state_hrt = 1;
	else if(BPM <= 76)
		state_hrt = 2;
	else if(BPM <= 150)
		state_hrt = 3;
	else
		state_hrt = 4;

	return state_hrt;
}

uint8_t getGSR(uint8_t pin){
	uint8_t GSR;
	GSR = analogRead(pin);

	return GSR;
}

byte getGRState(uint8_t GSR, uint8_t IGSR){
	int Value = abs((int)GSR - (int)IGSR);
	byte state_gsr = 0;
	if(Value < DEVIATION)
		state_gsr = 5;
	else
		state_gsr = 6;

	return state_gsr;
}

byte getStateValue(byte state_hrt, byte state_gsr){
	byte state = -1;

	/* <기분에 따른 반환값>
	*  7 우울 gloomy
	*  8 슬픔 sad
	*  9 평범/평소 normal
	*  10 기쁨 happy
	*  11 화남 angry
	*  -1 ERROR
	*/
	if(state_gsr == 5){
		if(state_hrt == 2)
			state = NORMAL;
		else if(state_hrt == 3)
			state = HAPPY;
		else
			state = ERROR;
	}
	else if(state_gsr == 6){
		if(state_hrt == 1)
			state = GLOOMY;
		else if(state_hrt == 2)
			state = SAD;
		else if(state_hrt == 3 || state_hrt == 4)
			state = ANGRY;
		else
			state = -1;
	}
	else
		state = -1;
	return state;
}
