#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h" 
///////arduino_secrets.h 파일에 정보를 입력

char ap_ssid[] = AP_SECRET_SSID;        // AP모드의 SSID
char ap_pass[] = AP_SECRET_PASS;        // AP모드의 PASS
char st_ssid[] = ST_SECRET_SSID;        // ST모드의 SSID
char st_pass[] = ST_SECRET_PASS;        // ST모드의 PASS

int status = WL_IDLE_STATUS;
bool networkInitialized = false;    // WiFi_AP - WiFi_ST 스위칭을 위한 변수
bool wifiModeFlag = false;           // WiFi_AP 연결시 true WiFi_ST 연결시 false

WiFiServer server(80);
WiFiClient client;

char server_add[] = "192.168.4.1";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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
      wifiSTMode();
    }
    else
    {
      wifiAPMode();
    }
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
    change_ST();
    
}

void wifiSTMode()
{

// attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(st_ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(st_ssid, st_pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();
  change_AP();
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

void change_ST() {  
  // AP_MODE 연결 후 5초가 지나면 자동으로 BLE 모드로 전환
  for(int i = 0; i <= 5; i++){
    delay(1000);
    if( i == 5){
      networkInitialized = false;
      wifiModeFlag = false;
    }
  }
  Serial.println();
}
void change_AP() {  
  // ST_MODE 연결 후 5초가 지나면 자동으로 BLE 모드로 전환
  for(int i = 0; i <= 5; i++){
    delay(1000);
    if( i == 5){
      networkInitialized = false;
      wifiModeFlag = true;
    }
  }
  Serial.println();
}
