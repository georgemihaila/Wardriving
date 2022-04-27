#ifndef CustomWiFi_h_
#define CustomWiFi_h_
#include "TFTDisplay.h"
class CustomWiFi{
    public:
        CustomWiFi(TFTDisplay* display);
        bool initWiFi(int maxReattempts = 3);
        bool makeSureWiFiConnectionUp();
};

#endif
