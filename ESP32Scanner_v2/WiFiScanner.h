#ifndef WiFiScanner_h_
#define WiFiScanner_h_
#include <vector>
using namespace std;
#include "WiFiNetwork.h"
#include "ScannerBase.h"
#include "WiFiScanChunk.h"

class WiFiScanner : public ScannerBase<WiFiNetwork> {
  public:
    void scanAsync();
    bool scanCompleted();
    vector<WiFiNetwork> getResults();
    WiFiScanChunk* chunk = new WiFiScanChunk(135, 240 / 2 - 20, 0, 20);
};

#endif
