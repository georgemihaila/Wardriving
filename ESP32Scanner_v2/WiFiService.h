#ifndef WiFiService_h_
#define WiFiService_h_

#include <WiFi.h>
#include "Arduino.h"

class WiFiService {
  public:
    bool initWiFi(int maxReattempts);
    bool makeSureWiFiConnectionUp();
  private:
    String IpAddress2String(const IPAddress& ipAddress);
};

#endif