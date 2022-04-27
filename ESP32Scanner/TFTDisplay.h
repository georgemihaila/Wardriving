#include <TFT_eSPI.h>
#include "SplashScreen.h"

class TFTDisplay {
    public:
        void init();
        void refresh(SplashScreen splashScreen);
        void printSingleString(String text);
};
