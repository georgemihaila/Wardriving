#ifndef BatteryChunk_h_
#define BatteryChunk_h_

#include "TimedChunk.h"
#include "Battery.h"
#include "TFTDisplay.h"

class BatteryChunk : public TimedChunk {
  public:
    BatteryChunk(unsigned long updateEvery) : TimedChunk(updateEvery){

    };
    virtual void update(TFTDisplay* tft);
  private: 
    Battery* _battery = new Battery();
};

#endif