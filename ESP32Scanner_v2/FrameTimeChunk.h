#ifndef FrameTimeChunk_h_
#define FrameTimeChunk_h_

#include "TimedChunk.h"
#include <TFT_eSPI.h>

class FrameTimeChunk : public TimedChunk
{
public:
 FrameTimeChunk(unsigned long updateEvery) : TimedChunk(updateEvery)
  {
      
  };
  virtual void update(TFT_eSPI *tft);

private:
  unsigned long _lastUpdateTime = 0;
};

#endif