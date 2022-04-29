#ifndef BatteryChunk_h_
#define BatteryChunk_h_

#include "Chunk.h"
#include "Battery.h"
#include <TFT_eSPI.h>

class BatteryChunk : public Chunk {
  public:
    virtual bool hasUpdate();
    virtual void update(TFT_eSPI* tft);
  private: 
    Battery* _battery = new Battery();
    unsigned long _lastUpdateTimestamp = 0;
    unsigned long _updateEvery = 1000;
};

#endif