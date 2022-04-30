#ifndef BluetoothScanner_h_
#define BluetoothScanner_h_
#include <vector>
using namespace std;
#include "BluetoothDevice.h"
#include "ScannerBase.h"

class BluetoothScanner : public ScannerBase<BluetoothDevice> {
  public:
    void scanAsync();
    vector<BluetoothDevice> getResults();
    bool scanCompleted();
  private:
    bool _isScanRunning = false;
};

#endif
