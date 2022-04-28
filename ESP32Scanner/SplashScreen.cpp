#include "SplashScreen.h"
#include "Arduino.h"

double batteryVoltage;
bool offline;
int wiFiNetworksAround;
int sessionWiFiNetworks;
bool gpsHeardFrom;
int nSatellites;
int latestWiFiNetworkCount[135];
int latestBTCount[135];
int newWiFiNetworks = 0;

unsigned long lastLoopTimeMs;

SplashScreen::SplashScreen(){
  offline = true;
  gpsHeardFrom = false;
  nSatellites = 0; 
  
  wiFiNetworksAround = 0;
  sessionWiFiNetworks = 0;

  for(int i = 0; i < 135 - 1; i++){
    latestWiFiNetworkCount[i] = 0;
    latestBTCount[i] = 0;
  }
}

void appendLastElement(int (&data)[135], int entry){
  data[127] = entry;
  for(int i = 0; i < 134; i++){
    data[i] = data[i + 1];
  } 
}

void SplashScreen::appendLatestWifiNetworkCount(int count){
  appendLastElement(latestWiFiNetworkCount, count);
}

void SplashScreen::appendLatestBTCount(int count){
  appendLastElement(latestBTCount, count);
}
