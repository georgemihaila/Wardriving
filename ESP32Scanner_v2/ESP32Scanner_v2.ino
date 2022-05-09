#include "DependencyContainer.h"

#define NORMAL 1
#define QUICK_START 0
#define NOLOG 2

DependencyContainer *_dependencyContainer;

void autosendIfHomeAfterStartup()
{
  if (_dependencyContainer->wifiService->offlineMode)
    return;

  if (_dependencyContainer->wifiService->initWiFi(0))
  {
    _dependencyContainer->api->createNewSession();
    _dependencyContainer->dataManager->sendCollectedDataToServer(_dependencyContainer->display);
    _dependencyContainer->wifiService->disconnect();
  }
}

void setup()
{
  Serial.begin(115200);
  // esp_log_level_set("*", ESP_LOG_DEBUG);
  ESP_LOGI("*", "ESP32 up");

  _dependencyContainer = new DependencyContainer(autosendIfHomeAfterStartup);
  if (!_dependencyContainer->sdCard->init())
  {
    _dependencyContainer->display->printAt("SD card error", 0, 20);
    delay(1000);
    ESP.restart();
  }
  else
  {
    _dependencyContainer->display->printAt("SD OK", 0, 20);
    _dependencyContainer->display->printAt("Usage: " + _dependencyContainer->sdCard->getUsedSpace(), 0, 40);
  }
  Serial.println("Mode: " + String(_dependencyContainer->modeThreeWaySwitch->getState()));
  if (_dependencyContainer->modeThreeWaySwitch == QUICK_START)
  {
    _dependencyContainer->wifiService->offlineMode = true;
  }

  Config config = _dependencyContainer->dataManager->getConfig();
  int prevWiFi = config.totalWiFiNetworks;
  int prevBT = config.totalBluetoothDevices;
  _dependencyContainer->scanService->setPreviousScanCounts(prevWiFi, prevBT);
  Serial.println("Previous WIFI: " + String(prevWiFi));
  Serial.println("Previous BT: " + String(prevBT));
  _dependencyContainer->display->clear();
}

void loop()
{
  _dependencyContainer->display->render(_dependencyContainer->splashScreen);
  _dependencyContainer->gpsService->update();
  _dependencyContainer->scanService->scan();
}