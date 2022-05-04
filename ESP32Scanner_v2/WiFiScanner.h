#ifndef WiFiScanner_h_
#define WiFiScanner_h_
#include <vector>
using namespace std;
#include "WiFiNetwork.h"
#include "ScannerBase.h"
#include "ScanChunk.h"

class WiFiScanner : public ScannerBase<WiFiNetwork>
{
public:
  WiFiScanner()
  {
    chunk = new ScanChunk("WiFi", 135, 240 / 2 - 20, 0, 20);
  };
  void scanAsync();
  bool scanCompleted();
  bool homeNetworkAround();
  vector<WiFiNetwork> getResults();
private:
  bool _firstScanRan = false; //Scan doesn't start in QS mode unless explicitly specified
};

#endif
