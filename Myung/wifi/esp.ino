#include <ESP8266WiFi.h> // ESP8266에서 Wi-Fi 기능을 사용하기 위한 라이브러리 입니다.
#include <PubSubClient.h>

const char* ssid = "test";
const char* password = "12345678";
const char* mqtt_server = "192.168.0.8";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password); // 앞서 설정한 ssid와 페스워드로 Wi-Fi에 연결합니다.
  while (WiFi.status() != WL_CONNECTED) { // 연결될 때 까지 0.5초 마다 Wi-Fi 연결상태를 확인합니다.
    delay(500);
  }
  randomSeed(micros()); // 렌덤 문자를 위한 렌덤 시드를 설정합니다.
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Topic에 메시지가 도착하면 실행됨
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP8266Client-"; // 클라이언트 ID를 설정합니다.
    clientId += String(random(0xffff), HEX); // 같은 이름을 가진 클라이언트가 발생하는것을 방지하기 위해, 렌덤 문자를 클라이언트 ID에 붙입니다.
    if (client.connect(clientId.c_str())) { // 앞서 설정한 클라이언트 ID로 연결합니다.
      // Publisher이기 때문에 Subscribe를 하지 않습니다.
    } else {
      delay(5000);
    }
  }
}

void setup() {
  setup_wifi();
  client.setServer(mqtt_server, 1883); // MQTT 서버에 연결합니다.
  client.setCallback(callback);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  snprintf (msg, 50, "7 8 9"); // 메시지는 배열로 전송되기 때문에 배열을 생성합니다.
  client.publish("inTopic", msg); // inTopic 토픽으로 메시지를 전송합니다.
  one = 1;
  delay(1000); // 메시지가 전송될 때 까지 기다립니다.
  ESP.deepSleep(0); // 딥슬립 시작 (절전)
}