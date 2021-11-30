#include <SoftwareSerial.h>
SoftwareSerial bluetooth(2,3);
#include <MPU6050.h> //(scl -> A5, sda -> A4 )
MPU6050 mpu;

#include <Servo.h>
Servo servo0; Servo servo1; Servo servo2; Servo servo3; Servo servo4; Servo servo5;
int servoPin0 = 4; int servoPin1 = 5; int servoPin2 = 6; int servoPin3 = 7; int servoPin4 = 8; //손가락
int servoPin5 = 9; //손목


void setup() {  
  Wire.begin();
  mpu.initialize();
  Serial.begin(9600);
  bluetooth.begin(9600);  
  // 손가락 모터 초기 설정
  servo0.attach(servoPin0); servo0.write(0);
  servo1.attach(servoPin1); servo1.write(0);
  servo2.attach(servoPin2); servo2.write(0);
  servo3.attach(servoPin3); servo3.write(0);
  servo4.attach(servoPin4); servo4.write(0);
  // 손목 모터 초기 설정
  servo5.attach(servoPin5);
}

void loop() {
  int flexValue0 = bluetooth.read(flexValue0);
  int flexValue1 = bluetooth.read(flexValue1);
  int flexValue2 = bluetooth.read(flexValue2);
  int flexValue3 = bluetooth.read(flexValue3);
  int flexValue4 = bluetooth.read(flexValue4);
  
  // 손가락 모터 구동
  int angle0 = map(flexValue0, 550, 350, 0, 180); servo0.write(angle0);
  servo0.write(angle0);
  int angle1 = map(flexValue1, 550, 350, 0, 180); servo1.write(angle1);
  servo1.write(angle1);  
  int angle2 = map(flexValue2, 550, 350, 0, 180); servo2.write(angle2);
  servo2.write(angle2);  
  int angle3 = map(flexValue3, 550, 350, 0, 180); servo3.write(angle3);
  servo3.write(angle3);  
  int angle4 = map(flexValue4, 550, 350, 0, 180); servo4.write(angle4);  
  servo4.write(angle4);  
    
  int gyroValue = bluetooth.read(gyroValue);
  int prevValue;
  
  // 손목 모터 구동
  if (gyroValue != prevVal)
  {
      servo5.write(gyroValue);     
      prevVal = gyroValue;
  } 
}
