#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

#include <WizFi250.h>

char ap_ssid[] = "test";        // AP모드의 SSID
char ap_pass[] = "12345678";        // AP모드의 PASS
char st_ssid[] = "Myung_2.4G";        // ST모드의 SSID
char st_pass[] = "dlaudgus1!";        // ST모드의 PASS
const char* mqtt_server = "broker.mqtt-dashboard.com"; // MQTT Broker 주소, 오픈 borker 주소.
 
int status = WL_IDLE_STATUS;

WiFiServer server(80);
WiFiClient n_client;
PubSubClient client(n_client);

String msg = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {;}
                  
  if (WiFi.status() == WL_NO_MODULE) {     
    Serial.println("Communication with WiFi module failed!");  
    while (true);
  } //와이파이 모듈 체크

  String fv = WiFi.firmwareVersion();     
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  } //버전 체크
  
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(st_ssid);

    status = WiFi.begin(st_ssid, st_pass);
    
    delay(1000);
  } // 와이파이 연결시도(연결이 되어 있지 않을때만 실행)

  Serial.println("공유기와 와이파이 연결은 잘 됩니다.");
  printWifiStatus();


  client.setServer(mqtt_server, 1883); // MQTT 서버에 연결합니다.
  client.setCallback(callback);
  switch2WiFiMode(); 
  wifiAPMode();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
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


bool switch2WiFiMode()
{
  status = WL_IDLE_STATUS;

  // WiFi 드라이버 다시 초기화
  wiFiDrv.wifiDriverDeinit();
  wiFiDrv.wifiDriverInit();
  
  return true;
}



void sendToData(){
  while(true){
    n_client = server.available();   // 로봇 팔 Client의 접속을 기다림
    
    if (n_client) {                             // 로봇 팔측 Client 접속했다면
      Serial.println("new client");           // 연결되었다고 시리얼 모니터에 띄움
      while (n_client.connected()) {            // 로봇 팔에 데이터를 전송
      
      byte b_val[5] = {1, 2, 3, 4, 5};
      n_client.write(b_val,5);
  
      }
    }
  }
}




void callback(char* topic, byte* payload, unsigned int length) {
  // Topic에 메시지가 도착하면 실행되는 콜백입니다.
  Serial.print(topic);
  Serial.print(" ");
  for (int i = 0; i < length; i++) {
    msg +=(char)payload[i];
  }
  Serial.print(msg);
  Serial.println();

  
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
      client.subscribe("inTopic"); // inTopic 토픽을 듣습니다.
    } else {
      delay(5000);
    }
  }
}
