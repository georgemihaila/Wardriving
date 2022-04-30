#ifndef WiFiChunk_h_
#define WiFiChunk_h_

#include "TimedChunk.h"
#include "WiFiService.h"
#include "TFTDisplay.h"

class WiFiChunk : public TimedChunk {
  public:
    WiFiChunk(WiFiService* wifiService, unsigned long updateEvery) : TimedChunk(updateEvery){
      _wifiService = wifiService;
    };
    virtual void update(TFTDisplay* tft);
  private:
    WiFiService* _wifiService;
};

#endif