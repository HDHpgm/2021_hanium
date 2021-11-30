#include <Arduino_LSM6DS3.h>
#include <Servo.h>

Servo myservo;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }
  myservo.attach(10);
}

void loop() {
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
  }
  int wrist = x * 100;
  Serial.println(wrist);
  int ra = constrain(map(wrist, -90, 90, 0, 140), 0, 140);
  myservo.write(ra);
  
}
