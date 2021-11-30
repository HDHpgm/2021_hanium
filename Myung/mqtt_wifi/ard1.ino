#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "myESP01";       // your network SSID (name)
char pass[] = "12345678";   // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
char server[] = "192.168.4.1";    // name address for Google (using DNS)
WiFiClient client;

byte flex_1_val, flex_2_val, flex_3_val, flex_4_val, flex_5_val;

void setup() {
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

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

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

void loop() {
  // 서버에서 전송한 데이터가 있다면
  while (client.available()) {
    flex_1_val = client.read();
    
    flex_2_val = client.read();

    flex_3_val = client.read();
    
    flex_4_val = client.read();

    flex_5_val = client.read();

    Serial.print(flex_1_val);
    Serial.print(" ");
    Serial.print(flex_2_val);
    Serial.print(" ");
    Serial.print(flex_3_val);
    Serial.print(" ");
    Serial.print(flex_4_val);
    Serial.print(" ");
    Serial.print(flex_5_val);
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
