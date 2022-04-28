#include "WiFiScanner.h"
#include <WiFi.h>
#include "DataManager.h"
#include "CustomGPS.h"
#include "CustomSDCard.h"
#include "CustomWiFi.h"
#include "TFTDisplay.h"

#define TIME_BEFORE_FORCE_CACHE 30000
#define COLLECT_UNIQUE_SIZE 250

DataManager* _wsDataManager;
CustomGPS* _wsCustomGPS;
CustomSDCard* _wsCustomCard;
CustomWiFi* _wsWifi;
TFTDisplay* _wsDisplay;
SplashScreen* _wsSplashScreen;

WiFiScanner::WiFiScanner(DataManager* dataManager, CustomGPS* customGPS, CustomSDCard* sdCard, CustomWiFi* wifi, TFTDisplay* _display, SplashScreen* splashScreen){
    _wsDataManager = dataManager;
    _wsCustomGPS = customGPS;
    _wsCustomCard = sdCard;
    _wsWifi = wifi;
    _wsDisplay = _display;
    _wsSplashScreen = splashScreen;
}

char HexLookUp[] = "0123456789abcdef";
String bssidToString(uint8_t *bssid){
  String result;
  for (int i = 0; i < 6; i++){
    if (i % 2 == 1){
      result += ":";
    }
    result += HexLookUp[bssid[i] >> 4];
    result += HexLookUp[bssid[i] & 0x0F];
  }
  return result;
}

bool addDeviceToListIfNewAndSendDataIfQueueFull(String (&collectedDevices)[COLLECT_UNIQUE_SIZE], int& collectedDeviceCount, int& sessionDevices, String deviceData, String deviceType, void (&sendFunction)()){
  for (int i = 0; i < collectedDeviceCount; i++){
    if (collectedDevices[i].equals(deviceData)){
      return false;
    }
  }
  _wsCustomCard->appendCollectionLogFile(deviceType, deviceData);
  collectedDevices[collectedDeviceCount++] = deviceData;
  sessionDevices++;
  Serial.println("New " + deviceType + " device found. Session total: " + String(sessionDevices));
  if (collectedDeviceCount == COLLECT_UNIQUE_SIZE){
    sendFunction();
    collectedDeviceCount = 0; //Start over, keep queue for comparison
  }
  return true;
}

int wsSessionWiFiNetworks = 0;
String collectedWiFiDevices[COLLECT_UNIQUE_SIZE];
int collectedWiFiDeviceCount = 0;

long lastWiFiDataSentMs = 0;
int lastWiFiSendIndex = 0;

void sendQueueData(String data[], int dataCount, int& lastSendIndex, long& lastDataSentMs, String path, String dataType, long maxTimeAllowedBeforeCachingMs){
  bool connected = _wsWifi->makeSureWiFiConnectionUp();
  if (lastSendIndex > dataCount) //Queue has filled in the meantime
  {
    lastSendIndex = 0;
  }
  if (connected){
    long startMs = millis();
    bool forceCache = false;
    for (int i = lastSendIndex; i < dataCount; i++){
      if (!forceCache && maxTimeAllowedBeforeCachingMs > 0){
        if (millis() - startMs >= maxTimeAllowedBeforeCachingMs){
          forceCache = true;
          Serial.println("Force caching because requests took too long");
        }
      }
      bool ok = false;
      if (!forceCache){
        _wsDisplay->setCurrentAction(dataType + " " + String(i + 1) + "/" + String(dataCount));
        ok = _wsDataManager->postData(data[i], dataType);
      }
      if (!ok){ //Don't bother retrying now, save to SD
        _wsDisplay->setCurrentAction(dataType + ">SD " + String(i + 1) + "/" + String(dataCount));
        _wsCustomCard->writeFailedSendToSDCard(dataType, data[i]);
      }
    }
  }
 else{
    Serial.println("WiFi not available, saving data to SD card");
    //appendFile(SD, String("/" + dataType + ".log").c_str(), data.c_str());
    for (int i = lastSendIndex; i < dataCount; i++){
      _wsDisplay->setCurrentAction(dataType + ">SD (" + String(i) + "/" + String(dataCount) + ")");
      _wsCustomCard->writeFailedSendToSDCard(dataType, "\"" + data[i] + "\"");
    }
    
 }
    lastDataSentMs = millis();
    lastSendIndex = dataCount;
}

void sendWiFiData(){
  sendQueueData(collectedWiFiDevices, collectedWiFiDeviceCount, lastWiFiSendIndex, lastWiFiDataSentMs, "WiFi/ProcessRawString", "WiFi", TIME_BEFORE_FORCE_CACHE);
}

bool addWiFiDeviceIfNewAndSendDataIfQueueFull(String data){
  return addDeviceToListIfNewAndSendDataIfQueueFull(collectedWiFiDevices, collectedWiFiDeviceCount, wsSessionWiFiNetworks, data, "WiFi", sendWiFiData);
}

void WiFiScanner::scan(){
    int n = WiFi.scanNetworks();
    _wsSplashScreen->wiFiNetworksAround = n;
    int newNetworks = 0;
    for (int i = 0; i < n; ++i) {
            _wsCustomGPS->yield();
            String data = String(WiFi.SSID(i));
            data += "," + String(WiFi.encryptionType(i));
            data += "," + String(WiFi.channel(i));
            data += "," + String(bssidToString(WiFi.BSSID(i)));
            data += "," + _wsCustomGPS->generateLocationCSV();
            if (addWiFiDeviceIfNewAndSendDataIfQueueFull(data)){
              newNetworks++;
              _wsSplashScreen->sessionWiFiNetworks++;
              _wsCustomCard->appendCollectionLogFile("WiFi", data);
        }
    }
    _wsSplashScreen->appendLatestWifiNetworkCount(_wsSplashScreen->wiFiNetworksAround);
    _wsSplashScreen->newWiFiNetworks = newNetworks;
}
#define WIFI_SEND_DATA_EVERY_MS 10000

void WiFiScanner::tick(){
  if (millis() - lastWiFiDataSentMs > WIFI_SEND_DATA_EVERY_MS){ //Enough time elapsed for an autosend
    if (lastWiFiSendIndex < collectedWiFiDeviceCount){ //New WiFi devices added since last send
      _wsDisplay->setCurrentAction("WiFi autosend");
      sendWiFiData();
    }
  }
}
