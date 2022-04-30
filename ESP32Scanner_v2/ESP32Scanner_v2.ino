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
  esp_log_level_set("*", ESP_LOG_DEBUG);
  ESP_LOGI("status", "ESP32 up");
  _wifiService->initWiFi(0);
}

void loop() {
  _display->render(_splashScreen);
  _gpsService->update();
  _scanService->scan();
  yield();
  Serial.println("\n" + String(ESP.getFreeHeap() / 1024) + "kB");
}
