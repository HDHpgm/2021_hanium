//flex 센서로 서보모터, LED 제어하기
#include <Servo.h>

//객체 및 핀 번호 변수 선언
Servo servothumb;
Servo servoindex;
Servo servomiddle;
Servo servoringfinger;
Servo servopinky;

//define the flex sensor input pins
int flex_1 = A1;
int flex_2 = A2; 
int flex_3 = A3; 
int flex_4 = A4; 
int flex_5 = A5;

//define variables for flex sensor values
int flex_1_val;
int flex_2_val;
int flex_3_val;
int flex_4_val;
int flex_5_val;

void setup() {
  Serial.begin(9600);
  
  //서보모터 핀 및 초기 각도 설정
  servothumb.attach(2);
  servoindex.attach(3);
  servomiddle.attach(4);
  servoringfinger.attach(5);
  servopinky.attach(6);

  servothumb.write(0);
  servoindex.write(0);
  servomiddle.write(0);
  servoringfinger.write(0);
  servopinky.write(0);
}

void loop() {
  //flex센서 값 읽어오기
  flex_1_val = analogRead(flex_1); 
  flex_1_val = map(flex_1_val, 85, 35, 0, 150);
  servothumb.write(flex_1_val);
  
  flex_2_val = analogRead(flex_2);
  flex_2_val = map(flex_2_val, 85, 35, 0, 150);
  servoindex.write(flex_2_val);
  
  flex_3_val = analogRead(flex_3);
  flex_3_val = map(flex_3_val, 85, 35, 0, 150);
  servomiddle.write(flex_3_val);
 
  flex_4_val = analogRead(flex_4);
  flex_4_val = map(flex_4_val, 85, 35, 0, 150);
  servoringfinger.write(flex_4_val);
  
  flex_5_val = analogRead(flex_5);
  flex_5_val = map(flex_5_val, 85, 35, 0, 150);
  servopinky.write(flex_5_val);
  
  
  //map(value, fromLow, fromHigh, toLow, toHigh)
  //
  //매개변수
  //value: 변환할 수
  //
  //fromLow: 현재 범위 값의 하한
  //
  //fromHigh: 현재 범위 값의 상한
  //
  //toLow: 목표 범위 값의 하한
  //
  //toHigh: 목표 범위 값의 상한

}
