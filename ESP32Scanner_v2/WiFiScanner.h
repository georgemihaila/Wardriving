#ifndef WiFiScanner_h_
#define WiFiScanner_h_
#include <vector>
using namespace std;
#include "WiFiNetwork.h"
#include "ScannerBase.h"

class WiFiScanner : public ScannerBase<WiFiNetwork> {
  public:
    void scanAsync();
    bool scanCompleted();
    vector<WiFiNetwork> getResults();
    int wiFiNetworksAround;
};

#endif
