#include "WiFiScanChunk.h"
#include "Arduino.h"

bool WiFiScanChunk::hasUpdate()
{
  return _lastRefreshedAtTotalScans != totalScans;
}

void WiFiScanChunk::update(TFT_eSPI *tft)
{
  tft->drawLine(0, _top, 135, _top, TFT_WHITE);

  printAt(tft, String(networksAround), _left, _top + 2);

  tft->drawLine(0, _top + _height, 135, _top + _height, TFT_WHITE);
  _lastRefreshedAtTotalScans = totalScans;
}

double WiFiScanChunk::customMap(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void WiFiScanChunk::graph(TFT_eSPI *tft, int data[], int count, int yOffset, bool invertVertically, int min, int max, int height)
{
  int graphBottom = 64 + yOffset - height;
  for (int i = 0; i < count - 1; i++)
  {
    int h1 = (int)customMap(data[i], min, max, 0, height);
    int y1 = graphBottom - h1;
    if (invertVertically)
    {
      y1 = graphBottom - height;
    }
    tft->drawLine(i, y1, i, y1 + h1, TFT_WHITE);
  }
}

int WiFiScanChunk::max(int a, int b)
{
  if (a >= b)
    return a;
  return b;
}

int WiFiScanChunk::max(int data[], int count)
{
  int max = data[0];
  for (int i = 0; i < count; i++)
  {
    if (data[i] > max)
    {
      max = data[i];
    }
  }
  return max;
}

void WiFiScanChunk::graph(TFT_eSPI *tft, int data[], int count, int yOffset, bool invertVertically)
{
  graph(tft, data, count, yOffset, invertVertically, 0, max(data, count), _height);
}

void WiFiScanChunk::graph(TFT_eSPI *tft, int data[], int count, int yOffset, int max, bool invertVertically)
{
  graph(tft, data, count, yOffset, invertVertically, 0, max, _height);
}

void WiFiScanChunk::printAt(TFT_eSPI *tft, String text, int x, int y, int size, uint16_t color, uint16_t backgroundColor)
{
  tft->setTextSize(size);
  tft->setTextColor(color, backgroundColor);
  tft->setCursor(x, y, 2);
  tft->println(text);
}

void WiFiScanChunk::printAt(TFT_eSPI *tft, String text, int x, int y, int size)
{
  printAt(tft, text, x, y, size, TFT_WHITE, TFT_BLACK);
}

void WiFiScanChunk::printAt(TFT_eSPI *tft, String text, int x, int y)
{
  printAt(tft, text, x, y, 1);
}
