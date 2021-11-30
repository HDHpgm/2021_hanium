#include <SoftwareSerial.h> 
#define BT_RXD 2 
#define BT_TXD 3 
SoftwareSerial ESP_wifi(BT_RXD, BT_TXD); 
void setup() { 
  Serial.begin(9600); //시리얼모니터 통신속도는 9600,115200 등등 모두 가능
  ESP_wifi.begin(9600); //esp-01모듈에 따라, 115200으로 해야 동작될 수 있음
  ESP_wifi.setTimeout(5000); 
  delay(1000); 
} 
void loop() { 
if (Serial.available()){
  ESP_wifi.write(Serial.read()); 
} 
if (ESP_wifi.available()) { 
  Serial.write(ESP_wifi.read()); 
  }
}