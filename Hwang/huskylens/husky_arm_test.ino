#include "HUSKYLENS.h"
#include <Arduino.h>
#include "wiring_private.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // 서보모터 최소 출력 값 = 0도
#define SERVOMAX  600 // 서보모터 최대 출력 값 = 180도 

HUSKYLENS huskylens;
Uart mySerial (&sercom0, 5, 6, SERCOM_RX_PAD_1, UART_TX_PAD_0);

void printResult(HUSKYLENSResult result);

// Attach the interrupt handler to the SERCOM
void SERCOM0_Handler() {
  mySerial.IrqHandler();
}

void setup() {
    // Reassign pins 5 and 6 to SERCOM alt
    pinPeripheral(5, PIO_SERCOM_ALT);
    pinPeripheral(6, PIO_SERCOM_ALT);
    
    Serial.begin(115200);
    Wire.begin();
    mySerial.begin(9600);    
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(1000);
    }
    pwm.begin();
    pwm.setPWM(1,0,0);  //엄지
    pwm.setPWM(2,0,0);  //검지
    pwm.setPWM(3,0,0);  //중지
    pwm.setPWM(4,0,0);  //약지
    pwm.setPWM(5,0,0);  //소지
    pwm.setPWM(8,0,0);  //손목
    pwm.setPWMFreq(60); //오작동 한다면 50Hz에서 조금씩 바꿔보기 
}

void loop() {      
    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);

            if(result.ID == 2) {
              rock();
              delay(1000);
            }
            else if(result.ID == 3) {
              scissor();
              delay(1000);
            }
            else if(result.ID == 1) {
              paper();
              delay(1000);
            }
        }    
    }

}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}

void rock() {
    pwm.setPWM(1,0,500); //pca9685모듈의 1번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(2,0,500); //pca9685모듈의 2번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(3,0,500); //pca9685모듈의 3번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(4,0,500); //pca9685모듈의 4번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(5,0,500); //pca9685모듈의 5번 포트에 연결된 서보를 ra만큼 회전
    Serial.println("rock");
}
void scissor() {
    pwm.setPWM(1,0,150); //pca9685모듈의 1번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(2,0,500); //pca9685모듈의 2번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(3,0,500); //pca9685모듈의 3번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(4,0,150); //pca9685모듈의 4번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(5,0,150); //pca9685모듈의 5번 포트에 연결된 서보를 ra만큼 회전
    Serial.println("scissor");
}
void paper() {
    pwm.setPWM(1,0,150); //pca9685모듈의 1번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(2,0,150); //pca9685모듈의 2번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(3,0,150); //pca9685모듈의 3번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(4,0,150); //pca9685모듈의 4번 포트에 연결된 서보를 ra만큼 회전
    pwm.setPWM(5,0,150); //pca9685모듈의 5번 포트에 연결된 서보를 ra만큼 회전
    Serial.println("paper");
}

void wrist() {
    pwm.setPWM(8,0,600);  
}
