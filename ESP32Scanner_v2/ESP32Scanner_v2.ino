#include "DependencyContainer.h"

#define NORMAL 1
#define QUICK_START 0
#define NOLOG 2

DependencyContainer *_dc;

void autosendIfHomeAfterStartup()
{
  if (_dc->wifiService->offlineMode)
    return;

  if (_dc->wifiService->initWiFi(0))
  {
    _dc->api->createNewSession();
    _dc->dataManager->sendCollectedDataToServer(_dc->display);
    _dc->wifiService->disconnect();
  }
}

void setup()
{
  Serial.begin(115200);
  // esp_log_level_set("*", ESP_LOG_DEBUG);
  ESP_LOGI("*", "ESP32 up");

  _dc = new DependencyContainer(autosendIfHomeAfterStartup);
  if (!_dc->sdCard->init())
  {
    _dc->display->printAt("SD card error", 0, 20);
    delay(1000);
    ESP.restart();
  }
  else
  {
    _dc->display->printAt("SD OK", 0, 20);
    _dc->display->printAt("Usage: " + _dc->sdCard->getUsedSpace(), 0, 40);
  }
  Serial.println("Mode: " + String(_dc->modeThreeWaySwitch->getState()));
  if (_dc->modeThreeWaySwitch == QUICK_START)
  {
    _dc->wifiService->offlineMode = true;
  }

  Config config = _dc->dataManager->getConfig();
  int prevWiFi = config.totalWiFiNetworks;
  int prevBT = config.totalBluetoothDevices;
  _dc->scanService->setPreviousScanCounts(prevWiFi, prevBT);
  Serial.println("Previous WIFI: " + String(prevWiFi));
  Serial.println("Previous BT: " + String(prevBT));
  _dc->display->clear();
}

void loop()
{
  _dc->display->render(_dc->splashScreen);
  _dc->gpsService->update();
  _dc->scanService->scan();
  if (_dc->bluetoothService != NULL)
  {
    _dc->bluetoothService->yield();
  }
}