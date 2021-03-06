#ifndef WiFiChunk_h_
#define WiFiChunk_h_

#include "TimedChunk.h"
#include "WiFiService.h"
#include <TFT_eSPI.h>

class WiFiChunk : public TimedChunk
{
public:
  WiFiChunk(WiFiService *wifiService, unsigned long updateEvery) : TimedChunk(updateEvery)
  {
    _wifiService = wifiService;
  };
  virtual void update(TFT_eSPI *tft);

private:
  WiFiService *_wifiService;
};

#endif