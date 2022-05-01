#include "GPSChunk.h"
#include "GPSService.h"
#include "Arduino.h"
#include <TFT_eSPI.h>
#include <SPI.h>

void GPSChunk::update(TFT_eSPI *tft)
{
  if (_gpsService->gpsHeardFrom)
  {
    uint16_t color = TFT_WHITE;
    if (_gpsService->nSatellites == 0)
    {
      color = TFT_RED;
    }
    if (_gpsService->nSatellites < 3)
    {
      color = TFT_YELLOW;
    }
    if (_gpsService->nSatellites >= 4)
    {
      color = TFT_WHITE;
    }

    tft->setTextSize(1);
    tft->setTextColor(color, TFT_BLACK);
    tft->setCursor(0, 0, 2);
    tft->println("G");
  }
}