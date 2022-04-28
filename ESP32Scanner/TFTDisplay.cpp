#include "TFTDisplay.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define TFT_WIDTH  135
#define TFT_HEIGHT 240

#define CHART_HEIGHT 40

void TFTDisplay::init(){
    tft.init();
}

double customMap(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void graph(int data[], int count, int yOffset, bool invertVertically, int min, int max, int height){
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

int max(int a, int b){
  if (a >= b)
    return a;
  return b;
}

int max(int data[], int count){
  int max = data[0];
  for(int i = 0; i < count; i++){
    if (data[i] > max){
      max = data[i];
    }
  }
  return max;
}

void graph(int data[], int count, int yOffset, bool invertVertically = false, int height = CHART_HEIGHT){
  graph(data, count, yOffset, invertVertically, 0, max(data, count), height);
}

void graph(int data[], int count, int yOffset, int max, bool invertVertically = false, int height = CHART_HEIGHT){
  graph(data, count, yOffset, invertVertically, 0, max, height);
}

void dualLineGraphCentered(int data1[135], int data2[135]){
  int absoluteMax = max(max(data1, 135), max(data2, 135));
  int yOffset = 240 - CHART_HEIGHT;
  graph(data1, 135, yOffset-CHART_HEIGHT, absoluteMax); //top
  graph(data2, 135, yOffset, absoluteMax, true);//bottom
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
int maxFPS = 1;

void printBatteryVoltage(SplashScreen* splashScreen){
    if (splashScreen->batteryVoltage == 0)
      return;
    uint16_t batteryTextColor = TFT_BLUE;
    if (splashScreen->batteryVoltage <= 4.2){
        batteryTextColor = TFT_WHITE;
    }
    if (splashScreen->batteryVoltage < 3.6){
        batteryTextColor = TFT_YELLOW;
    }
    if (splashScreen->batteryVoltage < 3.5){
        batteryTextColor = TFT_RED;
    }
    printAt(String(splashScreen->batteryVoltage) + "V", 95, 0, 1, batteryTextColor, TFT_BLACK); 
}

void printWiFiScanStats(SplashScreen* splashScreen){
  printAt("WiFi: " + String(splashScreen->newWiFiNetworks) + "/" +  String(splashScreen->wiFiNetworksAround) + "/" + String(splashScreen->sessionWiFiNetworks), 0, 20);
}

void printBTScanStats(SplashScreen* splashScreen){
  printAt("BT:   " + String(splashScreen->newBTDevices) + "/" +  String(splashScreen->btDevicesAround) + "/" + String(splashScreen->sessionBTDevices), 0, 40);
}

void printOfflineMode(SplashScreen* splashScreen){
  if (splashScreen->offline){
    printAt("offline", 0, 0);
  }
  else{
    printAt("CCE_24", 0, 0);
  }
}

void printLastLoopTime(SplashScreen* splashScreen){
  printAt(String(splashScreen->lastLoopTimeMs) + "ms", 0, TFT_HEIGHT - 20);
}

void printGPSInfo(SplashScreen* splashScreen){
  if (splashScreen->gpsHeardFrom){
    uint16_t color = TFT_WHITE;
    if (splashScreen->nSatellites == 0){
      color = TFT_RED;
    }
    if (splashScreen->nSatellites < 3){
      color = TFT_YELLOW;
    }
    if (splashScreen->nSatellites >= 4){
      color = TFT_WHITE;
    }
    printAt("GPS:  " + String(splashScreen->nSatellites), 0, 60, 1, color, TFT_BLACK);
  }
}

SplashScreen* lastSplashScreen;
String currentAction = "";

void printCurrentAction(){
    if (!currentAction.equals("")){
      printAt(currentAction, 0, 100);
    }
}

void TFTDisplay::refresh(SplashScreen* splashScreen, bool limitFPS) {
    if (limitFPS){
      if (millis() - lastRefreshTimestamp < 1000 / maxFPS){ //Max FPS timer
          return;
      }
    }

    clear();
    printBatteryVoltage(splashScreen);
    printOfflineMode(splashScreen);
    printGPSInfo(splashScreen);

    printCurrentAction();
    printWiFiScanStats(splashScreen);
    printBTScanStats(splashScreen);

    dualLineGraphCentered(splashScreen->latestWiFiNetworkCount, splashScreen->latestBTCount);
    printLastLoopTime(splashScreen);
    
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
    //refresh(lastSplashScreen, false);
    printCurrentAction();
}

void TFTDisplay::clear(){
  tft.fillScreen(TFT_BLACK);
}
