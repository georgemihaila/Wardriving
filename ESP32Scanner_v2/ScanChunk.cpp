#include "ScanChunk.h"
#include "Arduino.h"

String formatLargeNumber(unsigned long n)
{
  if (n < 1000)
    return String(n);
  if (n < 1000000)
    return String(n / 1000) + "." + String(((n * 100) % 1000) / 100) + "k";
  return String((n / 1000000)) + "." + String(((n * 100000) % 1000000) / 100000) + "m";
}

bool ScanChunk::hasUpdate()
{
  return _lastRefreshedAtTotalScans != totalScans; // Called when a new scan is completed
}

void ScanChunk::update(TFT_eSPI *tft)
{
  appendLastElement(networksAround); // Assume only drawn ONCE, when a scan is completed

  // tft->drawLine(0, _top, 30, _top, TFT_WHITE);

  graph(tft);
  printAt(tft, dataType, _left, _top + 2);
  printAt(tft, String(newNetworks) + "/" + String(networksAround) + "/" + String(totalNetworks), _left, _top + 2 + 20);
  // printAt(tft, lastScanTimeMs + "ms", 0, _top + _height - 12);
  //  tft->drawLine(0, _top + _height, 30, _top + _height, TFT_WHITE);
  _lastRefreshedAtTotalScans = totalScans;
}

int ScanChunk::max(int a, int b)
{
  if (a >= b)
    return a;
  return b;
}

int ScanChunk::minimum(int a, int b)
{
  if (a <= b)
    return a;
  return b;
}

int ScanChunk::max(int data[], int count)
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

void ScanChunk::graph(TFT_eSPI *tft)
{
  graph(tft, _latestEntries, 30, _top, false, 0, max(_latestEntries, 30), _height);
}

void ScanChunk::graph(TFT_eSPI *tft, int data[], int count, int yOffset, bool invertVertically)
{
  graph(tft, data, count, yOffset, invertVertically, 0, max(data, count), _height);
}

void ScanChunk::graph(TFT_eSPI *tft, int data[], int count, int yOffset, int max, bool invertVertically)
{
  graph(tft, data, count, yOffset, invertVertically, 0, max, _height);
}

double ScanChunk::customMap(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void ScanChunk::graph(TFT_eSPI *tft, int data[], int count, int yOffset, bool invertVertically, int min, int max, int height)
{
  if (min == max) // no data, don't try do do anything
    return;

  int graphBottom = yOffset + height;
  int minIndex = count - _totalEntries;
  if (minIndex < 0)
  {
    minIndex = 0;
  }
  tft->fillRect(0, yOffset, _width + _left, yOffset + height, TFT_BLACK); // Clear chart area
  int rectWidth = 135 / minimum(30, _totalEntries);
  for (int i = count - 1, ri = 0; i >= minIndex; i--, ri++)
  {
    int h1 = (int)customMap(data[i], min, max, 0, height);
    int y = graphBottom - h1;
    if (invertVertically)
    {
      y = graphBottom + h1;
    }
    int x1 = _width - (ri + 1) * rectWidth;
    tft->fillRect(x1, y, rectWidth, h1, TFT_WHITE);
    if (_nSatellites[i] >= 4)
    {
      tft->fillRect(x1, graphBottom - 5, rectWidth, 5, TFT_BLUE);
    }

    if (rectWidth >= 4)
    {
      tft->drawLine(x1, _top, x1, graphBottom, TFT_BLACK);
      tft->drawLine(x1 + rectWidth, _top, x1 + rectWidth, graphBottom, TFT_BLACK);
    }
  }
}

void ScanChunk::printAt(TFT_eSPI *tft, String text, int x, int y, int size, uint16_t color, uint16_t backgroundColor)
{
  tft->setTextSize(size);
  tft->setTextColor(color, backgroundColor);
  tft->setCursor(x, y, 2);
  tft->println(text);
}

void ScanChunk::printAt(TFT_eSPI *tft, String text, int x, int y, int size)
{
  printAt(tft, text, x, y, size, TFT_WHITE, TFT_BLACK);
}

void ScanChunk::printAt(TFT_eSPI *tft, String text, int x, int y)
{
  printAt(tft, text, x, y, 1);
}

void ScanChunk::appendLastElement(int entry)
{
  _totalEntries++;
  _latestEntries[30 - 1] = entry;
  for (int i = 0; i < 30 - 1; i++)
  {
    _latestEntries[i] = _latestEntries[i + 1];
  }
}

void ScanChunk::addNSatellites(int nSatellites)
{
  _nSatellites[30 - 1] = nSatellites;
  for (int i = 0; i < 30 - 1; i++)
  {
    _nSatellites[i] = _nSatellites[i + 1];
  }
}