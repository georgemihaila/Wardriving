#include "WiFiScanChunk.h"

bool WiFiScanChunk::hasUpdate(){
  Serial.println("Check wifi shicn");
  return _lastRefreshedAtTotalNetworks != totalNetworks;
}

void WiFiScanChunk::update(TFT_eSPI* tft){
  Serial.println("Updating wifi shicn");
  tft->setTextSize(1);
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setCursor(_left, _top, 2);
  tft->println(networksAround);
  _lastRefreshedAtTotalNetworks = totalNetworks;
}