#ifndef ScannerBase_h_
#define ScannerBase_h_
#include <vector>
using namespace std;

template <class T> class ScannerBase {
  public:
    virtual void scanAsync();
    virtual vector<T> getResults();
    virtual bool scanCompleted();
};

#endif
