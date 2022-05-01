#include "TFTDisplay.h"
#include "Page.h"
#include "SplashScreen.h"
#include "LEDBlinker.h"
#include "GPSService.h"
#include "WiFiService.h"
#include "WiFiScanner.h"
#include "BluetoothScanner.h"
#include "ScanService.h"
#include "API.h"
#include "SDCard.h"
#include "DataManager.h"

TFTDisplay *_display;
GPSService *_gpsService;
WiFiService *_wifiService;
WiFiScanner *_wifiScanner;
SplashScreen *_splashScreen;
BluetoothScanner *_bluetoothScanner;
ScanService *_scanService;
API *_api;
SDCard *_sdCard;
DataManager* _dataManager;

// Use this because some methods use serial before Serial.begin(...)
void initializeServices()
{
  _display = new TFTDisplay();
  _gpsService = new GPSService();
  _wifiService = new WiFiService();
  _wifiScanner = new WiFiScanner();
  _bluetoothScanner = new BluetoothScanner();
  _splashScreen = new SplashScreen(_gpsService, _wifiService, _wifiScanner, _bluetoothScanner);
  _sdCard = new SDCard();
  _dataManager = new DataManager(_sdCard, _gpsService);
  _scanService = new ScanService(_wifiScanner, _bluetoothScanner, _gpsService, _dataManager);
  _api = new API("http://10.10.0.241:6488/");
}

void setup()
{
  Serial.begin(115200);
  esp_log_level_set("*", ESP_LOG_INFO);
  ESP_LOGI("*", "ESP32 up");

  initializeServices();
  if (!_sdCard->init())
  {
    _display->printAt("SD card error", 0, 20);
    delay(1000);
    ESP.restart();
  }
  else
  {
    _display->printAt("SD OK", 0, 20);
  }
  if (_wifiService->initWiFi(0))
  {
     _display->printAt("WiFi OK", 0, 40);
    _api->createNewSession();
  }

  _display->clear();
}

void loop()
{
  _display->render(_splashScreen);
  _gpsService->update();
  _scanService->scan();
  yield();
  ESP_LOGI("*", "\n" + String(ESP.getFreeHeap() / 1024) + "kB");
}