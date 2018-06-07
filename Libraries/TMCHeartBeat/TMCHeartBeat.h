#ifndef LiquidCrystal_I2C_h
	#include <LiquidCrystal_I2C.h>
#endif
#ifndef __DFRobot_HEARTRATE__H
	#include <DFRobot_Heartrate.h>
#endif
#ifndef __TMC_SENSOR__H
	#define __TMC_SENSOR__H
#endif

#define DEVIATION	50
#define GLOOMY		7
#define SAD		8
#define	NORMAL		9
#define	HAPPY		10
#define	ANGRY		11
#define	ERROR		-1

void setCount(byte *co);
uint8_t getBPM(uint8_t pin, LiquidCrystal_I2C* pLCD);
byte getHRState(uint8_t BPM);
uint8_t getGSR(uint8_t pin);
byte getGRState(uint8_t GSR, uint8_t IGSR);
byte getStateValue(byte state_hrt, byte state_gsr);
