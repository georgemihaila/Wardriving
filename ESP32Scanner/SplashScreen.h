#ifndef SplashScreen_h_
#define SplashScreen_h_

#include "Arduino.h"

class SplashScreen{
    public:
        SplashScreen();
        double batteryVoltage;
        bool offline;
        
        int wiFiNetworksAround;
        int newWiFiNetworks;
        int sessionWiFiNetworks;
        int latestWiFiNetworkCount[135];
        void appendLatestWifiNetworkCount(int count);

        int btDevicesAround;
        int newBTDevices;
        int sessionBTDevices;
        int latestBTCount[135];
        void appendLatestBTCount(int count);

        bool gpsHeardFrom;
        String altitude;
        String longitude;
        String latitude;
        int nSatellites;

        unsigned long lastLoopTimeMs;
};

#endif
