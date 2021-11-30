#include <ArduinoBLE.h>
int flex_1 = A1;
int flex_2 = A2;
int flex_3 = A3;
int flex_4 = A4;
int flex_5 = A5;

int flex_1_val;
int flex_2_val;
int flex_3_val;
int flex_4_val;
int flex_5_val;

void setup() {
  Serial.begin(9600);

  while (!Serial);
  
  // initialize the BLE hardware
  if (!BLE.begin()) {
    Serial.println("* Starting BLE module failed!");
    while (1);
  }

  // BLE device의 이름을 설정하는 부분.
  BLE.setLocalName("Nano 33 BLE (Central)"); 
  BLE.advertise();
  
  Serial.println("Arduino Nano 33 BLE Sense (Central Device)");
  Serial.println(" ");
 
// client는 해당 UUID를 service로 advertising하고 있는 BLE device를 찾게된다. 상대의 UUID를 알고있다면 이런 식으로 연결할 수도 있다.
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

void loop() {
  // 인근에 advertising 중인 peripheral device를 발견했는지 확인해준다.
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();
    
    if (peripheral.localName() != "Arduino Nano 33 BLE (Peripheral)") {
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // 연결 실패 시 재연결 시도
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  BLECharacteristic flexCharacteristic1 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic flexCharacteristic2 = peripheral.characteristic("19b10002-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic flexCharacteristic3 = peripheral.characteristic("19b10003-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic flexCharacteristic4 = peripheral.characteristic("19b10004-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic flexCharacteristic5 = peripheral.characteristic("19b10005-e8f2-537e-4f6c-d104768a1214");


  
  while (peripheral.connected()) {
    flex_1_val = analogRead(flex_1);
    flex_2_val = analogRead(flex_2);
    flex_3_val = analogRead(flex_3);
    flex_4_val = analogRead(flex_4);
    flex_5_val = analogRead(flex_5);
    
    // BLE 통신에서 넘겨주는 데이터 타입은 uint8_t이다. 정수나 실수를 그대로 넘겨줄 수 없기 때문에 받는 쪽에서는 적절한 캐스팅을 해줘야한다.
    flexCharacteristic1.writeValue((byte)flex_1_val);
    flexCharacteristic2.writeValue((byte)flex_2_val);
    flexCharacteristic3.writeValue((byte)flex_3_val);
    flexCharacteristic4.writeValue((byte)flex_4_val);
    flexCharacteristic5.writeValue((byte)flex_5_val);

    }
}
