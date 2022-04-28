#include "CustomGPS.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

#include <SoftwareSerial.h>
#include "TFTDisplay.h"

#include "TinyGPSPlus.h"
TinyGPSPlus gps;

#define PORT_TX 12
#define PORT_RX 13

SoftwareSerial gpsPort;

TFTDisplay* gpsDisplayRef;
SplashScreen* gpsSplashScreenRef;

CustomGPS::CustomGPS(TFTDisplay* display, SplashScreen* splashScreen){
  gpsDisplayRef = display;
  gpsSplashScreenRef = splashScreen;
  
  gpsPort.begin(9600, SWSERIAL_8N1, PORT_RX, PORT_TX, false);
  if (!gpsPort) {
      gpsDisplayRef->setCurrentAction("Invalid SoftwareSerial pin configuration, check config"); 
      delay (1000);
      ESP.restart();
  } 
}

String CustomGPS::generateLocationCSV(){
  return gpsSplashScreenRef->longitude + "," + gpsSplashScreenRef->latitude + "," + gpsSplashScreenRef->altitude;
}

#define GPS_UPDATE_MS 1000
long lastGPSUpdateMs = 0;

void gpsDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (gpsPort.available()) {
      int c = gpsPort.read();
      gps.encode(c);
      Serial.print((char)c);
      if (!gpsSplashScreenRef->gpsHeardFrom){
        gpsSplashScreenRef->gpsHeardFrom = true;
      }
    }
  } while (millis() - start < ms);
}


void GPSUpdate(){
    gpsDisplayRef->setCurrentAction("GPS");
    bool updated = false;
    if (gps.location.isUpdated()){
      String latitude = String(gps.location.lat());
      gpsSplashScreenRef->latitude = latitude;
      String longitude = String(gps.location.lng());
      gpsSplashScreenRef->longitude = longitude;
      updated = true;
    }
    if (gps.altitude.isUpdated()){
     String altitude = String(gps.altitude.meters());
     gpsSplashScreenRef->altitude = altitude;
     updated = true;
    }
    gpsSplashScreenRef->nSatellites = gps.satellites.value();
    if (updated){
      Serial.println("Longitude: " + gpsSplashScreenRef->longitude + "\nLatitude: " + gpsSplashScreenRef->latitude + "\nAltitude: " + gpsSplashScreenRef->altitude);
    }
}

void CustomGPS::tick(){
  GPSUpdate();
}
