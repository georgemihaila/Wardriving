#ifndef LEDBlinker_h_
#define LEDBlinker_h_

class LEDBlinker
{
public:
  LEDBlinker(int pin);
  void flick();

private:
  int _pin;
  bool _isOn = false;
};

#endif
