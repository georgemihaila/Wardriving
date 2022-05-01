#include "StatusBarAnimationChunk.h"

// animation takes 2s
int StatusBarAnimationChunk::getX()
{
  return (millis() % 2000) * 135 / 2000;
}

void StatusBarAnimationChunk::update(TFT_eSPI *tft)
{
  int x = getX();
  //tft->drawRect(135 - x, y - 3, x, 3, TFT_GREEN);
  tft->drawLine(0, y, 135, y, TFT_BLACK);
  tft->drawLine(x, y, 135 - x, y, TFT_GREEN);
}