#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//(0~4096까지 가능하나, 150일때 서보모터의 각도가 0도이고, 600일 때 서보모터의 각도가 180도임.)
#define SERVOMIN  150 // 서보모터 최소 출력 값 = 0도
#define SERVOMAX  600 // 서보모터 최대 출력 값 = 180도 

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
  pwm.begin();
  pwm.setpwm(1,0,0);  //엄지
  pwm.setpwm(2,0,0);  //검지
  pwm.setpwm(3,0,0);  //중지
  pwm.setpwm(4,0,0);  //약지
  pwm.setpwm(5,0,0);  //소지
  pwm.setPWMFreq(60); //오작동 한다면 50Hz에서 조금씩 바꿔보기 
}

void loop() {
    
    flex_1_val = analogRead(flex_1);
    flex_1_val = constrain(map(flex_1_val, 85, 35, 150, 450), 150, 450); //받은 값의 범위 0~180을 펄스길이150~600으로 매핑해주고, ra의 최소,최대를 150,600을 넘지 않게 해준다.
    pwm.setPWM(1,0,flex_1_val); //pca9685모듈의 0번 포트에 연결된 서보를 ra만큼 회전
    
    flex_2_val = analogRead(flex_2); 
    flex_2_val = constrain(map(flex_2_val, 85, 35, 150, 450), 150, 450); //받은 값의 범위 0~180을 펄스길이150~600으로 매핑해주고, ra의 최소,최대를 150,600을 넘지 않게 해준다.
    pwm.setPWM(2,0,flex_2_val); //pca9685모듈의 0번 포트에 연결된 서보를 ra만큼 회전
    
    flex_3_val = analogRead(flex_3); 
    flex_3_val = constrain(map(flex_3_val, 85, 35, 150, 450), 150, 450); //받은 값의 범위 0~180을 펄스길이150~600으로 매핑해주고, ra의 최소,최대를 150,600을 넘지 않게 해준다.
    pwm.setPWM(3,0,flex_3_val); //pca9685모듈의 0번 포트에 연결된 서보를 ra만큼 회전
    
    flex_4_val = analogRead(flex_4); 
    flex_4_val = constrain(map(flex_4_val, 85, 35, 150, 450), 150, 450); //받은 값의 범위 0~180을 펄스길이150~600으로 매핑해주고, ra의 최소,최대를 150,600을 넘지 않게 해준다.
    pwm.setPWM(4,0,flex_4_val); //pca9685모듈의 0번 포트에 연결된 서보를 ra만큼 회전
    
    flex_5_val = analogRead(flex_5); 
    flex_5_val = constrain(map(flex_5_val, 85, 35, 150, 450), 150, 450); //받은 값의 범위 0~180을 펄스길이150~600으로 매핑해주고, ra의 최소,최대를 150,600을 넘지 않게 해준다.
    pwm.setPWM(5,0,flex_5_val); //pca9685모듈의 0번 포트에 연결된 서보를 ra만큼 회전
    
}
