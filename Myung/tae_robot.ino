#include <SPI.h>
#include <WiFiNINA.h>
#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


char ssid[] = "test";       // your network SSID (name)
char pass[] = "12345678";   // your network password (use for WPA, or use as key for WEP)


int status = WL_IDLE_STATUS;

char server[] = "192.168.4.1";    // name address for Google (using DNS)

WiFiClient client;

//객체 및 핀 번호 변수 선언
//Servo servothumb;
//Servo servoindex;
//Servo servomiddle;
//Servo servoringfinger;
//Servo servopinky;

int flex_1_val;
int flex_2_val;
int flex_3_val;
int flex_4_val;
int flex_5_val;


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
//  servothumb.attach(2);
//  servoindex.attach(3);
//  servomiddle.attach(4);
//  servoringfinger.attach(5);
//  servopinky.attach(6);
//
//  servothumb.write(0);
//  servoindex.write(0);
//  servomiddle.write(0);
//  servoringfinger.write(0);
//  servopinky.write(0);

  pwm.begin();
  pwm.setPWM(1,0,150);  //엄지
  pwm.setPWM(2,0,150);  //검지
  pwm.setPWM(3,0,150);  //중지
  pwm.setPWM(4,0,150);  //약지
  pwm.setPWM(5,0,150);  //소지
  pwm.setPWMFreq(60); //오작동 한다면 50Hz에서 조금씩 바꿔보기 


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
}

void loop() {
  // 서버에서 전송한 데이터가 있다면
  while (client.available()) {
    flex_1_val = client.read();
    flex_1_val = map(flex_1_val,85,35,0,150);
    pwm.setPWM(1,0,flex_1_val);
    
    flex_2_val = client.read();
    flex_2_val = map(flex_2_val,85,35,0,150);
    pwm.setPWM(2,0,flex_2_val);

    flex_3_val = client.read();
    flex_3_val = map(flex_3_val,85,35,0,150);
    pwm.setPWM(3,0,flex_3_val);
    
    flex_4_val = client.read();
    flex_4_val = map(flex_4_val,85,35,0,150);
    pwm.setPWM(4,0,flex_4_val);

    flex_5_val = client.read();
    flex_5_val = map(flex_5_val,85,35,0,150);
    pwm.setPWM(5,0,flex_5_val);
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
