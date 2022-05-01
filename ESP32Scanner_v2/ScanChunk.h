#ifndef ScanChunk_h_
#define ScanChunk_h_

#include "Chunk.h"

class ScanChunk : public Chunk
{
public:
  ScanChunk(int w, int h, int l, int t)
  {
    _width = w;
    _height = h;
    _left = l;
    _top = t;

    for (int i = 0; i < 135; i++){
      _latestEntries[i] = 0;
      _nSatellites[i] = 0;
    }
  };
  virtual bool hasUpdate();
  virtual void update(TFT_eSPI *tft);
  int networksAround = 0;
  int newNetworks = 0;
  int totalNetworks = 0;
  int totalScans = 0;
  
  void addNSatellites(int nSatellites);
protected:
  double customMap(double x, double in_min, double in_max, double out_min, double out_max);
  int max(int a, int b);
  int minimum(int a, int b);
  int max(int data[], int count);
  void graph(TFT_eSPI* tft);
  void graph(TFT_eSPI *tft, int data[], int count, int yOffset, bool invertVertically);
  void graph(TFT_eSPI *tft, int data[], int count, int yOffset, int max, bool invertVertically);
  void graph(TFT_eSPI *tft, int data[], int count, int yOffset, bool invertVertically, int min, int max, int height);
  void printAt(TFT_eSPI *tft, String text, int x, int y, int size, uint16_t color, uint16_t backgroundColor);
  void printAt(TFT_eSPI *tft, String text, int x, int y, int size);
  void printAt(TFT_eSPI *tft, String text, int x, int y);
  void appendLastElement(int entry);
  void addGPSDataPoint();
  
  int _width;
  int _height;
  int _left;
  int _top;
  int _lastRefreshedAtTotalScans = 0;
  int _latestEntries[135];
  
  int _latestNSatellites = 0;
  int _nSatellites[135];
private:
  unsigned long _totalEntries = 0;
};

#endif