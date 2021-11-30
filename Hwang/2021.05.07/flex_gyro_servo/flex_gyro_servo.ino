#include <Servo.h>
#include <MPU6050.h>

//객체 및 핀 번호 변수 선언
MPU6050 mpu;
Servo servo;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int val1;
int prevVal;
int flexPin = A0, servoPin = 9;

void setup() {
  //서보모터 핀 및 초기 각도 설정

  servo.attach(servoPin);
  servo.write(0);
  
 //gyro 초기 설정
 
  Wire.begin();
  Serial.begin(9600);

  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");  

}

void loop() {
  //flex센서 값 읽어오기
  int flexValue = analogRead(flexPin);

  //센서 값 map() 함수로 서보모터 각도로 변환
  int angle = map(flexValue, 550, 350, 0, 180);
  servo.write(angle);

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
 
 //gyro 작동
  val1 = map(ay, -17000, 17000, 0, 179);
  if (val1 != prevVal)
  {
      servo.write(val1);
      prevVal = val1;
  }
 
  delay(50);
  
  
}
