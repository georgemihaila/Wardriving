#ifndef TFTDisplay_h_
#define TFTDisplay_h_

#include <TFT_eSPI.h>
#include "SplashScreen.h"

class TFTDisplay {
    public:
        void init();
        void refresh(SplashScreen* splashScreen, bool limitFPS);
        void printSingleString(String text);
        void setCurrentAction(String text);
};

#endif
