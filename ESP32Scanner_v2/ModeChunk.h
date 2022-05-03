#ifndef ModeChunk_h_
#define ModeChunk_h_

#include "TimedChunk.h"
#include "Arduino.h"
#include "ThreeWaySwitch.h"

class ModeChunk : public TimedChunk
{
public:
    ModeChunk(ThreeWaySwitch *threeWaySwitch, unsigned long updateEvery) : TimedChunk(updateEvery)
    {
        _threeWaySwitch = threeWaySwitch;
    }
    virtual void update(TFT_eSPI *tft);

private:
    ThreeWaySwitch *_threeWaySwitch;
};

#endif