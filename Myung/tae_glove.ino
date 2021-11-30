#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

char ap_ssid[] = "test";        // AP모드의 SSID
char ap_pass[] = "12345678";        // AP모드의 PASS
char st_ssid[] = "myung";        // ST모드의 SSID
char st_pass[] = "31963196";        // ST모드의 PASS
const char* mqtt_server = "192.168.0.8"; // MQTT Broker 주소
 
int status = WL_IDLE_STATUS;

WiFiServer server(80);
WiFiClient n_client;
PubSubClient client(n_client);

int flex_1 = A1;    // 엄지
int flex_2 = A2;    // 검지
int flex_3 = A3;    // 중지
int flex_4 = A4;    // 약지
int flex_5 = A5;    // 소지

int flex_1_val = 1;
int flex_2_val = 2;
int flex_3_val = 3;
int flex_4_val = 4;
int flex_5_val = 5;

int flex[] = {A1,A2,A3,A4,A5};
float flex_val[] = {1,2,3,4,5};
float lock_val[] = {0,0,0,0,0};
int new_val[] = {0,0,0,0,0};
float sensitivity = 0.25;

void setup() {
  Serial.begin(9600);

  lock_val[0] = analogRead(flex[0]);
  lock_val[1] = analogRead(flex[1]);
  lock_val[2] = analogRead(flex[2]);
  lock_val[3] = analogRead(flex[3]);
  lock_val[4] = analogRead(flex[4]);

  while (!Serial) {;}
                  
  if (WiFi.status() == WL_NO_MODULE) {     //와이파이 모듈 체크
    Serial.println("Communication with WiFi module failed!");  
    while (true);
  }

  String fv = WiFi.firmwareVersion();      //버전 체크
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  
  // 와이파이 연결시도(연결이 되어 있지 않을때만 실행)
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(st_ssid);
    
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(st_ssid, st_pass);
    
    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();

  client.setServer(mqtt_server, 1883);
  reconnect();
  client.setCallback(callback);
}

void loop() { 
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg +=(char)payload[i];
  }
  Serial.print(msg);
  Serial.println();
  if (msg == "1"){
    switch2WiFiMode(); 
    wifiAPMode();
    client.disconnect();
  }
}

void wifiAPMode()
{
  status = WiFi.beginAP(ap_ssid, ap_pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }
  
  while (true) {
    if (status != WiFi.status()) {
      // it has changed update the variable
      status = WiFi.status();
  
      if (status == WL_AP_CONNECTED) {
        // a device has connected to the AP
        Serial.println("Device connected to AP");
        break;
      }
    }
  }
  server.begin();
  sendToData();
}

void sendToData(){
  while(true){
    n_client = server.available();   // 로봇 팔 Client의 접속을 기다림
    
    if (n_client) {                             // 로봇 팔측 Client 접속했다면
      Serial.println("new client");           // 연결되었다고 시리얼 모니터에 띄움
      while (n_client.connected()) {            // 로봇 팔에 데이터를 전송
       
      for(int i=0; i<sizeof(flex_val)/sizeof(flex_val[0]); i++){
        flex_val[i] = analogRead(flex[i]);
        new_val[i] = int(lock_val[i] * (1 - sensitivity) + flex_val[i] * sensitivity);
        //로우패스 필터 (고정값을 계속 0.9배하고 새로 감지한값의 10%만 변수에 더해서 나중에는 10%의 값만 이용해서 정확도를 높이는 방법)
      }
    
      new_val[0] = map(new_val[0],16,35,0,100);
      new_val[1] = map(new_val[1],22,40,0,100);
      new_val[2] = map(new_val[2],26,48,0,100);
      new_val[3] = map(new_val[3],92,110,0,100);
      new_val[4] = map(new_val[4],87,110,0,100);
    
      for(int j=0; j<5; j++){
        Serial.print(new_val[j]);
        Serial.print(" ");
      }
      Serial.println("");
      byte b_val[5] = {byte(new_val[0]), byte(new_val[1]), byte(new_val[2]), byte(new_val[3]), byte(new_val[4])};
      n_client.write(b_val,5);
  
      }
    }
  }
}


bool switch2WiFiMode()
{
  status = WL_IDLE_STATUS;

  // WiFi 드라이버 다시 초기화
  wiFiDrv.wifiDriverDeinit();
  wiFiDrv.wifiDriverInit();
  
  return true;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ClientID")) { //change to ClientID
      Serial.println("connected");

      // Once connected, publish an announcement...
      client.publish("connect", "test msg");
       
      // ... and resubscribe
      client.subscribe("send");
       
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
    }
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
