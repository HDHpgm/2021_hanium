#include <ArduinoBLE.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "utility/wifi_drv.h"
#include "arduino_secrets.h"


#define BLE_UUID_NETWORK_CONFIG_SERVICE              "343D2964-5ECF-2297-4463-609011571F24"
#define BLE_UUID_NETWORK_ENABLE_CHARACTERISTIC       "767B22E7-EA6C-B017-286A-55B68310FD9D"

BLEService networkConfigService( BLE_UUID_NETWORK_CONFIG_SERVICE );
BLEBoolCharacteristic networkEnableCharacteristic( BLE_UUID_NETWORK_ENABLE_CHARACTERISTIC , BLERead | BLEWrite );

///////WIFI 이름과 패스워드를 arduino_secrets.h에 저장
char ssidName[] = SECRET_SSID;
char ssidPass[] = SECRET_PASS;


int status = WL_IDLE_STATUS;
bool networkInitialized = false;    // Wifi - Ble 스위칭을 위한 변수
bool wifiModeFlag = true;           // Wifi 연결시 true Ble 연결시 false

void setup()
{
  Serial.begin( 9600 );
  while ( !Serial );
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
      wifiMode();
    }
  }
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
      {// 연결된 BLE 디바이스로 넘어온 값이 있다면 Wifi로 전환
        networkInitialized = false;
        wifiModeFlag = true;
        BLE.stopAdvertise();
        BLE.end();
        return;
      }
    }
  }
}


void wifiMode()
{
  int connectCount = 0;

  if ( status != WL_CONNECTED )
  {
    while ( status != WL_CONNECTED )
    {
      connectCount++;
      Serial.print( "WiFi attempt: " );
      Serial.println( connectCount );

      if( connectCount > 10 ) // 연결 10번 시도 실패시 Ble 모드로 전환
      {
        networkInitialized = false;
        wifiModeFlag = false;
        Serial.println( "WiFi connection failed" );
        return;
      }
      Serial.print( "Attempting to connect to SSID: " );
      Serial.println( ssidName );

      status = WiFi.begin( ssidName, ssidPass );

      if( status != WL_CONNECTED )
      {
        // wait 10 seconds for connection:
        delay( 10000 );
      }
    }
    printWiFiStatus();
    onMessageReceived();
  }
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

void onMessageReceived() {  
  // 와이파이 연결 후10초가 지나면 자동으로 BLE 모드로 전환
  // 임의로 이렇게 설정함 버튼을 연결해서 눌러서 바꾸던지 원하는대로 수정
  for(int i = 0; i <= 10; i++){
    delay(1000);
    if( i == 10){
      networkInitialized = false;
      wifiModeFlag = false;
    }
  }
  Serial.println();
}
