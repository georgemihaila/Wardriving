#include "SDCard.h"



#include <Arduino.h>

#define HSPI_MISO   27
#define HSPI_MOSI   26
#define HSPI_SCLK   25
#define HSPI_SS     33

bool SDCard::init(){
    _sdspi = new SPIClass(HSPI);
    _sdspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);
    return SD.begin(HSPI_SS, *_sdspi);
}

void SDCard::appendFile(const char * path, const char * message){
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
}