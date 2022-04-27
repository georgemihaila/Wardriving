#ifndef DataManager_h_  
#define DataManager_h_

#include <Arduino.h>

#include "CustomSDCard.h"
#include "TFTDisplay.h"

class DataManager {
    public:
        DataManager(CustomSDCard* sdCard, TFTDisplay* display, String serverName, bool offline);
        void setOfflineMode(bool offline);
        bool postData(String data, String dataType);
        void sendQueueData(String data[], int dataCount, int& lastSendIndex, long& lastDataSentMs, String path, String dataType, long maxTimeAllowedBeforeCachingMs);
        void trySendFailedData(String dataType);
};

#endif
