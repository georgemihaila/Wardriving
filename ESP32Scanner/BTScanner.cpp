#include "BTScanner.h"
#include <WiFi.h>
#include "DataManager.h"
#include "CustomGPS.h"
#include "CustomSDCard.h"
#include "CustomWiFi.h"
#include "TFTDisplay.h"

#define TIME_BEFORE_FORCE_CACHE 30000
#define COLLECT_UNIQUE_SIZE 250

DataManager* _btDataManager;
CustomGPS* _btCustomGPS;
CustomSDCard* _btCustomCard;
TFTDisplay* _btDisplay;
SplashScreen* _btSplashScreen;

BTScanner::BTScanner(DataManager* dataManager, CustomGPS* customGPS, CustomSDCard* sdCard, TFTDisplay* _display, SplashScreen* splashScreen){
    _btDataManager = dataManager;
    _btCustomGPS = customGPS;
    _btCustomCard = sdCard;
    _btDisplay = _display;
    _btSplashScreen = splashScreen;
}

bool addBTDeviceToListIfNewAndSendDataIfQueueFull(String (&collectedDevices)[COLLECT_UNIQUE_SIZE], int& collectedDeviceCount, int& sessionDevices, String deviceData, String deviceType, void (&sendFunction)()){
  for (int i = 0; i < collectedDeviceCount; i++){
    if (collectedDevices[i].equals(deviceData)){
      return false;
    }
  }
  _btCustomCard->appendCollectionLogFile(deviceType, deviceData);
  collectedDevices[collectedDeviceCount++] = deviceData;
  sessionDevices++;
  Serial.println("New " + deviceType + " device found. Session total: " + String(sessionDevices));
  if (collectedDeviceCount == COLLECT_UNIQUE_SIZE){
    sendFunction();
    collectedDeviceCount = 0; //Start over, keep queue for comparison
  }
  return true;
}

int btSessionBTDevices = 0;
String collectedBTDevices[COLLECT_UNIQUE_SIZE];
int collectedBTDeviceCount = 0;

long lastBTDataSentMs = 0;
int lastBTSendIndex = 0;

void sendBTQueueData(String data[], int dataCount, int& lastSendIndex, long& lastDataSentMs, String path, String dataType, long maxTimeAllowedBeforeCachingMs){
  bool connected = _btWifi->makeSureWiFiConnectionUp();
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
        _btDisplay->setCurrentAction(dataType + " " + String(i + 1) + "/" + String(dataCount));
        ok = _btDataManager->postData(data[i], dataType);
      }
      if (!ok){ //Don't bother retrying now, save to SD
        _btDisplay->setCurrentAction(dataType + ">SD " + String(i + 1) + "/" + String(dataCount));
        _btCustomCard->writeFailedSendToSDCard(dataType, data[i]);
      }
    }
  }
 else{
    Serial.println("WiFi not available, saving data to SD card");
    //appendFile(SD, String("/" + dataType + ".log").c_str(), data.c_str());
    for (int i = lastSendIndex; i < dataCount; i++){
      _btDisplay->setCurrentAction(dataType + ">SD (" + String(i) + "/" + String(dataCount) + ")");
      _btCustomCard->writeFailedSendToSDCard(dataType, "\"" + data[i] + "\"");
    }
    
 }
    lastDataSentMs = millis();
    lastSendIndex = dataCount;
}

void sendBTData(){
  sendBTQueueData(collectedBTDevices, collectedBTDeviceCount, lastBTSendIndex, lastBTDataSentMs, "WiFi/ProcessRawString", "WiFi", TIME_BEFORE_FORCE_CACHE);
}

bool addBTDeviceIfNewAndSendDataIfQueueFull(String data){
  return addBTDeviceToListIfNewAndSendDataIfQueueFull(collectedBTDevices, collectedBTDeviceCount, btSessionBTDevices, data, "WiFi", sendBTData);
}

void BTScanner::scan(){
  /*
    int n = WiFi.scanDevices();
    _btSplashScreen->wiFiDevicesAround = n;
    int newDevices = 0;
    for (int i = 0; i < n; ++i) {
            String data = String(WiFi.SSID(i));
            data += "," + String(WiFi.encryptionType(i));
            data += "," + String(WiFi.channel(i));
            data += "," + String(bssidToString(WiFi.BSSID(i)));
            data += "," + _btCustomGPS->generateLocationCSV();
            if (addBTDeviceIfNewAndSendDataIfQueueFull(data)){
              newDevices++;
              _btSplashScreen->sessionBTDevices++;
              _btCustomCard->appendCollectionLogFile("WiFi", data);
        }
    }
    _btSplashScreen->appendLatestWifiNetworkCount(_btSplashScreen->wiFiDevicesAround);
    _btSplashScreen->newWiFiDevices = newDevices;
    */
}
#define BT_SEND_DATA_EVERY_MS 10000

void BTScanner::tick(){
  if (millis() - lastBTDataSentMs > BT_SEND_DATA_EVERY_MS){ //Enough time elapsed for an autosend
    if (lastBTSendIndex < collectedBTDeviceCount){ //New WiFi devices added since last send
      _btDisplay->setCurrentAction("WiFi autosend");
      sendBTData();
    }
  }
}
