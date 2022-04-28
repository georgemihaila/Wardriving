#ifndef CustomGPS_h_
#define CustomGPS_h_

#include <Arduino.h>
#include "TFTDisplay.h"

class CustomGPS {
    public:
        CustomGPS(TFTDisplay* display, SplashScreen* splashScreen);
        String generateLocationCSV();
        void tick();
};

#endif
