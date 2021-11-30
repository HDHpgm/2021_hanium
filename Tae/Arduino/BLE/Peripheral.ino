#include <ArduinoBLE.h>

BLEService Service("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE Service

// BLE Characteristic - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic FlexCharacteristic1("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);


void setup() {
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // 디바이스 이름 설정
  BLE.setLocalName("Arduino Nano 33 BLE (Peripheral)");

  // service를 광고할 service 목록에 추가한다.
  BLE.setAdvertisedService(Service);

  // 사전에 정의한 characteristic을 service에 추가한다.
  Service.addCharacteristic(FlexCharacteristic);
  
  // 이제 server를 BLE device에 등록한다.
  BLE.addService(Service);
  
  // start advertising
  BLE.advertise();

  Serial.println("Nano 33 BLE (Peripheral Device)");
  Serial.println(" ");
}

void loop() {
  // 연결 대기
  BLEDevice central = BLE.central();
  Serial.println("- Discovering central device...");
  delay(500);
  
  // 성공적으로 연결되었다면
  if (central) {
    Serial.print("Connected to central: ");
    // 연결된 central 디바이스의 MAC 주소를 출력
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
        if (FlexCharacteristic.value()) {   // 전송된 값이 있다면
          Serial.println(FlexCharacteristic.value());  // 값을 출력
        }
     }

    // 연결 실패 시 출력
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
