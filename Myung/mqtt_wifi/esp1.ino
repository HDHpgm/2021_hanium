#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Set WiFi credentials
#define WIFI_SSID "Myung_2.4G"
#define WIFI_PASS "dlaudgus1!"
 
// Set AP credentials
#define AP_SSID "myESP01"
#define AP_PASS "12345678"

WiFiServer server(80);
WiFiClient ard_client[2];

WiFiClient mqtt_client;
PubSubClient client(mqtt_client);
const char* mqtt_server = "broker.mqtt-dashboard.com";

long lastMsg = 0;
char msg[50];
int count = 0;


void setup()
{
  Serial.begin(9600);
  Serial.println();
  delay(10);
  
  wifi_setup();
  client.setServer(mqtt_server, 1883); // MQTT 서버에 연결합니다. 
  client.setCallback(callback);
  //send_mqtt();
  
  delay(300);

  send_controller();
  
}


void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void send_mqtt(){
  while(true){
    snprintf (msg, 50, "7 8 9"); // 메시지는 배열로 전송되기 때문에 배열을 생성합니다.
    client.publish("myung_esp_topic", msg); // inTopic 토픽으로 메시지를 전송합니다.
    delay(300); // 메시지가 전송될 때 까지 기다립니다.
  }
}


void send_controller(){
  
  while(true){
    ard_client[0] = server.available();   // 컨트롤러 Client의 접속을 기다림
    if (ard_client[0]) {                             // 로봇 팔측 Client 접속했다면
      Serial.println(ard_client[0]);           // 연결되었다고 시리얼 모니터에 띄움
      delay(5000);
      while (ard_client[0].connected()) {            // 로봇 팔에 데이터를 전송
        byte b_val[5] = {1, 2, 3, 4, 5};
        ard_client[0].write(b_val,5);
        delay(200);
      }
    }
  }
}


//void send_robot(){
//  while(true){
//    robot_client = robot_server.available();   // 로봇팔 Client의 접속을 기다림
//    if (robot_client) {                             // 로봇 팔측 Client 접속했다면
//      Serial.println("로봇팔 접속 성공");           // 연결되었다고 시리얼 모니터에 띄움
//      while (robot_client.connected()) {            // 로봇 팔에 데이터를 전송
//        byte b_val[5] = {6, 7, 8, 9, 10};
//        robot_client.write(b_val,5);
//      }
//    }
//  }
//}



void callback(char* topic, byte* payload, unsigned int length) {
  String aaa="";
  Serial.print(topic);
  Serial.print(" ");
  for (int i = 0; i < length; i++) {
    Serial.print((String)payload[i]);
  }
  Serial.println();

}


void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP8266Client-"; // 클라이언트 ID를 설정합니다.
    clientId += String(random(0xffff), HEX); // 같은 이름을 가진 클라이언트가 발생하는것을 방지하기 위해, 렌덤 문자를 클라이언트 ID에 붙입니다.
    if (client.connect(clientId.c_str())) { // 앞서 설정한 클라이언트 ID로 연결합니다.
      client.subscribe("myung_esp_topic");
      client.subscribe("myung_esp_topic2");
    } else {
      delay(5000);
    }
  }
}



void wifi_setup() {
  // Setup serial port
  // Begin Access Point
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASS);
  //WiFi.mode(WIFI_STA);
  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  // Connecting to WiFi...
  Serial.print("와이파이 연결중 : ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
 
  // Connected to WiFi
  Serial.println();
  Serial.println("공유기 와이파이 연결 성공");
  Serial.print("IP address for network ");
  Serial.print(WIFI_SSID);
  Serial.print(" : ");
  Serial.println(WiFi.localIP());
  Serial.print("IP address for network ");
  Serial.print(AP_SSID);
  Serial.print(" : ");
  Serial.print(WiFi.softAPIP());
  Serial.println();

  Serial.println("AP, STA 모드 사용 시작");
  delay(500);

  server.begin();
}

