#include "TFTDisplay.h"
#include "SplashScreen.h"
#include "CustomSDCard.h"
#include "CustomWiFi.h"
#include "DataManager.h"

#define SERVER_NAME "http://10.10.0.241:6488/"

TFTDisplay* _display;
SplashScreen _splashScreen;
CustomSDCard _card;
CustomWiFi* _wifi;
DataManager* _dataManager;

void printSingleStringToSerialAndDisplay(String text){
    Serial.println(text);
    _display->printSingleString(text);
}

#define BATTERY_CORRECTION_FACTOR 1.990049

double getVoltage(){
    return (double)analogReadMilliVolts(15) * BATTERY_CORRECTION_FACTOR / 1000;
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 scanner up");
 
  Serial.println("Init display");
  _display = new TFTDisplay();
  _display->init();
  if (getVoltage() < 3.4){
    printSingleStringToSerialAndDisplay("Low battery");
    delay(1000);
    ESP.restart();
  }
  if (!_card.init()){
    printSingleStringToSerialAndDisplay("SD card error");
    delay(1000);
    ESP.restart();
  }
  _wifi = new CustomWiFi(_display);
  if (_wifi->initWiFi(0)){
    _splashScreen.offline = false;
    
  }
  else{
    _splashScreen.offline = true;
  }
  _dataManager = new DataManager(_card, _display, SERVER_NAME, _splashScreen.offline);
}

void refreshBatteryVoltage(){
  _splashScreen.batteryVoltage = getVoltage();
}

void loop() {
  refreshBatteryVoltage();
  _display->refresh(_splashScreen, true);
  if (!_splashScreen.offline){
    //Keep WiFi up, if it disconnects give up and run in offline mode
    bool ok = _wifi->makeSureWiFiConnectionUp();
    if (!ok){
      _splashScreen.offline = true;
    }
  }
}
