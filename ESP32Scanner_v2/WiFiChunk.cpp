#include "WIFIChunk.h"
#include "WiFiService.h"
#include "Arduino.h"
#include <TFT_eSPI.h>
#include <SPI.h>

void WiFiChunk::update(TFT_eSPI* tft){
  bool connected = _wifiService->makeSureWiFiConnectionUp();
  tft->setTextSize(1);
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setCursor(12, 0, 2);
  tft->println(connected?"W":"");
}