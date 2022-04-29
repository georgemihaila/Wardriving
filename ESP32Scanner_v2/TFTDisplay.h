#ifndef TFTDisplay_h_
#define TFTDisplay_h_

#include <TFT_eSPI.h>
#include <SPI.h>
#include "Page.h"

class TFTDisplay {
  public:
      TFTDisplay();
      void clear();
      void render(Page* page);
  private:
      TFT_eSPI* _tft;
};

#endif
