#ifndef TimedChunk_h_
#define TimedChunk_h_

#include "Chunk.h"

class TimedChunk : public Chunk {
  public:
    TimedChunk(unsigned long updateEvery){
      _updateEvery = updateEvery;
    };
    bool hasUpdate();
    void markUpdated();
  private:
    unsigned long _updateEvery;
    unsigned long _lastUpdateTimestamp = 0;
};

#endif
