#include "SplashScreen.h"
#include "BatteryChunk.h"
#include <TFT_eSPI.h>

SplashScreen::SplashScreen(){
  addChunk(new BatteryChunk());
}