#include "Battery.h"
#include "Arduino.h"
#define BATTERY_CORRECTION_FACTOR 1.94901706186

double Battery::getVoltage()
{
  return (double)analogReadMilliVolts(36) * BATTERY_CORRECTION_FACTOR / 1000;
}