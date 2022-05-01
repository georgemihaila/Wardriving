#ifndef TFTDisplayExtensions_h_
#define TFTDisplayExtensions_h_

#include <TFT_eSPI.h>
#include <SPI.h>
#include "Page.h"

class TFTDisplayExtensions
{
public:
    void dualLineGraphCentered(TFT_eSPI *tft, int data1[135], int data2[135], int height)
    {
        int absoluteMax = max(max(data1, 135), max(data2, 135));
        int yOffset = 240 - height;
        graph(tft, data1, 135, yOffset - height, absoluteMax); // top
            graph(tft, data2, 135, yOffset, absoluteMax
    }

};

#endif
