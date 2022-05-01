#ifndef StatusBarAnimation_h_
#define StatusBarAnimation_h_

#include "TimedChunk.h"

class StatusBarAnimationChunk : public TimedChunk
{
public:
  StatusBarAnimationChunk(unsigned long updateEvery) : TimedChunk(updateEvery){

                                                       };
  virtual void update(TFT_eSPI *tft);

private:
  int getX();
  int y = 239;
};

#endif