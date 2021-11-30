#include <SoftwareSerial.h>

SoftwareSerial BTSerial(4,5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Master!");
  BTSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(BTSerial.available()){
    byte data = BTSerial.read();
    Serial.write(data);
  }

  while(Serial.available()){
    byte data = Serial.read();
    BTSerial.write(data);
  }
}
