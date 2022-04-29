#ifndef CustomSDCard_h_
#define CustomSDCard_h_

#include <Arduino.h>
#include "TFTDisplay.h"
#include "SplashScreen.h"
class CustomSDCard {
    public:
        CustomSDCard(TFTDisplay* display, SplashScreen* splashScreen);
        bool init();
        int getNumberOfLines(String filename);
        int getNumberOfFailedDataSends(String dataType);
        void appendFile(const char * path, const char * message);
        void appendCollectionLogFile(String dataType, String data);
        bool deleteFile(const char * path);
        void writeFailedSendToSDCard(String dataType, String data);
        void renameFile(const char * path1, const char * path2);
        String getFailedFileName(String dataType);
};

#endif
