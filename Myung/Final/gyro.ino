#include "MPU9250.h"
#include <Adafruit_PWMServoDriver.h>
MPU9250 mpu;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int x, y, z;


void setup() {
    Serial.begin(115200);
    Wire.begin();
    pwm.begin();
    delay(2000);

    if (!mpu.setup(0x68)) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }

    pwm.setPWM(13,0,150);  //팔꿈치
    pwm.setPWM(14,0,150);  //어깨
    pwm.setPWM(15,0,150);  //견갑골
}

void loop() {
    if (mpu.update()) {
            print_roll_pitch_yaw();  
            int rx = constrain(map(x, 0, 90, 596, 1369), 596, 1369);  
            int ry = constrain(map(y, 0, 90, 596, 1369), 596, 1369);  
            Serial.print(rx);
            Serial.print(", ");
            Serial.print(ry);
            delay(100);       
    }
}

void print_roll_pitch_yaw() {
    x = mpu.getYaw();
    y = mpu.getPitch();
    z = mpu.getRoll();
}