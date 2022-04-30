#ifndef Updateable_h_
#define Updateable_h_

#include "TFTDisplay.h"

class Updateable {
  public:
    virtual bool hasUpdate();
    virtual void update(TFTDisplay* tft);
};

#endif