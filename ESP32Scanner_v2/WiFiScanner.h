#ifndef WiFiScanner_h_
#define WiFiScanner_h_
#include <vector>
using namespace std;
#include "WiFiNetwork.h"
#include "ScannerBase.h"

class WiFiScanner : public ScannerBase<WiFiNetwork> {
  public:
    vector<WiFiNetwork> scan();
    int wiFiNetworksAround;
};

#endif
