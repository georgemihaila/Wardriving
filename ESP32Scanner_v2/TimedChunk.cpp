#include "TimedChunk.h"

bool TimedChunk::hasUpdate()
{
  return millis() - _lastUpdateTimestamp >= _updateEvery;
}

void TimedChunk::markUpdated()
{
  _lastUpdateTimestamp = millis();
}