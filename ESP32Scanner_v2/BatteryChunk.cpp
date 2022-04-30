#include "BatteryChunk.h"
#include "Battery.h"
#include "Arduino.h"
#include "TFTDisplay.h"
#include <SPI.h>

void BatteryChunk::update(TFTDisplay* tft){
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
  if (voltage > 4.2){
    tft->println("USB");
  } else{
    tft->println(String(voltage) + "V");
  }
}