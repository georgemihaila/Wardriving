#include "FrameTimeChunk.h"

void FrameTimeChunk::update(TFT_eSPI *tft)
{
  int updateTime = millis() - _lastUpdateTime;
  _lastUpdateTime = millis();
  String text = String(updateTime) + "ms";
  tft->setTextSize(1);
  tft->setTextColor(TFT_BLACK, TFT_WHITE);
  tft->setCursor(120 - text.length() * 6, 240 - 15, 2);
  tft->println("        ");
  tft->setCursor(120 - text.length() * 6, 240 - 15, 2);
  tft->println(text);
}