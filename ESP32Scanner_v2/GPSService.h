#ifndef GPSService_h_
#define GPSService_h_

#include <Arduino.h>
#include "TFTDisplay.h"

class GPSService
{
public:
  GPSService();
  String generateLocationCSV();
  void update();
  void yield();

  bool gpsHeardFrom = false;
  double longitude = 0;
  double latitude = 0;
  double altitude = 0;
  double speedMetersPerSecond = 0;
  double speedKmPerHour = 0;
  int nSatellites = 0;
  double getXMetersFromOrigin();
  double getYMetersFromOrigin();

private:
  void changeFrequency();
  void gpsDelay(unsigned long ms);
  void sendPacket(byte *packet, byte len);
  unsigned long _lastUpdatedAtMS = 0;
  unsigned long _updateEveryMS = 3000;
};

#endif
