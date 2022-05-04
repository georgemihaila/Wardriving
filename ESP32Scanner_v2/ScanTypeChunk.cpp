#include "ScanTypeChunk.h"
#include "Arduino.h"
#include <TFT_eSPI.h>
#include <SPI.h>

void ScanTypeChunk::update(TFT_eSPI *tft)
{
    String s = "W   ";
    switch (_threeWaySwitch->getState())
    {
    case 1:
        s = "W+B";
        break;
    case 2:
        s = "B   ";
        break;
    }
    tft->setTextSize(1);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setCursor(36, 0, 2);
    tft->println(s);
}