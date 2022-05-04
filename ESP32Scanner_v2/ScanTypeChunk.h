#ifndef ScanTypeChunk_h_
#define ScanTypeChunk_h_

#include "TimedChunk.h"
#include "Arduino.h"
#include "ThreeWaySwitch.h"

class ScanTypeChunk : public TimedChunk
{
public:
    ScanTypeChunk(ThreeWaySwitch *threeWaySwitch, unsigned long updateEvery) : TimedChunk(updateEvery)
    {
        _threeWaySwitch = threeWaySwitch;
    }
    virtual void update(TFT_eSPI *tft);

private:
    ThreeWaySwitch *_threeWaySwitch;
};

#endif