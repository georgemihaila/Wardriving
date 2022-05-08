#include "SDCard.h"
#include <Arduino.h>
#include "FS.h"

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

File SDCard::getFileForAppend(String path)
{
    for (int i = 0; i < 3; i++)
    {
        File f = SD.open(path, FILE_APPEND);
        if (f)
        {
            return f;
        }
    }
    Serial.println("Failed to open file for appending [getFileForAppend(...)]");
    ESP.restart();
}

void SDCard::appendFile(const char *path, const char *message)
{
    int errorCount = 0;
    bool ok = false;
    do
    {
        File file = SD.open(path, FILE_APPEND);// fileCache.getFileForAppend(SD, path);
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
                /*
                Serial.print(message);
                Serial.print(" > ");
                Serial.println(path);*/
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

String SDCard::getUsedSpace()
{
    int used = SD.usedBytes() / (1024 * 1024);
    int total = SD.totalBytes() / (1024 * 1024);
    return String(used) + "MB/" + String(total) + "MB";
}

bool SDCard::renameFile(const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    return SD.rename(path1, path2);
}

void SDCard::sendAllLinesFor(String dataType, API *api, TFTDisplay *display, int y)
{
    display->clear();
    vector<String> files = listDir("/", 0);
    vector<String> targetFiles;
    for (int i = 0; i < files.size(); i++)
    {
        if (files[i].startsWith("/" + dataType))
        {
            Serial.println("Sending " + files[i]);
            String buffer;
            File file = SD.open(files[i]);
            if (!file)
            {
                Serial.println("Can't open " + String(files[i]));
            }
            int line = 0;
            while (file.available())
            {
                line++;
                buffer = file.readStringUntil('\n');
                buffer.trim();
                display->printAt(files[i] + "\n" + String(line), 0, y + 20);
                int result = api->postData(buffer, dataType);
                if (result == 201)
                {
                    appendFile("/Sent " + dataType + ".csv", buffer);
                }
                else
                {
                    appendFile("/Failed " + dataType + ".csv", buffer);
                }
            }
            file.close();
            deleteFile(files[i]);
        }
    }
}

int SDCard::getNumberOfLines(String filename)
{
    File file = SD.open(filename);
    if (!file)
    {
        Serial.println("Can't open " + filename);
        return 0;
    }
    int line = 0;
    String buffer;
    while (file.available())
    {
        line++;
        buffer = file.readStringUntil('\n');
    }
    file.close();
    return line;
}

vector<String> SDCard::listDir(const char *dirname, uint8_t levels)
{

    vector<String> result;
    Serial.printf("Listing directory: %s\n", dirname);

    File root = SD.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return result;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return result;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            // Serial.print("  DIR : ");
            // Serial.println(file.name());
            if (levels)
            {
                vector<String> subdirs = listDir(file.name(), levels - 1);
                for (int i = 0; i < subdirs.size(); i++)
                {
                    result.push_back(subdirs[i]);
                }
            }
        }
        else
        {
            result.push_back(file.name());
            // Serial.println(file.name());
        }
        file = root.openNextFile();
    }
    return result;
}

bool SDCard::createDir(const char *path)
{

    Serial.printf("Creating Dir: %s\n", path);
    if (SD.mkdir(path))
    {
        Serial.println("Dir created");
        return true;
    }
    else
    {
        Serial.println("mkdir failed");
        return false;
    }
}
bool SDCard::removeDir(const char *path)
{

    Serial.printf("Removing Dir: %s\n", path);
    if (SD.rmdir(path))
    {
        Serial.println("Dir removed");
        return true;
    }
    else
    {
        Serial.println("rmdir failed");
        return false;
    }
}

String SDCard::readFile(String filename)
{
    File file = SD.open(filename);
    if (!file)
    {
        Serial.println("Can't open " + filename);
        return "";
    }
    int line = 0;
    String result;
    while (file.available())
    {
        line++;
        result += file.readStringUntil('\n');
    }
    file.close();
    return result;
}

bool SDCard::deleteFile(String path)
{
    Serial.print("Deleting file: " + path);
    if (SD.remove(path))
    {
        Serial.println(" OK");
        return true;
    }
    else
    {
        Serial.println(" FAIL");
        return false;
    }
}

void SDCard::writeFile(String path, String text)
{
    deleteFile(path);
    appendFile(path, text);
}