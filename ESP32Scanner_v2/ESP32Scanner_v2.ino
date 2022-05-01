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

TFTDisplay *_display;
GPSService *_gpsService;
WiFiService *_wifiService;
WiFiScanner *_wifiScanner;
SplashScreen *_splashScreen;
BluetoothScanner *_bluetoothScanner;
ScanService *_scanService;
API* _api;

// Use this because some methods use serial before Serial.begin(...)
void initializeServices()
{
  _display = new TFTDisplay();
  _gpsService = new GPSService();
  _wifiService = new WiFiService();
  _wifiScanner = new WiFiScanner();
  _bluetoothScanner = new BluetoothScanner();
  _splashScreen = new SplashScreen(_gpsService, _wifiService, _wifiScanner, _bluetoothScanner);
  _scanService = new ScanService(_wifiScanner, _bluetoothScanner, _gpsService);
  _api = new API("http://10.10.0.241:6488/");
}

void setup()
{
  Serial.begin(115200);
  initializeServices();

  esp_log_level_set("*", ESP_LOG_INFO);
  ESP_LOGI("*", "ESP32 up");
  _wifiService->initWiFi(0);

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
