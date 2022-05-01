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
  int nSatellites = 0;

private:
  void changeFrequency();
  void gpsDelay(unsigned long ms);
  void sendPacket(byte *packet, byte len);
};

#endif
