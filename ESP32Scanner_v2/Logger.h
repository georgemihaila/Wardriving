#ifndef Logger_h_
#define Logger_h_

#include "Arduino.h"

class Logger{
  public:
    Logger();
    void log(String message);
};

#endif