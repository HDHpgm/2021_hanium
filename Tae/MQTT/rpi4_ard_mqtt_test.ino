#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

const char* ssid = "myung";        // WiFi의 SSID
const char* pass = "31963196";     // WiFi의 비밀번호
const char* mqtt_server = "192.168.0.8"; // MQTT Broker 주소
 
int status = WL_IDLE_STATUS;

WiFiClient n_client;
PubSubClient client(n_client);

void setup() {
  Serial.begin(9600);
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
    Serial.println(ssid);
    
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    
    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() { 
  if (!client.connected()) {
    reconnect();
  }
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
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ClientID")) { //change to ClientID
      Serial.println("connected");

      // Once connected, publish an announcement...
      client.publish("connect", "connect");
       
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
