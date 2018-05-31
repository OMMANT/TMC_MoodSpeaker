#define USE_LCD	1

typedef PinNumber int;
class TMC_Heartrate{
public:
	TMC_Heartrate(PinNumber pin){ pin_ = pin; }
	TMC_Heartrate(PinNumber pin, uint8_t mode){ pin_ = pin; mode_ = mode}
	uint8_t getValue();
	byte	getState();
	void	printToLCD();
	void	printToSerial();

private:
	PinNumber pin_ = -1;
	boolean mode_ = false;		//false: not to use LCD
	uint8_t BPM;
	byte	state_;
};

class TMC_GSR{
public:
	TMC_GSR(PinNumber pin){ pin_ = pin; }
	TMC_GSR(PinNumber pin, uint8_t mode){ pin_ = pin; mode_ = mode;}
	uint8_t getValue();
	byte	getState();
	void	printToLCD();
	void	printToSerial();

private:
	PinNumber pin_ = -1;
	boolean mode_ = false;
	uint8_t GSR;
	byte	state_;
};
