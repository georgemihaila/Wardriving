#ifndef SplashScreen_h_
#define SplashScreen_h_

#include "Page.h"
#include "GPSService.h"
#include "WiFiService.h"
#include "WiFiScanner.h"

class SplashScreen : public Page{
  public:
    SplashScreen(GPSService* gpsService, WiFiService* wifiService, WiFiScanner* wifiScanner);
};

#endif
