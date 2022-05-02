#include "GPSService.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

#include <SoftwareSerial.h>
#include "TFTDisplay.h"

#include "TinyGPSPlus.h"
TinyGPSPlus gps;

#define PORT_TX 12
#define PORT_RX 13

SoftwareSerial gpsPort;

void GPSService::sendPacket(byte *packet, byte len)
{
  for (byte i = 0; i < len; i++)
  {
    gpsPort.write(packet[i]); // GPS is HardwareSerial
  }
}

void GPSService::changeFrequency()
{
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

GPSService::GPSService()
{
  gpsPort.begin(9600, SWSERIAL_8N1, PORT_RX, PORT_TX, false);
  if (!gpsPort)
  {
    Serial.println("Invalid SoftwareSerial pin configuration, check config");
    delay(1000);
    ESP.restart();
  }
}

String GPSService::generateLocationCSV()
{
  return String(longitude, 12) + "," + String(latitude, 12) + "," + String(altitude, 12);
}

void GPSService::gpsDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (gpsPort.available())
    {
      int c = gpsPort.read();
      gps.encode(c);
      //Serial.print((char)c);
      if (!gpsHeardFrom)
      {
        gpsHeardFrom = true;
      }

      if (millis() - start >= ms)
        break;
      if (gps.location.isUpdated())
      {
        break;
      }
    }
  } while (millis() - start < ms);
}

void GPSService::update()
{ /*
   if (millis() - _lastUpdatedAtMS < _updateEveryMS){
     return;
   }*/
  yield();
  // gpsDelay(1500);
  bool updated = false;
  if (gps.location.isUpdated())
  {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    updated = true;
  }
  if (gps.altitude.isUpdated())
  {
    altitude = gps.altitude.meters();
    updated = true;
  }
  nSatellites = gps.satellites.value();
  _lastUpdatedAtMS = millis();
}

void GPSService::yield()
{
  gpsDelay(0);
}

#define LAT 44.46228918598267
#define LNG 26.129464210079554
#define K 111139

double GPSService::getXMetersFromOrigin()
{
  return (LNG - longitude) * K;
}

double GPSService::getYMetersFromOrigin()
{
  return (LAT - latitude) * K;
}