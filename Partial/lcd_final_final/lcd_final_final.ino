#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
//윗 세개는 기본 포함



void setup() {
  lcd.init();         //lcd사용을 위해 꼭 필요한 함수니깐 set업문에 추가 요망
  lcd.backlight();    //lcd사용을 위해 꼭 필요한 함수니깐 set업문에 추가 요망
}



bool new_data = false;
//loop문 내의 정의 필요
//lcd화면 변경 할건지 확인 하게 해줄 변수니깐 추가해주고



lcd.setCursor(4, 0);
lcd.print("Measuring");
lcd.setCursor(13, 0);
new_data = true;
unsigned long lcd_timer;
lcd_timer = millis();
/*앞 여섯줄은 측정 모드 시작할 시 앞에 써주세요
첫 세줄은 LCD에 측정 중이라고 뜨게 하는 줄,
네번째 줄은 새로운 값이 들어왔다고 알려주는 변수임.
마지막 둘줄은 lcd 변경해주는 주기로 쓸거니깐 정의해주기*/



//lcd_measuring함수는 측정 모드에서 계속 반복문에 넣어주길. measuring 계속 프린트 하고 있을 거임
void lcd_measuring() {
  unsigned int lcd_counter = 0;
  
  if ((millis() - lcd_timer) > 600) {
    lcd_counter++;
    if (lcd_counter % 3 == 0) {
      lcd.setCursor(13, 0);
      lcd.print(".  ");
    }
    else if (lcd_counter % 3 ==1) {
      lcd.setCursor(13, 0);
      lcd.print(".. ");
    }
    else {
      lcd.setCursor(13, 0);
      lcd.print("...");
    }
  }
}




//loop문 안에서 돌려주세요. lcd모니터에서 심박수랑 감정 보여줌
//heartRate변수 대신에 실제 심박수 변수명을 넣어줘
//state변수 대신에 실제 감정 변수 넣어주고 switch값에도 감정에 맞는거 넣기
//gsr값 센서에 의해 감정이 변하면 변했을때 \
new_data = true;
//줄 추가해 주기
if (new_data == true) {
  a = (heartRate % 1000) / 100;   //심박수 100의 자리
  b = (heartRate % 100) / 10;     //심박수 10의 자리
  c = (heartRate % 10);       //심박수 1의 자리

  String feeling;

  lcd.setCursor(0, 0);
  lcd.print("Heartrate:");
  
  lcd.setCursor(10, 1);
  lcd.print(a);
  lcd.print(b);
  lcd.print(c);

  lcd.setCursor(13, 0);
  lcd.print("bpm");

  lcd.setCursor(0, 1);
  lcd.print("Feeling:");

  switch (state) {
    case 7
      feeling = "GLOOMY";
      break;

    case 8
      feeling = "   SAD";
      break;

    case 9
      feeling = "NORMAL";
      break;

    case 10
      feeling = " HAPPY";
      break;

    case 11
      feeling = " ANGRY";
      break;
  }

  lcd.setCursor(10, 0);
  lcd.print(Feeling);

  new_data = false;
}
