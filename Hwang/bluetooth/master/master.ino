#include <SoftwareSerial.h>
#include <MPU6050.h> //(scl -> A5, sda -> A4 )
SoftwareSerial bluetooth(2,3); //(RX, TX)
MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int val;
int prevVal;

// flex센서 데이터 핀 
int flexPin0 = A0;
int flexPin1 = A1;
int flexPin2 = A2;
int flexPin3 = A3;
int flexPin4 = A4;
 
void setup(){
  Serial.begin(9600);     // 시리얼 통신 속도 설정
  bluetooth.begin(9600);  // 블루투스 통신 속도 설정
  Wire.begin();
  mpu.initialize();  
  
}
 
void loop(){
  //flex센서 값 읽어오기
  int flexValue0 = analogRead(flexPin0);
  int flexValue1 = analogRead(flexPin1);  
  int flexValue2 = analogRead(flexPin2); 
  int flexValue3 = analogRead(flexPin3);
  int flexValue4 = analogRead(flexPin4);

  //블루투스에 데이터 전송 손가락.
  bluetooth.write(flexValue0);
  bluetooth.write(flexValue1);
  bluetooth.write(flexValue2);
  bluetooth.write(flexValue3);
  bluetooth.write(flexValue4);
  delay(100);

  // gyro센서 값 읽어오기
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  gyroValue = map(ay, -17000, 17000, 0, 179); 
  
  // 블루투스에 데이터 전송 손목.
  bluetooth.write(gyroValue);  
  delay(100);

  
}
