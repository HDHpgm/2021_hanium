#include <ArduinoBLE.h>

byte a = 0;
byte b = 100;
unsigned char write_data[] = "hello";

void setup() {
  Serial.begin(9600);
  while(!Serial);

  if(!BLE.begin()){
    Serial.println("BLE START FAIL");
    while(1);
  }

  BLE.setLocalName("Myung Ras BLE");
  BLE.advertise();

  Serial.println("Myung Ras BLE");
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");

}

void loop() {
  BLEDevice peripheral = BLE.available();
  if(peripheral){
    Serial.print("found : ");
    Serial.println(peripheral.address());
    Serial.println(peripheral.localName());
    Serial.println(peripheral.advertisedServiceUuid());

    BLE.stopScan();
    control(peripheral);
  }
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

void control(BLEDevice peripheral){
  Serial.println("conn....");

  if(peripheral.connect()){
    Serial.println("conn success");
  }
  else{
    Serial.println("conn fail");
    return;
  }

  Serial.println("discovering attributes...");
  if(peripheral.discoverAttributes()){
    Serial.println("attributes discover");
  }
  else{
    Serial.println("attributes discover fail");
    peripheral.disconnect();
    return;
  }

  BLECharacteristic blechar = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");

  while(peripheral.connected()){
    for(int i=0; i<10; i++){
      blechar.writeValue(write_data[i]);  
      Serial.print((char)write_data[i]);
    }
    Serial.println("");
    delay(1000);

    
  }
}
