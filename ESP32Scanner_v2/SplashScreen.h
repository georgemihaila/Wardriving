#ifndef SplashScreen_h_
#define SplashScreen_h_

#include "Page.h"
#include "GPSService.h"
#include "WiFiService.h"

class SplashScreen : public Page{
  public:
    SplashScreen(GPSService* gpsService, WiFiService* wifiService);
};

#endif
