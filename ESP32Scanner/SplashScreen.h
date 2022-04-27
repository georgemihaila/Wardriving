#ifndef SplashScreen_h_
#define SplashScreen_h_

class SplashScreen{
    public:
        SplashScreen();
        double batteryVoltage;
        bool offline;
        
        int wiFiNetworksAround;
        int sessionWiFiNetworks;
};

#endif
