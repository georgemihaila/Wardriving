#include "Logger.h"
#include "TFTDisplay.h"
#include "Page.h"
#include "SplashScreen.h"
#include "LEDBlinker.h"
#include "GPSService.h"
#include "WiFiService.h"

Logger* _logger = new Logger();
TFTDisplay* _display = new TFTDisplay();
GPSService* _gpsService = new GPSService();
WiFiService* _wifiService = new WiFiService();
SplashScreen* _splashScreen = new SplashScreen(_gpsService, _wifiService);

void setup() {
  _logger->log("ESP32 up");
  _wifiService->initWiFi(0);
}

void loop() {
  _display->render(_splashScreen);
  _gpsService->update();
  yield();
  Serial.println("\n" + String(ESP.getFreeHeap() / 1024) + "kB");
}
