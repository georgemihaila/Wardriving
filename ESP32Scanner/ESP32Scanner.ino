#include "TFTDisplay.h"
#include "SplashScreen.h"
#include "CustomSDCard.h"
#include "CustomWiFi.h"
#include "DataManager.h"
#include "WiFiScanner.h"
#include "CustomGPS.h"
#include "BTScanner.h"

#define SERVER_NAME "http://10.10.0.241:6488/"

TFTDisplay* _display;
SplashScreen* _splashScreen = new SplashScreen();
CustomSDCard* _card;
CustomWiFi* _wifi;
DataManager* _dataManager;
WiFiScanner* _wifiScanner;
CustomGPS* _customGPS;
BTScanner* _btScanner;

void printSingleStringToSerialAndDisplay(String text){
    Serial.println(text);
    _display->printSingleString(text);
}

#define BATTERY_CORRECTION_FACTOR 1.94901706186

double getVoltage(){
    return (double)analogReadMilliVolts(36) * BATTERY_CORRECTION_FACTOR / 1000;
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
  _card = new CustomSDCard();
  if (!_card->init()){
    printSingleStringToSerialAndDisplay("SD card error");
    delay(1000);
    ESP.restart();
  }
  _wifi = new CustomWiFi(_display);
  if (_wifi->initWiFi(0)){
    _splashScreen->offline = false;
    
  }
  else{
    _splashScreen->offline = true;
  }
  _dataManager = new DataManager(_card, _display, SERVER_NAME, _splashScreen->offline);
  if (!_splashScreen->offline){
    _dataManager->createNewSession();
  }
  _customGPS = new CustomGPS(_display, _splashScreen);
  _wifiScanner = new WiFiScanner(_dataManager, _customGPS, _card, _wifi, _display, _splashScreen);
  _btScanner = new WiFiScanner(_dataManager, _customGPS, _card, _display, _splashScreen);
}

void refreshBatteryVoltage(){
  _splashScreen->batteryVoltage = getVoltage();
}

unsigned long lastLoopTimestamp = 0;

void loop() {
  refreshBatteryVoltage();

  _display->setCurrentAction("WiFi");
  _wifiScanner->scan();

  _display->setCurrentAction("BT");
  _btScanner->scan();

  _customGPS->tick();
  
  if (!_splashScreen->offline){
    _btScanner->tick();
    _wifiScanner->tick();
    //Keep WiFi up, if it disconnects give up and run in offline mode
    bool ok = _wifi->makeSureWiFiConnectionUp();
    if (!ok){
      _splashScreen->offline = true;
    }
  }
  
  _display->refresh(_splashScreen, false);
  _splashScreen->lastLoopTimeMs = millis() - lastLoopTimestamp;
  lastLoopTimestamp = millis();
}
