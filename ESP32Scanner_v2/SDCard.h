#ifndef SDCard_h_
#define SDCard_h_

#include <SPI.h>
#include <SD.h>
#include <SD_MMC.h>
#include "TFTDisplay.h"
#include "API.h"
#include <vector>
using namespace std;

class SDCard
{
public:
    bool init();
    void appendFile(String path, String message);
    void appendFile(const char *path, const char *message);
    void writeFile(String path, String text);
    bool deleteFile(String path);
    vector<String> readAllLines(String path);
    vector<String> readAllLines(const char *path);
    void sendAllLines(String path, String dataType, API *api, TFTDisplay *display, int y);
    String getUsedSpace();
    int getNumberOfLines(String filename);
    bool removeDir(const char *path);
    bool createDir(const char *path);
    vector<String> listDir(const char *dirname, uint8_t levels);
    String readFile(String filename);

private:
    SPIClass *_sdspi;
};

#endif
