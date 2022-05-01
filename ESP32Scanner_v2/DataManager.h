#ifndef DataManager_h_
#define DataManager_h_

#include "SDCard.h"
#include "GPSService.h"

class DataManager
{
    public:
        DataManager(SDCard *sdCard, GPSService *gpsService)
        {
            _sdCard = sdCard;
            _gpsService = gpsService;
        }
        
        bool updateChunk();
    private:
        SDCard *_sdCard;
        GPSService *_gpsService;
        int _chunkLengthMeters = 10; //Square chunks
        int _currentXChunk = 0;
        int _currentYChunk = 0;
};

#endif
