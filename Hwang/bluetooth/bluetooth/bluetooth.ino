#include <SoftwareSerial.h>
SoftwareSerial bluetooth(2,3);
 
void setup(){
  Serial.begin(9600);
  bluetooth.begin(9600);
}
 
void loop(){
  
  if (bluetooth.available()) {
    Serial.write(bluetooth.read());
  }
  if (Serial.available()) {
    bluetooth.write(Serial.read());
  }
}
