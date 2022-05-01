#ifndef SDCard_h_
#define SDCard_h_

#include <SPI.h>
#include <SD.h>
#include <SD_MMC.h>

class SDCard {
    public:
        bool init();
        void SDCard::appendFile(const char * path, const char * message);
    private:
        SPIClass* _sdspi;
};

#endif
