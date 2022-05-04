#include "ModeChunk.h"
#include "Arduino.h"
#include <TFT_eSPI.h>
#include <SPI.h>

void ModeChunk::update(TFT_eSPI *tft)
{
    String s = "Q";
    switch (_threeWaySwitch->getState())
    {
    case 1:
        s = "N";
        break;
    case 2:
        s = "U";
        break;
    }
    tft->setTextSize(1);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setCursor(24, 0, 2);
    tft->println(s);
}