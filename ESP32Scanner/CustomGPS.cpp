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

void sendPacket(byte *packet, byte len) {
 for (byte i = 0; i < len; i++)
 {
 gpsPort.write(packet[i]); // GPS is HardwareSerial
 }
}

void changeFrequency() {
    byte packet[] = {
        0xB5, // 
        0x62, // 
        0x06, // 
        0x08, // 
        0x06, // length
        0x00, // 
        0xFA, // measRate, hex 64 = dec 100 ms
        0x00, // 
        0x01, // navRate, always =1
        0x00, // 
        0x01, // timeRef, stick to GPS time (=1)
        0x00, // 
        0x7A, // CK_A
        0x12, // CK_B
    };
    sendPacket(packet, sizeof(packet));
}

CustomGPS::CustomGPS(TFTDisplay* display, SplashScreen* splashScreen){
  gpsDisplayRef = display;
  gpsSplashScreenRef = splashScreen;
  
  gpsPort.begin(9600, SWSERIAL_8N1, PORT_RX, PORT_TX, false);
  if (!gpsPort) {
      gpsDisplayRef->setCurrentAction("Invalid SoftwareSerial pin configuration, check config"); 
      delay (1000);
      ESP.restart();
  } 
   changeFrequency();
}

String CustomGPS::generateLocationCSV(){
  return gpsSplashScreenRef->longitude + "," + gpsSplashScreenRef->latitude + "," + gpsSplashScreenRef->altitude;
}

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
    gpsDelay(1500);
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

void CustomGPS::yield(){
  gpsDelay(0);
}

void CustomGPS::tick(){
  GPSUpdate();
}
