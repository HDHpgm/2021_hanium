#include <Servo.h>
#include <ArduinoBLE.h>

BLEService Service("19c10000-e8f2-537e-4f6c-d104768a1214");
//BLEUnsignedCharCharacteristic FlexChar("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead|BLEWrite|BLENotify);
BLEStringCharacteristic char1("19c10001-e8f2-537e-4f6c-d104768a1111", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic char2("19c10001-e8f2-537e-4f6c-d104768a2222", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic char3("19c10001-e8f2-537e-4f6c-d104768a3333", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic char4("19c10001-e8f2-537e-4f6c-d104768a4444", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic char5("19c10001-e8f2-537e-4f6c-d104768a5555", BLERead|BLEWrite|BLENotify, 10);

Servo servo;

int servopin = 5;
String data = "";


int fin1, fin2, fin3, fin4, fin5;

int old1 = 0;
int old2 = 0;
int old3 = 0;
int old4 = 0;
int old5 = 0;

unsigned long prev1 = 0;
unsigned long prev2 = 0;
unsigned long prev3 = 0;
unsigned long prev4 = 0;
unsigned long prev5 = 0;

int delay_time = 0;

void setup() {
  Serial.begin(115200);
  servo.attach(5);
  servo.write(150);
  while(!Serial);

  if(!BLE.begin()){
    Serial.println("start ble fail");
    while(1);
  }

  BLE.setLocalName("tae nano 33 ble");
  BLE.setAdvertisedService(Service);
  Service.addCharacteristic(char1);
  Service.addCharacteristic(char2);
  Service.addCharacteristic(char3);
  Service.addCharacteristic(char4);
  Service.addCharacteristic(char5);
  BLE.addService(Service);

  BLE.advertise();
  Serial.println("nano33 ble peripheral");
  Serial.println(BLE.address());
  
}

void loop(){
  BLEDevice central = BLE.central();
  Serial.println("finding central device");
  delay(100);

  if(central){
    Serial.println("conn to central : ");
    Serial.println(central.address()); //MAC

    while(central.connected()){
        unsigned long current = millis();

        if(current - prev1 >= delay_time){
          fin1 = char1.value().toInt();
          prev1 = current;
          if(abs(fin1 - old1)>0){
            Serial.println(fin1);
            old1 = fin1;  
          }
        }

        if(current - prev2 >= delay_time+1){
          fin2 = char2.value().toInt();
          prev2 = current;  
          if(abs(fin2 - old2)>0){
            //Serial.println(fin2);
            old2 = fin2;  
          }
        }

        if(current - prev3 >= delay_time+2){
          fin3 = char3.value().toInt();
          prev3 = current; 
          if(abs(fin3 - old3)>0){
            //Serial.println(fin3);
            old3 = fin3;  
          }
        }
        
        if(current - prev4 >= delay_time+3){
          fin4 = char4.value().toInt();
          prev4 = current; 
          if(abs(fin4 - old4)>0){
            //Serial.println(fin4);
            old4 = fin4;  
          } 
        }

        if(current - prev5 >= delay_time+4){
          fin5 = char5.value().toInt();
          prev5 = current;  
          if(abs(fin5 - old5)>0){
            //Serial.println(fin5);
            old5 = fin5;  
          }
        }
        

//        Serial.print(fin1);
//        Serial.print(" ");
//        Serial.print(fin2);
//        Serial.print(" ");
//        Serial.print(fin3);
//        Serial.print(" ");
//        Serial.print(fin4);
//        Serial.print(" ");
//        Serial.println(fin5);
        delay(100);
    }
  }
  else{
    Serial.print(F("disconn from central"));
    Serial.println(central.address());
  }
}
