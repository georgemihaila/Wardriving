#ifndef SDCard_h_
#define SDCard_h_

#include <SPI.h>
#include <SD.h>
#include <SD_MMC.h>
#include "TFTDisplay.h"
#include "API.h"
#include <vector>
using namespace std;
#include "FileCache.h"

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
    void sendAllLinesFor(String dataType, API *api, TFTDisplay *display, int y);
    String getUsedSpace();
    int getNumberOfLines(String filename);
    bool removeDir(const char *path);
    bool createDir(const char *path);
    bool renameFile(const char * path1, const char * path2);
    vector<String> listDir(const char *dirname, uint8_t levels);
    String readFile(String filename);
    File getFileForAppend(String path);

private:
    SPIClass *_sdspi;
};

#endif
