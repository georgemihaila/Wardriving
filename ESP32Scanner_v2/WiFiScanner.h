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
    chunk = new ScanChunk(135, 240 / 2 - 20, 0, 20);
  };
  void scanAsync();
  bool scanCompleted();
  vector<WiFiNetwork> getResults();
};

#endif
