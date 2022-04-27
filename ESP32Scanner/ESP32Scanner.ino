#include "TFTDisplay.h"
#include "SplashScreen.h"
#include "CustomSDCard.h"

TFTDisplay display;
SplashScreen splashScreen;
CustomSDCard sdCard;

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 scanner up");
  
  Serial.println("Init display");
  display.init();
  if (!sdCard.init()){
    String text = "SD card error";
    Serial.println(text);
    display.printSingleString(text);
    delay(1000);
    ESP.restart();
  }
}

#define BATTERY_CORRECTION_FACTOR 1.990049

void refreshBatteryVoltage(){
  splashScreen.batteryVoltage = (double)analogReadMilliVolts(15) * BATTERY_CORRECTION_FACTOR / 1000;
}

void loop() {
  refreshBatteryVoltage();
  display.refresh(splashScreen);
}
