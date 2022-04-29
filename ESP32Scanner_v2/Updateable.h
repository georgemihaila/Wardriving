#ifndef Updateable_h_
#define Updateable_h_
#include <TFT_eSPI.h>

class Updateable {
  public:
    virtual bool hasUpdate();
    virtual void update(TFT_eSPI* tft);
};

#endif