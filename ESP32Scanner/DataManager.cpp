#include "DataManager.h"
#include "CustomSDCard.h"

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <SD.h>
#include <SD_MMC.h>

#include "TFTDisplay.h"


CustomSDCard* _sdCard;
bool _offline;
String _serverName;
HTTPClient _http;
WiFiClient client;
TFTDisplay* _dmdisplayref;

DataManager::DataManager(CustomSDCard* sdCard, TFTDisplay* display, String serverName, bool offline){
    _sdCard = sdCard;
    _offline = offline;
    _serverName = serverName;
    _dmdisplayref = display;
}

void DataManager::setOfflineMode(bool offline){
    _offline = offline;
}

bool DataManager::postData(String data, String dataType){
    bool ok = false;
    String serverPath = _serverName + dataType + "/ProcessRawString";
    _http.begin(client, serverPath);
    _http.addHeader("Content-Type", "application/json");
    String message = data;
    bool isJSON = data[0] == '"';
    if (!isJSON){
      message = "\"" + data + "\"";
    }
    int httpResponseCode = _http.POST(message);
    //Serial.println(message);
    if (httpResponseCode == 201) {
      //All good
      Serial.println(" [OK]");
      ok = true;
    }
    else if (httpResponseCode < 0){ //ESP error?
    }
    else {
      Serial.print("HTTP response code: ");
      Serial.println(httpResponseCode);
    }
    _http.end();
    return ok;
}

void DataManager::sendQueueData(String data[], int dataCount, int& lastSendIndex, long& lastDataSentMs, String path, String dataType, long maxTimeAllowedBeforeCachingMs){
  bool connected = !_offline;
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
        _dmdisplayref->setCurrentAction(dataType + " " + String(i + 1) + "/" + String(dataCount));
        ok = postData(data[i], dataType);
      }
      if (!ok){ //Don't bother retrying now, save to SD
        _dmdisplayref->setCurrentAction(dataType + ">SD " + String(i + 1) + "/" + String(dataCount));
        _sdCard->writeFailedSendToSDCard(dataType, data[i]);
      }
    }
  }
 Serial.println("Fast WiFi not available, saving data to SD card");
    //appendFile(SD, String("/" + dataType + ".log").c_str(), data.c_str());
    for (int i = lastSendIndex; i < dataCount; i++){
      _dmdisplayref->setCurrentAction(dataType + ">SD (" + String(i) + "/" + String(dataCount) + ")");
      _sdCard->writeFailedSendToSDCard(dataType, "\"" + data[i] + "\"");
    }
    lastDataSentMs = millis();
    lastSendIndex = dataCount;
}

char getRandomChar(){
   byte randomValue = random(0, 37);
   char letter = randomValue + 'a';
   if(randomValue > 26)
      letter = (randomValue - 26) + '0';
   return letter;
}

String getRandomString(int length){
  String result = "";
  for(int i = 0; i < length; i++){
    result += getRandomChar();
  }
  return result;
}

File failedDataFile;
String dmBuffer;
void DataManager::trySendFailedData(String dataType){
  String filename = _sdCard->getFailedFileName(dataType);
  failedDataFile = SD.open(filename);
  if (!failedDataFile) {
    Serial.println("Can't open " + filename);
    return;
  }
   int line = 0;
   while (failedDataFile.available()) {
    line++;
    _dmdisplayref->setCurrentAction("Failed " + dataType + " (" + line + ")");
    dmBuffer = failedDataFile.readStringUntil('\n');
    Serial.println(dmBuffer);
    int totalTries = 0;
    while (totalTries++ < 3){
      dmBuffer.trim();
      if (postData(dmBuffer, dataType)){
        break;
      }
      else{
        Serial.println("Retrying " + dmBuffer + "...");
      }
    }
    if (totalTries == 3){
      //Completely failed
      _sdCard->appendFile(("/completely failed " + dataType + ".log").c_str(), dmBuffer.c_str());
    }
  }
  failedDataFile.close();
  String randomString = getRandomString(16);
  //deleteFile(SD, filename.c_str());
  _sdCard->renameFile(filename.c_str(), (filename + "." + randomString + ".old.log").c_str());
}
