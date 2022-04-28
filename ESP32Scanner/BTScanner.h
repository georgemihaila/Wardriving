#ifndef BTScanner_h_
#define BTScanner_h_

#include "DataManager.h"
#include "ScannerBase.h"
#include "CustomGPS.h"
#include "CustomSDCard.h"
#include "CustomWiFi.h"
#include "TFTDisplay.h"

class BTScanner : ScannerBase {
    public:
        BTScanner(DataManager* dataManager, CustomGPS* customGPS, CustomSDCard* sdCard, CustomWiFi* wifi, TFTDisplay* _display, SplashScreen* splashScreen);
        void scan();
        void tick();
};

#endif
