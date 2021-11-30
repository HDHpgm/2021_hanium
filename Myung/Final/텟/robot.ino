#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Arduino_LSM6DS3.h>

char ap_ssid[] = "test";        // AP모드의 SSID
char ap_pass[] = "12345678";        // AP모드의 PASS
char st_ssid[] = "myung";        // ST모드의 SSID
char st_pass[] = "31963196";        // ST모드의 PASS
const char* mqtt_server = "broker.mqtt-dashboard.com"; // MQTT Broker 주소, 오픈 borker 주소.
int status = WL_IDLE_STATUS;
char server[] = "192.168.4.1";    // name address for Google (using DNS)

WiFiServer server(80);
WiFiClient n_client;
PubSubClient client(n_client);

String msg = "";
String dohang_count = "0";
byte b_val[5] = {1, 2, 3, 4, 5};
String ta = "";

int fin[5];
float x, y, z;
float max_x, max_y, max_z;
float min_x, min_y, min_z;


void wifi_set(){
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
    Serial.println(st_ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(st_ssid, st_pass);

    delay(1000);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    client.println();
  }
}

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  while (!Serial) {;}
  delay(1000);

  wifi_set();
  Serial.println("wifi 연결 잘됌");

  client.setServer(mqtt_server, 1883); // MQTT 서버에 연결합니다.
  client.setCallback(callback);
  reconnect();
  Serial.println("mqtt 접속 대기");
  dohang_count="0";
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()) {
    reconnect();
  }
  client.loop();
  if(dohang_count=="1"){
    wifiAPMode();
  }
}

void wifiAPMode()
{
  dohang_count="2";
  Serial.println("AP모드 변경");
  status = WL_IDLE_STATUS;
  // WiFi 드라이버 다시 초기화
  wiFiDrv.wifiDriverDeinit();
  wiFiDrv.wifiDriverInit();
  delay(100);
  status = WiFi.beginAP(ap_ssid, ap_pass);
  Serial.println("컨트롤러 접속 대기");
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
  ReceiveData();
}




void ReceiveData(){
  while(true){
    n_client = server.available();   // 로봇 팔 Client의 접속을 기다림   
    if (n_client) {           
      Serial.println("컨트롤러 접속완료"); // 로봇 팔측 Client 접속했다면      
      while (n_client.connected()) {            // 로봇 팔에 데이터를 전송
        int a = n_client.read();
        int b = n_client.read();
        int c = n_client.read();
        int d = n_client.read();
        int e = n_client.read();
        int f = n_client.read();
        int g = n_client.read();

        Serial.print(a);
        Serial.print(" ");
        
        Serial.print(b);
        Serial.print(" ");
        
        Serial.print(c);
        Serial.print(" ");
        
        Serial.print(d);
        Serial.print(" ");
        
        Serial.print(e);
        Serial.print(" ");
        
        Serial.print(f);
        Serial.print(" ");
        
        Serial.print(g);
        Serial.print(" ");
        
        Serial.println();
      }
    }
  }
}




void callback(char* topic, byte* payload, unsigned int length) {
  // Topic에 메시지가 도착하면 실행되는 콜백입니다.
  msg="";
  Serial.print(topic);
  Serial.print(" ");
  for (int i = 0; i < length; i++) {
    msg +=(char)payload[i];
  }
  Serial.println(msg);
  dohang_count=(String)msg;
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


void reconnect() {
  while (!client.connected()) {
    String clientId = "ArduinoNANO33IoTClinet-"; // 클라이언트 ID를 설정합니다.
    clientId += String(random(0xffff), HEX); // 같은 이름을 가진 클라이언트가 발생하는것을 방지하기 위해, 렌덤 문자를 클라이언트 ID에 붙입니다.
    if (client.connect(clientId.c_str())) { // 앞서 설정한 클라이언트 ID로 연결합니다.
      client.subscribe("dohang_topic2"); // inTopic 토픽을 듣습니다.
    } else {
      delay(5000);
    }
  }
}


void floatToBuff(byte udpBuffer[4], float sensorVal)
{
  byte *sensorValByte = reinterpret_cast<byte*>(&sensorVal);
  memcpy(udpBuffer, sensorValByte, sizeof(sensorValByte));
}
