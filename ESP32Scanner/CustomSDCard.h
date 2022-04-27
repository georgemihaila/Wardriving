#ifndef CustomSDCard_h
#define CustomSDCard_h

#include <Arduino.h>

class CustomSDCard {
    public:
        bool init();
        int getNumberOfLines(String filename);
        int getNumberOfFailedDataSends(String dataType);
        void appendFile(const char * path, const char * message);
        void appendCollectionLogFile(String dataType, String data);
        bool deleteFile(const char * path);
        void writeFailedSendToSDCard(String dataType, String data);
};

#endif
