#include <ArduinoBLE.h>

BLEService Service("19b10000-e8f2-537e-4f6c-d104768a1214");

//BLEUnsignedCharCharacteristic FlexChar("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead|BLEWrite|BLENotify);
BLEStringCharacteristic FlexChar("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead|BLEWrite|BLENotify, 50);

byte read_data = 0;
String data = "";
String data2 = "";
int ch = 0;

String fin1 = "";
String fin2 = "";
String fin3 = "";
String fin4 = "";
String fin5 = "";

int flex_1 = A0;
int flex_2 = A1;
int flex_3 = A2;
int flex_4 = A3;
int flex_5 = A4;

int flex_1_val;
int flex_2_val;
int flex_3_val;
int flex_4_val;
int flex_5_val;

int sum=0;
int old=0;
int count = 0;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  if(!BLE.begin()){
    Serial.println("start ble fail");
    while(1);
  }

  BLE.setLocalName("myung nano 33 ble");
  BLE.setAdvertisedService(Service);
  Service.addCharacteristic(FlexChar);
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
      flex_1_val = analogRead(flex_1); 
      flex_1_val = map(flex_1_val, 0, 800, 10, 99);
      
      flex_2_val = analogRead(flex_2);
      flex_2_val = map(flex_2_val, 0, 800, 10, 99);
      
      flex_3_val = analogRead(flex_3);
      flex_3_val = map(flex_3_val, 0, 800, 10, 99);
      
      flex_4_val = analogRead(flex_4);
      flex_4_val = map(flex_4_val, 0, 800, 10, 99);
      
      flex_5_val = analogRead(flex_5);
      flex_5_val = map(flex_5_val, 0, 800, 10, 99);

      fin1 = "1"+(String)flex_1_val;
      fin2 = "2"+(String)flex_2_val;
      fin3 = "3"+(String)flex_3_val;
      fin4 = "4"+(String)flex_4_val;
      fin5 = "5"+(String)flex_5_val;

      data = fin1+" "+fin2+" "+fin3+" "+fin4+" "+fin5;
      sum = flex_1_val + flex_2_val + flex_3_val + flex_4_val + flex_5_val;

      if(abs(sum - old)>2){
        FlexChar.writeValue(data);
        Serial.println(data);        
        old = sum;
        delay(10);
      }
      else{
        old = sum;
        count = count+1;
        if(count>15){
          FlexChar.writeValue(data);
          Serial.println(data);
          count = 0;
        }
        delay(100);
      }
     }
  }

  else{
    Serial.print(F("disconn from central"));
    Serial.println(central.address());
  }
}
