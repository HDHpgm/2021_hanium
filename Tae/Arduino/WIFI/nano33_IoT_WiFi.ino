#include <SPI.h>
#include <WiFiNINA.h>

const char* ssid = "";        // WiFi의 SSID
const char* pass = "";     // WiFi의 비밀번호
int status = WL_IDLE_STATUS;

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
}

void loop() {     
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
