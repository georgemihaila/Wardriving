#include "ScanChunk.h"
#include "Arduino.h"

bool ScanChunk::hasUpdate()
{
  return _lastRefreshedAtTotalScans != totalScans; // Called when a new scan is completed
}

void ScanChunk::update(TFT_eSPI *tft)
{
  appendLastElement(networksAround); // Assume only drawn ONCE, when a scan is completed

  // tft->drawLine(0, _top, 135, _top, TFT_WHITE);

  graph(tft);
  printAt(tft,  String(/*newNetworks*/0) + "/" + String(networksAround) + "/" + String(totalNetworks) , _left, _top + 2);

  // tft->drawLine(0, _top + _height, 135, _top + _height, TFT_WHITE);
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
  graph(tft, _latestEntries, 135, _top, false, 0, max(_latestEntries, 135), _height);
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
  int graphBottom = yOffset + height;
  int minIndex = count - _totalEntries;
  if (minIndex < 0){
    minIndex = 0;
  }
  tft->fillRect(0, yOffset, _width + _left, yOffset + height, TFT_BLACK); //Clear chart area
  int rectWidth = count / minimum(135, _totalEntries);
  for (int i = count - 1, ri = 0; i >= minIndex; i--, ri++)
  {
    int h1 = (int)customMap(data[i], min, max, 0, height);
    int y = graphBottom - h1;
    if (invertVertically)
    {
      y = graphBottom + h1;
    }
    uint8_t color = TFT_WHITE;
    if (_nSatellites[i] < 4){
      color = TFT_YELLOW;
    }
    int x1 = _width - (ri + 1) * rectWidth;
    tft->fillRect(x1, y, rectWidth, h1, color);
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
  _latestEntries[134] = entry;
  for (int i = 0; i < 134; i++)
  {
    _latestEntries[i] = _latestEntries[i + 1];
  }
}

void ScanChunk::addNSatellites(int nSatellites)
{
  _nSatellites[134] = nSatellites;
  for (int i = 0; i < 134; i++)
  {
    _nSatellites[i] = _nSatellites[i + 1];
  }
}