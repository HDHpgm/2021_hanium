#include <SPI.h>
#include <WiFiNINA.h>
#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>

char ssid[] = "test";       // your network SSID (name)
char pass[] = "12345678";   // your network password (use for WPA, or use as key for WEP)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int status = WL_IDLE_STATUS;
char server[] = "192.168.4.1";    // name address for Google (using DNS)
WiFiClient client;

byte flex_1_val, flex_2_val, flex_3_val, flex_4_val, flex_5_val, flex_6_val, flex_7_val;
int num1, num2, num3, num4, num5, num6;


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pwm.begin();
  pwm.setPWM(1,0,150);  //엄지
  pwm.setPWM(2,0,150);  //검지
  pwm.setPWM(3,0,150);  //중지
  pwm.setPWM(4,0,150);  //약지
  pwm.setPWM(5,0,150);  //소지
  pwm.setPWM(13,0,160);  //소지
  pwm.setPWMFreq(60); //오작동 한다면 50Hz에서 조금씩 바꿔보기 
600



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

    delay(3000);
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
    num1 = map(flex_1_val, 0, 150, 450, 150);
    
    flex_2_val = client.read();
    num2 = map(flex_2_val, 0, 150, 450, 150);

    flex_3_val = client.read();
    num3 = map(flex_3_val, 0, 150, 450, 150);
    
    flex_4_val = client.read();
    num4 = map(flex_4_val, 0, 150, 450, 150);

    flex_5_val = client.read();
    num5 = map(flex_5_val, 0, 150, 450, 150);

    flex_6_val = client.read();
    num6 = map(flex_6_val, 0, 150, 160, 600);
    
    pwm.setPWM(5,0,num1);
    pwm.setPWM(4,0,num2);
    pwm.setPWM(3,0,num3);
    pwm.setPWM(2,0,num4);
    pwm.setPWM(1,0,num5);
    pwm.setPWM(13,0,num6);

    Serial.print(num1);
    Serial.print(" ");
    Serial.print(num2);
    Serial.print(" ");
    Serial.print(num3);
    Serial.print(" ");
    Serial.print(num4);
    Serial.print(" ");
    Serial.print(num5);
    Serial.print(" ");
    Serial.print(num6);
    Serial.println(" ");


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
