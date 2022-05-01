#ifndef Chunk_h_
#define Chunk_h_

#include "Updateable.h"

class Chunk : public Updateable
{
public:
  virtual bool hasUpdate();
};

#endif
