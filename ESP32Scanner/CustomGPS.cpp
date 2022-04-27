#include "CustomGPS.h"
#include <Arduino.h>

bool gpsHeardFrom = false;
int nSatellites = 0;
String longitude;
String latitude;
String altitude;

String CustomGPS::generateLocationCSV(){
  return longitude + "," + latitude + "," + altitude;
}