#ifndef SDCard_h_
#define SDCard_h_

#include <SPI.h>
#include <SD.h>
#include <SD_MMC.h>

#include <vector>
using namespace std;

class SDCard {
    public:
        bool init();
        void appendFile(String path, String message);
        void appendFile(const char * path, const char * message);
        vector<String> readAllLines(String path);
        vector<String> readAllLines(const char* path);
    private:
        SPIClass* _sdspi;
};

#endif
