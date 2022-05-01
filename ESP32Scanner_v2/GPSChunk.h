#ifndef GPSChunk_h_
#define GPSChunk_h_

#include "TimedChunk.h"
#include "GPSService.h"
#include <TFT_eSPI.h>

class GPSChunk : public TimedChunk
{
public:
  GPSChunk(GPSService *gpsService, unsigned long updateEvery) : TimedChunk(updateEvery)
  {
    _gpsService = gpsService;
  };
  virtual void update(TFT_eSPI *tft);

private:
  GPSService *_gpsService;
};

#endif