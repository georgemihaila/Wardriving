#include "TFTDisplay.h"
#include <TFT_eSPI.h>

#include "Page.h"

#define TFT_WIDTH  135
#define TFT_HEIGHT 240

TFTDisplay::TFTDisplay(){
  _tft = new TFT_eSPI();
  _tft->init();
  _tft->setTextFont(2);
  clear();
}

void TFTDisplay::clear(){
  _tft->fillScreen(TFT_BLACK);
}

void TFTDisplay::render(Page* page){
  if (page->hasUpdate()){
    page->update(_tft);
  }
}