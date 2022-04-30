#ifndef TFTDisplay_h_
#define TFTDisplay_h_

class Page;
#include <TFT_eSPI.h>
#include <SPI.h>

class TFTDisplay : public TFT_eSPI {
  public:
      TFTDisplay();
      void clear();
      void render(Page* page);
  private:
      TFT_eSPI* _tft;
};

#endif
