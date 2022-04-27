#include "SplashScreen.h"

double batteryVoltage;
bool offline;
int wiFiNetworksAround;
int sessionWiFiNetworks;

SplashScreen::SplashScreen(){
  offline = true;
  wiFiNetworksAround = 0;
  sessionWiFiNetworks = 0;
}
