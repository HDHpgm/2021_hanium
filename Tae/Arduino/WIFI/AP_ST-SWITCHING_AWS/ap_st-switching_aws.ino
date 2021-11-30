#include <SPI.h>
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "utility/wifi_drv.h"
#include "arduino_secrets.h" 


char ap_ssid[] = AP_SECRET_SSID;        // AP모드의 SSID
char ap_pass[] = AP_SECRET_PASS;        // AP모드의 PASS
char st_ssid[] = ST_SECRET_SSID;        // ST모드의 SSID
char st_pass[] = ST_SECRET_PASS;        // ST모드의 PASS
const char broker[]      = SECRET_BROKER;       // AWS 엔드 포인트
const char* certificate  = SECRET_CERTIFICATE;  // AWS와 연결하기 위한 키 인증서


int status = WL_IDLE_STATUS;
bool networkInitialized = false;    // WiFi_AP - WiFi_ST 스위칭을 위한 변수
bool wifiModeFlag = false;           // WiFi_AP 연결시 true WiFi_ST 연결시 false

WiFiServer server(80);
WiFiClient client;
BearSSLClient sslClient(client); // Used for SSL/TLS connection, integrates with ECC508
MqttClient    mqttClient(sslClient);


char server_add[] = "192.168.4.1";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

  // Set a callback to get the current time
  // used to validate the servers certificate
  ArduinoBearSSL.onGetTime(getTime);

  // Set the ECCX08 slot to use for the private key
  // and the accompanying public certificate for it
  sslClient.setEccSlot(3, certificate);

  //aws에서 보낸 메시지를 출력하는 함수
  mqttClient.onMessage(onMessageReceived);
  // check for the WiFi module:
}


void loop() {
 if( !networkInitialized )
  {
    if( !wifiModeFlag )
    {
      Serial.print( "Switch to WiFi_ST: " );
      if( !switch2WiFiMode() )
      {
        Serial.println( "failed" );
      }
      else
      {
        networkInitialized = true;
        Serial.println( "success" );
      }
    }
    else
    {
      Serial.print( "Switch to WiFi_AP: " );
      if( !switch2WiFiMode() )
      {
        Serial.println( "failed" );
      }
      else
      {
        networkInitialized = true;
        Serial.println( "success" );
      }
    }
  }
  else
  {
    if( !wifiModeFlag )
    {
      if (WiFi.status() != WL_CONNECTED) {
        wifiSTMode();
      }
      
      if (!mqttClient.connected()) {
        // MQTT가 연결이 안되었다면 연결 시도
        connectMQTT();
      }

      // poll for new MQTT messages and send keep alives
      mqttClient.poll();
    }
    else
    {
      wifiAPMode();
    }
  }
}

unsigned long getTime() {
  // get the current time from the WiFi module
  return WiFi.getTime();
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
    
}

void wifiSTMode()
{
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(st_ssid);
  Serial.print(" ");

  while (WiFi.begin(st_ssid, st_pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the network");
  Serial.println();
}

bool switch2WiFiMode()
{
  status = WL_IDLE_STATUS;

  // WiFi 드라이버 다시 초기화
  wiFiDrv.wifiDriverDeinit();
  wiFiDrv.wifiDriverInit();
  
  return true;
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

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(broker);
  Serial.println(" ");

  while (!mqttClient.connect(broker, 8883)) {
    // AWS IOT CORE 연결 시도
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

  // 주제 구독
  mqttClient.subscribe("arduino/incoming");
}


void onMessageReceived(int messageSize) {
  // 받은 내용의 길이와 크기 출력
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // AWS에서 전송된 내용을 출력
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
    if(messageSize == 10){
        networkInitialized = false;
        wifiModeFlag = true;
    }
  }
  Serial.println();
}
