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
#include "Config.h"
#include "ThreeWaySwitch.h"

#define NORMAL 1
#define QUICK_START 0
#define NO_LOG 2

TFTDisplay *_display;
GPSService *_gpsService;
WiFiService *_wifiService;
WiFiScanner *_wifiScanner;
SplashScreen *_splashScreen;
BluetoothScanner *_bluetoothScanner;
ScanService *_scanService;
API *_api;
SDCard *_sdCard;
DataManager *_dataManager;
ThreeWaySwitch *_modeThreeWaySwitch = new ThreeWaySwitch(21, 22, HIGH);

void autosendIfHomeAfterStartup()
{
  if (_wifiService->offlineMode)
    return;

  if (_wifiService->initWiFi(0))
  {
    _api->createNewSession();
    _dataManager->sendCollectedDataToServer(_display);
    _wifiService->disconnect();
  }
}

// Use this because some methods use serial before Serial.begin(...)
void initializeServices()
{
  _display = new TFTDisplay();
  _gpsService = new GPSService();
  _wifiService = new WiFiService();
  _wifiScanner = new WiFiScanner();
  _bluetoothScanner = new BluetoothScanner();
  _splashScreen = new SplashScreen(_gpsService, _wifiService, _wifiScanner, _bluetoothScanner, _modeThreeWaySwitch);
  _sdCard = new SDCard();
  _api = new API("http://10.10.0.241:6488/");
  _dataManager = new DataManager(_sdCard, _gpsService, _api);
  _scanService = new ScanService(_wifiScanner, _bluetoothScanner, _gpsService, _dataManager, autosendIfHomeAfterStartup, _modeThreeWaySwitch);
}

void setup()
{
  Serial.begin(115200);
  // esp_log_level_set("*", ESP_LOG_DEBUG);
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
    _display->printAt("Usage: " + _sdCard->getUsedSpace(), 0, 40);
  }

  if (_modeThreeWaySwitch->getState() != QUICK_START)
  {
    autosendIfHomeAfterStartup(); // Use this method for now
  }
  else if (_modeThreeWaySwitch == QUICK_START)
  {
    _wifiService->offlineMode = true;
  }

  Config config = _dataManager->getConfig();
  int prevWiFi = config.totalWiFiNetworks;
  int prevBT = config.totalBluetoothDevices;
  _scanService->setPreviousScanCounts(prevWiFi, prevBT);
  Serial.println("Previous WIFI: " + String(prevWiFi));
  Serial.println("Previous BT: " + String(prevBT));
  _display->clear();
}

void loop()
{
  _display->render(_splashScreen);
  _gpsService->update();
  _scanService->scan();
}