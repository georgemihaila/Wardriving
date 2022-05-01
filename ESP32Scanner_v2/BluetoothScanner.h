#ifndef BluetoothScanner_h_
#define BluetoothScanner_h_
#include <vector>
using namespace std;
#include "BluetoothDevice.h"
#include "ScannerBase.h"
#include "ScanChunk.h"
#include "Chunk.h"

class BluetoothScanner : public ScannerBase<BluetoothDevice>
{
public:
  BluetoothScanner()
  {
    chunk = new ScanChunk(135, 240 / 2 - 20, 0, 120 + 20);
  };
  virtual void scanAsync();
  virtual bool scanCompleted();
  virtual vector<BluetoothDevice> getResults();

private:
  bool _isScanRunning = false;
};

#endif
