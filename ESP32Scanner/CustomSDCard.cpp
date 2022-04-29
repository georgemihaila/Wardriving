#include "CustomSDCard.h"

#include <SPI.h>
#include <SD.h>
#include <SD_MMC.h>

#include <Arduino.h>

#define HSPI_MISO   27
#define HSPI_MOSI   26
#define HSPI_SCLK   25
#define HSPI_SS     33
SPIClass *sdspi = NULL;

#include "TFTDisplay.h"
#include "SplashScreen.h"

TFTDisplay* _cscTFT;
SplashScreen* _cscSS;
CustomSDCard::CustomSDCard(TFTDisplay* tft, SplashScreen* splashScreen){
  _cscTFT = tft;
  _cscSS = splashScreen;
}

bool CustomSDCard::init(){
    sdspi = new SPIClass(HSPI);
    sdspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);
    return SD.begin(HSPI_SS, *sdspi);
}

String buffer;

String CustomSDCard::getFailedFileName(String dataType){
  return "/failed_" + dataType + ".log";
}

int CustomSDCard::getNumberOfLines(String filename) {
  File file = SD.open(filename);
  if (!file) {
    Serial.println("Can't open " + filename);
    return 0;
  }
   int line = 0;
   while (file.available()) {
    line++;
    buffer = file.readStringUntil('\n');
  }
  file.close();
  return line;
}

int CustomSDCard::getNumberOfFailedDataSends(String dataType){
  return getNumberOfLines(getFailedFileName(dataType));
}

void CustomSDCard::appendFile(const char * path, const char * message){
    int errorCount = 0;
    bool ok = false;
    do{
        File file = SD.open(path, FILE_APPEND);
        if(!file){
            Serial.println("Failed to open file for appending");
            errorCount++;
            return;
        }
        else{
            if(file.println(message)){
                errorCount = 0;
                ok = true;
                Serial.print(message);
                Serial.print(" > ");
                Serial.println(path);
            } 
            else {
                errorCount++;
                Serial.println("Write error");
            }
        }
        file.close();
    }
    while (!ok && errorCount < 3);
    if (errorCount == 3){
      Serial.println("Too many SD card errors");
      ESP.restart();
    }
    _cscSS->linesWritten++;
}

void CustomSDCard::appendCollectionLogFile(String dataType, String data){
  appendFile(String("/" + dataType + ".log").c_str(), data.c_str());
}

bool CustomSDCard::deleteFile(const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(SD.remove(path)){
    Serial.println("File deleted");
    return true;
  } else {
    Serial.println("Delete failed");
    return false;
  }
}

void CustomSDCard::writeFailedSendToSDCard(String dataType, String data){
  /*
  if (dataType.equals("Bluetooth")){
      _sdSplashScreen->sessionBTDevices++;
  }
  else if (dataType.equals("WiFi")){
      _sdSplashScreen->sessionWiFiNetworks++;
  }
  */
  String filename = getFailedFileName(dataType);
  String text = data;
  appendFile(filename.c_str(), text.c_str());
}

void CustomSDCard::renameFile(const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (SD.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}
