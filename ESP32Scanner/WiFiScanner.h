#ifndef WiFiScanner_h_
#define WiFiScanner_h_

#include "DataManager.h"
#include "ScannerBase.h"
#include "CustomGPS.h"
#include "CustomSDCard.h"
#include "CustomWiFi.h"
#include "TFTDisplay.h"

class WiFiScanner : ScannerBase {
    public:
        WiFiScanner(DataManager* dataManager, CustomGPS* customGPS, CustomSDCard* sdCard, CustomWiFi* wifi, TFTDisplay* _display, SplashScreen* splashScreen);
        void scan();
        void tick();
};

#endif
