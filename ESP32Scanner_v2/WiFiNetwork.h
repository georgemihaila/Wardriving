#ifndef WiFiNetwork_h_
#define WiFiNetwork_h_
#include "Arduino.h"

class WiFiNetwork
{
public:
  String SSID;
  String BSSID;
  String encryptionType;
  int channel;
};

#endif
