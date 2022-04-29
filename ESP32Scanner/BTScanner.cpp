#include "BTScanner.h"
#include <WiFi.h>
#include "DataManager.h"
#include "CustomGPS.h"
#include "CustomSDCard.h"
#include "CustomWiFi.h"
#include "TFTDisplay.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define TIME_BEFORE_FORCE_CACHE 2000
#define COLLECT_UNIQUE_SIZE 50

DataManager* _btDataManager;
CustomGPS* _btCustomGPS;
CustomSDCard* _btCustomCard;
TFTDisplay* _btDisplay;
SplashScreen* _btSplashScreen;
CustomWiFi* _btWifi;

BLEScan* pBLEScan;



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
  sendBTQueueData(collectedBTDevices, collectedBTDeviceCount, lastBTSendIndex, lastBTDataSentMs, "Bluetooth/ProcessRawString", "Bluetooth", TIME_BEFORE_FORCE_CACHE);
}

bool addBTDeviceToListIfNewAndSendDataIfQueueFull(String (&collectedDevices)[COLLECT_UNIQUE_SIZE], int& collectedDeviceCount, int& sessionDevices, String deviceData, String deviceType){
  for (int i = 0; i < collectedDeviceCount; i++){
    if (collectedDevices[i].equals(deviceData)){
      return false;
    }
  }
  _btCustomCard->appendCollectionLogFile(deviceType, deviceData);
  collectedDevices[collectedDeviceCount++] = deviceData;
  sessionDevices++;
  Serial.println("New " + deviceType + " device found. Session total: " + String(sessionDevices));
  if (collectedDeviceCount >= COLLECT_UNIQUE_SIZE){
    sendBTData();
    collectedDeviceCount = 0; //Start over, keep queue for comparison
  }
  return true;
}

bool addBTDeviceIfNewAndSendDataIfQueueFull(String data){
  return addBTDeviceToListIfNewAndSendDataIfQueueFull(collectedBTDevices, collectedBTDeviceCount, btSessionBTDevices, data, "Bluetooth");
}

int quickBTScanTime = 1;
int deepBTScanTime = 1;
int quickBTScans = 0;

int scanTime = 1; //In seconds

int bluetoothDevicesCount[128];
int currentBluetoothScanCount = 0;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      String data = _btCustomGPS->generateLocationCSV() + "," + String(advertisedDevice.toString().c_str());
      _btCustomGPS->yield();
      if (addBTDeviceIfNewAndSendDataIfQueueFull(data)){
        _btSplashScreen->newBTDevices++;
        _btSplashScreen->sessionBTDevices++;
      }
      currentBluetoothScanCount++;
      _btSplashScreen->btDevicesAround++;
    }
};

int getBTScanTime(){
  if (++quickBTScans > 5){
    quickBTScans = 0;
    return deepBTScanTime;
  }
  else{
    return quickBTScanTime;
  }
}

void BTScanner::scan(){
  int btScanTime = getBTScanTime();
  currentBluetoothScanCount = 0;
  _btSplashScreen->newBTDevices = 0;
  _btSplashScreen->btDevicesAround = 0;
  BLEScanResults foundDevices = pBLEScan->start(btScanTime, false);
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  _btSplashScreen->appendLatestBTCount(_btSplashScreen->btDevicesAround);
}
#define BT_SEND_DATA_EVERY_MS 10000

void BTScanner::tick(){
  if (millis() - lastBTDataSentMs > BT_SEND_DATA_EVERY_MS){ //Enough time elapsed for an autosend
    if (lastBTSendIndex < collectedBTDeviceCount){ //New WiFi devices added since last send
      _btDisplay->setCurrentAction("BT autosend");
      sendBTData();
      lastBTDataSentMs = millis();
    }
  }
}

BTScanner::BTScanner(DataManager* dataManager, CustomGPS* customGPS, CustomSDCard* sdCard, CustomWiFi* wifi, TFTDisplay* _display, SplashScreen* splashScreen){
    _btDataManager = dataManager;
    _btCustomGPS = customGPS;
    _btCustomCard = sdCard;
    _btWifi = wifi;
    _btDisplay = _display;
    _btSplashScreen = splashScreen;

    BLEDevice::init("ESP32 Scanner");
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);  // less or equal setInterval value
}
