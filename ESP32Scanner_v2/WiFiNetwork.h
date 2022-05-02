#ifndef WiFiNetwork_h_
#define WiFiNetwork_h_
#include "Arduino.h"

#include "StringableObject.h"

class WiFiNetwork
{
public:
  String SSID;
  String BSSID;
  String encryptionType;
  int channel;

  String toString()
  {
    return SSID + "," + encryptionType + "," + String(channel) + "," + BSSID;
  }
};

#endif
