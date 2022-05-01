#include "SDCard.h"
#include <Arduino.h>

#define HSPI_MISO 27
#define HSPI_MOSI 26
#define HSPI_SCLK 25
#define HSPI_SS 33

bool SDCard::init()
{
    _sdspi = new SPIClass(HSPI);
    _sdspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);
    return SD.begin(HSPI_SS, *_sdspi);
}

void SDCard::appendFile(String path, String message)
{
    appendFile(path.c_str(), message.c_str());
}

void SDCard::appendFile(const char *path, const char *message)
{
    int errorCount = 0;
    bool ok = false;
    do
    {
        File file = SD.open(path, FILE_APPEND);
        if (!file)
        {
            Serial.println("Failed to open file for appending");
            errorCount++;
            return;
        }
        else
        {
            if (file.println(message))
            {
                errorCount = 0;
                ok = true;
                Serial.print(message);
                Serial.print(" > ");
                Serial.println(path);
            }
            else
            {
                errorCount++;
                Serial.println("Write error");
            }
        }
        file.close();
    } while (!ok && errorCount < 3);
    if (errorCount == 3)
    {
        Serial.println("Too many SD card errors");
        ESP.restart();
    }
}

vector<String> SDCard::readAllLines(String path)
{
    return readAllLines(path.c_str());
}

vector<String> SDCard::readAllLines(const char *path)
{
    vector<String> result;
    String buffer;
    File file = SD.open(path);
    if (!file)
    {
        Serial.println("Can't open " + String(path));
    }
    int line = 0;
    while (file.available())
    {
        line++;
        buffer = file.readStringUntil('\n');
        result.push_back(buffer);
    }
    file.close();
    return result;
}