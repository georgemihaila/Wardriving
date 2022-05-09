#ifndef DependencyContainerh
#define DependencyContainerh

#include "TFTDisplay.h"
#include "Page.h"
#include "SplashScreen.h"
#include "LEDBlinker.h"
#include "GPSService.h"
#include "WiFiService.h"
#include "WiFiScanner.h"
#include "BluetoothScanner.h"
#include "ScanService.h"
#include "API.h"
#include "SDCard.h"
#include "DataManager.h"
#include "Config.h"
#include "ThreeWaySwitch.h"
#include "BluetoothService.h"

#include "Arduino.h"

class DependencyContainer
{
public:
    DependencyContainer(void (*_autosendFunction)())
    {
        display = new TFTDisplay();
        gpsService = new GPSService();
        wifiService = new WiFiService();
        wifiScanner = new WiFiScanner();
        bluetoothScanner = new BluetoothScanner();
        splashScreen = new SplashScreen(gpsService, wifiService, wifiScanner, bluetoothScanner, modeThreeWaySwitch, scanTypeThreeWaySwitch);
        sdCard = new SDCard();
        api = new API("http://10.10.0.241:6488/");
        dataManager = new DataManager(sdCard, gpsService, api);
        scanService = new ScanService(wifiScanner, bluetoothScanner, gpsService, dataManager, _autosendFunction, modeThreeWaySwitch, scanTypeThreeWaySwitch);
    }
    TFTDisplay *display;
    GPSService *gpsService;
    WiFiService *wifiService;
    WiFiScanner *wifiScanner;
    SplashScreen *splashScreen;
    BluetoothScanner *bluetoothScanner;
    ScanService *scanService;
    API *api;
    SDCard *sdCard;
    DataManager *dataManager;
    ThreeWaySwitch *modeThreeWaySwitch = new ThreeWaySwitch(21, 22, HIGH);
    ThreeWaySwitch *scanTypeThreeWaySwitch = new ThreeWaySwitch(17, 2, LOW);
};

#endif
