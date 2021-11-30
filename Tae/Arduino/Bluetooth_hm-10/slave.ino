#include <SoftwareSerial.h>

SoftwareSerial BTSerial(4,5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Slave!");
  BTSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(A5);
  
  while(BTSerial.available()){
    byte data = BTSerial.read();
    Serial.write(data);
  }

  while(Serial.available()){
    BTSerial.println(sensorValue);
    delay(100);
  }
}
