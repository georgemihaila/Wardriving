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
    if (splashScreen.batteryVoltage == 0)
      return;
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
  else{
    printAt("CCE_24", 0, 0);
  }
}

void printGPSInfo(SplashScreen splashScreen){
  
}

SplashScreen lastSplashScreen;
String currentAction = "";

void TFTDisplay::refresh(SplashScreen splashScreen, bool limitFPS) {
    if (limitFPS){
      if (millis() - lastRefreshTimestamp < 1000 / maxFPS){ //Max FPS timer
          return;
      }
    }

    tft.fillScreen(TFT_BLACK);
    printBatteryVoltage(splashScreen);
    printOfflineMode(splashScreen);
    printGPSInfo(splashScreen);

    if (!currentAction.equals("")){
      printAt(currentAction, 0, 70);
    }
    
    lastRefreshTimestamp = millis();
    lastSplashScreen = splashScreen;
}

void TFTDisplay::printSingleString(String text) {
    tft.fillScreen(TFT_BLACK);
    printAt(text, 0, 0);
}

void TFTDisplay::setCurrentAction(String text){
    Serial.println(text);
    currentAction = text;
    refresh(lastSplashScreen, false);
}
