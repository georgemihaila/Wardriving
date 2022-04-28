#include "SplashScreen.h"
#include "Arduino.h"

SplashScreen::SplashScreen(){
  offline = true;
  gpsHeardFrom = false;
  nSatellites = 0; 
  newWiFiNetworks = 0;

  btDevicesAround = 0;
  newBTDevices = 0;
  sessionBTDevices = 0;
  
  wiFiNetworksAround = 0;
  sessionWiFiNetworks = 0;

  for(int i = 0; i < 135; i++){
    latestWiFiNetworkCount[i] = 0;
    latestBTCount[i] = 0;
  }
}

void appendLastElement(int (&data)[135], int entry){
  data[134] = entry;
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
