#ifndef ScannerBase_h_
#define ScannerBase_h_
#include <vector>
using namespace std;
#include "ScanChunk.h"

template <class T>
class ScannerBase
{
public:
  virtual void scanAsync();
  virtual vector<T> getResults();
  virtual bool scanCompleted();
  ScanChunk *chunk;
};

#endif
