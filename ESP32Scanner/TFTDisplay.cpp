#include "TFTDisplay.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void TFTDisplay::init(){
    tft.init();
}

void printAt(String text, int x, int y, int size, uint16_t color, uint16_t backgroundColor){
  tft.setTextSize(size);
  tft.setTextColor(color, backgroundColor);
  tft.setCursor(x, y, 2);
  tft.println(text);
}

void printAt(String text, int x, int y, int size){
  printAt(text, x, y, size, TFT_WHITE, TFT_BLACK);
}

void printAt(String text, int x, int y){
  printAt(text, x, y, 1);
}

unsigned long lastRefreshTimestamp = 0;
int maxFPS = 10;

void printBatteryVoltage(SplashScreen splashScreen){
    uint16_t batteryTextColor = TFT_BLUE;
    if (splashScreen.batteryVoltage <= 4.2){
        batteryTextColor = TFT_WHITE;
    }
    if (splashScreen.batteryVoltage < 3.6){
        batteryTextColor = TFT_YELLOW;
    }
    if (splashScreen.batteryVoltage < 3.5){
        batteryTextColor = TFT_RED;
    }
    printAt(String(splashScreen.batteryVoltage) + "V", 95, 0, 1, batteryTextColor, TFT_BLACK); 
}

void printOfflineMode(SplashScreen splashScreen){
  if (splashScreen.offline){
    printAt("offline", 0, 0);
  }
}

void printGPSInfo(SplashScreen splashScreen){
  
}

void TFTDisplay::refresh(SplashScreen splashScreen) {
    if (millis() - lastRefreshTimestamp >= 1000 / maxFPS){ //Max FPS timer
        
        tft.fillScreen(TFT_BLACK);
        printBatteryVoltage(splashScreen);
        printOfflineMode(splashScreen);
        printGPSInfo(splashScreen);

        lastRefreshTimestamp = millis();
    }
}

void TFTDisplay::printSingleString(String text) {
    tft.fillScreen(TFT_BLACK);
    printAt(text, 0, 0);
}
