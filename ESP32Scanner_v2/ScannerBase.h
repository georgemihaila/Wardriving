#ifndef ScannerBase_h_
#define ScannerBase_h_
#include <vector>
using namespace std;

template <class T> class ScannerBase {
  public:
      virtual vector<T> scan();
      virtual void yield();
};

#endif
