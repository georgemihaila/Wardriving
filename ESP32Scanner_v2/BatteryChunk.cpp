#include "BatteryChunk.h"
#include "Battery.h"
#include "Arduino.h"
#include <TFT_eSPI.h>


bool BatteryChunk::hasUpdate(){
  return millis() - _lastUpdateTimestamp >= _updateEvery;
}

void BatteryChunk::update(TFT_eSPI* tft){
  double voltage = _battery->getVoltage();
  if (voltage == 0)
      return;

  uint16_t batteryTextColor = TFT_BLUE;
  if (voltage <= 4.2){
    batteryTextColor = TFT_WHITE;
  }
  if (voltage < 3.6){
    batteryTextColor = TFT_YELLOW;
  }
  if (voltage < 3.5){
    batteryTextColor = TFT_RED;
  }

  tft->setTextSize(1);
  tft->setTextColor(batteryTextColor, TFT_BLACK);
  tft->setCursor(95, 0, 2);
  tft->print(String(voltage) + "V");
  Serial.println("Battery chunk update");
  _lastUpdateTimestamp = millis();
}