#ifndef Page_h_
#define Page_h_

#include "Updateable.h"
#include "Chunk.h"
#include <vector>
#include <TFT_eSPI.h>
using namespace std;

class Page : public Updateable
{
public:
  void addChunk(Chunk *chunk);
  bool hasUpdate();
  void update(TFT_eSPI *tft);

private:
  vector<Chunk *> _chunks;
};

#endif
