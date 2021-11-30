#include <MPU6050.h>
#include <Servo.h>
 
MPU6050 mpu;
Servo servo;
 
int16_t ax, ay, az;
int16_t gx, gy, gz;
 
int val;
//int prevVal;

 
void setup() 
{
    Wire.begin();
    Serial.begin(9600);
    
//  Serial.println("Initialize MPU");
    mpu.initialize();
//  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
    servo.attach(9);

}
 
void loop() 
{

    
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
 
    val = map(ay, -10000, 10000, 0, 180);
//    if (val != prevVal)
//    {
//        servo.write(val);
//        prevVal = val;
//    }
    
    servo.write(val);
    Serial.println(val);
    delay(50);
    
}
