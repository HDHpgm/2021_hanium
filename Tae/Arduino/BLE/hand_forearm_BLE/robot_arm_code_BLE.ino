#include <Servo.h>
#include <ArduinoBLE.h>

//객체 및 핀 번호 변수 선언
Servo servothumb;
Servo servoindex;
Servo servomiddle;
Servo servoringfinger;
Servo servopinky;

int flex_1_val;
int flex_2_val;
int flex_3_val;
int flex_4_val;
int flex_5_val;

BLEService Service("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE Service 설정


BLEIntCharacteristic FlexCharacteristic1("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic FlexCharacteristic2("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic FlexCharacteristic3("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic FlexCharacteristic4("19B10004-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic FlexCharacteristic5("19B10005-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

void setup() {
  Serial.begin(9600);
  
  //서보모터 핀 및 초기 각도 설정
  servothumb.attach(2);
  servoindex.attach(3);
  servomiddle.attach(4);
  servoringfinger.attach(5);
  servopinky.attach(6);

  servothumb.write(0);
  servoindex.write(0);
  servomiddle.write(0);
  servoringfinger.write(0);
  servopinky.write(0);

  while (!Serial);

  // 초기화
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // 디바이스 이름 설정
  BLE.setLocalName("Arduino Nano 33 BLE (Peripheral)");

  // service를 광고할 service 목록에 추가한다.
  BLE.setAdvertisedService(Service);

  // 사전에 정의한 characteristic을 service에 추가한다.
  Service.addCharacteristic(FlexCharacteristic1);
  Service.addCharacteristic(FlexCharacteristic2);
  Service.addCharacteristic(FlexCharacteristic3);
  Service.addCharacteristic(FlexCharacteristic4);
  Service.addCharacteristic(FlexCharacteristic5);
  
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

    while (central.connected()) {
        if (FlexCharacteristic1.value() && FlexCharacteristic2.value() && FlexCharacteristic3.value() &&
            FlexCharacteristic4.value() && FlexCharacteristic5.value()) {   // 넘어온 값이 있는 지 확인
          flex_1_val = FlexCharacteristic1.value();
          flex_1_val = map(flex_1_val,85,35,0,150);
          servothumb.write(flex_1_val);

          flex_2_val = FlexCharacteristic2.value();
          flex_2_val = map(flex_2_val,85,35,0,150);
          servoindex.write(flex_2_val);

          flex_3_val = FlexCharacteristic3.value();
          flex_3_val = map(flex_3_val,85,35,0,150);
          servomiddle.write(flex_3_val);

          flex_4_val = FlexCharacteristic4.value();
          flex_4_val = map(flex_4_val,85,35,0,150);
          servoringfinger.write(flex_4_val);

          flex_5_val = FlexCharacteristic5.value();
          flex_5_val = map(flex_5_val,85,35,0,150);
          servopinky.write(flex_5_val);

        }
     }

    // 연결 실패 시 출력
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
