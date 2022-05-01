#include "DataManager.h"

bool DataManager::updateChunk()
{
    int x = int(_gpsService->getXMetersFromOrigin() / _chunkLengthMeters);
    int y = int(_gpsService->getYMetersFromOrigin() / _chunkLengthMeters);

    bool updated = false;
    if (x != _currentXChunk)
    {
        updated = true;
        _currentXChunk = x;
    }
    if (y != _currentYChunk)
    {
        updated = true;
        _currentYChunk = y;
    }
    return updated
}