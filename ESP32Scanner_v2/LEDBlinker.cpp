#include "LEDBlinker.h"
#include "Arduino.h"

LEDBlinker::LEDBlinker(int pin)
{
  _pin = pin;

  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
}

void LEDBlinker::flick()
{
  _isOn = !_isOn;
  if (_isOn)
  {
    digitalWrite(_pin, HIGH);
  }
  else
  {
    digitalWrite(_pin, LOW);
  }
}