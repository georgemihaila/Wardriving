#include "TFTDisplay.h"
#include "Page.h"
#include "SplashScreen.h"
#include "LEDBlinker.h"
#include "GPSService.h"
#include "WiFiService.h"
#include "WiFiScanner.h"
#include "BluetoothScanner.h"
#include "ScanService.h"

TFTDisplay* _display = new TFTDisplay();
GPSService* _gpsService = new GPSService();
WiFiService* _wifiService = new WiFiService();
SplashScreen* _splashScreen = new SplashScreen(_gpsService, _wifiService);
WiFiScanner* _wifiScanner = new WiFiScanner();
BluetoothScanner* _bluetoothScanner = new BluetoothScanner();
ScanService* _scanService = new ScanService(_wifiScanner, _bluetoothScanner);

void setup() {
  Serial.begin(115200);
  esp_log_level_set("*", ESP_LOG_INFO);
  ESP_LOGI("*", "ESP32 up");
  _wifiService->initWiFi(0);

  _display->clear();
}

void loop() {
  _display->render(_splashScreen);
  _gpsService->update();
  _scanService->scan();
  yield();
  ESP_LOGI("*", "\n" + String(ESP.getFreeHeap() / 1024) + "kB");
}
