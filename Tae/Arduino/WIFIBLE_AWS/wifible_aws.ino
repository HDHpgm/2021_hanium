#include <ArduinoBLE.h>
#include <SPI.h>
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "utility/wifi_drv.h"
#include "arduino_secrets.h"


#define BLE_UUID_NETWORK_CONFIG_SERVICE              "343D2964-5ECF-2297-4463-609011571F24"
#define BLE_UUID_NETWORK_ENABLE_CHARACTERISTIC       "767B22E7-EA6C-B017-286A-55B68310FD9D"

BLEService networkConfigService( BLE_UUID_NETWORK_CONFIG_SERVICE );
BLEBoolCharacteristic networkEnableCharacteristic( BLE_UUID_NETWORK_ENABLE_CHARACTERISTIC , BLERead | BLEWrite );

///////WIFI 이름과 패스워드 AWS에 접속하기 위한 엔드포인트와 인증서 arduino_secrets.h에 저장
char ssidName[] = SECRET_SSID;
char ssidPass[] = SECRET_PASS;
const char broker[]      = SECRET_BROKER;
const char* certificate  = SECRET_CERTIFICATE;

WiFiClient    wifiClient;            // Used for the TCP socket connection
BearSSLClient sslClient(wifiClient); // Used for SSL/TLS connection, integrates with ECC508
MqttClient    mqttClient(sslClient);

unsigned long lastMillis = 0;

int status = WL_IDLE_STATUS;
bool networkInitialized = false;    // Wifi - Ble 스위칭을 위한 변수
bool wifiModeFlag = true;           // Wifi 연결시 true Ble 연결시 false

void setup()
{
  Serial.begin( 9600 );
  while ( !Serial );

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
}

void loop()
{
  if( !networkInitialized )
  {
    if( !wifiModeFlag )
    {
      Serial.print( "Switch to BLE: " );
      if( !switch2BleMode() )
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
      Serial.print( "Switch to WiFi: " );
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
      bleMode();
    }
    else
    {
      if (WiFi.status() != WL_CONNECTED) {
        wifiMode();
      }
      
      if (!mqttClient.connected()) {
        // MQTT client is disconnected, connect
        connectMQTT();
      }

      // poll for new MQTT messages and send keep alives
      mqttClient.poll();
    }
  }
}

unsigned long getTime() {
  // get the current time from the WiFi module
  return WiFi.getTime();
}

void bleMode()
{
  BLEDevice central = BLE.central();

  if ( central )
  {
    Serial.print( "Connected to central: " );
    Serial.println( central.address() );

    while ( central.connected() )
    {
      if( networkEnableCharacteristic.written() )
      {
        networkInitialized = false;
        wifiModeFlag = true;
        return;
      }
    }
  }
}


void wifiMode()
{
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssidName);
  Serial.print(" ");

  while (WiFi.begin(ssidName, ssidPass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the network");
  Serial.println();
}

bool switch2BleMode()
{
  if ( !BLE.begin() )
  {
    return false;
  }

  // set advertised local name and service UUID:
  BLE.setDeviceName( "Arduino Nano 33 IoT" );
  BLE.setLocalName( "Arduino Nano 33 IoT" );
  BLE.setAdvertisedService( networkConfigService );

  // add the characteristic to the service
  networkConfigService.addCharacteristic( networkEnableCharacteristic );

  // add service
  BLE.addService( networkConfigService );

  // set the initial value for the characeristic:
  networkEnableCharacteristic.writeValue( false );

  BLE.advertise();

  return true;
}


bool switch2WiFiMode()
{

  status = WL_IDLE_STATUS;

  // WiFi 드라이버 다시 초기화
  wiFiDrv.wifiDriverDeinit();
  wiFiDrv.wifiDriverInit();

  return true;
}

void printWiFiStatus()
{
  Serial.print( "SSID: " );
  Serial.println( WiFi.SSID() );

  IPAddress ip = WiFi.localIP();
  Serial.print( "IP address: " );
  Serial.println( ip );

  long rssi = WiFi.RSSI();
  Serial.print( "Signal strength (RSSI):" );
  Serial.print( rssi );
  Serial.println( " dBm" );
}

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(broker);
  Serial.println(" ");

  while (!mqttClient.connect(broker, 8883)) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe("arduino/incoming");
}


void onMessageReceived(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
    if(messageSize == 10){
        networkInitialized = false;
        wifiModeFlag = false;
    }
  }
  Serial.println();
}
