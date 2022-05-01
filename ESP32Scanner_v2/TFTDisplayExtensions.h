#ifndef TFTDisplayExtensions_h_
#define TFTDisplayExtensions_h_

#include <TFT_eSPI.h>
#include <SPI.h>
#include "Page.h"

class TFTDisplayExtensions {
    public:
        static void clear(TFT_eSPI* tft){
            tft->fillScreen(TFT_BLACK);
        }
        
        static void printAt(TFT_eSPI* tft, String text, int x, int y, int size, uint16_t color, uint16_t backgroundColor){
            tft->setTextSize(size);
            tft->setTextColor(color, backgroundColor);
            tft->setCursor(x, y, 2);
            tft->println(text);
        }

        static void printAt(TFT_eSPI* tft, String text, int x, int y, int size){
            printAt(tft, text, x, y, size, TFT_WHITE, TFT_BLACK);
        }

        static void printAt(TFT_eSPI* tft, String text, int x, int y){
            printAt(tft, text, x, y, 1);
        }

        static double customMap(double x, double in_min, double in_max, double out_min, double out_max) {
            return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }

        static void graph(TFT_eSPI* tft, int data[], int count, int yOffset, bool invertVertically, int min, int max, int height){
            int graphBottom = 64 + yOffset - height;
            for(int i = 0; i < count - 1; i++){
                int h1 = (int)customMap(data[i], min, max, 0, height);
                int y1 = graphBottom - h1;
                if (invertVertically){
                y1 = graphBottom - height;
                }
                tft.drawLine(i, y1, i, y1 + h1, TFT_WHITE);
            }
        }

        static int max(int a, int b){
            if (a >= b)
                return a;
            return b;
        }

        static int max(int data[], int count){
            int max = data[0];
            for(int i = 0; i < count; i++){
                if (data[i] > max){
                    max = data[i];
                }
            }
            return max;
        }

        static void graph(TFT_eSPI* tft, int data[], int count, int yOffset, bool invertVertically = false, int height){
            graph(data, count, yOffset, invertVertically, 0, max(data, count), height);
        }

        static void graph(TFT_eSPI* tft, int data[], int count, int yOffset, int max, bool invertVertically = false, int height){
            graph(data, count, yOffset, invertVertically, 0, max, height);
        }

        static void dualLineGraphCentered(TFT_eSPI* tft, int data1[135], int data2[135], int height){
            int absoluteMax = max(max(data1, 135), max(data2, 135));
            int yOffset = 240 - height;
            graph(tft, data1, 135, yOffset-height, absoluteMax); //top
            graph(tft, data2, 135, yOffset, absoluteMax
        }
    private:
        TFTDisplayExtensions();
};

#endif
