#ifndef WiFiScanChunk_h_
#define WiFiScanChunk_h_

#include "TimedChunk.h"

class WiFiScanChunk : public Chunk {
  public:
    WiFiScanChunk(int w, int h, int l, int t){
      _width = w;
      _height = h;
      _left = l;
      _top = t;
    };
    virtual bool hasUpdate();
    virtual void update(TFT_eSPI* tft);
    int networksAround = 0;
    int newNetworks = 0;
    int totalNetworks = 0;
    int totalScans = 0;
  private:
    int _width;
    int _height;
    int _left;
    int _top;
    int _lastRefreshedAtTotalScans = 0;
};

#endif