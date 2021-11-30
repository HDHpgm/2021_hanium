#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
//#include <MPU9250.h> 자이로 센서
//
//MPU9250 mpu;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//(0~4096까지 가능하나, 150일때 서보모터의 각도가 0도이고, 600일 때 서보모터의 각도가 180도임.)
#define SERVOMIN  150 // 서보모터 최소 출력 값 = 0도
#define SERVOMAX  600 // 서보모터 최대 출력 값 = 180도 

char ssid[] = "";        // your network SSID (name)
char pass[] = "";    // your network password (use for WPA, or use as key for WEP)


int status = WL_IDLE_STATUS;

char server[] = "192.168.4.1";  

WiFiClient client;

int flex_1_val;
int flex_2_val;
int flex_3_val;
int flex_4_val;
int flex_5_val;
//int elbow;
//int shoulder;


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //Wire.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(5000);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    client.println();
  }

  pwm.begin();
  pwm.setPWM(1,0,150);  //엄지
  pwm.setPWM(2,0,150);  //검지
  pwm.setPWM(3,0,150);  //중지
  pwm.setPWM(4,0,150);  //약지
  pwm.setPWM(5,0,150);  //소지
  pwm.setPWMFreq(60); //오작동 한다면 50Hz에서 조금씩 바꿔보기 

//  if (!mpu.setup(0x68)) {  // change to your own address
//        while (1) {
//            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
//            delay(5000);
//        }
//    }
}

void loop() {
  // 서버에서 전송한 데이터가 있다면
  while (client.available()) {
    flex_1_val = analogRead(flex_1);
    flex_1_val = constrain(map(flex_1_val, 85, 35, 150, 450), 150, 450); //받은 값의 범위 0~180을 펄스길이150~600으로 매핑해주고, ra의 최소,최대를 150,600을 넘지 않게 해준다.
    pwm.setPWM(1,0,flex_1_val); //pca9685모듈의 1번 포트에 연결된 서보를 flex_1_val 회전
    
    flex_2_val = analogRead(flex_2); 
    flex_2_val = constrain(map(flex_2_val, 85, 35, 150, 450), 150, 450); //받은 값의 범위 0~180을 펄스길이150~600으로 매핑해주고, ra의 최소,최대를 150,600을 넘지 않게 해준다.
    pwm.setPWM(2,0,flex_2_val); //pca9685모듈의 2번 포트에 연결된 서보를 flex_2_val만큼 회전
    
    flex_3_val = analogRead(flex_3); 
    flex_3_val = constrain(map(flex_3_val, 85, 35, 150, 450), 150, 450); //받은 값의 범위 0~180을 펄스길이150~600으로 매핑해주고, ra의 최소,최대를 150,600을 넘지 않게 해준다.
    pwm.setPWM(3,0,flex_3_val); //pca9685모듈의 3번 포트에 연결된 서보를 flex_3_val 회전
    
    flex_4_val = analogRead(flex_4); 
    flex_4_val = constrain(map(flex_4_val, 85, 35, 150, 450), 150, 450); //받은 값의 범위 0~180을 펄스길이150~600으로 매핑해주고, ra의 최소,최대를 150,600을 넘지 않게 해준다.
    pwm.setPWM(4,0,flex_4_val); //pca9685모듈의 4번 포트에 연결된 서보를 flex_4_val만큼 회전
    
    flex_5_val = analogRead(flex_5); 
    flex_5_val = constrain(map(flex_5_val, 85, 35, 150, 450), 150, 450); //받은 값의 범위 0~180을 펄스길이150~600으로 매핑해주고, ra의 최소,최대를 150,600을 넘지 않게 해준다.
    pwm.setPWM(5,0,flex_5_val); //pca9685모듈의 5번 포트에 연결된 서보를 flex_5_val만큼 회전

//    elbow = constrain(map((int)mpu.getPitch(), 0, 90, 596, 1369), 596, 1369);
//    pwm.setPWM(5,0,flex_5_val); //pca9685모듈의 5번 포트에 연결된 서보를 elbow만큼 회전
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
