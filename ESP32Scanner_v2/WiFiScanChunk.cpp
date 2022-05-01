#include "WiFiScanChunk.h"
#include "Arduino.h"

bool WiFiScanChunk::hasUpdate(){
  return _lastRefreshedAtTotalScans != totalScans;
}

void WiFiScanChunk::update(TFT_eSPI* tft){
  tft->drawLine(0, _top, 135, _top, TFT_WHITE);

  //TFTDisplayExtensions::printAt(tft, String(networksAround), _left, _top + 2);

  tft->drawLine(0, _top + _height, 135, _top + _height, TFT_WHITE);
  _lastRefreshedAtTotalScans = totalScans;
}