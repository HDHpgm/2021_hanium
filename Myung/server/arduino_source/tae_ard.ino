#include <Servo.h>
#include <ArduinoBLE.h>

BLEService Service("19c10000-e8f2-537e-4f6c-d104768a1214");
//BLEUnsignedCharCharacteristic FlexChar("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead|BLEWrite|BLENotify);
BLEStringCharacteristic FlexChar("19c10001-e8f2-537e-4f6c-d104768a1214", BLERead|BLEWrite|BLENotify, 20);

Servo servo;

int servopin = 5;


byte read_data = 0;
String data = "mss daa ccc ddd";
String data2 = "";
int ch = 0;

int fin1, fin2, fin3, fin4, fin5;
int old_sum = 0;


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
    servo.write(0);
    delay(1000);
    servo.write(100);
    delay(1000);
    while(central.connected()){
        String charValue = FlexChar.value();
        fin1 = charValue.substring(0,2).toInt();
        fin2 = charValue.substring(3,5).toInt();
        fin3 = charValue.substring(6,8).toInt();
        fin4 = charValue.substring(9,11).toInt();
        fin5 = charValue.substring(12,14).toInt();

        int sum_data = fin1 + fin2 + fin3 + fin4 + fin5;

        if(abs(sum_data - old_sum)>2){
          Serial.print("엄지:");
          Serial.print(fin1);
          Serial.print(" ");
    
          Serial.print("검지:");
          Serial.print(fin2);
          Serial.print(" ");
    
          Serial.print("중지:");
          Serial.print(fin3);
          Serial.print(" ");
    
          Serial.print("약지:");
          Serial.print(fin4);
          Serial.print(" ");
    
          Serial.print("새끼:");
          Serial.print(fin5);
          Serial.println(" ");          

          old_sum = sum_data;
          servo.write(fin2);
          delay(10);
        }
        else{
          old_sum = sum_data;
          delay(100);
        }
    }
  }
  else{
    Serial.print(F("disconn from central"));
    Serial.println(central.address());
  }
}
